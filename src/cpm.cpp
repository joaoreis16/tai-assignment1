#include <iostream>

#include <fstream>
#include <unordered_map>
#include <string>
#include <list>
#include <iterator>




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

bool contains(list<string> list, string element) {
    return find(list.begin(), list.end(), element) != list.end();

}


int main(int argc, char** argv) {

    unordered_map<string, list<int> > un_map;

    string string_example = "o texto o texto e o texto";
    list<string> k_word_read_vector;
    // read_file(file_name);

    int k = 5;

    // populate the map with substrings of size k, save the index of the k_word in the list
    for (int i = 0; i < string_example.length() - k; i++) {
        string k_string = string_example.substr(i, k);


        if (contains(k_word_read_vector, k_string)) {
            un_map[k_string].push_back(i);
            cout << "a palavra já apareceu, siga ver qual será a proxima letra" << " --  " << i << "  --  " << endl;

            // ent agora temos de ir ao nosso hash e ir procurar pela palavra que ja apareceu para sabermos o indice que foi pra depois
            // ir buscar o indice da proxima palavra pra saber qual é a mais provavel de aparecer, mas é pra ir buscar à lista (neste caso k_word_read_vector)

            string word = un_map.find(k_string)->first;
            list<int> index = un_map.find(k_string)->second; // estou só a ir buscar o ultimo index
            cout << "a palavra é: " << word << " e o indice é " << index.back() << endl;


            //ir a lista k_word_read_vector e ver qual é a proxima palavra mais provavel de aparecer (index + 1)
            
            auto it = k_word_read_vector.begin();
            int next_index = index.back() + 1;
            cout << "aquiiiiii " << next_index << endl;
            advance(it, next_index);
            cout << "a seguir a --> " << word << " <-- a palavra mais provavel de aparecer é --> " << *it << " <-- " << endl;
        }

        un_map[k_string].push_back(i);
        k_word_read_vector.push_back(k_string);
    }

    cout << "\n\n\n" << endl;

    int index = 0;
    for (auto it = k_word_read_vector.begin(); it != k_word_read_vector.end(); ++it) {
        cout << index << " -> " << *it << '\n';
        index++;
    }
    

    
    cout << "\n\n\n" << endl;

    // print the map
    for (auto it = un_map.begin(); it != un_map.end(); it++) {
        cout << it->first << " => ";
        for (int i : it->second) {
            cout << i << " ";
        }
        cout << endl;
    }



    return 0;
}