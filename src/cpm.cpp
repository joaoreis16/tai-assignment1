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

    unordered_map<string, list<int> > un_map;

    string file_name = "o texto o texto e o texto";
    //read_file(file_name);

    int k = 5;

    for (int i = 0; i < file_name.length() - k; i++) {
        string k_gram = file_name.substr(i, k);

        list<int> pos;
        if (un_map.find(k_gram) == un_map.end()){
            pos.push_back(i);
            un_map.insert(make_pair(k_gram, pos));
        } else {
            list<int> pos = un_map[k_gram];
            pos.push_back(i);
            un_map.insert(make_pair(k_gram, pos));
        }


        un_map.insert(make_pair(k_gram, i));
    }

    for (auto it = un_map.begin(); it != un_map.end(); it++) {
        cout << it->first << " => ";
        for (int i : it->second) {
            cout << i << " ";
        }
        cout << endl;
    }



    return 0;
}