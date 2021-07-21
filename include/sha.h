// This file is part of a crypto library, exam for the Algorithms class.
// All of these files are under MIT license.
//
// SHA declaration document:
// https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

#ifndef HMACSHA256_SHA_H_
#define HMACSHA256_SHA_H_

#include <array>
#include <cstdint>

namespace hmacsha256 {
class SHA {
public:
    SHA()
    {
        digest_ = new uint8_t[32];
    }

    ~SHA()
    {
        delete digest_;
    }

    // get digest as array of bytes
    virtual uint8_t* digest() = 0;

    virtual std::string hexdigest() = 0;

protected:
    uint8_t* digest_;

    // work variables: a, b, c, d, e, f, g, h
    uint32_t vars_[8];

    // data message, in bytes
    // M,  "message to be hashed"
    uint8_t message_[64];

    // length L of the original message
    uint32_t message_l_;

    // lenght of bits
    uint64_t bit_len_;

    // the constructor
    virtual void init(const uint8_t* data, uint32_t length) = 0;

    // padding
    virtual void pad() = 0;

    // main function
    virtual void transform() = 0;
};

} // namespace hmacsha256

#endif // HMACSHA256_SHA_H_
