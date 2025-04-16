// FibonacciEncoder.cpp
#include "FibonacciEncoder.h"

FibonacciEncoder::FibonacciEncoder() : Encoder() {
}

std::vector<uint32_t> FibonacciEncoder::fibonacci(uint32_t max) {
    std::vector<uint32_t> fib = { 1, 2 };
    while (fib.back() < max) {
        uint32_t next = fib[fib.size() - 1] + fib[fib.size() - 2];
        if (next > max) break;
        fib.push_back(next);
    }
    return fib;
}

std::vector<bool> FibonacciEncoder::encode(const std::vector<uint32_t>& data) {
    encodedBits.clear();
    this->nums = data;

    for (uint32_t num : data) {
        if (num == 0) {
            //pripad pro 0
            encodedBits.push_back(1);
            encodedBits.push_back(1);
            continue;
        }

        //generuju Fib az do num
        std::vector<uint32_t> fib = fibonacci(num);
        
        //alokuju potrebnou delku kodovaneho slova (zatim 0)
        std::vector<bool> codeword(fib.size(), 0);
        
        //koduju cislo jdu odzadu
        uint32_t remainder = num;
        for (int i = fib.size() - 1; i >= 0; i--) {
            if (fib[i] <= remainder) {
                codeword[i] = 1;
                remainder -= fib[i];
            }
        }
        
        for (bool bit : codeword) {
            encodedBits.push_back(bit);
        }

        encodedBits.push_back(1);
    }

    return encodedBits;
}

std::vector<uint32_t> FibonacciEncoder::decode(const std::vector<bool>& sequence) {
    std::vector<uint32_t> decoded;
    size_t i = 0;

    while (i < sequence.size()) {
        // Hledáme konec kódového slova (dvì po sobì jdoucí jednièky)
        std::vector<bool> codeword;
        bool prevBit = false;
        
        while (i < sequence.size()) {
            bool bit = sequence[i++];
            codeword.push_back(bit);
            
            if (prevBit && bit) {
                //dve po sobe jdouci 1
                break;
            }
            
            prevBit = bit;
        }
        
        //pripad pro 0
        if (codeword.size() == 2 && codeword[0] && codeword[1]) {
            decoded.push_back(0);
            continue;
        }
        
        //decoding
        uint32_t num = 0;
        std::vector<uint32_t> fib = fibonacci(1000000);
        
        //sectu fib cisla kde je jedna - igonruju posledni jednicu (oddelovac)
        for (size_t j = 0; j < codeword.size() - 1; j++) {
            if (codeword[j] && j < fib.size()) {
                num += fib[j];
            }
        }
        
        decoded.push_back(num);
    }

    return decoded;
}
