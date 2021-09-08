// Name: Azael Consolo
// CSE 4610

#include<fstream>;

using namespace std;

Class Sdisk {
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

	fstring iofile;

	iofile.open(diskname.c_str(), ios::in);

	if (iofile.good()){
		iofile.close();
		return;
	}

	else {
		iofile.close();
		
		ofstream outfile;
		outfile.open(diskname.c_str());

		for (int i = 0; i < numberOfBlocks * blockSize; i++){
		
		}
	}
}
