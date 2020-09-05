#include <cstring>
#include <saltysd/saltysd_core.h>
#include <saltysd/saltysd_dynamic.h>
#include <saltysd/saltysd_ipc.h>
#include <string>
#include <switch_min.h>

#include "header.hpp"

uint16_t logStringIndex = 0;
char logString[1024];

void writeToLog (const char* str) {
	uint16_t stringLength = strlen (str);
	if (logStringIndex + stringLength < sizeof (logString)) {
		memcpy (&logString[logStringIndex], str, stringLength);
		logStringIndex += stringLength;
	}
}

uint8_t canWriteToLog () {
	// Give a generous estimate
	return logStringIndex < (sizeof (logString) - 100);
}

void writePointerToFile (void* ptr, FILE* file) {
	SaltySDCore_fwrite (&ptr, sizeof (ptr), 1, file);
}

int main (int argc, char* argv[]) {
	writeToLog ("Alive\n");

	// Do stuff

	writeToLog ("Injection finished\n");
}