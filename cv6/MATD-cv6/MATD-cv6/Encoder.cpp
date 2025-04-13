#include "Encoder.h"
#include <iostream>

Encoder::Encoder(const std::vector<uint32_t>& nums) {
    this->nums = nums;
}

void Encoder::printEncoded() {
    for (bool bit : encodedBits)
        std::cout << bit;
    std::cout << std::endl;
}

size_t Encoder::getEncodedSizeBits() {
    return encodedBits.size();
}

size_t Encoder::getOriginalSizeBits() {
    return sizeof(uint32_t) * nums.size() * 8;
}

