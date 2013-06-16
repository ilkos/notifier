#ifndef NOTIFIER_HPP_
#define NOTIFIER_HPP_

#include <vector>
#include <string>
#include <sys/inotify.h>
#include <cstdio>

class Notifier {
public:
	Notifier();
	Notifier (const std::string& file, int flags);
	~Notifier();

	struct Event {
		friend class Notifier;

		const std::string& getFile() const;
		bool isCreate() const;
		bool isModify() const;

	private:
		int mMask;
		std::string mFile;
	};

	void watch(const std::string& file, int flags);
	bool getNextEvent(Event& output);

private:
	struct Watched {
		Watched(const std::string f, int d) : file(f), descriptor(d) {}
		std::string file;
		int descriptor;
	};
	std::vector<Watched> mWatched;
	int mInotifyFd;

	static const int EVENTSZ = sizeof(struct inotify_event) + FILENAME_MAX + 1;
	char mBuffer[EVENTSZ * 4];
	int mBufferLen;
	int mBufferCursor;
};

#endif /* NOTIFIER_HPP_ */
