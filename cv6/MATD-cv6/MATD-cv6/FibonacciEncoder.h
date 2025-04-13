// FibonacciEncoder.h
#pragma once
#include "Encoder.h"

class FibonacciEncoder : public Encoder {
private:
    std::vector<uint32_t> fibonacci(uint32_t max);

public:
    FibonacciEncoder();
    std::vector<bool> encode(const std::vector<uint32_t>& data) override;
    std::vector<uint32_t> decode(const std::vector<bool>& sequence) override;
};
