#ifndef __FS_H__
#define __FS_H__

#define PATH_DELIM	"/"

typedef int fd_t;

class FS
{
public:
	virtual fd_t open(const char * path, const char * mode) = 0;
	virtual int read(fd_t filedesc, unsigned char * buffer, int size) = 0;
	virtual int write(fd_t filedesc, const unsigned char * buffer, int size) = 0;
	virtual int seek(fd_t filedesc, int offset) = 0;
	virtual void close(fd_t filedesc) = 0;
};

#endif