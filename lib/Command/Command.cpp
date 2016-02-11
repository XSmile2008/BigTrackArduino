#include "Command.h"

Command::Command(byte bytes[], int size) {
	printBytes(bytes, size);
	int pos = 1;
	valid = true;
	key = bytes[pos++];
	argsCount = bytes[pos++];
	args = new Argument*[argsCount];
	for (int i = 0; i < argsCount; i++) args[i] = NULL;

	Serial.print(F("key = ")); Serial.println(key);
	Serial.print(F("argsCount = ")); Serial.println(argsCount);

	for (int i = 0; i < argsCount; i++) {
		if (pos + 2 > size - 2) {
			valid = false;
			return;
		}
		byte argKey = bytes[pos++];
		byte argSize = bytes[pos++];
		// if (pos + argSize > size - 2) {
		// 	valid = false;
		// 	return;
		// }
		args[i] = new Argument(argKey, argSize, &bytes[pos]);
		args[i]->toString();
		pos += argSize;
		Serial.print(F("pos = ")); Serial.println(pos);
	}
}

Command::Command(byte key) {
	Command::key = key;
	argsCount = 0;
}

Command::~Command() {
	for (int i = 0; i < argsCount; i++) delete args[i];
	delete[] args;
}

bool Command::isValid() {
	return valid;
}

byte Command::getKey() {
	return key;
}

Argument* Command::getArg(byte key) {
	for (int i = 0; i < argsCount; i++) {
		if (args[i]->getKey() == key) return args[i];
	}
	return NULL;
}

/**@depreceted*/
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
