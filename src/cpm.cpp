#include <iostream>
#include "hashTable.cpp"

#include <fstream>
#include <unordered_map>
#include <string>



using namespace std;

//////// compile and run /////////
//                              //
//  g++ -o bin/cpm src/cpm.cpp  //
//  ./bin/cpm                   //
//                              //
//////////////////////////////////


void read_file(string file_name) {
    ifstream file;
    file.open(file_name);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Unable to open file";
    }
}


int main(int argc, char** argv) {

    /*Read File*/

    string file_name = "../example/chry.txt";
    //read_file(file_name);

    unordered_map<string, int> un_map;

    un_map.insert(make_pair("o tex", 1));
    un_map.insert(make_pair(" text", 2));   

    for(auto& p: un_map)
        std::cout << " " << p.first << " => " << p.second << '\n';


    return 0;
}