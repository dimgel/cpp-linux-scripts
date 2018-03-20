// How to build with gcc-7.3.0: 
// # c++ build-n-run.cpp -lstdc++fs -o /usr/local/bin/build-n-run

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string>
#include <vector>
#include <experimental/filesystem>

using namespace std;
using namespace std::experimental::filesystem;


// No such thing in stdlib, and I don't want to link boost.
string replaceAll(string s, const string& search, const string& replace) {
	auto pos = s.find(search);
	while (pos != string::npos) {
		s.replace(pos, search.size(), replace);
		pos = s.find(search, pos + replace.size());
	}
	return s;
}


// I call gcc via `system(3)` which starts shell. Slow. OK for compilation, but not for running compiled script.
int fastExec(const char* cmd, char** argv) {
	auto pid = fork();
	if (pid == -1) {
		fputs("ERROR: fork() failed", stderr);
		exit(1);
	}
	if (pid == 0) {
		// Returns only if fails. Screw errno until needed.
		execv(cmd, argv);
		fputs("ERROR: execv() failed", stderr);
		exit(1);
	} else {
		int status;
		if (waitpid(pid, &status, 0) == -1) {
			fputs("ERROR: waitpid() failed", stderr);
			exit(1);
		}
		return status;
	}
}


int main(int argc, char** argv) {
	path me(*argv++);
	if (argc < 2) {
		fprintf(stderr, "ERROR: Usage: %s SCRIPT.cpp [ARG...]\n", me.filename().c_str());
		exit(1);
	}
	
	path source(*argv);
	if (source.extension() != ".cpp") {
		fputs("ERROR: Given script must have .cpp extension", stderr);
		exit(1);
	}
	
	char* home = getenv("HOME");
	if (home == nullptr || *home == '\0') {
		fputs("ERROR: HOME environment variable is not set", stderr);
		exit(1);
	}
	path target(string(home) + "/.cache/" + me.filename().string() + "/" + replaceAll(canonical(source), "/", "--").substr(2));
	target.replace_extension();
	
	if (!exists(target) || last_write_time(source) > last_write_time(target)) {
		puts("Recompiling...");
		
		create_directories(target.parent_path());
		
		// https://stackoverflow.com/a/1003654/4247442
		string cmd = "tail -n +2 \"" + source.string() + "\" | c++ -o \"" + target.string() + "\" -xc++ -Wall -lstdc++fs -";
		if (system(cmd.c_str()) != 0) {
			exit(1);
		}
	}
	
	return fastExec(target.c_str(), argv);
}
