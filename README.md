# Writing linux scripts in C++

They say compiled languages are inconvenient for scripting because you have to recompile your script everytime you edit it. 
But you don't have to do it manually. Here's the solution for C++.

**Disclaimer.** Don't tell me writing scripts in C++ is bad idea. Don't be dogmatic. All rules have exceptions.

## Idea

In bash scripts, first line always starts with "[shebang](https://en.wikipedia.org/wiki/Shebang_(Unix))" followed by name of a bash interpreter. 
When you run bash script, effectively bash interpreter is started with script's filename in first command-line argument.

        #!/bin/bash
        echo "Hello world!"

Same approach is used for Python scripts, Scala scripts, etc. And I did just the same.

## Howto

1. Clone or download this repo.

2. Build and install `build-n-run.cpp`: `make && sudo make install`. It goes into `/usr/local/bin/`.

3. Create C++ source file, prepend it with shebang line `#!/usr/local/bin/build-n-run`, make it executable `chmod +x script.cpp`. See `example.cpp`.
   
4. You can specify custom build command in script's 2nd line. `build-n-run` expands `~` into current user's home directory in `-I~/...` and `-L~/...` options. 
   See `example-custom-compile.cpp`.

5. Run your script: `./script.cpp`. `build-n-run` will compile it on first run and after everytime you edit / touch it.

Script `/aaa/bbb/ccc.cpp` is compiled into `~/.cache/build-n-run/aaa--bbb--ccc`. If compilation fails, `build-n-run` will exit with status code `1`.

**ATTENTION.** Compiler's output is not redirected. So if compilation succeeded but with warnings, those warnings will still go to `stdio` 
and will prepend your script's output.   
     

## Known discussions

- [linux.org.ru](https://www.linux.org.ru/forum/development/14090221) (Russian)

- [reddit.com](https://www.reddit.com/r/linux/comments/85gaqy/writing_linux_scripts_in_c/)
