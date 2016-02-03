#ifndef Command_h
#define Command_h
#include "Arduino.h"
#include "MemoryFree.h"
#include "Argument.h"

class Command {
	public:
		Command(byte bytes[], int size);
		~Command();
		bool isValid();
		byte getCommand();
		Argument* getArg(byte key);
		void printBytes(byte bytes[], int size);
	private:
		bool valid;
		byte command;
		int argsCount;
		Argument **args;
};

#endif
