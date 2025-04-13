// FibonacciEncoder.cpp
#include "FibonacciEncoder.h"

FibonacciEncoder::FibonacciEncoder(const std::vector<uint32_t>& nums) : Encoder(nums) {
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

std::vector<bool> FibonacciEncoder::encode() {
    encodedBits.clear();

    for (uint32_t num : nums) {
        if (num == 0) {
            // Speci�ln� p��pad pro 0
            encodedBits.push_back(1);
            encodedBits.push_back(1);
            continue;
        }

        // Vygenerujeme Fibonacciho posloupnost a� do hodnoty num
        std::vector<uint32_t> fib = fibonacci(num);
        
        // Vytvo��me k�dov� slovo (zat�m bez koncov� jedni�ky)
        std::vector<bool> codeword(fib.size(), false);
        
        // K�dujeme ��slo
        uint32_t remainder = num;
        for (int i = fib.size() - 1; i >= 0; i--) {
            if (fib[i] <= remainder) {
                codeword[i] = true;
                remainder -= fib[i];
            }
        }
        
        // P�id�me k�dov� slovo do v�sledku
        for (bool bit : codeword) {
            encodedBits.push_back(bit);
        }
        
        // P�id�me koncovou jedni�ku
        encodedBits.push_back(1);
    }

    return encodedBits;
}

std::vector<uint32_t> FibonacciEncoder::decode(const std::vector<bool>& sequence) {
    std::vector<uint32_t> decoded;
    size_t i = 0;

    while (i < sequence.size()) {
        // Hled�me konec k�dov�ho slova (dv� po sob� jdouc� jedni�ky)
        std::vector<bool> codeword;
        bool prevBit = false;
        
        while (i < sequence.size()) {
            bool bit = sequence[i++];
            codeword.push_back(bit);
            
            if (prevBit && bit) {
                // Na�li jsme dv� po sob� jdouc� jedni�ky
                break;
            }
            
            prevBit = bit;
        }
        
        // Speci�ln� p��pad pro k�d "11" (reprezentuje 0)
        if (codeword.size() == 2 && codeword[0] && codeword[1]) {
            decoded.push_back(0);
            continue;
        }
        
        // Dek�dujeme ��slo
        uint32_t num = 0;
        std::vector<uint32_t> fib = fibonacci(1000000); // Generujeme dostate�n� velkou Fibonacciho posloupnost
        
        // Se�teme Fibonacciho ��sla odpov�daj�c� jedni�k�m v k�dov�m slov�
        // (ignorujeme posledn� jedni�ku, kter� je jen odd�lova�)
        for (size_t j = 0; j < codeword.size() - 1; j++) {
            if (codeword[j] && j < fib.size()) {
                num += fib[j];
            }
        }
        
        decoded.push_back(num);
    }

    return decoded;
}
