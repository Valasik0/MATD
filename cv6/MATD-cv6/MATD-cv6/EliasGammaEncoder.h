// EliasGammaEncoder.h
#pragma once
#include "Encoder.h"

class EliasGammaEncoder : public Encoder {
public:
    EliasGammaEncoder(const std::vector<uint32_t>& nums);
    std::vector<bool> encode() override;
    std::vector<uint32_t> decode(const std::vector<bool>& sequence) override;
};
