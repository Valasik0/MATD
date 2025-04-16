// EliasGammaEncoder.cpp
#include "EliasGammaEncoder.h"

EliasGammaEncoder::EliasGammaEncoder() : Encoder() {
}

std::vector<bool> EliasGammaEncoder::encode(const std::vector<uint32_t>& data) {
    encodedBits.clear();
    this->nums = data;

    for (uint32_t num : data) {
        if (num == 0) {
            encodedBits.push_back(1);
            continue;
        }

        //zjistim delku v bitech (binarni posun)
        uint32_t length = 0;
        uint32_t temp = num;
        while (temp > 0) {
            temp >>= 1;
            length++;
        }

        //unarni cast - (length-1) nul nasledovanych jednickou
        for (uint32_t i = 0; i < length - 1; i++) {
            encodedBits.push_back(0);
        }
        encodedBits.push_back(1);

        //binarni reprezentace cisla - vynecham jednicku
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
        //podminka pro nulu
        if (i < sequence.size() && sequence[i] == 1 &&
            (i + 1 == sequence.size() || sequence[i + 1] == 1)) {
            decodedNumbers.push_back(0);
            i++;
            continue;
        }

        //pocitam nuly a posouvam se
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

        //jsem v binarni casti a prevadim z bin do dec
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
