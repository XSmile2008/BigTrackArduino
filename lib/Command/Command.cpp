#include "Command.h"
#include "Arduino.h"

//TODO:write destructor

Command::Command(byte bytes[], int size) {
	printBytes(bytes, size);
	command = bytes[1];
	int pos = 2;
	argsCount = 0;
	Serial.print("command = "); Serial.println(command);
	//while (pos < size - 2) {//last two is \r\n
		args[argsCount]->key = bytes[pos++];
		args[argsCount]->size = bytes[pos++];
		args[argsCount]->value = &bytes[pos];
		pos += size;
		argsCount++;

		Serial.print("arg.key = "); Serial.println(args[argsCount - 1]->key);
		Serial.print("arg.size = "); Serial.println(args[argsCount - 1]->size);
		Serial.print("arg.value = "); Serial.println((int) args[argsCount - 1]->value);
		Serial.println(freeMemory());
	//}
}

void Command::printBytes(byte bytes[], int size) {
	Serial.print("find end of command, command length = "); Serial.println(size);
	for (int i = 0; i < size; i++) {
		Serial.print((char) bytes[i]);
	}
	Serial.println();
	for (int i = 0; i < size; i++) {
		Serial.print(bytes[i]); Serial.print(", ");
	}
	Serial.println("\r\n--------------------");
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
