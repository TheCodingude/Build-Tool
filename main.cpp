#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string.h>

using namespace std;


void execute_command(char* command){
    if(system(command) != 0){
        cerr << "Unable to run command: " << command << endl; 
    } 
}

string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}


string CHECK_OS(){
    #ifdef _WIN32
        return "windows";
    #elif __linux__
        return "linux";
    #elif __APPLE__
        return "macintosh";
    #endif
}

string removeNewlines(char* line) {
    char newstr[999];
    int index = 0;
    int size = sizeof(line) - 1;
    int smth = 0;

    while(index < size){
        if (line[index] == '\n' || line[index] == '\r'){
            cout << "IN" << endl;
            index++;
        }
        else{
            newstr[index] = line[index];
        }
        index++;
    }

    string line_a = convertToString(newstr, sizeof(newstr));

    return line_a;

}



void read_file(){
    // fopen is better idc
    FILE *file = fopen("BUILD", "r");

    if (file == NULL){
        cerr << "File 'BUILD' does not exist or there was another issue\n";
        exit(1);
    }

    string os = CHECK_OS();
    // bool in_os_only = false;
    string os_tag = "none";

    char buffer[1024]; // A buffer to hold the read data

    

    while (fgets(buffer, sizeof(buffer), file) != nullptr) {


        string line = removeNewlines(buffer);
        
        

        if (line == "[WINDOWS]"){ // CHECK IF CURRENTLY IN OS SECTION AND WHICH OS IT IS
            cout << "PLEASE CUM IN HERE" << endl;
            os_tag = "windows";
        }
        else if(line == "[LINUX]"){
            os_tag = "linux";
        }
        else if(line == "[MAC]"){
            os_tag = "macintosh";
        }
        else if(line == "[END]"){
            os_tag = "none";
        }
        else if (os_tag != "none"){
            if(os_tag == os){
                execute_command(buffer);
            }
        }
        else{
            execute_command(buffer);
        }




        
    }

    // Close the file when done
    fclose(file);

    // chatgpt is our friend :)
}



int main(){
    read_file();
    return 0;
}