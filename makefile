.PHONY: clean
clean:
	rm -rf target

.PHONY: all
all: target/build-n-run

target/build-n-run: src/build-n-run.cpp
	mkdir -p target
	c++ src/build-n-run.cpp -o target/build-n-run -std=c++17 -O2 -fno-rtti -flto -Wall -Wextra

install:
	$(error Use 'sudo make install-system' or 'make install-user')

install-system:
	mkdir -p /usr/local/bin
	cp target/build-n-run /usr/local/bin/

install-user:
	mkdir -p ~/bin
	cp target/build-n-run ~/bin/
