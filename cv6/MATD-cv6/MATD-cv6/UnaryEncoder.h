#pragma once
#include <vector>
#include "Encoder.h"

class UnaryEncoder : public Encoder {
public:
    UnaryEncoder();
    std::vector<bool> encode(const std::vector<uint32_t>& data) override;
    std::vector<uint32_t> decode(const std::vector<bool>& sequence) override;
};