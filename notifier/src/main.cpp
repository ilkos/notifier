#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <unistd.h>

#include "Notifier.hpp"
#include "Exec.hpp"

using namespace std;

void help(const char* name) {
	cout << "Usage: " << name << " [ -m[odify] | -a[append] ] -d <file> <callback>" << endl;
	exit(-1);
}

int main(int argc, char* const argv[]) {
	int opt;
	int flags = 0;
	string filename;
	while ((opt = getopt(argc, argv, "amd:")) != -1) {
		switch(opt) {
		case 'a':
			flags |= IN_CREATE;
			break;
		case 'm':
			flags |= IN_MODIFY;
			break;
		case 'd':
			filename = std::string(optarg);
			break;
		default:
			help(argv[0]);
			break;
		}
	}

	if (!flags || filename.empty() || optind >= argc) {
		help(argv[0]);
	}

	cerr << "Monitoring " << filename << endl;

	Exec callback(argv[optind]);

	vector<string> execargs(argc - (optind + 1));
	for (int i = optind + 1, j = 0; i < argc; ++i, ++j) {
		execargs[j] = argv[i];
		callback << execargs[j];
	}

	Notifier notifier(filename, flags);
	Notifier::Event event;

	callback << event.getFile();

	while (notifier.getNextEvent(event)) {
		callback.run();
	}

	exit(0);
}
