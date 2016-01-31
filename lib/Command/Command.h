#ifndef Command_h
#define Command_h
#include "Arduino.h"

//TODO: may be use templates
template <typename T>
struct Argument {
	byte key;
	byte size;
	T* value;
};

// struct Arg {
// 	byte arg;
// 	byte type;
// 	void* value;
// };

class Command {
	public:
		Command(byte bytes[], int size);
		void printBytes(byte bytes[], int size);
		byte getCommand();
		Argument<void>* getArg(byte key);
		Argument<int>* getIntArg(byte key);
	private:
		byte command;
		int argsCount;
		Argument<void>* args[5];
};

#endif
