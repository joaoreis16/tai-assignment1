#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <list>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <chrono>

using namespace std;

///////////////////////////////////////////////////////

bool contains(list<string> list, string element);

void read_file();

string read_char(int k);

void bring_back(int k);

void print_words_read_list(list<string> words_read);

void print_unordered_map();

void predict(char **argv);

void print_char_average_bits();

/////////////// global variables //////////////////////

static unordered_map<string, list<int> > un_map;
static unordered_map<char, float> char_bits;
static unordered_map<char, int> char_occurrences;

static string file_name;
static ifstream file;

static vector<string> k_word_read_vector;
static float alpha = 1;
static int K = 4;
static string word;
static int total_characters = 0;


static int N_hits = 0;
static int N_fails = 0;
static float threshold = 4;
static int map_size = 4;
float bits = 0;
char predicted_next_char;

float highest_prob = 0;
float lowest_bits = 0;
int lowest_index = 0;
int actual_index = 0;
bool end_of_file = false;



///////////////////////////////////////////////////////

float calculate_probability(string word, char next_char)
{

    return (float)(N_hits + alpha) / (N_hits + N_fails + 2 * alpha);
}

float calculate_bits(float prob)
{
    return (float)-log2(prob);
}

int main(int argc, char **argv)
{
    auto start = std::chrono::high_resolution_clock::now();

    if (argc < 2)
        printf("[usage]: %s file_path", argv[0]);

    int opt;
    actual_index = 0;


    while ((opt = getopt(argc, argv, "f:a:t:k:")) != -1) {
        switch (opt) {
            case 'f':
                file_name = optarg;
                break;
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
                std::cerr << "Usage: " << argv[0] << " -f <string> -a <int> -t <float> -k <int>" << std::endl;
                return 1;
        }
    }

    cout << "alpha-" << alpha << endl;
    cout << "threshold-" << threshold << endl;
    cout << "K-" << K << endl;
    word = read_char(K);

    do
    {
        if (word == "")
            break;
        //cout << "actual_index => " << actual_index << endl;

        if (un_map.count(word) >0)
        {
            predict(argv);
        }
        else
        {

            //  the map mantains only (map_size) references to the words, so if we have more than that words, we remove the oldest one
            if (un_map[word].size() > map_size)
            {
                un_map[word].pop_front();
            }
            // add the word to the list
            k_word_read_vector.push_back(word);
            // add the word to the map
            un_map[word].push_back(k_word_read_vector.size() - 1);
            // add max bits to write 
            bits += log2(4); // 4 is the number of possible chars
            word = read_char(K);
            total_characters++;
            actual_index++;
        }

    } while (!end_of_file);
    // print k_word_read_vector just to check
    // for (int j=0; j<k_word_read_vector.size(); j++){
    //     cout<<"k_word_read_vector["<<j<<"] => "<<k_word_read_vector[j]<<endl;
    // }
    // print_unordered_map();
    cout << "total_bits-" << int(bits) << endl;
    float average_bits = bits / total_characters;
    cout << "Average_number_of_bits-" << average_bits << endl;
    print_char_average_bits();

    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    //cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    cout << "Elapsed_time-" << elapsed.count() << std::endl;
    return 0;
}

void predict(char **argv)

{
    
    string k_string = un_map.find(word)->first;  // dá a key do mapa
    list<int> index = un_map.find(word)->second; // ir buscar os values do map (lista de indices)

    // in case the word is not in the map, but it is in the list, we add it to the map (this happens when we reach the threshold)
    if (lowest_index <= actual_index)
    {
        // cout<<"adicionei word => " << word << endl;

        if (un_map[word].size() > map_size)
        {
            un_map[word].pop_front();
        }

        // add the word to the list
        k_word_read_vector.push_back(word);
        // add the word to the map
        un_map[word].push_back(k_word_read_vector.size() - 1);
    }

    float prob_hit;
    float bits_to_write = 0;
    int new_index = 0;
    highest_prob = 0;
    lowest_bits = 0;

    // para cada elemento da lista de indices (index) vamos calcular a probabilidade de acerto e a quantidade de bits e escolher o que tem maior probabilidade
    for (int i : index)
    {

        int d = i; // variavel auxiliar para aumentar o indice

        // bring back the pointer to the index of the word (this happens when we reach the threshold, so we need to bring back the pointer to the index of the word)
        if ((new_index - actual_index) > 0)
        {
            // cout<<"lol" << endl;
            bring_back(new_index - actual_index);
        }

        new_index = actual_index;
        N_fails = 0;
        N_hits = 0;
        bits_to_write = 0;

        // calculr razao entre o numero de acertos e o numero de erros se for nan ou inf entao é 0
        float ratio = (float)N_fails;
        // if (isinf(ratio) || isnan(ratio)) ratio = 0; // no caso de usar um ratio entre hits e fails

        while (ratio < threshold)
        {
            word = read_char(K);
            new_index++;
            if (word == "") break;
            // calculate the probability of hit
            prob_hit = calculate_probability(k_string, predicted_next_char);

            //  calculate the number of bits
            bits_to_write += calculate_bits(prob_hit);

            // get the predicted next char
            predicted_next_char = k_word_read_vector[d + 1][k_word_read_vector[d + 1].size() - 1];




            //count the number of occurrences of each char
            if(char_occurrences.find(predicted_next_char) == char_occurrences.end()){
                char_occurrences[predicted_next_char] = 1;
            }else{
                char_occurrences[predicted_next_char]++;
            }
        
            if (char_bits.find(predicted_next_char) == char_bits.end())
            {
                char_bits[predicted_next_char] = calculate_bits(prob_hit);
            }
            else
            {
                char_bits[predicted_next_char] += calculate_bits(prob_hit);
            }

            //  if the next char is the same as the predicted one, then we have a hit
            if (predicted_next_char == word[word.size() - 1])
            {
                N_hits++;
            }
            else
            {
                N_fails++;
            }

            ratio = (float)N_fails;
            // if (isinf(ratio) || isnan(ratio)) ratio = 0; // no caso de usar um ratio entre hits e fails


            
            if (N_fails == threshold) break; //threshold reached

            // check if this index get farther than the lowest, if so, then we add the word to the map and the list
            if (new_index >= lowest_index)
            {
                // add the word to the map and the list
                if (un_map[word].size() > map_size)
                {
                    un_map[word].pop_front();
                }
                k_word_read_vector.push_back(word);
                un_map[word].push_back(k_word_read_vector.size() - 1);
                lowest_index = k_word_read_vector.size() - 1;
            }


            d++;
        }

        // Save the probability of hit, the number of bits and the index of char where we stoped
        // if the probability of hit is higher than the previous one, then we save the new one
        // if the probability of hit is the same as the previous one, then we save the one with the lowest number of bits
        // if the probability of hit is the same as the previous one and the number of bits is the same as the previous one, then we save the one with the lowest index

        if (prob_hit > highest_prob)
        {
            highest_prob = prob_hit;
            lowest_bits = bits_to_write;
            lowest_index = new_index;

        }
        else if (prob_hit == highest_prob)
        {
            if (bits_to_write < lowest_bits)
            {

                lowest_bits = bits_to_write;
                lowest_index = new_index;

            }
            else if (bits_to_write == lowest_bits)
            {
                lowest_index = new_index;
            }
        }

    }

    // bring back the pointer to the highest index to read the next char on the next iteration
    if (((lowest_index)-new_index) < 0)
    {
        bring_back((new_index - lowest_index));

    }
    else
    {
        // read the next char till we reach the lowest index to read the next char on the next iteration
        while (lowest_index != new_index)
        {
            word = read_char(K);
            // cout<< "andei para a frente => "<<word<<endl;
            new_index++;
        }
    }
;
    // if the next char is empty, then we have reached the end of the file
    if (word == "")
    {
        //cout << "EOF" << endl;
        end_of_file = true;
    }
    actual_index = lowest_index;
    bits += lowest_bits;

    cout << "avg bits: " << bits_to_write << endl;
}


void print_words_read_list(list<string> words_read_list)
{
    cout << "words_read" << endl;
    int index = 0;
    for (auto it = words_read_list.begin(); it != words_read_list.end(); ++it)
    {
        cout << index << " -> " << *it << '\n';
        index++;
    }
}

void print_unordered_map()
{
    cout << "unorderer_map" << endl;
    for (auto it = un_map.begin(); it != un_map.end(); it++)
    {
        cout << it->first << " => ";
        for (int i : it->second)
        {
            cout << i << " ";
        }
        cout << endl;
    }
}

bool contains(list<string> list, string element)
{
    return find(list.begin(), list.end(), element) != list.end();
}

// Function to read and return "k" chars from the file every time it is called
string read_char(int k)
{

    static string c;
    if (file.eof())
    {
        end_of_file = true;
        return "";
    }
    if (!file.is_open())
    {
        file.open(file_name);
        char buffer[k];
        file.read(buffer, k);
        c = string(buffer, k);
        bits += log2(4) * k;

    }
    // else bring the pointer k-1 positions back and read k chars
    else
    {
        // bring the pointer k-1 positions back
        file.seekg(-k + 1, ios::cur);
        char buffer[k];
        file.read(buffer, k);
        c = string(buffer, k);
    }
    // check if the file has less than k chars
    if (file.gcount() < k)
    {
        c = c.substr(0, file.gcount());
    }
    // if it is the end of the file, close the file
    if (file.eof())
    {
        end_of_file = true;
        file.close();
        return "";
    }

    return c;
}

// Function to bring the pointer k positions back
void bring_back(int k)
{

    if (!file.is_open())
    {
        file.open(file_name);
    }
    file.seekg(-k, ios::cur);
}

void print_char_average_bits()
{
    //cout << "Average number of bits per character:" << endl;
    for (const auto &entry : char_bits)
    {
        char character = entry.first;
        float total_bits = entry.second;
        int occurrences = char_occurrences[character];
        float result = occurrences == 0 ? 0 : total_bits / occurrences;
        //cout << character << ": " << result << endl;
    }
}
