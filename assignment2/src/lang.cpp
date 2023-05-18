#include <iostream>
#include <fstream>
#include <string>

#include "cpm.hpp"


using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Usage: ./lang <ri_file> <analysis_file> <model_parameters>" << endl;
        return 1;
    }

    string ri_filename   = argv[1];
    string text_filename = argv[2];

    int K = 4;
    float alpha = 1;
    float threshold = 4;

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
                std::cerr << "Usage: ./lang <ri_file> <analysis_file> -a <int> -t <float> -k <int>" << std::endl;
                return 1;
        }
    }

    // apply the copy model
    int num_bits = apply_cpm(ri_filename, K, threshold, alpha);
    cout << "[lang.cpp]: total bits = " << num_bits << endl;

    return 0;
}