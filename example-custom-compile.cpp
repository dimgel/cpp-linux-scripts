#!/usr/local/bin/build-n-run
//!cc= gcc {source} -o {target} -std=c++17 -march=native -O2 -fno-rtti -flto -Wall -Wextra -I~/include
#include <stdio.h>

int main() {
    puts("Hello WORLD!");
}
