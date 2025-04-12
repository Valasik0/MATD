#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "UnaryEncoder.h"
#include "EliasGammaEncoder.h"

int main() {
	std::vector<uint32_t> nums = {1, 2, 3, 4, 5};
	std::vector<bool> sequence = {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	UnaryEncoder ue = UnaryEncoder(nums);
	ue.encode();
	ue.printEncoded();
	
	std::cout << "Original size in bits: " << ue.getOriginalSizeBits() << std::endl;
	std::cout << "Encoded size in bits: " << ue.getEncodedSizeBits() << std::endl;

	std::cout << "Decoded sequence: ";
	for (uint32_t val : ue.decode(sequence))
		std::cout << val;

	std::cout << "------------------------------" << std::endl;

	EliasGammaEncoder eg = EliasGammaEncoder(nums);
	eg.encode();


	return 0;
}