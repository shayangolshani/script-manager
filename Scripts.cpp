// Scripts.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <filesystem>
#include "ScriptsUtils.h"
#include "Scripts.h"
/*look up libraries to use to parse arguments in C++*/

int main(int argc, char* argv[]) {
	std::string *input = new std::string[argc];
	for (int i = 0; i < argc; ++i) {
		input[i] = argv[i];
	}
	initScriptList();

	if (argc == 1) {
		printNoArgs();
		return 1;
	}
	if ((input[1] == "-l") || (input[1] == "--list")) {
		listScripts();
	}
	else if (input[1] == "-r" || input[1] == "--run") {
		runScript(input[2]);
	}
	else if (input[1] == "-u" || input[1] == "--upper") {
		std::cout << upperCase(input[2]) << std::endl;
	}
	else {
		printArgumentError();
		return 2;
	}

	return 0;
}

