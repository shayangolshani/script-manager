#pragma once
void printHelp();

void printNoArgs();

void initScriptList();

void listScripts();

void runScript(std::string scriptName);

std::string findScript(std::string scriptName);

void printArgumentError();

std::string upperCase(std::string s);

std::string checkVersionNumber(std::string path);
