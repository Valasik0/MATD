#include "EliasGammaEncoder.h"
#include <iostream>
#include <bitset>


EliasGammaEncoder::EliasGammaEncoder(std::vector<uint32_t> nums) {
	this->nums = nums;
}

size_t EliasGammaEncoder::getOriginalSizeBits() {
	size_t size = sizeof(uint32_t) * nums.size() * 8;
	return size;
}

std::vector<bool> EliasGammaEncoder::encode() {
    encodedBits.clear();

	for (uint32_t num : nums) {
		std::bitset<32> binary(num);
		std::string binaryString = binary.to_string();
		binaryString = binaryString.substr(binaryString.find('1'));

		uint32_t n = binaryString.length();
		std::string unaryPart(n - 1, '0');

		std::string eliasGammaNumber = unaryPart + binaryString;

		for (char bit : eliasGammaNumber) {
			encodedBits.push_back(bit - '0');
		}
	}

    return encodedBits;
}

std::vector<uint32_t> EliasGammaEncoder::decode(std::vector<bool> sequence) {
    std::vector<uint32_t> decodedNumbers;
    size_t i = 0;

    while (i < sequence.size()) {
        size_t unaryLength = 0;
        while (i < sequence.size() && sequence[i] == 0) {
            unaryLength++;
            i++;
        }
        size_t binaryLength = unaryLength + 1;

        uint32_t number = 0;
        for (size_t j = 0; j < binaryLength; j++) {
            if (i < sequence.size()) {
                number = (number << 1) | sequence[i];
                i++;
            }
        }
        decodedNumbers.push_back(number);
    }

    return decodedNumbers;
}

void EliasGammaEncoder::printEncoded() {
	for (bool bit : encodedBits)
		std::cout << bit;
	std::cout << std::endl;
}

size_t EliasGammaEncoder::getEncodedSizeBits() {
	return encodedBits.size();
}
