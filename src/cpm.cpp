#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <list>
#include <iterator>

using namespace std;

///////////////////////////////////////////////////////§

bool contains(list<string> list, string element);

void read_file(string file_name);

string read_char(string file_name, int k);

void print_words_read_list(list<string> words_read);

void print_unordered_map();

///////////////////////////////////////////////////////

// Global Variables
static unordered_map<string, list<int> > un_map;

static list<string> k_word_read_vector;

///////////////////////////////////////////////////////§

int main(int argc, char** argv) {


    string string_example = "o texto o texto e o texto";

    int k = 5;

    for (int i = 0; i < string_example.length() - k; i++) {
        string k_string = string_example.substr(i, k);

        if (contains(k_word_read_vector, k_string)) {
            string word = un_map.find(k_string)->first; // dá a key do mapa
            list<int> index = un_map.find(k_string)->second; // ir buscar os values do map (lista de indices)

            int next_index = index.back() + 1;
            
            int index2 = 0;
            for (auto it = k_word_read_vector.begin(); it != k_word_read_vector.end(); ++it) {
                if (index2 == next_index) {
                    cout << "the word is: " << k_string << " the next is: " <<*it << endl;
                }
                index2++;
            }
        }
        un_map[k_string].push_back(i);
        k_word_read_vector.push_back(k_string);
    }

    //print this just for debugging
    //print_words_read_list(k_word_read_vector);
    //print_unordered_map();

    return 0;
}

void print_words_read_list(list<string> words_read_list) {
    cout << "words_read" << endl;
    int index = 0;
    for (auto it = words_read_list.begin(); it != words_read_list.end(); ++it) {
        cout << index << " -> " << *it << '\n';
        index++;
    }
}

void print_unordered_map() {
    cout << "unorderer_map" << endl;
    for (auto it = un_map.begin(); it != un_map.end(); it++) {
        cout << it->first << " => ";
        for (int i : it->second) {
            cout << i << " ";
        }
        cout << endl;
    }
}

 bool contains(list<string> list, string element) {
     return find(list.begin(), list.end(), element) != list.end();
}

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

// Function to read and return "k" chars from the file every time it is called
string read_char(string file_name, int k) {

    static ifstream file;
    static string c;
    if (!file.is_open()) {
        file.open(file_name);
    }

    // read k chars from the file
    char buffer[k];
    file.read(buffer, k);
    c= string(buffer, k);

    // check if the file has less than k chars
    if (file.gcount() < k) {
        c = c.substr(0, file.gcount());
    }

    //if it is the end of the file, close the file
    if (file.eof()) {
        file.close();
    }

    return c;
}