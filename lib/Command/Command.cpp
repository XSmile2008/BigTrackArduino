#include "Command.h"
#include "ArrayList.cpp"

Command::Command(byte bytes[], int size) {
	printBytes(bytes, size);
	int pos = 1;
	valid = true;
	key = bytes[pos++];
	int argsCount = bytes[pos++];
	args = new ArrayList<Argument*>(argsCount);
	//for (int i = 0; i < argsCount; i++) args[i] = NULL;

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
		args-> add(new Argument(argKey, argSize, &bytes[pos]));
		args->get(i)->print();
		pos += argSize;
		Serial.print(F("pos = ")); Serial.println(pos);
	}
}

Command::Command(byte key) {
	Command::key = key;
	args = new ArrayList<Argument*>();
}

Command::~Command() {
	for (uint16_t i = 0; i < args->size(); i++) delete args->get(i);
	delete args;
}

bool Command::isValid() {
	return valid;
}

byte Command::getKey() {
	return key;
}

Argument* Command::getArg(byte key) {
	for (uint8_t i = 0; i < args->size(); i++) {
		if (args->get(i)->getKey() == key) return args->get(i);
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
