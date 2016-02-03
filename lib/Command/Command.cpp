#include "Command.h"

Command::Command(byte bytes[], int size) {
	printBytes(bytes, size);
	int pos = 1;
	command = bytes[pos++];
	argsCount = 5;//bytes[pos++];
	args = new Argument*[argsCount];
	for (int i = 0; i < argsCount; i++) args[i] = NULL;

	Serial.print(F("command = ")); Serial.println(command);
	Serial.print(F("argsCount = ")); Serial.println(argsCount);

	for (int i = 0; i < argsCount; i++) {
		if (pos > size - 3) {
			Serial.println("Is not valid, return");
			valid = false;
			return;
		}
		byte key = bytes[pos++];
		byte size = bytes[pos++];
		args[i] = new Argument(key, size, &bytes[pos]);
		//args[i] = new Argument(bytes, &pos);
		args[i]->toString();
		Serial.print("pos = "); Serial.println(pos);
	}
	//Serial.print("&args = "); Serial.println((int) args);
	//Serial.print("&args[0] = "); Serial.println((int) args[0]);
	Serial.println("construct complete");
}

Command::~Command() {
	for (int i = 0; i < argsCount; i++) delete args[i];
	delete[] args;
}

void Command::printBytes(byte bytes[], int size) {
	Serial.println(F("--------------------"));
	Serial.print(F("find end of command, command length = ")); Serial.println(size);
	for (int i = 0; i < size; i++) {
		Serial.print((char) bytes[i]);
	}
	Serial.println();
	for (int i = 0; i < size; i++) {
		Serial.print(bytes[i]); Serial.print(", ");
	}
	Serial.println(F("\r\n--------------------"));
}

byte Command::getCommand() {
	return command;
}

Argument* Command::getArg(byte key) {
	return NULL;
}
