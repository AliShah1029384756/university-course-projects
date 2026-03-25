// Group Roll Numbers: <ROLL_NO_1>, <ROLL_NO_2>
#include <cstdio>
#include <exception>
#include <iostream>
#include <windows.h>
#include "IMS.h"

int main() {
	AllocConsole();
	FILE* inStream = nullptr;
	FILE* outStream = nullptr;
	freopen_s(&inStream, "CONIN$", "r", stdin);
	freopen_s(&outStream, "CONOUT$", "w", stdout);
	freopen_s(&outStream, "CONOUT$", "w", stderr);

	try {
		IMS app;
		app.run();
	}
	catch (const std::exception& ex) {
		std::cerr << "Fatal error: " << ex.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Fatal unknown error occurred." << std::endl;
	}

	std::cout << "\nPress Enter to close...";
	std::cin.get();
	return 0;
}
