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

        double get_probability(const vector<char>& sequence, char symbol) {
            string kword = get_k_word(sequence, sequence.size());
            int KWordCount = KWordCounts[kword];
            int symbolCount = symbolCounts[kword][symbol];
            
            if (KWordCount == 0) return 0.0;
            return static_cast<double>(symbolCount) / KWordCount;
        }

    private:
        string get_k_word(const vector<char>& sequence, int currentIndex) {
            string word = "";
            int startIndex = currentIndex - order;
            
            for (int i = startIndex; i < currentIndex; i++) {
                word += sequence[i];
            }

            // cout << "word: " << word << endl;
            return word;
        }
};

int main() {
    // Example usage
    FiniteContextModel model(3); // Create a finite-context model with order 3

    vector<char> sequence = {'A', 'T', 'P', 'T', 'T', 'A', 'P', 'P', 'T', 'A', 'P', 'A'};
    
    // Train the model with the sequence
    model.train(sequence);

    // Estimate the probability of a non-hit symbol 'P' given the sequence 'TAP'
    double probability = model.get_probability({'T', 'A', 'P'}, 'P');
    
    cout << "Probability: " << probability << endl;
    
    return 0;
}