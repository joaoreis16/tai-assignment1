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

void bring_back(string file_name, int k);

void print_words_read_list(list<string> words_read);

void print_unordered_map();

/////////////// global variables //////////////////////

static unordered_map<string, list<int> > un_map;
static list<string> k_word_read_vector;
static int alpha = 1;
static int K = 5;

static int N_hits  = 0;
static int N_fails = 0;

///////////////////////////////////////////////////////

float calculate_probability(string word, char next_char) {
    return (float) (N_hits + alpha)/(N_hits + N_fails + 2 * alpha);
}

int main(int argc, char** argv) {

    if (argc < 2) printf("[usage]: %s file_path", argv[0]);
    
    int i = 0;
    string word;
    char predicted_next_char;

    do {
        word = read_char(argv[1], K);
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

                    char actual_char = word.back();

                    if (predicted_next_char != NULL) {
                        if (predicted_next_char == actual_char) N_hits++;
                        else N_fails++;
                    }

                    string next_word = *it;
                    predicted_next_char = next_word.back();

                    cout << "the word is: '" << word << "' the next is: '" << next_word << "'" << endl;
                    cout << "I predict : " << predicted_next_char << endl;

                    prob_hit = calculate_probability(word, predicted_next_char);
                }
                index2++;
            }
            cout << "P(hit) = " << prob_hit << "\n" << endl;
        }

        un_map[word].push_back(i);
        k_word_read_vector.push_back(word);
        i++;

    } while (true);

    // print this just for debugging
    // print_words_read_list(k_word_read_vector);
    // print_unordered_map();

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

// Function to read and return "k" chars from the file every time it is called
string read_char(string file_name, int k) {

    static ifstream file;
    static string c;
    if (!file.is_open()) {
        file.open(file_name);
        char buffer[k];
        file.read(buffer, k);
        c = string(buffer, k);

    }
    //else bring the pointer k-1 positions back and read k chars
    else {
        // bring the pointer k-1 positions back
        file.seekg(-k+1, ios::cur);
        char buffer[k];
        file.read(buffer, k);
        c = string(buffer, k);

    }
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

// Function to bring the pointer k positions back
void bring_back(string file_name, int k) {
    static ifstream file;
    if (!file.is_open()) {
        file.open(file_name);
    }
    file.seekg(-k, ios::cur);
}