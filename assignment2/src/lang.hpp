#ifndef MYFUNCTIONS_A
#define MYFUNCTIONS_A

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

// Function to calculate estimate bits for the target file using the un_map model and the k_word_read_vector
float estimate_bits(string target_file_name, unordered_map<string, list<int> > model, vector<string> vector_model);

// Function to read a word from the target file
string read_word (int k);

// Function to bring the pointer k positions back
void bring_back_target(int k);

float get_estimated_bits(string target_file_name, string ri_filename, int k, float t, float a);

#endif // MYFUNCTIONS_A