#ifndef Command_h
#define Command_h
#include "Arduino.h"
#include "Argument.h"

class Command {
	public:
		Command(String command);
		String getCommand();
		Argument* getArg(int index);
		Argument* getArg(String key);
		String toString();
	private:
		void parseArgs(String args);
		String command;
		int argsCount;
		Argument* args[5];
};

#endif