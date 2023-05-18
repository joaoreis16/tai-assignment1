#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

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

bool contains(list<string> list, string element);

void read_file();

string read_char(int k);

void bring_back(int k);

void print_words_read_list(list<string> words_read);

void print_unordered_map();

void predict();

void print_char_average_bits();

int apply_cpm(string filename, int k, float t, float a);

#endif // MYFUNCTIONS_H