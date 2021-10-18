#ifndef OS2_PROJECT_LOGGER_H_
#define OS2_PROJECT_LOGGER_H_

#include <STDIO.H>

class Logger {
public:
	Logger(const char*);
	virtual ~Logger();
	void write(const char *format, ...);
private:
	FILE* fileStream;
	char *location;
};

#endif /* OS2_PROJECT_LOGGER_H_ */
