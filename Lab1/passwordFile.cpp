// Name: Azael Consolo
// CSE 4610
// Lab 1

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iterator>

using namespace std;

class PasswordFile {
public:
    PasswordFile(string newFilename); // opens the file and reads the names /  passwords in the vectors user and password.
    void addPw(string newUser, string newPassword); // this adds a new user / password to the vectors and wrties the vectors to the file filename
    void delUser(string userToDelete); // this deletes a user that is currently saved in a file
    bool checkPw(string checkUser, string checkPassword); // return true if user exists and password matches.
    static void newSalt(int ns);

private:
    static int salt;
    string filename; // the file that contains password information
    string encrypt(string s);
    string decrypt(string s);
    vector<string> user; // the list of usernames
    vector<string> password; // the list of passwords
    void synch(); // writes the user / password vectors to the password file
};


PasswordFile::PasswordFile(string newFilename){
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


int PasswordFile::salt=1;

void PasswordFile::newSalt(int ns){
    salt = ns;
}

string PasswordFile::encrypt(string s){
    for (int i = 0; i < s.size(); i++){
        s[i] += salt;
    }

    return s;
}

string PasswordFile::decrypt(string s){
    for (int i = 0; i < s.size(); i++){
        s[i] -= salt;
    }

    return s;
}


void PasswordFile::synch(){
    ofstream outstream;
    outstream.open(filename.c_str());

    for (int i = 0; i < user.size(); i++){
        outstream << user[i] << " " << password[i] << endl;
    }

    outstream.close();
}


void PasswordFile::addPw(string newUser, string newPassword){
    string encryptedUser = encrypt(newUser);
    string encryptedPass = encrypt(newPassword);
    vector<string> :: iterator it;

    for (it = user.begin(); it != user.end(); it++){
        if (*it == encryptedUser){
            cout << "Username already exists!" << endl;
            return;
        }
    }

    user.push_back(encryptedUser);
    password.push_back(encryptedPass);

    synch();

    cout << "Username: " << newUser << " added successfully!" << endl;
    cout << "Encrypted Username: " << encryptedUser << " Encrypted Password: " << encryptedPass << endl << endl;

}

void PasswordFile::delUser(string userToDelete){
    string encryptedUser = encrypt(userToDelete);
    vector<string>:: iterator it1, it2;

    it2 = password.begin();

    for (it1 = user.begin(); it1 != user.end(); it1++){
        if (*it1 == encryptedUser){

            it1->erase();
            it2->erase();
            synch();

            cout << userToDelete << " deleted successfully" << endl;

            return;
        }

        it2++;
    }
}

bool PasswordFile::checkPw(string checkUser, string checkPassword){
    string encryptedUser = encrypt(checkUser);
    string encryptedPass = encrypt(checkPassword);

    for (int i = 0; i < password.size(); i++){

        int compareUser = user[i].compare(encryptedUser);
        int comparePassword = password[i].compare(encryptedPass);

        if (compareUser == 0 && comparePassword == 0){

            cout << "username: " << checkUser << " and password match!" << endl;

            return true;
        }
    }

    cout << "username: " << checkUser << " and password do not match :-(" << endl;

    return false;
}


int main(){

    PasswordFile passFile("password.txt");
    string username;
    string password;
    bool isTrue = true;

    while (isTrue){
        cout << "Enter a username to add (Enter 0 to exit): ";
        cin >> username;

        if (username == "0"){
            break;
        }

        cout << "Enter a password: ";
        cin >> password;

        passFile.addPw(username, password);
    }

    cout << endl;

    while (isTrue){
        cout << "Enter a username to delete (Enter 0 to exit): ";
        cin >> username;

        if (username == "0"){
            break;
        }

        passFile.delUser(username);
    }

    cout << endl;

    while (isTrue){
        cout << "Enter a username to check (Enter 0 to exit): ";
        cin >> username;

        if (username == "0"){
            break;
        }

        cout << "Enter the password for this username (Enter 0 to exit): ";
        cin >> password;

        passFile.checkPw(username, password);
    }

    cout << endl;

    return 0;

}
