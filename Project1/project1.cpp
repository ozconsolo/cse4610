// Name: Azael Consolo
// CSE 4610

#include<fstream>
#include<iostream>

using namespace std;

class Sdisk {
public:
	Sdisk(string diskname, int numberOfBlocks, int blockSize);
	int getBlock(int blockNumber, string& buffer);
	int putBlock(int blockNumber, string buffer);
	int getNumberOfBlocks();  // accessor function
	int getBlockSize();       // accessor function

private:
	string diskname;          // file name of software-disk
	int numberOfBlocks;       // number of blocks on the disk
	int blockSize;            // block size in bytes
};

Sdisk::Sdisk(string diskname,int numberOfBlocks, int blockSize){

	this -> diskname = diskname;
	this -> numberOfBlocks = numberOfBlocks;
	this -> blockSize = blockSize;

	ifstream iofile;

	iofile.open(diskname.c_str(), ios::in);

	if (iofile.good()){
		iofile.close();
		return;
	}

	else {
		iofile.close();

		ofstream outfile;
		outfile.open(diskname.c_str(), ios::out);

		for (int i = 0; i < numberOfBlocks * blockSize; i++){
			outfile.put('#');
		}

		outfile.close();
		return;
	}
}


// int Sdisk::getBlock(int blockNumber, string& buffer){
//
// }
//
//
// int Sdisk::putBlock(int blockNumber, string buffer){
//
// }


int Sdisk::getNumberOfBlocks(){
	return numberOfBlocks;
}


int Sdisk::getBlockSize(){
	return blockSize;
}


int main(){
	Sdisk filedisk("newDisk",5,10);

	return 0;
}
