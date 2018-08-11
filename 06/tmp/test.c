#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define TEST_STRING "b9c2282a294c"
#define BUFFER_LEN 256

int main()
{
	int fd;
	int ret;
	static char rec[BUFFER_LEN];

	fd = open("/dev/eudyptula", O_RDWR);
	if (0 > fd) {
		printf("Failed to open device\n");
		return errno;
	}

	printf("Writing \"%s\" to device\n", TEST_STRING);
	ret = write(fd, TEST_STRING, strlen(TEST_STRING));
	if (0 > ret) {
		printf("Failed to write to device\n");
		close(fd);
		return errno;
	}

	printf("Reading from device\n");
	ret = read(fd, rec, BUFFER_LEN);
	if (0 > ret) {
		printf("Error reading from device\n");
		close(fd);
		return errno;
	}

	printf("Recieved: \"%s\"\n", rec);

	close(fd);
	return 0;
}
