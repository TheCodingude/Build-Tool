#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

map<string, string> variables;
vector<string> keys; 


void build_error(string error){
    cout << "BUILD ERROR: " << error << endl;
    exit(1);
}

void build_warning(string warning){
    cout << "BUILD WARNING: " << warning << endl;
} 

void execute_command(char* command){
    if(system(command) != 0){
        cerr << "Unable to run command: " << command << endl; 
    } 
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

vector<string> split(string input, char delimiter = ' ') {
    
    stringstream ss(input);

    vector<string> tokens;

    string token; 

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}





string removeNewlines(string line) {
    int size = sizeof(line);
    char newstr[size];
    int index = 0;

    while(index <= size){
        if (line[index] == '\n' || line[index] == '\r\n' || line[index] == ' '){
            index++;
        }
        else{
            newstr[index] = line[index];
            index++;
        }
        
    }

    return newstr;

}


void find_var(string line, int loe){ // loe = location of equals
    string name;
    string data;
    name = removeNewlines(line.substr(0, loe));
    int first = line.find('"') + 1;
    int second = line.find('"', first);
    data = line.substr(first, second-first);
    variables[name] = data;
    keys.push_back(name);
}

bool check_if_var(string line){
    string newline = "";
    bool found = false;
    bool already_found = false;
    vector<string> tokens = split(line);
    for (const std::string& token : tokens) {
        for(string i : keys){
            if(i == token){
                already_found = true;
                newline = newline + ' ' + variables[token];
                found = true;
                continue;
            }else{
                found = false;
            }
        }
        if(!found && already_found == false){
            newline = newline + ' ' + token;
        }
    }
    cout << newline << endl;


}

void read_file(){
    // fopen is better idc
    FILE *file = fopen("BUILD", "r");

    if (file == NULL){
        cerr << "File 'BUILD' does not exist or there was another issue\n";
        exit(1);
    }

    string os = CHECK_OS();
    string os_tag = "none";

    char buffer[1024]; // A buffer to hold the read data



    while (fgets(buffer, sizeof(buffer), file) != nullptr) {


        string line = removeNewlines(buffer);
        
        if(line.substr(0, 2) == "//"){
            continue;   
        }

        string winline = line.substr(0, 9);
        string linuxline = line.substr(0, 7);
        string macline = line.substr(0, 5);
        line = (string) buffer;
        int equal = line.find("=");

        if (winline == "[WINDOWS]"){ // CHECK IF CURRENTLY IN OS SECTION AND WHICH OS IT IS
            os_tag = "windows";
        }
        else if(linuxline == "[LINUX]"){
            os_tag = "linux";
        }
        else if(macline == "[MAC]"){
            os_tag = "macintosh";
        }   
        else if(macline == "[END]"){ // this was is also mac line just because they're the same length lol
            os_tag = "none";
        }
        else if(equal <= sizeof(line) && equal >= 0){
            find_var(line, equal);
        }
        else if (os_tag != "none"){
            if(os_tag == os){
                
                execute_command(buffer);
            }
            else{
                continue;
            }
            
        }
        else{
            check_if_var(buffer);
        }

    }

    // Close the file when done
    fclose(file);

    // chatgpt is our friend :)
}



int main(int argc, char** argv){

    (void) argc;
    (void) argv;
    read_file();
    return 0;
}