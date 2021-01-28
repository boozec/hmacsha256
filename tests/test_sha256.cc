#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sha256.h"

TEST_CASE("SHA256 digest", "[format]") {
    auto sha = hmacsha256::SHA256("hello, world");
    sha.digest();

    REQUIRE(sha.hexdigest() == "09ca7e4eaa6e8ae9c7d261167129184883644d07dfba7cbfbc4c8a2e08360d5b");
}
