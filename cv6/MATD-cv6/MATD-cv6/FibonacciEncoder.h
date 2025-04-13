#pragma once
#include <vector>

class FibonacciEncoder
{
private:
	std::vector<uint32_t> nums;
	std::vector<bool> encodedBits;

public:
	FibonacciEncoder(std::vector<uint32_t> nums);
	std::vector<bool> encode();
	std::vector<uint32_t> decode(std::vector<bool> sequence);
	void printEncoded();
	size_t getEncodedSizeBits();
	size_t getOriginalSizeBits();
};


