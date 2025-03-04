#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <vector>
#include <map>
#include <chrono>

#include "cpm.hpp"
#include "lang.hpp"

using namespace std;

const char* ri_folder;
string target_text;
static int K = 4;
static float threshold = 4;
static float alpha = 1;

static bool fcmodel_flag = false;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Usage: ./findlang <ri_foldername> <target_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> -f)" << endl;
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
                cerr << "Usage: ./findlang <ri_foldername> <target_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> -f)" << endl;
                return 1;
        }
    }

    // to check if the file exist
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

    auto start = std::chrono::high_resolution_clock::now();

    string lang_found;
    float min_bits = 99999999999.0;
    for (const auto& filename : all_files) {

        string folder;
        folder.assign(ri_folder);
        string temp = folder + "/";
        string file_path = temp + filename;

        cout << "\n>> file: " << filename << endl;  // Print the file name

        float bits;
        if (fcmodel_flag) {
            train_fcm(file_path, K, threshold, alpha);

            bits = apply_fcm(target_text);

        } else {
            bits = get_estimated_bits(target_text, file_path, K, threshold, alpha);
        }

        cout << "estimated bits = " << bits << endl;

        if (bits < min_bits && bits != 0) {
            min_bits = bits;
            lang_found = filename;
        }
    }

    size_t extensionPos = lang_found.find_last_of(".");
    string language  = lang_found.substr(0, extensionPos);

    cout << "\n\nthe language of the file '" << target_text << "' is " << language << " (bits = " << min_bits << ")" << endl;

    size_t foundPos = target_text.find(language);

    int result = 0;
    if (foundPos != std::string::npos) {
        cout << "it's correct!!!" << endl;
        result = 1;

    } else {
        cout << "it's wrong :(" << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "execution time = " << elapsed.count() << std::endl;

    // Write results to a file
    ofstream outputFile("results/findlang_results.txt", ios::app);
    if (outputFile.is_open()) {
        outputFile << "\n" << alpha << " " << threshold << " " << K << " " << result << " " << fcmodel_flag << " " << min_bits << " " << elapsed.count() << endl;
        outputFile.close();

    } else {
        cout << "Failed to open the results file." << endl;
    }

    return 0;
}