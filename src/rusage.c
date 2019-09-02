#include <stdint.h>

struct disk_info_t {
     uint64_t read_bytes;
     uint64_t write_bytes;
};

#if defined(__linux__)

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATIC_BUF_LEN 4096

int proc_disk_info(struct disk_info_t *diskbuf)
{
     char buf[STATIC_BUF_LEN] = { 0 };
     int fd;
     int ret;
     ssize_t nbytes;

     // Open
     fd = open("/proc/self/io", O_RDONLY);
     if (fd < 0) {
          return errno;
     }

     // Read file into buffer
     nbytes = read(fd, buf, STATIC_BUF_LEN);
     if (nbytes < 0) {
          ret = errno;
          goto finish;
     }

     // Extract information from buffer
     unsigned long long rchar, wchar, syscr, syscw, read_bytes, write_bytes, cancelled_write_bytes;
     sscanf(buf, "rchar: %llu\nwchar: %llu\nsyscr: %llu\nsyscw: %llu\nread_bytes: %llu\nwrite_bytes: %llu\ncancelled_write_bytes: %llu\n", &rchar, &wchar, &syscr, &syscw, &read_bytes, &write_bytes, &cancelled_write_bytes);
     diskbuf->read_bytes = read_bytes;
     diskbuf->write_bytes = write_bytes;

finish:
     close(fd);
     return ret;
}

#elif defined(__APPLE__)

#include <libproc.h>
#include <unistd.h>
#include <sys/errno.h>

int proc_disk_info(struct disk_info_t *buf)
{
     rusage_info_current r;
     if (proc_pid_rusage(getpid(), RUSAGE_INFO_CURRENT, (rusage_info_t *)&r) == 0) {
          buf->read_bytes = r.ri_diskio_bytesread;
          buf->write_bytes = r.ri_diskio_byteswritten;
          return 0;
     } else {
          return errno;
     }
}

#else

#error "This platform is not supported right now"

#endif

