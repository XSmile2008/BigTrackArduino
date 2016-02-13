#ifndef Command_h
#define Command_h
#include "Arduino.h"
#include "MemoryFree.h"
#include "ArrayList.h"
#include "Argument.h"

class Command {
	public:
		Command(byte bytes[], int size);
		Command(byte key);
		~Command();
		bool isValid();
		byte getKey();
		Argument* getArg(byte key);
		void printBytes(byte bytes[], int size);
	private:
		bool valid;
		byte key;
		List<Argument*> *args;
};

#endif
