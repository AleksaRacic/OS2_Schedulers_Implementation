#include <STDARG.H>
#include <string.h>
#include "Logger.h"
#include "kernel.h"

void synchronizedPrintf(const char *format, ...);

char* parseLocation(const char *log_name)
{
	char *result = new char[strlen(log_name) + 13];
	strcpy(result, "../Logs/");
	strcat(result, log_name);
	strcat(result, ".txt");
	return result;
}

Logger::Logger(const char *name){
	location = parseLocation(name);
	fileStream = fopen((const char*)location, "w");
	fclose(fileStream);
}

Logger::~Logger() {
	delete location;
}

void Logger::write(const char *format, ...){
	fileStream = fopen((const char*)location, "a");
	if(fileStream != NULL){
		va_list args;
		systemLock();
		va_start(args, format);
		vfprintf(fileStream, format, args);
		va_end(args);
		systemUnlock();
		fclose(fileStream);
	}

}

