// EliasGammaEncoder.h
#pragma once
#include "Encoder.h"

class EliasGammaEncoder : public Encoder {
public:
    EliasGammaEncoder();
    std::vector<bool> encode(const std::vector<uint32_t>& data) override;
    std::vector<uint32_t> decode(const std::vector<bool>& sequence) override;
};
