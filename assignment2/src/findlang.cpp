#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>

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
                cerr << "Usage: ./findlang <ri_file> <analysis_file> -a <int> -t <float> -k <int>" << endl;
                return 1;
        }
    }

    DIR* directory = opendir(ri_folder);

    if (directory) {
        dirent* entry;
        while ((entry = readdir(directory)) != nullptr) {
            if (entry->d_type == DT_REG) {  // Check if it's a regular file

                string filename = entry->d_name;
                cout << "\n\n>> file: " << filename << endl;  // Print the file name

                float bits = get_estimated_bits(target_text, filename, K, threshold, alpha);
                cout << "estimated bits = " << bits << endl;
            }
        }
        closedir(directory);

    } else {
        cout << "Failed to open the directory." << endl;
    }

    return 0;
}