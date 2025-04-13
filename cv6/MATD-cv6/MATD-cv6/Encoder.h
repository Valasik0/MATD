#pragma once
#include <vector>

class Encoder {
protected:
    std::vector<uint32_t> nums;
    std::vector<bool> encodedBits;

public:
    Encoder(const std::vector<uint32_t>& nums);
    virtual ~Encoder() = default;

    virtual std::vector<bool> encode() = 0;
    virtual std::vector<uint32_t> decode(const std::vector<bool>& sequence) = 0;

    void printEncoded();
    size_t getEncodedSizeBits();
    size_t getOriginalSizeBits();
    void setData(const std::vector<uint32_t>& newData) {
        nums = newData;
        encodedBits.clear();
    }
};

