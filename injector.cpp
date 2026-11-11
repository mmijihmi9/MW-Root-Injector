#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Injector", __VA_ARGS__)

int main(int argc, char *argv[]) {
    if (argc != 3) {
        LOGI("Usage: %s <pid> <lib_path>", argv[0]);
        return 1;
    }
    pid_t pid = atoi(argv[1]);
    const char* lib_path = argv[2];
    
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
        LOGI("Attach failed");
        return -1;
    }
    waitpid(pid, NULL, WUNTRACED);
    
    LOGI("Attached to PID: %d. Injecting: %s", pid, lib_path);
    
    // ملاحظة: الحقن الفعلي عبر ptrace معقد في أندرويد 14+
    // هذا الهيكل يوضح الآلية الأساسية
    
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    return 0;
}
