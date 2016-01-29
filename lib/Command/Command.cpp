#include "Command.h"
#include "Arduino.h"
#include "Argument.h"

//TODO:write destructor

Command::Command(String command) {
	command.trim();//remove extraneous spaces on the beginning or the end of command
	if (command.startsWith(":")) {
		Serial.println("parsing...");
		int dividerPos = command.indexOf(' ');
		this->command = (dividerPos == -1) ? command : command.substring(0, dividerPos);
		Serial.println("command=" + this->command);
		parseArgs(command.substring(dividerPos));
	} else {
		this->command = "";
		Serial.println("Not a command");
	}
	Serial.println("end parsing");
}

void Command::parseArgs(String args) {
	Serial.println("args=" + args);
	argsCount = 0;
	while (argsCount < 5) {
		int dividerPos = args.lastIndexOf('-');
		if (dividerPos == -1) break; 
		this->args[argsCount] = new Argument(args.substring(dividerPos));
		args = args.substring(0, dividerPos);
		argsCount++;
	}
	//Reverse args array
	for (int i = 0; i < argsCount / 2; i++) {
		Argument* temp = this->args[i];
		this->args[i] = this->args[argsCount - i - 1];
		this->args[argsCount - i - 1] = temp;
	}
	Serial.print("argsCount = "); Serial.println(argsCount);
}

String Command::getCommand() {
	return command;
}

Argument* Command::getArg(int index) {
	if (index >= argsCount) return NULL;
	else return args[index];
}

Argument* Command::getArg(String key) {
	for (int i = 0; i < argsCount; i++) {
		if (args[i]->getKey().equals(key)) return args[i];
		return NULL;
	}
}

String Command::toString() {
	String s = this->command;
	for (int i = 0; i < argsCount; i++) {
		s = s + " " + args[i]->toString();
	}
	return s;
}