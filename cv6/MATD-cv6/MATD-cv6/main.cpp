#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdlib> 
#include <functional>
#include "UnaryEncoder.h"
#include "EliasGammaEncoder.h"
#include "FibonacciEncoder.h"
#include "Encoder.h"

std::vector<std::string> generateRandomWords(int count, int min_len, int max_len) {
	std::vector<std::string> words;
	const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < count; i++) {
		std::string tmp_s;
		int rnd_len = min_len + (rand() % (max_len - min_len + 1));
		tmp_s.reserve(rnd_len);

		for (int i = 0; i < rnd_len; ++i) {
			tmp_s += alphabet[rand() % (sizeof(alphabet) - 1)];
		}

		words.push_back(tmp_s);
	}

	return words;
}

std::vector<std::pair<std::string, uint32_t>> generateWordDocPairs(
	std::vector<std::string>& words, 
	int docCount, 
	int pairCount) {

	std::vector<std::pair<std::string, uint32_t>> pairs;

	for (int i = 0; i < pairCount; i++) {
		int rnd_idx_word = rand() % (words.size());
		int rnd_doc = rand() % docCount;

		pairs.push_back({ words[rnd_idx_word], rnd_doc });
	}

	return pairs;
}

void buildInvertedIndex(
	std::vector<std::pair<std::string, uint32_t>>& pairs,
	std::unordered_map<std::string, std::vector<uint32_t>>& invertedIndex) {

	for (auto& pair : pairs) {
		std::string& word = pair.first;
		uint32_t docID = pair.second;
		invertedIndex[word].push_back(docID);
	}
}

std::vector<uint32_t> deltaEncode(const std::vector<uint32_t>& sorted) {
	std::vector<uint32_t> deltas;
	if (sorted.empty()) return deltas;

	deltas.push_back(sorted[0]);
	for (size_t i = 1; i < sorted.size(); i++) {
		deltas.push_back(sorted[i] - sorted[i - 1]);
	}
	return deltas;
}

void printInvertedIndex(std::unordered_map<std::string, std::vector<uint32_t>>& invertedIndex) {
	std::cout << "\nInverted index:\n";
	for (auto idx : invertedIndex) {
		std::cout << idx.first << ": ";
		for (uint32_t id : idx.second) {
			std::cout << id << " ";
		}
		std::cout << std::endl;
	}
}

void applyDeltaEncoding(std::unordered_map<std::string, std::vector<uint32_t>>& index) {
	for (auto& entry : index) {
		std::vector<uint32_t>& docIDs = entry.second;
		std::sort(docIDs.begin(), docIDs.end());
		docIDs = deltaEncode(docIDs);
	}

}

int main() {
	std::vector<uint32_t> nums = { 1, 2, 3, 4, 5 };
	std::vector<bool> sequence = { 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UnaryEncoder ue = UnaryEncoder(nums);
	ue.encode();
	ue.printEncoded();

	std::cout << "Original size in bits: " << ue.getOriginalSizeBits() << std::endl;
	std::cout << "Encoded size in bits: " << ue.getEncodedSizeBits() << std::endl;

	std::cout << "Decoded sequence: ";
	for (uint32_t val : ue.decode(sequence))
		std::cout << val;

	std::cout << std::endl << "------------------------------" << std::endl;

	EliasGammaEncoder eg = EliasGammaEncoder(nums);
	std::vector<bool> encoded = eg.encode();
	eg.printEncoded();

	std::cout << "Original size in bits: " << eg.getOriginalSizeBits() << std::endl;
	std::cout << "Encoded size in bits: " << eg.getEncodedSizeBits() << std::endl;

	std::cout << "Decoded sequence: ";
	for (uint32_t val : eg.decode(encoded))
		std::cout << val;

	std::cout << std::endl << "------------------------------" << std::endl;

	FibonacciEncoder fe(nums);
	std::vector<bool> encodedFib = fe.encode();
	fe.printEncoded();

	std::cout << "Original size in bits: " << fe.getOriginalSizeBits() << std::endl;
	std::cout << "Encoded size in bits: " << fe.getEncodedSizeBits() << std::endl;

	std::cout << "Decoded sequence: ";
	for (uint32_t val : fe.decode(encodedFib))
		std::cout << val << " ";
	std::cout << std::endl;

	std::cout << std::endl << "------------------------------" << std::endl;

	std::vector<std::string> words = generateRandomWords(20, 3, 6);
	auto pairs = generateWordDocPairs(words, 20, 25);

	std::unordered_map<std::string, std::vector<uint32_t>> invertedIndex;
	buildInvertedIndex(pairs, invertedIndex);	

	printInvertedIndex(invertedIndex);

	applyDeltaEncoding(invertedIndex);

	printInvertedIndex(invertedIndex);

	return 0;
}