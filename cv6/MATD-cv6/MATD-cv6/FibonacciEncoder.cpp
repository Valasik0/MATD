#include "FibonacciEncoder.h"
#include <iostream>
#include <bitset>


FibonacciEncoder::FibonacciEncoder(std::vector<uint32_t> nums) {
    this->nums = nums;
}

size_t FibonacciEncoder::getOriginalSizeBits() {
    size_t size = sizeof(uint32_t) * nums.size() * 8;
    return size;
}

std::vector<uint32_t> fibonacci(uint32_t max) {
    std::vector<uint32_t> fib = { 1, 2 };
    while (fib.back() <= max) {
        uint32_t next = fib[fib.size() - 1] + fib[fib.size() - 2];
        fib.push_back(next);
    }
    return fib;
}

std::vector<bool> FibonacciEncoder::encode() {
    encodedBits.clear();

    
    

    return encodedBits;
}

std::vector<uint32_t> FibonacciEncoder::decode(std::vector<bool> sequence) {
    std::vector<uint32_t> decoded;
    

    return decoded;
}

void FibonacciEncoder::printEncoded() {
    for (bool bit : encodedBits)
        std::cout << bit;
    std::cout << std::endl;
}

size_t FibonacciEncoder::getEncodedSizeBits() {
    return encodedBits.size();
}
