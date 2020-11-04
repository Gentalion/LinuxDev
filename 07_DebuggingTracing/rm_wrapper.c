#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <string.h>

typedef int (*wrapper)(int dirfd, const char *pathname, int flags);

int unlinkat(int dirfd, const char *pathname, int flags) {
    if (strstr(pathname, "FIX") != 0) {
        return EPERM;
    }

    wrapper original_unlinkat = dlsym(RTLD_NEXT, "unlinkat");
    return original_unlinkat(dirfd, pathname, flags);
}