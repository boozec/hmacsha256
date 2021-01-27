#include "sha256.h"
#include<cstring> // used for memset
#include<sstream> // used for stringstream
#include<iomanip> // used for setfill and setw

hmacsha256::SHA256::SHA256(const uint8_t* data, uint32_t length) {
    init(data, length);
}

void hmacsha256::SHA256::init(const uint8_t* data, uint32_t length) {
    digest_ = new uint8_t[32];
    message_l_ = bit_len_ = 0;
    vars_[0] = 0x6a09e667;
    vars_[1] = 0xbb67ae85;
    vars_[2] = 0x3c6ef372;
    vars_[3] = 0xa54ff53a;
    vars_[4] = 0x510e527f;
    vars_[5] = 0x9b05688c;
    vars_[6] = 0x1f83d9ab;
    vars_[7] = 0x5be0cd19;

    for(size_t i = 0; i < length; ++i) {
        message_[message_l_++] = data[i];
        if(message_l_ == 64) {
            transform();

            bit_len_ += 512;
            message_l_ = 0;
        }
    }
}

hmacsha256::SHA256::~SHA256() {
    delete digest_;
}

hmacsha256::SHA256::SHA256(const std::string& data) {
    init(reinterpret_cast<const uint8_t*> (data.c_str()), data.size());
}

uint8_t* hmacsha256::SHA256::digest() {
    uint8_t* hash = new uint8_t[32];

    pad();
    bigendian(hash);

    for(uint32_t i = 0; i < 32; ++i) {
        digest_[i] = hash[i];
    }

    return hash;
}

std::string hmacsha256::SHA256::hexdigest() {
    std::stringstream s;
    s << std::setfill('0') << std::hex;

    for(uint8_t i = 0 ; i < 32 ; i++) {
        s << std::setw(2) << (unsigned int) digest_[i];
    }

    return s.str();
}

uint32_t hmacsha256::SHA256::ch(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

uint32_t hmacsha256::SHA256::maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & (y | z)) | (y & z);
}

uint32_t hmacsha256::SHA256::rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32-n));
}

uint32_t hmacsha256::SHA256::big_sigma0(uint32_t x) {
    return SHA256::rotr(x, 2) ^ SHA256::rotr(x, 13) ^ SHA256::rotr(x, 22);
}

uint32_t hmacsha256::SHA256::big_sigma1(uint32_t x) {
    return SHA256::rotr(x, 6) ^ SHA256::rotr(x, 11) ^ SHA256::rotr(x, 25);
}

uint32_t hmacsha256::SHA256::sigma0(uint32_t x) {
    return SHA256::rotr(x, 7) ^ SHA256::rotr(x, 18) ^ (x >> 3);
}

uint32_t hmacsha256::SHA256::sigma1(uint32_t x) {
    return SHA256::rotr(x, 17) ^ SHA256::rotr(x, 19) ^ (x >> 10);
}

void hmacsha256::SHA256::transform() {
    uint32_t m[64];
    uint32_t h[8];

    for(uint8_t i = 0, j = 0; i < 16; ++i, j += 4) {
        m[i] = (message_[j] << 24) | (message_[j + 1] << 16) | (message_[j + 2] << 8) | (message_[j + 3]);
    }

    for(uint8_t i = 16; i < 64; ++i) {
        m[i] = SHA256::sigma1(m[i - 2]) + m[i - 7] + SHA256::sigma0(m[i - 15]) + m[i - 16];
    }

    for(uint8_t i = 0; i < 8; ++i)
        h[i] = vars_[i];

    for(uint8_t i = 0; i < 64; ++i) {
        auto S1 = SHA256::big_sigma1(h[4]);
        auto S0 = SHA256::big_sigma0(h[0]);
        auto t1 = h[7] + S1 + SHA256::ch(h[4], h[5], h[6]) + K[i] + m[i];
        auto t2 = S0 + SHA256::maj(h[0], h[1], h[2]);
        h[7] = h[6];
        h[6] = h[5];
        h[5] = h[4];
        h[4] = h[3] + t1;
        h[3] = h[2];
        h[2] = h[1];
        h[1] = h[0];
        h[0] = t1 + t2;
    }

    for(uint8_t i = 0; i < 8; ++i) {
        vars_[i]+=h[i];
    }

}

void hmacsha256::SHA256::pad() {
    // start of scanning
    uint64_t i = message_l_;

    // end of scanning
    uint8_t j = (i < 56) ? 56 : 64;

    // append '1' bit at the end of the block
    message_[i++] = 0x80;

    // append '0' bit, j times
    while(i < j) message_[i++] = 0x00;

    if(message_l_ >= 56) {
        transform();
        memset(message_, 0, 56);
    }

    bit_len_ += message_l_ * 8;
    message_[63] = bit_len_;
    message_[62] = bit_len_ >> 8;
    message_[61] = bit_len_ >> 16;
    message_[60] = bit_len_ >> 24;
    message_[59] = bit_len_ >> 32;
    message_[58] = bit_len_ >> 40;
    message_[57] = bit_len_ >> 48;
    message_[56] = bit_len_ >> 56;
    transform();
}

void hmacsha256::SHA256::bigendian(uint8_t* hash) {
    for (uint32_t i = 0; i < 4; ++i) {
        hash[i] = (vars_[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4] = (vars_[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8] = (vars_[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (vars_[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (vars_[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (vars_[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (vars_[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (vars_[7] >> (24 - i * 8)) & 0x000000ff;
    }
}
