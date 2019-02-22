
#include <errno.h>
#include <termios.h>

#include <bits/ensure.h>
#include <mlibc/sysdeps.hpp>

speed_t cfgetispeed(const struct termios *) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
speed_t cfgetospeed(const struct termios *) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int cfsetispeed(struct termios *, speed_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int cfsetospeed(struct termios *, speed_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int tcdrain(int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int tcflow(int fd, int action) {
	if(int e = mlibc::sys_tcflow(fd, action); e) {
		errno = e;
		return -1;
	}
	return 0;
}
int tcflush(int, int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

int tcgetattr(int fd, struct termios *attr) {
	if(int e = mlibc::sys_tcgetattr(fd, attr); e) {
		errno = e;
		return -1;
	}
	return 0;
}

pid_t tcgetsid(int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int tcsendbreak(int, int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

int tcsetattr(int fd, int opts, const struct termios *attr) {
	if(int e = mlibc::sys_tcsetattr(fd, opts, attr); e) {
		errno = e;
		return -1;
	}
	return 0;
}

