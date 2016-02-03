#include "Command.h"

Command::Command(byte bytes[], int size) {
	printBytes(bytes, size);
	command = bytes[1];
	argsCount = 1;//bytes[2];
	args = new Argument*[argsCount];
	int pos = 3;
	Serial.print("command = "); Serial.println(command);
	Serial.print("argsCount = "); Serial.println(argsCount);
	//while (pos < size - 2) {//last two is \r\n
		int i = 0;
		args[i] = new Argument(bytes, pos);
		args[i]->toString();
		Serial.print("&args = "); Serial.println((int) args);
		Serial.print("&args[0] = "); Serial.println((int) args[0]);
		Serial.println("construct complete");
	//}
}

Command::~Command() {
	for (int i = 0; i < argsCount; i++) delete args[i];
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

Argument* Command::getArg(byte key) {
	return NULL;
}
