#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdlib> 
#include <functional>
#include <chrono>
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
		int rnd_doc = rand() % docCount + 1;

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

std::vector<uint32_t> deltaDecode(const std::vector<uint32_t>& deltas) {
	std::vector<uint32_t> result;
	if (deltas.empty()) return result;;

	result.push_back(deltas[0]);

	for (size_t i = 1; i < deltas.size(); i++) {
		result.push_back(result.back() + deltas[i]);
	}

	return result;
}

std::unordered_map<std::string, std::vector<bool>> compressIndex(
	std::unordered_map<std::string, std::vector<uint32_t>>& index,
	Encoder& encoder,
	size_t& sizeBefore,
	size_t& sizeAfter)
{
	sizeBefore = 0;
	sizeAfter = 0;
	std::unordered_map<std::string, std::vector<bool>> compressed;

	applyDeltaEncoding(index);

	for (auto& entry : index) {
		const std::string& word = entry.first;
		const std::vector<uint32_t>& deltas = entry.second;

		std::vector<bool> encoded = encoder.encode(deltas);
		compressed[word] = encoded;

		sizeBefore += encoder.getOriginalSizeBits();
		sizeAfter += encoder.getEncodedSizeBits();
		//std::cout << encoder.getOriginalSizeBits() << " " << encoder.getEncodedSizeBits() << std::endl;
	}

	return compressed;
}

std::unordered_map<std::string, std::vector<uint32_t>> decompressIndex(
	std::unordered_map<std::string, std::vector<bool>>& compressedIndex,
	Encoder& encoder)
{	
	std::unordered_map<std::string, std::vector<uint32_t>> decompressed;

	for (const auto& entry : compressedIndex) {
		const std::string& word = entry.first;
		const std::vector<bool>& bitSequence = entry.second;

		std::vector<uint32_t> decoded = encoder.decode(bitSequence);
		decompressed[word] = deltaDecode(decoded);
	}

	return decompressed;
}

bool findDocIDInIndex(const std::unordered_map<std::string, std::vector<uint32_t>>& index, uint32_t docID) {
	for (const auto& entry : index) {
		const std::vector<uint32_t>& docs = entry.second;
		if (std::find(docs.begin(), docs.end(), docID) != docs.end()) {
			return true;
		}
	}
	return false;
}

int main() {
	
	std::vector<uint32_t> nums = {0, 1, 2, 3, 5, 4, 8,20,100 };
	UnaryEncoder ue = UnaryEncoder();
	std::vector<bool> encodedUe = ue.encode(nums);
	ue.printEncoded();

	std::cout << "Original size in bits: " << ue.getOriginalSizeBits() << std::endl;
	std::cout << "Encoded size in bits: " << ue.getEncodedSizeBits() << std::endl;

	std::cout << "Decoded sequence: ";
	for (uint32_t val : ue.decode(encodedUe))
		std::cout << val << " ";

	std::cout << std::endl << "------------------------------" << std::endl;

	EliasGammaEncoder eg = EliasGammaEncoder();
	std::vector<bool> encoded = eg.encode(nums);
	eg.printEncoded();

	std::cout << "Original size in bits: " << eg.getOriginalSizeBits() << std::endl;
	std::cout << "Encoded size in bits: " << eg.getEncodedSizeBits() << std::endl;

	std::cout << "Decoded sequence: ";
	for (uint32_t val : eg.decode(encoded))
		std::cout << val << " ";

	std::cout << std::endl << "------------------------------" << std::endl;

	FibonacciEncoder fe = FibonacciEncoder();
	std::vector<bool> encodedFib = fe.encode(nums);
	fe.printEncoded();

	std::cout << "Original size in bits: " << fe.getOriginalSizeBits() << std::endl;
	std::cout << "Encoded size in bits: " << fe.getEncodedSizeBits() << std::endl;

	std::cout << "Decoded sequence: ";
	for (uint32_t val : fe.decode(encodedFib))
		std::cout << val << " ";

	std::cout << std::endl << "------------------------------" << std::endl;

	std::vector<std::string> words = generateRandomWords(1000, 3, 6);
	auto pairs = generateWordDocPairs(words, 1000, 1000000);

	std::unordered_map<std::string, std::vector<uint32_t>> invertedIndex;
	buildInvertedIndex(pairs, invertedIndex);	

	//printInvertedIndex(invertedIndex);

	UnaryEncoder ueTest = UnaryEncoder();
	FibonacciEncoder feTest = FibonacciEncoder();
	EliasGammaEncoder egTest = EliasGammaEncoder();

	size_t sizeBefore = 0;
	size_t sizeAfter = 0;

	auto compressedIndexUE = compressIndex(invertedIndex, ueTest, sizeBefore, sizeAfter);

	/*
	std::cout << "\nCompressed index:\n";
	for (const auto& entry : compressedIndex) {
		std::cout << entry.first << ": ";
		for (bool bit : entry.second) {
			std::cout << bit;
		}
		std::cout << std::endl;
	}*/

	auto decompressedIndexUE = decompressIndex(compressedIndexUE, ueTest);
	/*
	std::cout << "\nDecompressed index:\n";
	for (const auto& entry : decompressedIndex) {
		std::cout << entry.first << ": ";
		for (uint32_t id : entry.second) {
			std::cout << id << " ";
		}
		std::cout << std::endl;
	}*/
	std::cout << "Unary encoder: " << std::endl;
	std::cout << "Before compression: " << sizeBefore << "b" << std::endl;
	std::cout << "After compression: " << sizeAfter << "b" << std::endl;

	sizeBefore = 0;
	sizeAfter = 0;

	auto compressedIndexFE = compressIndex(invertedIndex, feTest, sizeBefore, sizeAfter);
	auto decompressedIndexFE = decompressIndex(compressedIndexFE, feTest);

	std::cout << "Fibonacci encoder: " << std::endl;
	std::cout << "Before compression: " << sizeBefore << "b" << std::endl;
	std::cout << "After compression: " << sizeAfter << "b" << std::endl;
	

	sizeBefore = 0;
	sizeAfter = 0;

	auto compressedIndexEG= compressIndex(invertedIndex, egTest, sizeBefore, sizeAfter);
	auto decompressedIndexEG = decompressIndex(compressedIndexFE, egTest);
	std::cout << "Elias gamma encoder: " << std::endl;
	std::cout << "Before compression: " << sizeBefore << "b" << std::endl;
	std::cout << "After compression: " << sizeAfter << "b" << std::endl;

	

	return 0;
}