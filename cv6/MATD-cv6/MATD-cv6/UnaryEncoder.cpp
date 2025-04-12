#include "UnaryEncoder.h"
#include <iostream>

UnaryEncoder::UnaryEncoder(std::vector<uint32_t> nums) {
    this->nums = nums;
}

size_t UnaryEncoder::getOriginalSizeBits() {
    size_t size = sizeof(uint32_t) * nums.size() * 8;
    return size;
}

void UnaryEncoder::encode() {
    encodedBits.clear();
    for (uint32_t num : nums) {
        for (uint32_t i = 0; i < num; ++i)
            encodedBits.push_back(0);
        encodedBits.push_back(1);
    }
}

std::vector<uint32_t> UnaryEncoder::decode(std::vector<bool> sequence) {
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

void UnaryEncoder::printEncoded() {
    for (bool bit : encodedBits)
        std::cout << bit;
    std::cout << std::endl;
}

size_t UnaryEncoder::getEncodedSizeBits() {
    return encodedBits.size();
}