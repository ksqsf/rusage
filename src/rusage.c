struct disk_info_t {
     unsigned long long read_bytes;
     unsigned long long write_bytes;
};

#if defined(__linux__)

#include <unistd.h>
#include <sys/errno.h>
#include <stdio.h>

#define STATIC_BUF_LEN 4096

int proc_disk_info(struct disk_info_t *buf)
{
     char sbuf[STATIC_BUF_LEN] = { 0 };
     char *buf = sbuf;
     int fd = open("/proc/self/io", O_RDONLY);
     off_t offset;
     size_t size;
     int ret;
     int nbytes;

     // Determine buffer size.
     offset = lseek(fd, 0, SEEK_END);
     if (offset == -1) {
          return errno;
     }
     size = (size_t) offset;

     // Rewind
     if (lseek(fd, 0, SEEK_SET) < 0) {
          return errno;
     }

     // Allocate buffer dynamically if sbuf is too small
     if (size > STATIC_BUF_LEN-1) {
          buf = (char *) malloc(size + 10);
          if (!buf) {
               return ENOMEM;
          }
          memset(buf, 0, size + 10);
     } else {
          memset(sbuf, 0, sizeof(sbuf));
     }

     // Read file into buffer
     nbytes = 0;
     do {
          ssize_t once = read(fd, buf + nbytes, size);
          if (once < 0) {
               ret = errno;
               goto finish;
          }
          else if (once == 0) {
               break;
          }
          nbytes += once;
     } while (nbytes < size);

     // Extract information from buffer
     unsigned long long rchar, wchar, syscr, syscw, read_bytes, write_bytes, cancelled_write_bytes;
     sscanf("rchar: %llu\nwchar: %llu\nsyscr: %llu\nsyscw: %llu\nread_bytes: %llu\nwrite_bytes: %llu\ncancelled_write_bytes: %llu\n", &rchar, &wchar, &syscr, &syscw, &read_bytes, &write_bytes, &cancelled_write_bytes);
     buf->read_bytes = read_bytes;
     buf->write_bytes = write_bytes;

finish:
     // Free dynamic buffer.
     if (buf != sbuf) {
          free(buf);
     }
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

