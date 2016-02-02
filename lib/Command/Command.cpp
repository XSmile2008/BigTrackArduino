#include "Command.h"
#include "Arduino.h"

//TODO:write destructor

Command::Command(byte bytes[], int size) {
	printBytes(bytes, size);
	command = bytes[1];
	argsCount = bytes[2];
	args = new Argument<void>*[argsCount];
	int pos = 3;
	Serial.print("command = "); Serial.println(command);
	Serial.print("argsCount = "); Serial.println(argsCount);
	//while (pos < size - 2) {//last two is \r\n
		int i = 0;
		args[i] = new Argument<void>;
		args[i]->key = bytes[pos++];
		args[i]->size = bytes[pos++];
		args[i]->value = &bytes[pos];
		pos += size;//TODO: this is error
		//i++;

		Serial.print("arg.key = "); Serial.println(args[i]->key);
		Serial.print("arg.size = "); Serial.println(args[i]->size);
		Serial.print("arg.value = "); Serial.println(*(int*) args[i]->value);
		Serial.print("&arg.value = "); Serial.println((int) args[i]->value);
		Serial.print("&args = "); Serial.println((int) args);
		Serial.print("&args[0] = "); Serial.println((int) args[0]);
		Serial.print("&args[1] = "); Serial.println((int) args[1]);
		Serial.print("&args[2] = "); Serial.println((int) args[2]);
		Serial.print("&args[3] = "); Serial.println((int) args[3]);
		Serial.print("&args[4] = "); Serial.println((int) args[4]);
		Serial.print("&bytes[pos] = "); Serial.println((int) &bytes[pos - size]);
	//}
}

Command::~Command() {
	delete[] args;
}

void Command::printBytes(byte bytes[], int size) {
	Serial.println("--------------------");
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
