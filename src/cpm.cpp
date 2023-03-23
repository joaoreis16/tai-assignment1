#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <list>
#include <iterator>
#include <algorithm>
#include <cmath>

using namespace std;

///////////////////////////////////////////////////////

bool contains(list<string> list, string element);

void read_file(string file_name);

string read_char(string file_name, int k);

void bring_back(string file_name, int k);

void print_words_read_list(list<string> words_read);

void print_unordered_map();

void predict(string word, char** argv);

/////////////// global variables //////////////////////

static unordered_map<string, list<int> > un_map;
static list<string> k_word_read_vector;
static int alpha = 1;
static int K = 7;

static int N_hits  = 0;
static int N_fails = 0;
static float threshold = 7;
float bits = 0;
char predicted_next_char;

float highest_prob = 0;
float lowest_bits = 0;
int lowest_index = 0;
int actual_index = 0;
bool end_of_file = false;


///////////////////////////////////////////////////////

float calculate_probability(string word, char next_char) {

    return (float) (N_hits + alpha)/(N_hits + N_fails + 2 * alpha);
}

float calculate_bits(float prob) {
    return (float) -log2(prob);
}

int main(int argc, char** argv) {

    if (argc < 2) printf("[usage]: %s file_path", argv[0]);
    
    actual_index = 0;
    string word;


    do {
        word = read_char(argv[1], K);
        if (word == "") break;
        cout << "content => " << word << endl;
        cout << "actual_index => " << actual_index << endl;


        if (contains(k_word_read_vector, word)) {
            predict(word, argv);
        }
        else{

            
            // the map mantains only 5 references to the words, so if we have more than 5 words, we remove the oldest one
            if (un_map[word].size() > 1) {
                un_map[word].pop_front();
            }
            // add the word to the list
            k_word_read_vector.push_back(word);
            // add the word to the map
            un_map[word].push_back(actual_index);
            //add max bits to write
            bits += log2(4); // 4 is the number of possible chars
        }

        actual_index++;

    } while ( !end_of_file);
    print_unordered_map();
    cout << "bits => " << bits << endl;


    // print this just for debugging
    // print_words_read_list(k_word_read_vector);
    

    return 0;
}

void predict(string word, char** argv) {
    if (contains(k_word_read_vector, word)) {
        string k_string = un_map.find(word)->first;         // dá a key do mapa
        list<int> index = un_map.find(word)->second;        // ir buscar os values do map (lista de indices)
        if (un_map[word].size() > 1) {
            un_map[word].pop_front();
        }

        // add the word to the list
        k_word_read_vector.push_back(word);
        // add the word to the map
        un_map[word].push_back(actual_index);
        float prob_hit;
        float bits_to_write=0;
        int new_index = 0;
        highest_prob = 0;
        lowest_bits = 0;

        
        // para cada elemento da lista de indices (index) vamos calcular a probabilidade de acerto e a quantidade de bits e escolher o que tem maior probabilidade
        for (int i : index) {
            //bring back the pointer to the file
            if ((new_index-actual_index)>0){
                bring_back(argv[1], new_index-actual_index);
            }
            //cout << "i => " << i << endl;
            //cout << "k_string => " << k_string << endl;
            //cout << "highest_prob => " << highest_prob << endl;
            //cout << "lowest_bits => " << lowest_bits << endl;
            new_index = actual_index;
            N_fails = 0;
            N_hits = 0;
            bits_to_write = 0;
            //calculr razao entre o numero de acertos e o numero de erros se for nan ou inf entao é 0
            float ratio = (float) N_fails;
            //if (isinf(ratio) || isnan(ratio)) ratio = 0; // no caso de usar um ratio entre hits e fails
            

            while ( ratio < threshold )
            {

                word = read_char(argv[1], K);
                //cout << "word => " << word << endl;
                new_index++;
                if (word == "") break;
                // calculate the probability of hit
                prob_hit = calculate_probability(k_string, predicted_next_char);
                //cout << "prob_hit => " << prob_hit << endl;
                // calculate the number of bits
                bits_to_write += calculate_bits(prob_hit);
                //cout << "bits_to_write => " << bits_to_write << endl;
                // get thenext char
                /*
                for (auto it = k_word_read_vector.begin(); it != k_word_read_vector.end(); ++it) {
                    if (*it == k_string) {
                        //save predicted next char
                        string next_word = *next(it, 1);
                        predicted_next_char = next_word[next_word.size() - 1];
                        k_string = next_word;
                        break;
                    }
                }
                */
                list<string>::iterator it= k_word_read_vector.begin();
                advance(it, i);
                i++;
                string next_word = *next(it, 1);
                predicted_next_char = next_word[next_word.size() - 1];
                k_string = next_word;
                
                
                //cout << "predicted_next_char => " << predicted_next_char << endl;
                //cout << "real_next_char => " << word[word.size() - 1] << endl;
                // if the next char is the same as the predicted one, then we have a hit
                if (predicted_next_char == word[word.size() - 1]) {
                    N_hits++;
                }
                else {
                    N_fails++;
                }
                //cout << "N_hits => " << N_hits << endl;
                //cout << "N_fails => " << N_fails << endl;
                
                ratio = (float) N_fails;
                //if (isinf(ratio) || isnan(ratio)) ratio = 0; // no caso de usar um ratio entre hits e fails
                // the map mantains only 5 references to the words, so if we have more than 5 words, we remove the oldest one
                if (un_map[word].size() > 1) {
                    un_map[word].pop_front();
                }

                // add the word to the list
                k_word_read_vector.push_back(word);
                // add the word to the map
                un_map[word].push_back(new_index);

                
            }

            // Save the probability of hit, the number of bits and the index of char where we stoped
            // if the probability of hit is higher than the previous one, then we save the new one
            // if the probability of hit is the same as the previous one, then we save the one with the lowest number of bits
            // if the probability of hit is the same as the previous one and the number of bits is the same as the previous one, then we save the one with the lowest index
            if (prob_hit > highest_prob) {
                highest_prob = prob_hit;
                lowest_bits = bits_to_write;
                lowest_index = new_index;
            }
            else if (prob_hit == highest_prob) {
                if (bits_to_write < lowest_bits) {
                    lowest_bits = bits_to_write;
                    lowest_index = new_index;
                }
                else if (bits_to_write == lowest_bits) {
                    if (new_index < lowest_index) {
                        lowest_index = new_index;

                    }
                }
            }
            //cout << "prob_hit => " << prob_hit << endl;
            //cout << "bits => " << bits << endl;
            //cout << "N_hits => " << N_hits << endl;
            //cout << "N_fails => " << N_fails << endl;
            //cout << "highest_prob => " << highest_prob << endl;
            //cout << "lowest_bits => " << lowest_bits << endl;
            
        }
        //cout << "lowest_index => " << lowest_index << endl;
        //cout << "lowest_bits => " << lowest_bits << endl;
        //cout << "new_index => " << new_index << endl;

        // bring back the pointer to the highest index
        if ((lowest_index - new_index) < 0) {
            bring_back(argv[1], new_index - lowest_index);
        }else{
            //read the next char till we reach the lowest index
            while (lowest_index != new_index) {
                word = read_char(argv[1], K);
                new_index++;
            }
        }
        // print the next char
        //cout << "next final => " << word << endl;
        // if the next char is empty, then we have reached the end of the file
        if (word == "") {
            cout << "EOF" << endl;
            end_of_file = true;
        }
        actual_index = lowest_index;
        bits += lowest_bits;
        cout << "bits => " << bits << endl;


    }
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
    if (file.eof()) {
        end_of_file = true;
        return "";
    }
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