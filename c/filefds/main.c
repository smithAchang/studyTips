#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/resource.h>

int count_open_files(pid_t pid) {
    char path[256];
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    
    struct rlimit rlim;
    
    // 获取当前进程的文件描述符限制
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
        printf("Current process file descriptor limits: soft=%lld, hard=%lld\n",
               (long long)rlim.rlim_cur, (long long)rlim.rlim_max);
    } else {
        perror("getrlimit failed");
    }
    
    // 构造/proc/[pid]/fd路径
    snprintf(path, sizeof(path), "/proc/%d/fd", pid);
    
    // 打开目录
    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir failed");
        return -1;
    }
    
    // 遍历目录中的文件(每个文件代表一个打开的文件描述符)
    while ((entry = readdir(dir)) != NULL) {
        // 跳过.和..
        if (entry->d_name[0] == '.') 
            continue;

        printf("Process %d has opend %s file descriptor\n", pid, entry->d_name);
        count++;
    }
    
    closedir(dir);
    return count;
}

int main() {
    pid_t pid = getpid(); // 获取当前进程ID
    int fd_count = count_open_files(pid);
    
    if (fd_count >= 0) {
        printf("Process %d has %d file descriptors open\n", pid, fd_count);
    }
    
    return 0;
}