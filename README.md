# Writing linux scripts in C++

Disclaimer: please stop telling me this is bad idea or bad implementation. I was just trying C++ filesystem API; had fun setting executable bit on a `.cpp` file; haven't yet created (and so far have no plans to create) any useful C++ scripts; uploaded it to github because was asked to; and was **very** surprised to get huge feedback, enhancement requests and even pull requests O_O for such a trivial next-to-hello-world program.

BTW, anyone needs remote Software Developer? Middle C++, or Senior Java / Scala / PHP / fullstack web. Resume: [http://dimgel.me/en/Dmitry_Grigoriev_resume_en-short.pdf](http://dimgel.me/en/Dmitry_Grigoriev_resume_en-short.pdf)

## In short

1. Compile and install `build-n-run.cpp`: `make && sudo make install`.

2. Create C++ source file, prepend it with shebang line `#!/usr/local/bin/build-n-run`, make it executable `chmod +x script.cpp` and run: `./script.cpp`. An example `hello.cpp` script is provided.

3. `build-n-run` compiles `script.cpp` to `~/.cache/build-n-run/` **only if source file is newer than compiled binary**, and then runs compiled binary.

## Concept

They say compiled languages are inconvenient for scripting because you have to recompile your script everytime you edit it. A little program `build-n-run.cpp` presented in this repo solves the problem.

In bash scripts, first line always starts with "[shebang](https://en.wikipedia.org/wiki/Shebang_(Unix))" followed by name of a bash interpreter. When you run bash script, effectively bash interpreter is run and this script is passed to it as first argument.

        #!/bin/bash
        echo "Hello world!"

Same approach is used for Python scripts, Scala scripts, etc. So I did the same: example script `hello.cpp` in this repo is normal C++ program prepended with shebang line:

        #!/usr/local/bin/build-n-run
        #include <stdio.h>
        int main() {
            puts("Hello world!");
        }

`build-n-run` automatically recompiles script **if it is new or edited** (i.e. if compiled binary does not exist or if source file's modification time is newer than binary's) and then runs it:

        $ hello.cpp
        Recompiling...    <--- First time you run your script, it's recompiled.
        Hello world!

        $ hello.cpp
        Hello world!      <--- No recompilation on next run if script is unchanged.

## Installation and use

1. Download `build-n-run.cpp` from this repo.

2. (Optional) Check its source code: you might want to update GCC options used to compile your scripts, or remove "Recompiling..." message if it messes with our scripts' output.

3. Build and install it. `make install` copies binary to `/usr/local/bin`; you can copy it anywhere you want, just don't forget to update path in scripts' shebang line. I use gcc 7.3.0, so I explicitly link `libstdc++fs.a` which implements `std::experimental::filesystem`.

        # make && sudo make install

That's all. Now write C++ program with shebang line, **chmod it to be executable**, and run:

        $ chmod +x hello.cpp
        $ ./hello.cpp
        Recompiling...
        Hello world!

Let's take a look at `~/.cache/build-n-run` directory:

        $ ls ~/.cache/build-n-run
        home--me--tmp--hello

I downloaded `hello.cpp` to `/home/me/tmp`, and compiled binary's name reflects source's full path, slashes replaced with `"--"`.

## Security concerns

Compiled cache is inside user's home, so scripts compiled by one user cannot be run by other users (see issue #1).

## Known discussions

- [linux.org.ru](https://www.linux.org.ru/forum/development/14090221) (Russian)

- [reddit.com](https://www.reddit.com/r/linux/comments/85gaqy/writing_linux_scripts_in_c/)
