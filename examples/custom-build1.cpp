#!/usr/local/bin/build-n-run
//!cc= gcc {source} -o {target} -std=c++17 -march=native -O2 -fno-rtti -flto -Wall -Wextra -I~/include
#include <cstdio>

int main() {
    puts("Built using custom command.");
}
