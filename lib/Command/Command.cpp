#include "Command.h"
#include "Arduino.h"

//TODO:write destructor

Command::Command(byte bytes[], int size) {
	command = bytes[1];
	int pos = 2;
	argsCount = 0;
	printBytes(bytes, size);
	// while (pos < size - 2) {//last two is \r\n
	// 	args[argsCount]->key = bytes[pos++];
	// 	args[argsCount]->size = bytes[pos++];
	// 	args[argsCount]->value = &bytes[pos];
	// 	pos += size;
	// 	argsCount++;
	// }
}

void Command::printBytes(byte bytes[], int size) {
	for (int i = 0; i < size; i++) {
		Serial.print((char) bytes[i]);
	}
}

byte Command::getCommand() {
	return command;
}

Argument<void>* Command::getArg(byte key) {
	return NULL;
}

Argument<int>* Command::getIntArg(byte key) {
	return NULL;
}
