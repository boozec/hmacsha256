#include<iostream>
#include "sha256.h"

int main(int argc, char** argv) {
    auto sha = hmacsha256::SHA256(argv[1]);
    sha.digest();
    std::cout << sha.hexdigest();

    return 0;
}
