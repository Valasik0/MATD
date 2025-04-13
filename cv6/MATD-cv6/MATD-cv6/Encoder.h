#pragma once
#include <vector>

class Encoder {
protected:
    std::vector<bool> encodedBits;
    std::vector<uint32_t> nums;

public:
    Encoder();
    virtual ~Encoder() = default;

    virtual std::vector<bool> encode(const std::vector<uint32_t>& data) = 0;
    virtual std::vector<uint32_t> decode(const std::vector<bool>& sequence) = 0;

    void printEncoded();
    size_t getEncodedSizeBits();
    size_t getOriginalSizeBits();
};

