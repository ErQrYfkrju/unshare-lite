#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <libgen.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define setlocale(...)
#define bindtextdomain(...)
#define textdomain(...)
#define USAGE_HELP_OPTIONS(...) "%s", ""
#define USAGE_MAN_TAIL(...) "%s", ""
#define xasprintf asprintf
#define xusleep usleep

#define USAGE_HEADER          "Usage:\n"
#define USAGE_OPTIONS         "Options:\n"
#define USAGE_SEPARATOR       "\n"
#define UTIL_LINUX_VERSION    "unknown"

#define _PATH_PROC_SETGROUPS  "/proc/self/setgroups"
#define _PATH_PROC_UIDMAP     "/proc/self/uid_map"
#define _PATH_PROC_GIDMAP     "/proc/self/gid_map"

#define _(x) x
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*(x)))

static long int strtoul_or_err(const char *x, const char *msg) {
    long int r = -1;
    char *endptr = NULL;
    if((r = strtoul(x, &endptr, 10)) < 0) {
        perror(msg);
        return -1;
    }
    return r;
}

static void close_stdout() {
    close(STDOUT_FILENO);
}

static void __attribute__((__noreturn__)) errtryhelp(int x) {
    fputs("Try 'unshare --help' for more information.\n", stderr);
    exit(x);
}

static void __attribute__((__noreturn__)) errexec(const char *p) {
    fprintf(stderr, "unshare: failed to execute %s: %s", p, strerror(errno));
    exit(EXIT_FAILURE);
}

// https://github.com/karelzak/util-linux/blob/82adb91f6eff659474007cb776ef0091c95aac1f/lib/exec_shell.c
void __attribute__((__noreturn__)) exec_shell(void) {
    const char *shell = getenv("SHELL");
    char *arg0, *shellc;
    const char *shell_basename;

    if(!shell)
        shell = "/bin/sh";

    shellc = strdup(shell);
    shell_basename = basename(shellc);
    arg0 = malloc(strlen(shell_basename) + 2);
    arg0[0] = '-';
    strcpy(arg0 + 1, shell_basename);

    execl(shell, arg0, NULL);
    errexec(shell);
}

static inline int __attribute__((__noreturn__)) _do_err(int adderr, int code, const char *fmt, ...) {
    fprintf(stderr, "unshare: ");
    if(fmt != NULL) {
        va_list argp;
        va_start(argp, fmt);
        vfprintf(stderr, fmt, argp);
        va_end(argp);
        if(adderr)
            fprintf(stderr, ": ");
    }
    if(adderr)
        fprintf(stderr, "%s", strerror(errno));
    fprintf(stderr, "\n");
    exit(code);
}

#define err(x, y, ...)  _do_err(1, x, y, ##__VA_ARGS__);
#define errx(x, y, ...) _do_err(0, x, y, ##__VA_ARGS__);

#include "all-io.h"
