#pragma once
#include <vector>

class UnaryEncoder {
private:
    std::vector<uint32_t> nums;
    std::vector<bool> encodedBits;

public:
    UnaryEncoder(std::vector<uint32_t> nums);

    void encode();
    std::vector<uint32_t> decode(std::vector<bool> sequence);
    void printEncoded();
    size_t getEncodedSizeBits();
    size_t getOriginalSizeBits();
};