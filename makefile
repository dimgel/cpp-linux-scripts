build-n-run: build-n-run.cpp
	c++ build-n-run.cpp -o build-n-run -std=c++17 -O2 -fno-rtti -flto -Wall -Wextra

install:
	cp build-n-run /usr/local/bin/

