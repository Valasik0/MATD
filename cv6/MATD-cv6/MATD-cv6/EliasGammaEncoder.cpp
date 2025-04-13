// EliasGammaEncoder.cpp
#include "EliasGammaEncoder.h"
#include <bitset>

EliasGammaEncoder::EliasGammaEncoder(const std::vector<uint32_t>& nums) : Encoder(nums) {
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

std::vector<uint32_t> EliasGammaEncoder::decode(const std::vector<bool>& sequence) {
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
