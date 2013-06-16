#include "Exec.hpp"

#include <cstring>
#include <unistd.h>
#include <iostream>
Exec::Exec(const std::string& file) :
		mFile(file) {
	*this << mFile;
}

Exec& Exec::operator<<(const std::string& argument) {
	mArgs.push_back(make_cref(argument));
	return *this;
}

void Exec::run() {
	if (!fork()) {
		char* args[mArgs.size() + 1];
		size_t i = 0;

		for (; i < mArgs.size(); ++i) {
			const std::string& s = mArgs[i];
			args[i] = const_cast<char*>(s.c_str());
		}
		args[i] = NULL;
		execvp(mFile.c_str(), args);
	}
}
