#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "UnaryEncoder.h"
#include "EliasGammaEncoder.h"
#include "FibonacciEncoder.h"

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

	std::vector<uint32_t> numsFib = { 1, 2, 3, 4, 5, 10, 19 };
	FibonacciEncoder fe(numsFib);
	std::vector<bool> encodedFib = fe.encode();
	fe.printEncoded();

	std::vector<uint32_t> decodedFib = fe.decode(encodedFib);

	std::cout << "Decoded sequence: ";
	for (uint32_t val : decodedFib)
		std::cout << val << " ";
	std::cout << std::endl;

	return 0;
}