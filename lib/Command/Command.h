#ifndef Command_h
#define Command_h
#include "Arduino.h"
#include "MemoryFree.h"
#include "ArrayList.h"
#include "Argument.h"

class Command {
	public:
		Command(byte key);
		Command(byte key, List<Argument*>* arguments);
		~Command();
		void serialize();
		static Command* deserialize(byte bytes[], int size);
		byte getKey();
		void setKey(byte key);
		List<Argument*>* getArguments();
		void setArguments(List<Argument*> arguments);
		Argument* getArgument(byte key);

		static const byte COMMAND_START[];
		static const byte COMMAND_END[];
	private:
		byte key;
		List<Argument*>* arguments;
};

#endif
