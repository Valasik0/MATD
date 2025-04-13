#pragma once
#include <vector>
#include "Encoder.h"

class UnaryEncoder : public Encoder {
public:
    UnaryEncoder(const std::vector<uint32_t>& nums);
    std::vector<bool> encode() override;
    std::vector<uint32_t> decode(const std::vector<bool>& sequence) override;
};