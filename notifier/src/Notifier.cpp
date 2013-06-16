#include "Notifier.hpp"

#include <stdexcept>
#include <cstring>
#include <cstdio>
#include <cerrno>

Notifier::Notifier () :
		mInotifyFd(inotify_init()),
		mBufferLen(0),
		mBufferCursor(0) {
	if (mInotifyFd < 0) {
		throw std::runtime_error(strerror(errno));
	}
}
Notifier::Notifier (const std::string& file, int flags) :
		mInotifyFd(inotify_init()),
		mBufferLen(0),
		mBufferCursor(0) {
	if (mInotifyFd < 0) {
		throw std::runtime_error(strerror(errno));
	}

	watch(file, flags);
}

Notifier::~Notifier() {
	std::vector<Watched>::iterator itEnd = mWatched.end();
	for (std::vector<Watched>::iterator it = mWatched.begin(); it != itEnd; ++it) {
		inotify_rm_watch(mInotifyFd, it->descriptor);
	}

	close(mInotifyFd);
}

void Notifier::watch(const std::string& file, int flags) {
	int wd = inotify_add_watch(mInotifyFd, file.c_str(), flags);
	if (wd < 0) {
		throw std::runtime_error(strerror(errno));
	}

	mWatched.push_back(Watched(file, wd));
}

bool Notifier::getNextEvent(Event& output) {
	if (mBufferCursor >= mBufferLen) {
		mBufferLen = read(mInotifyFd, mBuffer, sizeof(mBuffer));

		if (mBufferLen < 0) {
			throw std::runtime_error(strerror(errno));
		}
		mBufferCursor = 0;
	}

	struct inotify_event* cptr = (struct inotify_event*) &mBuffer[mBufferCursor];
	output.mFile = std::string(cptr->name, cptr->len);
	output.mMask = cptr->mask;

	mBufferCursor += sizeof(struct inotify_event) + cptr->len;
	return true;
}

const std::string& Notifier::Event::getFile() const {
	return mFile;
}

bool Notifier::Event::isCreate() const {
	return mMask & IN_CREATE;
}

bool Notifier::Event::isModify() const {
	return mMask & IN_MODIFY;
}
