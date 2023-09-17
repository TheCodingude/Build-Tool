#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


void find_file(){
    char line[50];
    FILE *file = fopen("BUILD", "r");

    if (file == NULL){
        cerr << "File 'BUILD' does not exist or there was another issue\n";
        exit(1);
    }

    

}



int main(){
    find_file();
}