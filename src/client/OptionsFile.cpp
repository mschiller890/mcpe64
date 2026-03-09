#include "OptionsFile.h"
#include <stdio.h>
#include <string.h>

OptionsFile::OptionsFile() {
#ifdef __APPLE__
	settingsPath = "./Documents/options.txt";
#elif defined(ANDROID)
	settingsPath = "options.txt";
#else
	settingsPath = "options.txt";
#endif
}

void OptionsFile::save(const StringVector& settings) {
	FILE* pFile = fopen(settingsPath.c_str(), "w");
	if(pFile != NULL) {
		for(StringVector::const_iterator it = settings.begin(); it != settings.end(); ++it) {
			fprintf(pFile, "%s\n", it->c_str());
		}
		fclose(pFile);
	}
}

StringVector OptionsFile::getOptionStrings() {
	StringVector returnVector;
	FILE* pFile = fopen(settingsPath.c_str(), "r");
	if(pFile != NULL) {
		char lineBuff[128];
		while(fgets(lineBuff, sizeof lineBuff, pFile)) {
			// Strip trailing newline
			size_t len = strlen(lineBuff);
			while(len > 0 && (lineBuff[len-1] == '\n' || lineBuff[len-1] == '\r'))
				lineBuff[--len] = '\0';
			if(len < 3) continue;
			// Split "key:value" into two separate entries to match update() pairing
			char* colon = strchr(lineBuff, ':');
			if(colon) {
				returnVector.push_back(std::string(lineBuff, colon - lineBuff));
				returnVector.push_back(std::string(colon + 1));
			}
		}
		fclose(pFile);
	}
	return returnVector;
}
