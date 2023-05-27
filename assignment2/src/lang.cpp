#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include <map>

#include "cpm.hpp"


using namespace std;
//////////////////////////////////////////////////////////////


// Function to calculate estimate bits for the target file using the un_map model and the k_word_read_vector
float estimate_bits(string target_file_name, unordered_map<string, list<int> > model, vector<string> vector_model);

// Function to read a word from the target file
string read_word (int k);

// Function to bring the pointer k positions back
void bring_back_target(int k);

// Function to bring the pointer k positions to the front
void bring_front_target(int k);

float get_estimated_bits(string target_file_name, string ri_filename, int k, float t, float a);

void reset();


/////////////////////global variables/////////////////////////

// Language model
static unordered_map<string, list<int> > model;
// K_word_read_vector
static vector<string> vector_model;
static string ri_filename;

static string text_filename;
static ifstream target_file;

static int K = 4;
static int threshold = 4;
static float alpha = 1;
// end of file flag for target file
static bool end_of_file_target = false;
static float bits = 0;

static int n_Fails = 0;
static int n_Hits = 0;
static char next_char;
static char predicted_next_char;

static int N_different_symbols = 0;

// map to save symbols position and the bits for each symbol
static map<int, pair<char, float> > char_average_bits;
//ordered map to save symbols position and the symbol
static map<int, char > char_map;


//////////////////////////////////////////////////////////////
float calculate_probability_lang(string word, char next_char)
{
    //return (float)(n_Hits + alpha) / (n_Hits + n_Fails + 2 * alpha);
    float prob = (float)(n_Hits + alpha) / (n_Hits + n_Fails + 2 * alpha);
    //cout << "prob: " << prob << endl;
    return prob;
}

float calculate_bits_lang(float prob)
{
    float bits = (float)-log2(prob);
    //cout << "bits: " << bits << endl;
    return (float)-log2(prob);
}
/*
 int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Usage: ./lang <ri_file> <analysis_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> )" << endl;
        return 1;
    }

    ri_filename   = argv[1];
    text_filename = argv[2];

    // to check if the file exist
    ifstream file_exist(ri_filename);
    if (!file_exist.good()) {
        // File does not exist
        cout << "[ERROR]: file '" << ri_filename << "' does not exist." << endl;
        exit(0);
    }

    // to check if the file exist
    ifstream file_exist1(text_filename);
    if (!file_exist1.good()) {
        // File does not exist
        cout << "[ERROR]: file '" << text_filename << "' does not exist." << endl;
        exit(0);
    }

    //read the parameters
    int opt;
    while ((opt = getopt(argc, argv, "a:t:k:")) != -1) {
        switch (opt) {
            case 'a':
                alpha = atof(optarg);
                break;
            case 't':
                threshold = atof(optarg); // changed to atof()
                break;
            case 'k':
                K = atoi(optarg);
                break;
            default:
                std::cerr << "Usage: ./lang <ri_file> <analysis_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> )" << std::endl;
                return 1;
        }
    }

    float bits = get_estimated_bits(text_filename, ri_filename, K, threshold, alpha);
    cout << "[lang.cpp]: estimated bits = " << bits << endl;
    return 0;
}  */

float get_estimated_bits(string target_filename, string filename, int k, float t, float a) {

    // reset structures
    reset();

    text_filename = target_filename;
    ri_filename = filename;
    alpha = a; K = k; threshold = t; 

    // apply the copy model
    float num_bits = apply_cpm(ri_filename, K, threshold, alpha);
    cout << "[lang.cpp]: total bits = " << num_bits << endl;

    // get un_map from cpm.cpp
    model = get_un_map();

    // get k_word_read_vector from cpm.cpp
    vector_model = get_k_word_read_vector();

    // get different symbols from cpm.cpp
    N_different_symbols = get_N_different_symbols();
    cout << "[lang.cpp]: N_different_symbols = " << N_different_symbols << endl;


    // estimate bits for the target file
    float bits = estimate_bits(target_filename, model, vector_model);

    return bits;
}

// Function to calculate estimate bits for the target file using the un_map model and the k_word_read_vector
float estimate_bits(string target_file_name, unordered_map<string, list<int>> model, vector<string> vector_model) {

    // read the target file
    string word = read_word(K);
    int first_time = 0;
    

    while (word != "") {
        int position = int(target_file.tellg());
        string save_word = word;


        // check if the word is in the model
        if (model.count(word) > 0) {
            //cout << "[lang.cpp]: word is in the model" << endl;

            // get the list of frequencies
            list<int> freq_list = model[word];
            float best_bits = 0;
            int best_index = 0;

            // map to save the bits for each symbol
            map<int,pair<char,float>> char_bits_best;
            map<int,pair<char,float>> char_bits;

            for (int i : freq_list) {
                //clear the char_bits
                char_bits.clear();


                // get word from the vector_model
                string word_from_vector = vector_model[i];

                n_Fails = 0;
                n_Hits = 0;
                string next_word = word;

                float bits_to_write = 0;
                int ratio = n_Fails;
                int j = i;
                word=save_word;
                int initial_position = int(target_file.tellg());
                

                while (ratio < threshold){
                    // calculate the bits to write and update the next_word to get the next char
                    float bits_sym = calculate_bits_lang(calculate_probability_lang(word_from_vector, next_char));
                    bits_to_write += bits_sym;
                    position = int(target_file.tellg());

                    char_bits.insert(pair<int,pair<char,float>>(position,{next_word[next_word.size()-1], bits_sym}));
                    
                    next_word = read_word(K);
                    
                    //in case the file ends before the threshold is reached
                    if (next_word == ""){
                        break;
                    }

                    // get the next char from the next_word and from the vector_model 
                    next_char = next_word[next_word.size()-1];

                    // check if we can get the predicted next char, if not break the loop and update the ratio
                    if (j+1 >= vector_model.size()){
                        n_Fails++;
                        ratio = n_Fails;
                        break;
                    }
                    
                    predicted_next_char = vector_model[j+1][vector_model[j+1].size()-1];

                    // check if the next char is equal to the predicted next char and update the hits and fails
                    if (next_char == predicted_next_char){
                        n_Hits++;
                    }
                    else{
                        n_Fails++;
                    }

                    // update the ratio and the auxiliar variable j
                    ratio = n_Fails;
                    j++;
                }

                int current_position = int(target_file.tellg());
                // if the ratio is equal to the threshold bring back the pointer for initial word and if the bits_to_write is less than the best_bits, update the best_bits
                //check if is the last index of the vector_model
                if (ratio==threshold && freq_list.back() != i){
                    
                    if (best_bits == 0){
                        best_bits = bits_to_write;
                        char_bits_best = char_bits;
                        best_index = int(target_file.tellg());
                    }
                    else if (bits_to_write < best_bits){
                        best_bits = bits_to_write;
                        char_bits_best = char_bits;
                        best_index = int (target_file.tellg());
                    }
                    
                    int back = current_position - initial_position;
                    //cout << "[lang.cpp]: back = " << back << endl;
                    bring_back_target(back+1);
                    word = read_word(K);
                }else{  // if the ratio is not equal to the threshold only update the best_bits if the bits_to_write is less than the best_bits

                    
                    if (best_bits == 0){
                        best_bits = bits_to_write;
                        char_bits_best = char_bits;
                        best_index = int(target_file.tellg());
                    }
                    else if (bits_to_write < best_bits){
                        best_bits = bits_to_write;
                        char_bits_best = char_bits;
                        best_index = int(target_file.tellg());
                    }
                    // if the ratio is equal to the threshold and the last index of the vector_model, we need to bring back the pointer to the point where the threshold was reached (the previous word)
                    if (ratio==threshold && freq_list.back() == i && best_bits == bits_to_write){
                        //cout << "[lang.cpp]: back = 1 lol" << endl;
                        bring_back_target(2);
                        word = read_word(K);
                        break;
                    }
                    if (best_index == current_position){
                        break;
                    }else if (best_index < current_position){
                        int back = current_position - best_index;
                        //cout << "[lang.cpp]: back else if= " << back << endl;
                        bring_back_target(back+2);
                        word = read_word(K);

                    }
                    else{
                        bring_front_target(best_index - current_position -2);
                        //cout << "[lang.cpp]: front else = " << best_index - current_position +1 << endl;
                        word = read_word(K);

                    }
                    
                    
                    
                }

                
            }
            bits += best_bits;

            // save on char_average_bits the position of the word and the bits to write from the char_bits_best
            for (auto it = char_bits_best.begin(); it != char_bits_best.end(); it++){
                char_average_bits.insert(pair<int, pair<char,float>>(it->first, it->second));

                
            }

            

        }
        // if the word is not in the model
        else {
            //cout << "[lang.cpp]: word not in model" << endl;
            //cout << "[lang.cpp]: N_different_symbols = " << N_different_symbols << endl;
            if (first_time!=0){
                float bits_symbol = log2(N_different_symbols);
                //cout << "[lang.cpp]: bits_symbol = " << bits_symbol << endl;
                bits += bits_symbol;                    
                // save on char_average_bits the position of the word and the bits to write
                char_average_bits.insert(pair<int, pair<char,float>>(position, {word[word.size()-1], bits_symbol}));
                //cout << "[lang.cpp]: index = " << char_average_bits.size() << endl;
                //cout << "[lang.cpp]: bits = " << bits_symbol << endl;
            }
            else{
                first_time=1;
            }
            
        }
        // read the next word
        word = read_word(K);
        //cout << "[lang.cpp]: word = " << word << endl;
    }
    cout << "[lang.cpp]: bits = " << bits << endl;
    return bits;
}

string read_word(int k)
{
    static string c;


    if (target_file.eof())
    {
        cout << "[lang.cpp]: end of file" << endl;
        end_of_file_target = true;
        return "";
    }

    if (!target_file.is_open())
    {
        cout << "[lang.cpp]: opening file" << endl;
        target_file.open(text_filename, ios::binary);

        char buffer[k];
        target_file.read(buffer, k);
        c = string(buffer, target_file.gcount());
        float bits_symbol = log2(N_different_symbols);
        bits += bits_symbol * k;

        // save on char_average_bits the position of the word and the bits to write
        for (int i = 0; i < k; i++)
        {
            char_average_bits.insert(pair<int, pair<char,float>>(char_average_bits.size(),{buffer[i], bits_symbol}));
        }
    }
    else
    {
        int current_position = int(target_file.tellg());
        target_file.seekg(-k + 1, ios::cur);
        char buffer[k];
        target_file.read(buffer, k);
        c = string(buffer, target_file.gcount());

    }

    if (target_file.gcount() < k-1)
    {
        c = c.substr(0, target_file.gcount());

        if (c.size() < k)
        {
            //cout << "[lang.cpp]: N_different_symbols = " << N_different_symbols << endl;

            float bits_symbol = log2(N_different_symbols);
            //cout << "[lang.cpp]: bits_symbol = " << bits_symbol << endl;
            bits += bits_symbol;

            // save on char_average_bits the position of the word and the bits to write
            for (int i = 0; i < c.size(); i++)
            {
                char_average_bits.insert(pair<int, pair<char,float>>(char_average_bits.size(),{c[i], bits_symbol}));
            }

            end_of_file_target = true;
            target_file.close();
            return "";
        }
        else
        {
            end_of_file_target = true;
            target_file.close();
            return c;
        }
    }

    if (target_file.eof())
    {
        cout << "[lang.cpp]: end of file" << endl;
        end_of_file_target = true;
        target_file.close();
        return "";
    }

    // add every character to the char_map if the index is not already in the map
    for (int i = 0; i < c.size(); i++)
    {
        int position = target_file.tellg();
        if (char_map.find(position - c.size() + i) == char_map.end())
        {
            char_map.insert(pair<int, char>(position - c.size() + i, c[i]));
        }
        
    }

    return c;
}

// Function to bring the pointer k positions back
void bring_back_target(int k)
{

    if (!target_file.is_open())
    {
        target_file.open(text_filename);
    }
    target_file.seekg(-k, ios::cur);
}

// Function to bring the pointer k positions to the front
void bring_front_target(int k)
{
    if (!target_file.is_open())
    {
        target_file.open(text_filename);
    }
    target_file.seekg(k, ios::cur);
}


void reset() {
    target_file = ifstream();
    end_of_file_target = false;
    bits = 0;
    n_Fails = 0;
    n_Hits = 0;
    char_average_bits.clear();
    char_average_bits = map<int, pair<char,float>>();
}

map<int, pair<char,float>> get_char_average_bits() {
    return char_average_bits;
}

int get_target_file_size() {
    ifstream file(text_filename, ios::binary | ios::ate);
    int size = file.tellg();
    file.close();
    return size;
}

map<int, char> get_char_map() {
    return char_map;
}