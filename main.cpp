#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


void execute_command(char* command){
    if(system(command) != 0){
        cerr << "Unable to run command: " << command << endl; 
    } 
}

void read_file(){
    FILE *file = fopen("BUILD", "r");

    if (file == NULL){
        cerr << "File 'BUILD' does not exist or there was another issue\n";
        exit(1);
    }

    char buffer[1024]; // A buffer to hold the read data

    while (fgets(buffer, sizeof(buffer), file) != nullptr) {

        // cout << buffer << endl;
        execute_command(buffer);
    }
    cout << buffer;
        // Close the file when done
    fclose(file);

    // chatgpt is our friend :)
}



int main(){
    read_file();
}