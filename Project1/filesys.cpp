#include "filesys.h"

using namespace std;

Filesys::Filesys(string diskname, int numberofblocks, int blocksize):Sdisk(diskname, numberofblocks, blocksize){
    
    string buffer;
    rootsize = getblocksize() / 13;
    fatsize = (getnumberofblocks() * 6 / getblocksize()) + 1;
    getblock(1, buffer);

    if (buffer[0] == '#'){
        
        // build the filesystem
        
        cout << "Building the filesystem" << endl;
        
        // Build the root
        for (int i = 0; i < rootsize; i++){

            filename.push_back("XXXXXX");
            firstblock.push_back(0);
        }

        // Build FAT
        fat.push_back(fatsize + 2);
        fat.push_back(-1);
        
        for (int i = 0; i < fatsize; i++){

            fat.push_back(-1);
        }

        for (int i = 2 + fatsize; i < getnumberofblocks(); i++){

            fat.push_back(i + 1);
        }

        fat[fat.size() - 1] = 0;
        fssynch();

    }
    else{

        // Read existing filesystem
        string buffer;
        getblock(1, buffer);
             
        istringstream istream;
        istream.str(buffer);

        for (int i = 0; i < rootsize; i++){
            string fn;
            int fb;

            istream >> fn >> fb;
            filename.push_back(fn);
            firstblock.push_back(fb);
        }

        string fatbuffer;
        istringstream istream2;

        for (int i = 0; i < fatsize; i++){

            string buffer1;

            getblock(i + 2, buffer1); 
            fatbuffer += buffer1;
        }

        istream2.str(fatbuffer);

        for(int i = 0; i < getnumberofblocks(); i++){

            int u;
            istream2 >> u;
            fat.push_back(u);
        }
    }
}


int Filesys::fsclose(){

    return 0;
}


int Filesys::fssynch(){

    // write root and fat to disk

    ostringstream ostream1, ostream2;

    for (int i = 0; i < rootsize; i++){

        ostream1 << filename[i] << " " << firstblock[i] << " ";
    }

    for (int i = 0; i < fat.size(); i++){

        ostream2 << fat[i] << " ";
    }

    vector<string> blocks1 = block(ostream1.str(), getblocksize());

    putblock(0, blocks1[0]);

    vector<string> blocks2 = block(ostream2.str(), getblocksize());

    for (int i = 0; i < fatsize; i++){

        putblock(i, blocks2[i - 1]);

    }

    return 0;
}


int Filesys::newfile(string file){

    // return 1 is successful
    // return 0 if fail

    for (int i = 0; i < filename.size(); i++){

        if (filename[i] == file){

            cout << "file exists! ";

            return 0;
        }
    }

    for (int i = 0; i < filename.size(); i++){

        if (filename[i] == "XXXXXX"){

            filename[i] = file;
            firstblock[i] = 0;

            fssynch();
            
            return 1; 
        }
    }
    
    cout << "no room in root ";

    return 0;
}


int Filesys::rmfile(string file){

    for (int i = 0; i < filename.size(); i++){

        if (filename[i] == file){

            if (firstblock[i] == 0){

                filename[i] = "XXXXXX";
                return fssynch();
            }
            else{

                // file is not empty
                cout << "Need empty file to remove" << endl;
                return 0; 
            }
        }
    }

    return 0;
}


int Filesys::getfirstblock(string file){

    for (int i = 0; i < filename.size(); i++){

        if (filename[i] == file){

            return firstblock[i];
        }
    }

    cout << "No such file ";

    return -1;
}


int Filesys::addblock(string file, string block){

    // Return allocated block number

    int first = getfirstblock(file);

    if (first == -1){
        
        cout << "No such file ";

        return 0;
    }

    int allocate = fat[0];
    fat[0] = fat[fat[0]];
    fat[allocate] = 0;

    if (first == 0){

        for (int i = 0; i < filename.size(); i++){

            if (filename[i] == file){

                firstblock[i] = allocate;
            }

        }
    
    }
    else{

        int b = first;

        while (fat[b] == 0){

            b = fat[b];
        }

        fat[b] = allocate;
    } 

    fssynch();
    putblock(allocate, block);

    return allocate;
}


int Filesys::delblock(string file, int blocknumber){

    if (!checkblock(file, blocknumber)){

        cout << "Unable to delete block " << blocknumber << endl;

        return 0;
    }

    int b = getfirstblock(file);

    if (blocknumber == b){

        for (int i = 0; i < filename.size(); i++){

            if (filename[i] == file){

                firstblock[i] = fat[blocknumber];
            }

            break;
        }
    }
    else {

        while (fat[b] != blocknumber){

            b = fat[b];
        }

        fat[b] = fat[blocknumber];
    }

    fat[blocknumber] = fat[0];
    fat[0] = blocknumber;
    fssynch();

    return 1;
}


int Filesys::readblock(string file, int blocknumber, string& buffer){

    if (checkblock(file, blocknumber)){

        getblock(blocknumber, buffer);

        return 1;
    }
    else {

        cout << "Block Number does not belong to this file";
        
        return 0;
    }
}


int Filesys::writeblock(string file, int blocknumber, string buffer){

    if (checkblock(file, blocknumber)){

        putblock(blocknumber, buffer);

        return 1;   
    }
    else {

        return 0;
    }
}


int Filesys::nextblock(string file, int blocknumber){

    if (checkblock(file, blocknumber)){

        return fat[blocknumber];
    }
   
   return 0;
}


bool Filesys::checkblock(string file, int blocknumber){

    // Checks if block number belongs to file

    int b = getfirstblock(file);

    if (b == -1){

        cout << "No such file ";

        return false;
    }

    if (b == blocknumber){

        return true;
    }

    while (fat[b] != 0){

        if (fat[b] == blocknumber){

            return true;
        }

        b = fat[b];
    }

    return false;
}


/*
vector<string> Filesys::ls(){

    vector<string> flist;

    for (int i = 0; i < fileName.size(); i++){

        if (fileName[i] != "XXXX"){

            flist.push_back(fileName[i]);
        }
    }

    return flist;
}
*/


vector<string> block(string s, int b){
    // blocks the buffer into a list of blocks of size b

    vector<string> blocks;
    int numberblocks = 0;

    if (s.length() % b == 0){

        numberblocks = s.length() / b;

    }
    else {

        numberblocks = s.length() / b + 1;

    }

    string tempblock;

    for (int i = 0; i < numberblocks; i++){
        
        tempblock = s.substr(b * i, b);
        blocks.push_back(tempblock);

    }

    int lastblock = blocks.size() - 1;

    for (int i = blocks[lastblock].length(); i < b; i++){

        blocks[lastblock] += '#';

    }

    return blocks;
}
