#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <list>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <vector>
#include <tuple>
#include <random>

using namespace std;

///////////////////////////////////////////////////////

void read_file();

void calculate_probabilities();

void print_unordered_map();

void predict();

/////////////// global variables //////////////////////

static unordered_map<string, tuple<int, int, int, int>> un_map; // this map will store a k-char sequence as a key and a tuple of probabilities for each symbol as a value
static string file_name;
static ifstream file;

static vector<string> k_word_read_vector;
static int K = 4;


///////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    file_name = argv[1];
    read_file();
    calculate_probabilities();
    print_unordered_map();
    for (int i = 0; i < 20; i++)
    {
        predict();
    }
    print_unordered_map();

    return 0;
}

void read_file()
{
    file.open(file_name);
    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        exit(1);
    }
    // read first K characters and then read one character at a time
    string word;
    for (int i = 0; i < K; i++)
    {
        char c;
        file.get(c);
        word += c;

    }
    k_word_read_vector.push_back(word);
    
    while (file.peek() != EOF)
    {
        char c;
        file.get(c);
        word += c;
        word.erase(0, 1);
        k_word_read_vector.push_back(word);
    }
    file.close();

}

void calculate_probabilities()
{
    for (int i = 0; i < k_word_read_vector.size(); i++)
    {
        string word = k_word_read_vector[i];
        cout << word << endl;

        //create a new entry in the map if the word is not in the map
        if (un_map.find(word) == un_map.end())
        {
            un_map[word] = make_tuple(0, 0, 0, 0);
        }

        if (i == k_word_read_vector.size() - 1)
        {
            break;
        }


        char next_char = k_word_read_vector[i + 1][K - 1];
        cout << next_char << endl;
        if (next_char == 't')
        {
            get<0>(un_map[word])++;
        }
        else if (next_char == 'e')
        {
            get<1>(un_map[word])++;
        }
        else if (next_char == 'x')
        {
            get<2>(un_map[word])++;
        }
        else if (next_char == 'o')
        {
            get<3>(un_map[word])++;
        }
        
    }

}

void print_unordered_map()
{
    cout << "unorderer_map" << endl;
    for (auto it = un_map.begin(); it != un_map.end(); it++)
    {
        cout << it->first << " " << get<0>(it->second) << " " << get<1>(it->second) << " " << get<2>(it->second) << " " << get<3>(it->second) << endl;
    }
}

void predict()
{
    // predict next char of the file based on the probabilities calculated

    //get the last K characters of the file
    string word = k_word_read_vector[k_word_read_vector.size() - 1];
    //cout << "old word: " << word << endl;

    //get the probabilities of each symbol for the last K characters
    tuple<int, int, int, int> probabilities = un_map[word];

    //calculate the probability of each symbol

    
    float sum = std::apply([](auto&&... args) { return (args + ...); }, probabilities);

    float probability_t = (float)get<0>(probabilities) / sum;
    float probability_e = (float)get<1>(probabilities) / sum;
    float probability_x = (float)get<2>(probabilities) / sum;
    float probability_o = (float)get<3>(probabilities) / sum;

    //pick a letter random using the probabilities calculated
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> d({probability_t, probability_e, probability_x, probability_o});

    int letter = d(gen);
    char next_char;

    //print the letter
    if (letter == 0)
    {
        cout << "t" << endl;
        //update the probabilities in the map
        un_map[word] = make_tuple(get<0>(probabilities) + 1, get<1>(probabilities), get<2>(probabilities), get<3>(probabilities));
        next_char = 't';
    }
    else if (letter == 1)
    {
        cout << "e" << endl;
        //update the probabilities in the map
        un_map[word] = make_tuple(get<0>(probabilities), get<1>(probabilities) + 1, get<2>(probabilities), get<3>(probabilities));
        next_char = 'e';
    }
    else if (letter == 2)
    {
        cout << "x" << endl;
        //update the probabilities in the map
        un_map[word] = make_tuple(get<0>(probabilities), get<1>(probabilities), get<2>(probabilities) + 1, get<3>(probabilities));
        next_char ='x';
    }
    else if (letter == 3)
    {
        cout << "o" << endl;
        //update the probabilities in the map
        un_map[word] = make_tuple(get<0>(probabilities), get<1>(probabilities), get<2>(probabilities), get<3>(probabilities) + 1);
        next_char = 'o';
    }

    //add the new letter to the vector
    
    word.erase(0, 1);
    word += next_char;
    //cout << "new word: " << word << endl;
    k_word_read_vector.push_back(word);

    if (un_map.find(word) == un_map.end())
    {
        un_map[word] = make_tuple(0, 0, 0, 0);
    }

    
    
    
}