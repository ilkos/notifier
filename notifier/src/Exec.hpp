#ifndef EXEC_HPP_
#define EXEC_HPP_

#include <string>
#include <vector>
#include "Util.hpp"

class Exec {
public:
	Exec(const std::string& file);

	Exec& operator<< (const std::string& argument);

	void run();

private:
	const std::string mFile;
	std::vector<Ref<const std::string> > mArgs;
};
#endif
