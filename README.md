# Writing linux scripts in C++

## Concept

They say compiled languages are inconvenient for scripting because you have to recompile your script everytime you edit it. A little program `build-n-run.cpp` presented in this repo solves the problem.

In bash scripts, first line always starts with "[shebang](https://en.wikipedia.org/wiki/Shebang_(Unix))" followed by name of a bash interpreter. When you run bash script, effectively bash interpreter is run and this script is passed to it as first argument.

        #!/bin/bash
        echo "Hello world!"

Same approach is used for Python scripts, Scala scripts, etc. So I did the same: file `hello.cpp` in this repo is normal C++ program prepended with shebang line:

        #!/usr/local/bin/build-n-run
        #include <stdio.h>
        int main() {
            puts("Hello world!");
        }

`build-n-run` automatically recompiles script *if it was edited* (by comparing source and binary modification times) and then runs it:

        $ hello.cpp
        Recompiling...    <--- first time you run your script, it's recompiled
        Hello world!

        $ hello.cpp
        Hello world!

## Installation and use

1. Download `build-n-run.cpp` from this repo.

2. [Optional] Check its source code: update GCC options used to compile your scripts; remove "Recompiling..." message if it messes with our scripts' output.

3. Compile and install it. I install to `/usr/local/bin`, so I run this command as root. You can install it anywhere you want, just don't forget to edit path in `hello.cpp`'s shebang line. I use gcc 7.3.0, so I explicitly link `libstdc++fs.a` which implements `std::experimental::filesystem`.

        # g++ build-n-run.cpp -lstdc++fs -o /usr/local/bin/build-n-run

4. Create directory for compiled scripts. Its full path must be the same as installed program's plus `.compiled` suffix. If that directory is shared among multiple users, grant global access to it.

        # mkdir /usr/local/bin/build-n-run.compiled
        # chmod a+rwx /usr/local/bin/build-n-run.compiled

That's all. Now write C++ program with shebang at first line, *chmod it to be executable*, and run:

        $ chmod +x hello.cpp
        $ ./hello.cpp
        Recompiling...
        Hello world!

Now let's take a look at `build-n-run.compiled` directory:

        $ ls /usr/local/bin/build-n-run.compiled
        home--me--tmp--hello

I downloaded `hello.cpp` to `/home/me/tmp`, and compiled binary's name reflects source's full path, slashes replaced with `"--"`.
