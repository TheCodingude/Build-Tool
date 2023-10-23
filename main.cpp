#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

map<string, string> variables;
 
string CHECK_OS(){
    #ifdef _WIN32
        return "windows";
    #elif __linux__
        return "linux";
    #elif __APPLE__
        return "macintosh";
    #endif
}

void build_error(string error){
    cerr << "BUILD ERROR: " << error << endl;
    exit(1);
}

void build_warning(string warning){
    cout << "BUILD WARNING: " << warning << endl;
} 

void build_info(string info){
    // cout << "----------------" << endl;
    cout << "[BUILD INFO]: " << info << endl;
    // cout << "----------------" << endl;
}

bool containsOnlyBlanks(const std::string& line) { return std::all_of(line.begin(), line.end(), [](char c) { return c == ' ' || c == '\t' || c == '\n'; }); }

inline bool ends_with(std::string const & value, std::string const & ending){
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}


void execute_command(const char* command){
    if (!containsOnlyBlanks(command)){
        string c = command;
        build_info("Running " + c);
    }
    if(system(command) != 0){
        cerr << "Unable to run command: " << command << endl; 
    } 
}

string removeWhitespace(string input) {
    input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());
    return input;
}



vector<string> get_keys(){
    std::vector<string> keys;
    for(map<string,string>::iterator it = variables.begin(); it != variables.end(); ++it) {
        keys.push_back(removeWhitespace(it->first));
    }
    return keys;
}

vector<string> split(string input, char delimiter = ' ') {
    
    stringstream ss(input);

    vector<string> tokens;

    string token; 

    while (getline(ss, token, delimiter)) {
        tokens.push_back(removeWhitespace(token));
    }

    return tokens;
}

string removeNewlines(string line) {
    int size = sizeof(line);
    char newstr[size];
    int index = 0;

    while(index <= size){
        if (line[index] == '\n' || line[index] == '\r'){
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
    name = line.substr(0, loe-1);
    int first = line.find('"') + 1;
    int second = line.find('"', first);
    data = line.substr(first, second-first);
    variables[name] = data;
}

void check_if_var(string line){
    string newline = "";
    bool found = false;
    bool already_found = false;
    vector<string> keys = get_keys();
    vector<string> tokens = split(line);
    for (string token : tokens) {
        already_found = false;
        for(string key : keys){
            
            if(key == token){
                already_found = true;
                newline = newline + ' ' + variables[token];
                found = true;
                continue;
            }else{
                found = false;
            }
        }
        if(!found && !already_found){
            newline = newline + ' ' + token;
        }
    }
    
    
    execute_command(newline.c_str());


}


string check_for_flags(string line){
    int exists = line.find("-");
    if (exists < sizeof(line) && exists >= 0){
        return line.substr(exists+1, sizeof(line) + 1);
    } else{
        return "nah fam";
    }
}

void read_file(const char* filename = "BUILD", bool look = false){
    // fopen is better idc

    string flag = "nah fam";
    FILE *file;

    if (look){
        flag = check_for_flags(filename);
        if (flag == "nah fam"){
            file = fopen(filename, "r");
        } else{
            file = fopen("BUILD", "r");
        }
    } else {
        file = fopen("BUILD", "r");
    }

    flag += ':';

    if (file == NULL){
        cerr << "File " << filename << " does not exist or there was another issue\n";
        exit(1);
    }


    string os = CHECK_OS();
    string os_tag = "none";

    char buffer[1024]; // A buffer to hold the read data

    bool in_section = false;
    


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

        if (containsOnlyBlanks(line)){
            continue;
        }

        
        int flag_len = flag.size();

        string flig = line.substr(0, flag_len);
        if(flag != "nah fam:"){
            // cout << "F: " << flag << endl;
            if(flig == flag && in_section == false){
                in_section = true;
                continue;
            }
            if(!in_section){
                continue;
            }
        }else if(ends_with(line, ":\n")){       // proably not the best way to do this but if it aint broke dont fix it
            exit(0);
        }

        


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
                
                check_if_var(buffer);
            }
            else{
                continue;
            }
            
        }
        else{
            check_if_var(buffer);
        }
        cout << '\n';   

    }

    // Close the file when done
    fclose(file);


    // chatgpt is our friend :)
}


int main(int argc, char** argv){
    if (argc == 1){
        read_file("BUILD", false);
    }else{
        read_file(argv[1], true);
    }
    return 0;
}