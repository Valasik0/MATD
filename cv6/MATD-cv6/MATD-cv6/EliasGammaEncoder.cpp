// EliasGammaEncoder.cpp
#include "EliasGammaEncoder.h"

EliasGammaEncoder::EliasGammaEncoder() : Encoder() {
}

std::vector<bool> EliasGammaEncoder::encode(const std::vector<uint32_t>& data) {
    encodedBits.clear();
    this->nums = data;

    for (uint32_t num : data) {
        // Speciální pøípad pro nulu (není v klasickém Eliasovì gamma kódování)
        if (num == 0) {
            // Mùžeme použít speciální kód pro nulu, napø. "1"
            encodedBits.push_back(1);
            continue;
        }

        // Najdeme délku binární reprezentace
        uint32_t length = 0;
        uint32_t temp = num;
        while (temp > 0) {
            temp >>= 1;
            length++;
        }

        // Unární èást - (length-1) nul následovaných jednièkou
        for (uint32_t i = 0; i < length - 1; i++) {
            encodedBits.push_back(0);
        }
        encodedBits.push_back(1);

        // Binární èást - vynecháme první jednièku (je implicitní)
        for (int i = length - 2; i >= 0; i--) {
            encodedBits.push_back((num >> i) & 1);
        }
    }

    return encodedBits;
}

std::vector<uint32_t> EliasGammaEncoder::decode(const std::vector<bool>& sequence) {
    std::vector<uint32_t> decodedNumbers;
    size_t i = 0;

    while (i < sequence.size()) {
        if (i < sequence.size() && sequence[i] == 1 &&
            (i + 1 == sequence.size() || sequence[i + 1] == 1)) {
            decodedNumbers.push_back(0);
            i++;
            continue;
        }

        size_t unaryLength = 0;
        while (i < sequence.size() && sequence[i] == 0) {
            unaryLength++;
            i++;
        }

        if (i < sequence.size()) {
            i++;
        }
        else {
            break;
        }

        uint32_t number = 1; 

        for (size_t j = 0; j < unaryLength; j++) {
            if (i < sequence.size()) {
                number = (number << 1) | sequence[i];
                i++;
            }
            else {
                break;
            }
        }

        decodedNumbers.push_back(number);
    }

    return decodedNumbers;
}
