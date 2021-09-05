#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iterator>

using namespace std;

class passwordFile {
public:
    passwordFile(string newFilename); // opens the file and reads the names /  passwords in the vectors user and password.
    void addPw(string newUser, string newPassword); // this adds a new user / password to the vectors and wrties the vectors to the file filename
    void delUser(string userToDelete); // this deletes a user that is currently saved in a file
    bool checkPw(string user, string password); // return true if user exists and password matches.

private:
    string filename; // the file that contains password information
    vector<string> user; // the list of usernames
    vector<string> password; // the list of passwords
    void synch(); // writes the user / password vectors to the password file
};


passwordFile::passwordFile(string newFilename){
    filename = newFilename;
    string u, p;

    ifstream infile;
    infile.open(filename.c_str());

    infile >> u >> p;

    while (infile.good()){
        user.push_back(u);
        password.push_back(p);
        infile >> u >> p;
    }

    infile.close();
}


void passwordFile::synch(){

    ofstream outstream;
    outstream.open(filename.c_str());

    for (int i = 0; i < user.size(); i++){

        outstream << user[i] << " " << password[i] << endl;
    }

    outstream.close();
}


void passwordFile::addPw(string newUser, string newPassword){

    user.push_back(newUser);
    password.push_back(newPassword);

    synch();

}

void passwordFile::delUser(string userToDelete){

    vector<string>:: iterator it1, it2;

    it2 = password.begin();

    for (it1 = user.begin(); it1 != user.end(); it1++){

        if (*it1 == userToDelete){

            it1->erase();
            it2->erase();
            synch();
            return;
        }

        it2++;
    }
}
int main(){
    passwordFile passFile("password.txt");
    passFile.delUser("azael");

    return 0;

}
