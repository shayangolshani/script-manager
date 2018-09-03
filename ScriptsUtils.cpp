
//TODO: migrate to POSIX popen() instead of system() https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix?rq=1
	//	handle arguments for scripts
#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include "ScriptsUtils.h"

namespace fs = std::experimental::filesystem;
std::vector<std::experimental::filesystem::v1::path> paths;
fs::path path("INSERT PATH STRING HERE");
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void printHelp() {
	printf("list possible arguments here\n");
}

void printNoArgs() {
	printf("No arguments found, type 'scripts --help for help\n");
}
void initScriptList() {
	for (std::experimental::filesystem::v1::directory_entry entry : fs::recursive_directory_iterator(path)) {
		if(!fs::is_directory(entry.path()))
			paths.push_back(entry.path());
	}
}
void listScripts() {
	printf("\n");
	/*for (std::experimental::filesystem::v1::directory_entry p : fs::recursive_directory_iterator(path)) {
		if (!fs::is_directory(p.path())) {
			std::experimental::filesystem::v1::path path = p.path();
			std::cout << path.filename() << std::endl;
		}
	}*/
	for (auto path : paths) {
		std::cout << path << std::endl;
	}
	printf("\n");
}

void runScript(std::string scriptName) {
	std::string pathName = findScript(scriptName);
	if (pathName == "NONE FOUND") {
		printf("could not find file\n");
		return;
	}
	else if (pathName == "MULTIPLE FOUND") {
		printf("found multiple files\n");
		return;
	}
	std::string extension = pathName.substr(pathName.find_last_of(".") + 1);
	if (extension == "ps1") { //run powershell
		std::cout << "found powershell script at: " << pathName << std::endl;
		system((std::string("start powershell.exe -noexit -NoProfile -f ") + pathName).c_str());
	}
	else if (extension == "py") { //run python
		std::cout << "found python script at: " << pathName << std::endl;
		std::string version = checkVersionNumber(pathName);
		system((std::string("py -") + version + " " + pathName).c_str());
	}
	else {
		std::cout << "could not recognize extension" << std::endl;
	}
}

//returns the index of the path that script is found in
std::string findScript(std::string scriptName) { //needs to handle multiple finds
	//std::cout << "given name: " << scriptName << std::endl;
	int numFound = 0;
	std::string foundPath;
	scriptName = upperCase(scriptName);
	for (std::experimental::filesystem::v1::path p : paths) {
		//std::cout << "finding in: " << p.string() << std::endl;
		std::string pName = upperCase(p.string());
		size_t index = pName.find(scriptName);
		if (index != std::string::npos) {
			//std::cout << "found!" << std::endl;
			++numFound;
			foundPath = p.string();
		}
	}
	if (numFound == 1)
		return foundPath;
	else if (numFound > 1)
		return "MULTIPLE FOUND";
	else
		return "NONE FOUND";
}

void printArgumentError() {
	printf("could not recognize arguments\n");
}

std::string upperCase(std::string s) {
	std::string copy = s;
	for (int i = 0; i < copy.length(); ++i) {
		copy[i] = toupper(copy[i]);
	}
	return copy;
}

std::string checkVersionNumber(std::string path) {
	std::fstream ifstream(path);
	std::string firstLine;
	ifstream >> firstLine;
	if (firstLine.substr(0, 2) != "#!")
		return "-1";
	int index = firstLine.find("python");
	//std::cout << firstLine.substr(index + 6, 1) << std::endl;
	return firstLine.substr(index + 6, 1);
}