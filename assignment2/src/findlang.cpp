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
static int K = 4;
static float threshold = 4;
static float alpha = 1;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Usage: ./findlang <ri_foldername> <target_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> )" << endl;
        return 1;
    }

    ri_folder   = argv[1];
    target_text = argv[2];

    // read the parameters
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
                cerr << "Usage: ./findlang <ri_foldername> <target_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> )" << endl;
                return 1;
        }
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


    string lang_found;
    float min_bits = 99999999999.0;
    for (const auto& filename : all_files) {
        cout << "\n\n>> file: " << filename << endl;  // Print the file name

        float bits = get_estimated_bits(target_text, filename, K, threshold, alpha);
        cout << "estimated bits = " << bits << endl;

        if (bits < min_bits && bits != 0) {
            min_bits = bits;
            lang_found = filename;
        }
    }

    size_t extensionPos = lang_found.find_last_of(".");
    string language  = lang_found.substr(0, extensionPos);

    cout << "\n\nthe language of the file '" << target_text << "' is " << language << " (bits = " << min_bits << ")" << endl;
    return 0;
}