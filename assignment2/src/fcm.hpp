#ifndef FINITE_CONTEXT_MODEL_HPP
#define FINITE_CONTEXT_MODEL_HPP

#include <unordered_map>
#include <vector>
#include <string>

class FiniteContextModel {
    private:
        std::unordered_map<std::string, std::unordered_map<char, int>> symbolCounts;
        std::unordered_map<std::string, int> KWordCounts;
        int order;
        float alpha;
        int N_different_symbols;

    public:
        FiniteContextModel(int order);

        void train(const std::vector<char>& sequence);

        double get_probability(const std::vector<char>& sequence, char next_symbol, float alpha, int N_different_symbols);

        void set_order(int order);

    private:
        std::string get_k_word(const std::vector<char>& sequence, int currentIndex);
};

FiniteContextModel::FiniteContextModel(int order) : order(order) {}

void FiniteContextModel::train(const std::vector<char>& sequence) {
    for (size_t i = order; i < sequence.size(); i++) {
        std::string kword = get_k_word(sequence, i);
        char next_symbol = sequence[i];

        symbolCounts[kword][next_symbol]++;
        KWordCounts[kword]++;
    }
}

double FiniteContextModel::get_probability(const std::vector<char>& sequence, char next_symbol,  float alpha, int N_different_symbols) {
    std::string kword = get_k_word(sequence, sequence.size());
    int KWordCount = KWordCounts[kword];
    int symbolCount = symbolCounts[kword][next_symbol];

    return static_cast<double>((symbolCount) + alpha) / (KWordCount + alpha * N_different_symbols);
}

void FiniteContextModel::set_order(int order) {
    this->order = order;

    // Reset the structures
    symbolCounts = std::unordered_map<std::string, std::unordered_map<char, int>>();
    KWordCounts = std::unordered_map<std::string, int>();
}

std::string FiniteContextModel::get_k_word(const std::vector<char>& sequence, int currentIndex) {
    std::string word = "";
    int startIndex = currentIndex - order;

    for (int i = startIndex; i < currentIndex; i++) {
        word += sequence[i];
    }

    return word;
}

#endif  // FINITE_CONTEXT_MODEL_HPP