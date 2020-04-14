#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <unistd.h>
#include <wait.h>

namespace fs = std::filesystem;

using std::string;
using fs::path;


// No such thing in stdlib, and I don't want to link boost.
string replaceAll(string s, const string& search, const string& replace) {
	auto pos = s.find(search);
	while (pos != string::npos) {
		s.replace(pos, search.size(), replace);
		pos = s.find(search, pos + replace.size());
	}
	return s;
}


// https://stackoverflow.com/a/2602060/4247442
string readFile(const path& p) {
	std::ifstream f(p);
	string s;
	f.seekg(0, std::ios::end);
	s.reserve(f.tellg());
	f.seekg(0, std::ios::beg);
	// Extra parentheses around the first argument is mandatory! See link above.
	s.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	return s;
}


// I call gcc via `system(3)` which starts shell. It's slow; OK for compilation, but not for running compiled script.
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
		status = WIFEXITED(status) ? WEXITSTATUS(status) : 127;
		printf("DEBUG: script returned status = %d\n", status);
		return status;
	}
}


int main(int argc, char** argv) {
	path me(*argv++);
	if (argc < 2) {
		fprintf(stderr, "ERROR: Usage: %s SCRIPT.cpp [ARG...]\n", me.filename().c_str());
		exit(1);
	}
	
	path script(*argv);
	if (script.extension() != ".cpp") {
		fputs("ERROR: Given script must have .cpp extension", stderr);
		exit(1);
	}
	
	char* home0 = getenv("HOME");
	if (home0 == nullptr || *home0 == '\0') {
		fputs("ERROR: HOME environment variable is not set", stderr);
		exit(1);
	}
	string home = home0;

	path target(home + "/.cache/" + me.filename().string() + "/" + replaceAll(canonical(script), "/", "--").substr(2));
	target.replace_extension();
	
	if (!fs::exists(target) || fs::last_write_time(script) > fs::last_write_time(target)) {
//		puts("Recompiling...");

		// GCC fails to link std::experimental::filesystem when compiles from stdin. 
		// So I'll store it into temporary source file, without first shebang line.
		// I don't use tmpfile() because I need temp file name.
		path source(target.string() + ".tmp.cpp");

		// If script's 2nd line starts with "//!cc=", it's custom compile command.
		string s = readFile(script);
		string cc;
		{
			std::regex r("^#![^\r\n]+\r?\n//!cc[ \t]*=[ \t]*([^\r\n]+)\r?\n.*$", std::regex::extended);
			std::smatch m;
			if (std::regex_match(s, m, r)) {
				cc = m[1];
			} else {
				cc = "c++ {source} -o {target} -std=c++17 -O2 -Wall";
			}
		}
		cc = replaceAll(cc, "{source}", "\"" + source.string() + "\"");
		cc = replaceAll(cc, "{target}", "\"" + target.string() + "\"");
		cc = replaceAll(cc, " -I~/", " -I" + home + "/");
		cc = replaceAll(cc, " -L~/", " -L" + home + "/");
		
		create_directories(target.parent_path());

		// Write to temporary source file, without first shebang line.
		std::ofstream(source, std::ios::out | std::ios::trunc) << s.substr(s.find('\n') + 1);

		// Just in case, how to make GCC read source from pipe: https://stackoverflow.com/a/1003654/4247442
//		puts(cc.c_str());
		int result = system(cc.c_str());
		remove(source);
		if (result != 0) {
			exit(1);
		}
	}
	
	return fastExec(target.c_str(), argv);
}
