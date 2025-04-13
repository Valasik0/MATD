// UnaryEncoder.cpp
#include "UnaryEncoder.h"

UnaryEncoder::UnaryEncoder(const std::vector<uint32_t>& nums) : Encoder(nums) {
}

std::vector<bool> UnaryEncoder::encode() {
    encodedBits.clear();
    for (uint32_t num : nums) {
        for (uint32_t i = 0; i < num; ++i)
            encodedBits.push_back(0);
        encodedBits.push_back(1);
    }
    return encodedBits;
}

std::vector<uint32_t> UnaryEncoder::decode(const std::vector<bool>& sequence) {
    std::vector<uint32_t> decoded;
    uint32_t count = 0;
    for (bool bit : sequence) {
        if (bit == 0) {
            count++;
        }
        else {
            decoded.push_back(count);
            count = 0;
        }
    }
    if (count > 0) {
        decoded.push_back(count);
    }
    return decoded;
}
