// This file is part of a crypto library, exam for the Algorithms class.
// All of these files are under MIT license.
//
// SHA declaration document:
// https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

#ifndef HMACSHA256_SHA256_H_
#define HMACSHA256_SHA256_H_

#include<cstdint>
#include<array>

namespace hmacsha256 {
class SHA256 {
public:
    // create a new SHA256 thanks an array of bytes
    SHA256(const uint8_t*, uint32_t);

    SHA256(const std::string&);

    ~SHA256();

    // get digest as array of bytes
    uint8_t* digest();

    std::string hexdigest();
private:
    uint8_t* digest_;

    // work variables: a, b, c, d, e, f, g, h
    uint32_t vars_[8];

    // data message, in bytes
    // M,  "message to be hashed"
    uint8_t message_[64];

    // length L of the original message
    uint32_t message_l_;

    // lengh of bits
    uint64_t bit_len_;

    // first 80 costansts 64bit words, paragraph 4.2.2
    std::array<uint32_t, 64> K = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
        0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
        0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
        0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
        0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
        0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
        0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
        0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
        0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    // the constructor
    void init(const uint8_t* data, uint32_t length);

    // Ch(x, y, z) = (x | y) ^ (~x & z)
    static uint32_t ch(uint32_t, uint32_t, uint32_t);

    // Maj(x, y, z) = (x | y) ^ (x & z) ^ (y & z)
    static uint32_t maj(uint32_t, uint32_t, uint32_t);

    // circular right shift. Takes two params: `x` a w-bit word and `n` an int
    // with 0 <= n < w.
    // w, in our case is 32
    //
    // rots is also defined in the NIST document as 
    // rotr(x) = (x >> n) or (x << w - n)
    static uint32_t rotr(uint32_t, uint32_t);

    // function defined in 4.4
    static uint32_t big_sigma0(uint32_t);

    // function defined in 4.5
    static uint32_t big_sigma1(uint32_t);

    // function defined in 4.6
    static uint32_t sigma0(uint32_t);

    // function defined in 4.7
    static uint32_t sigma1(uint32_t);

    // padding
    void pad();

    // main function
    void transform();

    // transform bytes in big-endian, because SHA256 wants big endian ordering
    // Takes 1 parameter, the array of bytes that we want to reorder
    void bigendian(uint8_t*);
};

} // namespace hmacsha256

#endif // HMACSHA256_SHA256_H_
