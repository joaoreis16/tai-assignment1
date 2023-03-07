#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <list>
#include <iterator>
#include <algorithm>

using namespace std;

///////////////////////////////////////////////////////

bool contains(list<string> list, string element);

void read_file(string file_name);

string read_char(string file_name, int k);

void print_words_read_list(list<string> words_read);

void print_unordered_map();

/////////////// global variables //////////////////////

static unordered_map<string, list<int> > un_map;
static list<string> k_word_read_vector;
static int alpha = 1;

///////////////////////////////////////////////////////

float calculate_probability(string word, string next_word) {
    int count, next_count = 0;;

    for (auto it = un_map.begin(); it != un_map.end(); it++) {
        if (it->first == word)      count++;
        if (it->first == next_word) next_count++;
    }

    return (float) (next_count + alpha)/(count + 2 * alpha);
}

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("[usage]: %s file_path", argv[0]);
    }
    
    int i = 0;
    int k = 5;
    string word;
    // string string_example = "o texto o texto e o texto";

    do {
        word = read_char(argv[1], k);
        cout << "content => " << word << endl;

        if (word == "") break;

        if (contains(k_word_read_vector, word)) {
            string k_string = un_map.find(word)->first;         // dá a key do mapa
            list<int> index = un_map.find(word)->second;        // ir buscar os values do map (lista de indices)

            int next_index = index.back() + 1;
            
            float prob_hit;
            int index2 = 0;
            for (auto it = k_word_read_vector.begin(); it != k_word_read_vector.end(); ++it) {
                if (index2 == next_index) {
                    cout << "the word is: '" << word << "' the next is: '" << *it << "'" << endl;
                    prob_hit = calculate_probability(word, *it);
                }
                index2++;
            }
            cout << "P(hit) = " << prob_hit << "\n" << endl;
        }

        un_map[word].push_back(i);
        k_word_read_vector.push_back(word);
        i++;

    } while (true);

    //print this just for debugging
    print_words_read_list(k_word_read_vector);
    print_unordered_map();

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

string read_file(char *file_name) {
    ifstream file;
    file.open(file_name);

    string str;
    string content;
    while (getline(file, str)) {
        content += str;
    } 

    return content;
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
    c = string(buffer, k);

    // check if the file has less than k chars
    if (file.gcount() < k) {
        c = c.substr(0, file.gcount());
    }

    //if it is the end of the file, close the file
    if (file.eof()) {
        file.close();
        return "";
    }

    return c;
}