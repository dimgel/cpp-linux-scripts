# Writing linux scripts in C++

## Concept

### In short (for experienced linux users)

- Create C++ source file, prepend it with shebang line `#!/path/to/build-n-run`, make it executable `chmod +x script.cpp` and run: `./script.cpp`.

- `build-n-run` compiles `script.cpp` to `~/bin/build-n-run.compiled/` **only if source file is newer than compiled binary**, and then runs compiled binary.

### In detail

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

3. Compile and install it. I install to `/usr/local/bin`, so I run this command as root; you can install it anywhere you want, just don't forget to update path in scripts' shebang line. I use gcc 7.3.0, so I explicitly link `libstdc++fs.a` which implements `std::experimental::filesystem`.

        # g++ build-n-run.cpp -lstdc++fs -o /usr/local/bin/build-n-run

That's all. Now write C++ program with shebang line, **chmod it to be executable**, and run:

        $ chmod +x hello.cpp
        $ ./hello.cpp
        Recompiling...
        Hello world!

Let's take a look at `~/bin/build-n-run.compiled` directory:

        $ ls ~/bin/build-n-run.compiled
        home--me--tmp--hello

I downloaded `hello.cpp` to `/home/me/tmp`, and compiled binary's name reflects source's full path, slashes replaced with `"--"`.

## Security concerns

Compiled cache is inside user's home, so scripts compiled by one user cannot be run by other users (see issue #1).
