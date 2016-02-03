#ifndef Command_h
#define Command_h
#include "Arduino.h"
#include "MemoryFree.h"
#include "Argument.h"

class Command {
	public:
		Command(byte bytes[], int size);
		~Command();
		void printBytes(byte bytes[], int size);
		byte getCommand();
		Argument* getArg(byte key);
	private:
		byte command;
		int argsCount;
		Argument **args;
};

#endif
