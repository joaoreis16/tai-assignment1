#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class FiniteContextModel {
    private:
        unordered_map<string, unordered_map<char, int>> symbolCounts;
        unordered_map<string, int> KWordCounts;
        int order;

    public:
        FiniteContextModel(int order) : order(order) {}

        void train(const vector<char>& sequence) {
            for (long unsigned int i = order; i < sequence.size(); i++) {
                string kword = get_k_word(sequence, i);
                char next_symbol = sequence[i];
                
                symbolCounts[kword][next_symbol]++;
                KWordCounts[kword]++;
            }
        }

        double get_probability(const vector<char>& sequence, char next_symbol, float alpha, int N_different_symbols) {
            string kword = get_k_word(sequence, sequence.size());
            int KWordCount = KWordCounts[kword];
            int symbolCount = symbolCounts[kword][next_symbol];

            return static_cast<double>((symbolCount) + alpha ) / (KWordCount + alpha * N_different_symbols);
        }

        void set_order(int order) {
            order = order;

            // reset the structures
            symbolCounts = unordered_map<string, unordered_map<char, int>>();
            KWordCounts  = unordered_map<string, int>();
        }

    private:
        string get_k_word(const vector<char>& sequence, int currentIndex) {
            string word = "";
            int startIndex = currentIndex - order;
            
            for (int i = startIndex; i < currentIndex; i++) {
                word += sequence[i];
            }

            return word;
        }
};