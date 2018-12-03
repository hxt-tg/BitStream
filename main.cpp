#include <iostream>
#include "bitstream.hpp"

using namespace std;

int main() {
    BitStream bs;
    bs.append_str("123");
    std::cout << bs << std::endl;
    return 0;
}

