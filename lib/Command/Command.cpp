#include "Command.h"
#include "ArrayList.cpp"

const byte Command::COMMAND_START[] = {':'};
const byte Command::COMMAND_END[] = {'\r', '\n'};

Command::Command(byte key) {
	Command::key = key;
	arguments = new ArrayList<Argument*>();
}

Command::Command(byte key, List<Argument*>* arguments) {
	Command::key = key;
	Command::arguments = arguments;
}

Command::~Command() {
	Serial.println(F("Command.destructor()"));
	for (uint16_t i = 0; i < arguments->size(); i++) delete arguments->get(i);
	delete arguments;
}

void Command::serialize() {//TODO: check on multy argument commands
	uint8_t size = 3 + 2;
	for (uint8_t i = 0; i < arguments->size(); i++) size += 2 + arguments->get(i)->getSize();
	byte bytes[size];
	bytes[0] = COMMAND_START[0];
	bytes[1] = key;
	bytes[2] = arguments->size();
	uint8_t pos = 3;
	for (uint8_t i = 0; i < arguments->size(); i++) {
		bytes[pos++] = arguments->get(i)->getKey();
		bytes[pos++] = arguments->get(i)->getSize();
		memcpy(&bytes[pos], arguments->get(i)->getValue(), arguments->get(i)->getSize());
		pos += arguments->get(i)->getSize();
	}
	bytes[size - 2] = COMMAND_END[0];
	bytes[size - 1] = COMMAND_END[1];

	Serial.println("serialized :");
	for (uint8_t i  = 0; i < size; i++) { Serial.print(bytes[i]); Serial.print(", "); }
	Serial.println();
}

Command* Command::deserialize(byte bytes[], int size) {
	int key = bytes[1];
	uint8_t argsCount = bytes[2];
	Serial.print(F("key = ")); Serial.println(key);
	Serial.print(F("argsCount = ")); Serial.println(argsCount);
	int pos = 3;
	List<Argument*>* arguments = new ArrayList<Argument*>(argsCount);
	for (uint8_t i = 0; i < argsCount; i++) {
		if (pos + 2 > size - 2) break;
		byte argKey = bytes[pos++];
		byte argSize = bytes[pos++];
		//if (pos + argSize > size - 2) break;
		arguments->add(new Argument(argKey, argSize, &bytes[pos]));
		arguments->get(i)->print();
		pos += argSize;
		Serial.print(F("pos = ")); Serial.println(pos);
	}
	if (arguments->size() == argsCount) {
		return new Command(key, arguments);
	} else {
		for (uint8_t i = 0; i < arguments->size(); i++) delete arguments->get(i);
		delete arguments;
		return NULL;
	}
}

byte Command::getKey() {
	return key;
}

void Command::setKey(byte key) {
	Command::key = key;
}

List<Argument*>* Command::getArguments() {
	return arguments;
}

void Command::setArguments(List<Argument*> arguments) {
	delete Command::arguments;//TODO: check it
	*Command::arguments = arguments;
}

Argument* Command::getArgument(byte key) {
	for (uint8_t i = 0; i < arguments->size(); i++) {
		if (arguments->get(i)->getKey() == key) return arguments->get(i);
	}
	return NULL;
}
