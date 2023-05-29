#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <vector>
#include <map>

#include "cpm.hpp"
#include "lang.hpp"

using namespace std;

const char* ri_folder;
string target_text;
static int K = 10;
static float threshold = 10;
static float alpha = 0.9;

static map<int, char> index_char;
static bool fcmodel_flag = false;

//map to save the index and the language
map<int, string> index_lang;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Usage: ./locatelang <ri_foldername> <target_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> -f)" << endl;
        return 1;
    }

    ri_folder   = argv[1];
    target_text = argv[2];

    // read the parameters
    int opt;
    while ((opt = getopt(argc, argv, "a:t:k:f")) != -1) {
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
            case 'f':
                fcmodel_flag = true;
                break;
            default:
                cerr << "Usage: ./locatelang <ri_foldername> <target_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> -f)" << endl;
                return 1;
        }
    }
    //check if file exist
    ifstream file_exist(target_text);
    if (!file_exist.good()) {
        // File does not exist
        cout << "[ERROR]: file '" << target_text << "' does not exist." << endl;
        exit(0);
    }
    

    // get the file names and save them 
    DIR* directory = opendir(ri_folder);
    vector<string> all_files;

    if (directory) {
        dirent* entry;

        while ((entry = readdir(directory)) != nullptr) {
            if (entry->d_type == DT_REG) {  // Check if it's a regular file
                
                string filename = entry->d_name;
                all_files.push_back(filename);
            }
        }
        closedir(directory);

    } else {
        cout << "Failed to open the folder '"<< ri_folder <<"'." << endl;
    }

    // create a map to save the results
    map<string, map<int,pair<char,float>>> results;
    map<string, int> file_sizes;
    int first = 1;

    // for each file in the folder
    for (string filename : all_files) {
        
        std::string folder;
        folder.assign(ri_folder);
        string temp = folder + "/";
        string file_path = temp + filename;

        // calculate the estimated bits
        float estimated_bits;
        if (fcmodel_flag) {
            train_fcm(file_path, K, threshold, alpha);

            estimated_bits = apply_fcm(target_text);
            cout << "Model FCM for file " << filename << " done." << endl;

        } else {
            estimated_bits = get_estimated_bits(target_text, file_path, K, threshold, alpha);
            cout << "Model CPM for file " << filename << " done." << endl;
        }
        
        // get un_map with symbols and their bits
        map<int,pair<char,float>> bits_map;
        if (fcmodel_flag) {
            bits_map = get_char_bits();

        } else {
            bits_map = get_char_average_bits();
        }

        int N_diff_symbols = get_N_different_symbols();
        file_sizes[filename] = N_diff_symbols;
        

        //print bits_map
        for (auto it = bits_map.begin(); it != bits_map.end(); it++) {
            //cout << "index: " << it->first << " char: " << it->second.first << " bits: " << it->second.second << endl;
            if (first==1) {
                index_char[it->first] = it->second.first;
                if (it->first == bits_map.size() - 1) {
                    first = 0;
                }
            }
        }

        // suavizar o map
        // fazer media de 3 em 3 
        for (auto it = bits_map.begin(); it != bits_map.end(); it++) {

            //cout << "index: " << it->first << " char: " << it->second.first << " bits: " << it->second.second << endl;
            
            if (it->first== bits_map.size() - 2) {
                break;
            }
            else {
                float sum = it->second.second + bits_map[it->first+1].second + bits_map[it->first+2].second;
                float average = sum/3;
                it->second.second = average;
            }
        }
       
        // save the result
        results[filename] = bits_map;
        

    }

    //check where a language begins and ends
    //get every word and for each word get the language based on the bits of each symbol and the threshold
    //if the language changes, save the index and the language
    //if the language is the same, keep going
    int target_file_size = get_target_file_size(target_text);
    cout << "target_file_size: " << target_file_size << endl;
    string language = "";
    string word = "";
    int first_index = 0;
    
    
    for (int i = 0; i <= target_file_size; i++) {
        if (word==""){
            first_index = i;
        }
        word += index_char[i];
        if ((isalpha(index_char[i])==0 && isdigit(index_char[i])==0 && word.size() >1)|| i == target_file_size ) {
            float lowest_value = 100000;
            string new_language = "";
            float min_value = 100000;
            int threshold = 0;
            for (auto it = results.begin(); it != results.end(); it++) {

                threshold = log2(file_sizes[it->first]);

                float value = 0;
                for (int j = first_index; j < i; j++) {
                    value += it->second[j].second;
                }
                if (value < min_value && value < threshold) {
                    min_value = value;
                    new_language = it->first;
                }
            }
            if (language != new_language && new_language != "") { 
                if (language != ""){
                    language = new_language;
                    index_lang[i] = language;
                }else{
                    language = new_language;
                    index_lang[0] = language;
                }
                
            }
            word = "";

        }

    }

    // update the index_lang map to mantain only languages that are longer than K*2
    int last_index = 0;
    string last_language = "";
    for (auto it = index_lang.begin(); it != index_lang.end(); it++) {
        if (last_language == "") {
            if (it->first > K*2) {
                last_language = it->second;
                last_index = it->first;
            }
        }
        else if (last_language != it->second) {
            if (target_file_size - it->first <= K*2) {
                //erase the last language
                index_lang.erase(it);
                break;
            }
            else{
                if (it->first - last_index <= K*2) {
                    
                    auto it2 = it;
                    it2--;
                    index_lang.erase(it2);
                }
                else {
                    last_language = it->second;
                    last_index = it->first;
                }
            }
        }
    }


    //print index_lang
    for (auto it = index_lang.begin(); it != index_lang.end(); it++) {
        cout << "Index: " << it->first << " Language: " << it->second << endl;
    }

    //print the result in the console with colors for each language detected and print the legend
    for (auto it = index_lang.begin(); it != index_lang.end(); it++) {  
        string console_print_begin = "";
        string console_print_end = "";
        
        if (it->second == "English.utf8"){
            console_print_begin = "\033[1;31m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Portuguese.utf8"){
            console_print_begin = "\033[1;32m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Spanish.utf8"){
            console_print_begin = "\033[1;33m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "French.utf8"){
            console_print_begin = "\033[1;34m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "German.utf8"){
            console_print_begin = "\033[1;35m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Italian.utf8"){
            console_print_begin = "\033[1;36m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Dutch.utf8"){
            console_print_begin = "\033[1;37m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Danish.utf8"){
            console_print_begin = "\033[1;41m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Swedish.utf8"){
            console_print_begin = "\033[1;42m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Finnish.utf8"){
            console_print_begin = "\033[1;43m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Hungarian.utf8"){
            console_print_begin = "\033[1;44m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Polish.utf8"){
            console_print_begin = "\033[1;45m";
            console_print_end = "\033[0m\n";
        }else if(it->second == "Czech.utf8"){
            console_print_begin = "\033[1;46m";
            console_print_end = "\033[0m\n";
        }else if(last_language == "Hungarian.utf8"){
            console_print_begin = "\033[1;47m";
            console_print_end = "\033[0m\n";
        }else if(last_language == "Polish.utf8"){
            console_print_begin = "\033[1;101m";
            console_print_end = "\033[0m\n";
        }else if(last_language == "Romanian.utf8"){
            console_print_begin = "\033[1;91m";
            console_print_end = "\033[0m\n";
        }else if(last_language == "Swedish.utf8"){
            console_print_begin = "\033[1;92m";
            console_print_end = "\033[0m\n";
        }
        //print the text in the console form index_char 
        auto next_it = it;
        next_it++;
        if (next_it == index_lang.end()) {
            next_it--;
            cout << console_print_begin;
            for (int i = it->first; i < index_char.size(); i++) {
                cout << index_char[i];
            }
            cout << console_print_end;
            break;
        }

        cout << console_print_begin;
        for (int i = it->first; i < next_it->first; i++) {
            cout << index_char[i];
        }
        cout << console_print_end;
        
    }

    return 0;
}