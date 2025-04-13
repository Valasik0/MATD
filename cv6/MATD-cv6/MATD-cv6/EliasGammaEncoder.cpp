// EliasGammaEncoder.cpp
#include "EliasGammaEncoder.h"

EliasGammaEncoder::EliasGammaEncoder() : Encoder() {
}

std::vector<bool> EliasGammaEncoder::encode(const std::vector<uint32_t>& data) {
    encodedBits.clear();
    this->nums = data;

    for (uint32_t num : data) {
        // Speci�ln� p��pad pro nulu (nen� v klasick�m Eliasov� gamma k�dov�n�)
        if (num == 0) {
            // M��eme pou��t speci�ln� k�d pro nulu, nap�. "1"
            encodedBits.push_back(1);
            continue;
        }

        // Najdeme d�lku bin�rn� reprezentace
        uint32_t length = 0;
        uint32_t temp = num;
        while (temp > 0) {
            temp >>= 1;
            length++;
        }

        // Un�rn� ��st - (length-1) nul n�sledovan�ch jedni�kou
        for (uint32_t i = 0; i < length - 1; i++) {
            encodedBits.push_back(0);
        }
        encodedBits.push_back(1);

        // Bin�rn� ��st - vynech�me prvn� jedni�ku (je implicitn�)
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
