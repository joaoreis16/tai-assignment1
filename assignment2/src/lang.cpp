#include <iostream>
#include <fstream>
#include <string>

#include "cpm.hpp"


using namespace std;
//////////////////////////////////////////////////////////////


// Function to calculate estimate bits for the target file using the un_map model and the k_word_read_vector
float estimate_bits(string target_file_name, unordered_map<string, list<int> > model, vector<string> vector_model);

// Function to read a word from the target file
string read_word (int k);

// Function to bring the pointer k positions back
void bring_back_target(int k);

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
static float threshold = 4;
static float alpha = 1;
// end of file flag for target file
static bool end_of_file_target = false;
static float bits = 0;

static int n_Fails = 0;
static int n_Hits = 0;
static char next_char;
static char predicted_next_char;

//////////////////////////////////////////////////////////////

/* int main(int argc, char* argv[]) {

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
} */

float get_estimated_bits(string target_filename, string filename, int k, float t, float a) {

    // reset structures
    reset();

    text_filename = target_filename;
    ri_filename = filename;
    alpha = a; K = k; threshold = t; 

    // apply the copy model
    int num_bits = apply_cpm(ri_filename, K, threshold, alpha);
    cout << "[lang.cpp]: total bits = " << num_bits << endl;

    // get un_map from cpm.cpp
    model = get_un_map();

    // get k_word_read_vector from cpm.cpp
    vector_model = get_k_word_read_vector();

    // estimate bits for the target file
    float bits = estimate_bits(target_filename, model, vector_model);

    return bits;
}

// Function to calculate estimate bits for the target file using the un_map model and the k_word_read_vector
float estimate_bits(string target_file_name, unordered_map<string, list<int>> model, vector<string> vector_model) {

    // read the target file
    string word = read_word(K);
    while (word != "") {

        // check if the word is in the model
        if (model.count(word) > 0) {

            // get the list of frequencies
            list<int> freq_list = model[word];
            float best_bits = 0;

            for (int i : freq_list) {

                cout << "[lang.cpp]: freq_list = " << i << endl;
                // get word from the vector_model
                string word_from_vector = vector_model[i];
                cout << "[lang.cpp]: word_from_vector = " << word_from_vector << endl;
                n_Fails = 0;
                n_Hits = 0;
                string next_word = word;
                cout << "[lang.cpp]: next_word before threshold= " << next_word << endl;
                float bits_to_write = 0;
                float ratio = (float)n_Fails;
                int j = i;

                while (ratio < threshold){
                    // calculate the bits to write and update the next_word to get the next char
                    bits_to_write += calculate_bits(calculate_probability(word_from_vector, next_char));
                    next_word = read_word(K);

                    //in case the file ends before the threshold is reached
                    if (next_word == ""){
                        break;
                    }

                    // get the next char from the next_word and from the vector_model 
                    next_char = next_word[next_word.size()-1];
                    predicted_next_char = vector_model[j+1][vector_model[j+1].size()-1];

                    // check if the next char is equal to the predicted next char and update the hits and fails
                    if (next_char == predicted_next_char){
                        n_Hits++;
                    }
                    else{
                        n_Fails++;
                    }

                    // update the ratio and the auxiliar variable j
                    ratio = (float)n_Fails;
                    j++;
                }

                // if the ratio is equal to the threshold bring back the pointer for initial word and if the bits_to_write is less than the best_bits, update the best_bits
                if (ratio == threshold){
                    int back = j-i;
                    bring_back_target(back+1);
                    word = read_word(K);
                    cout << "[lang.cpp]: next_word after threshold = " << next_word << endl;
                    if (best_bits == 0){
                        best_bits = bits_to_write;
                    }
                    else if (bits_to_write < best_bits){
                        best_bits = bits_to_write;
                    }
                }else{  // if the ratio is not equal to the threshold only update the best_bits if the bits_to_write is less than the best_bits
                    if (best_bits == 0){
                        best_bits = bits_to_write;
                    }
                    else if (bits_to_write < best_bits){
                        best_bits = bits_to_write;
                    }
                }

                bits += best_bits;
            }

        }
        // if the word is not in the model
        else {
            bits += log2(4);                    // Nota: descobrir qual deve ser o valor em vez de 4
        }
        // read the next word
        word = read_word(K);
        cout << "[lang.cpp]: word = " << word << endl;
    }
    cout << "[lang.cpp]: bits = " << bits << endl;
    return bits;
}

string read_word (int k) {
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
        target_file.open(text_filename);
        char buffer[k];
        target_file.read(buffer, k);
        c = string(buffer, k);
        bits += log2(4) * k;                // Nota: desccobrir qual deve ser o valor em vez de 4
    }
    // else bring the pointer k-1 positions back and read k chars
    else
    {
        // bring the pointer k-1 positions back
        target_file.seekg(-k + 1, ios::cur);
        char buffer[k];
        target_file.read(buffer, k);
        c = string(buffer, k);
    }
    // check if the file has less than k chars
    if (target_file.gcount() < k)
    {
        
        c = c.substr(0, target_file.gcount());
        if (c.size() < k){
            bits += log2(4);            // Nota: desccobrir qual deve ser o valor em vez de 4
            end_of_file_target = true;
            target_file.close();
            return "";
        }else{
            end_of_file_target = true;
            target_file.close();
            return c;
        }
    }
    // if it is the end of the file, close the file
    if (target_file.eof())
    {
        cout << "[lang.cpp]: end of file" << endl;
        end_of_file_target = true;
        target_file.close();
        return "";
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


void reset() {
    target_file = ifstream();
    end_of_file_target = false;
    bits = 0;
    n_Fails = 0;
    n_Hits = 0;
}