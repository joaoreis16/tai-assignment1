#include <iostream>
#include <fstream>
#include <string>


using namespace std;


// apply the copy model to know the 
int apply_cpm(string filename) {
    word = read_char(K);

    do {
        if (word == "") break;

        if (std::find(k_word_read_vector.begin(), k_word_read_vector.end(), word) != k_word_read_vector.end()) {
            predict();

        } else {
            //  the map mantains only (map_size) references to the words, so if we have more than that words, we remove the oldest one
            if (un_map[word].size() > map_size) {
                un_map[word].pop_front();
            }

            // add the word to the list
            k_word_read_vector.push_back(word);

            // add the word to the map
            un_map[word].push_back(k_word_read_vector.size() - 1);

            // add max bits to write 
            bits += log2(4); // 4 is the number of possible chars
            word = read_char(K);
            actual_index++;
        }

    } while (!end_of_file);

    cout << "bits int => " << int(bits) << endl;
    return int(bits);
}


int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Usage: ./lang <ri_file> <analysis_file> <model_parameters>" << endl;
        return 1;
    }

    string ri_filename   = argv[1];
    string text_filename = argv[2];

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
    apply_cpm(ri_filename);

    return 0;
}