#include <libproc.h>
#include <unistd.h>

unsigned long long proc_disk_read_bytes()
{
     rusage_info_current r;
     proc_pid_rusage(getpid(), RUSAGE_INFO_CURRENT, (rusage_info_t *)&r);
     return r.ri_diskio_bytesread;
}

unsigned long long proc_disk_written_bytes()
{
     rusage_info_current r;
     proc_pid_rusage(getpid(), RUSAGE_INFO_CURRENT, (rusage_info_t *)&r);
     return r.ri_diskio_byteswritten;
}
