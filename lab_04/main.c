#include <stdio.h>
#include "string.h"
#include "unistd.h"
#include "sys/types.h"
#include "dirent.h"

/*
Задание:
· В пользовательском режиме вывести на экран информацию об окружении процесса с комментариями;
· В пользовательском режиме вывести на экран информацию о состоянии процесса с комментариями;
· Вывести информацию из файла cmdline и директории fd.
*/

#define NUMBER_OF_COMMENTS 52
char* comments_for_stat[NUMBER_OF_COMMENTS] = { "1) pid", "2) comm","3) state",
    "4) ppid", "5) pgrp", "6) session" , "7) tty_nr" , "8) tpgid", "9) flags",
    "10) minflt", "11) cminflt", "12) majflt", "13) cmajflt", "14) utime", "15) stime",
    "16) cutime", "17) cstime", "18) priority", "19) nice", "20) num_threads",
    "21) itrealvalue", "22) starttime", "23) vsize", "24) rss", "25) rsslim",
    "26) startcode", "27) endcode", "28) startstack", "29) kstkesp", "30) kstkeip",
    "31) signal", "32) blocked", "33) sigignore", "34) sigcatch", "35) wchan",
    "36) nswap", "37) сnswap", "38) exit_signal", "39) processor", "40) rt_priority",
    "41) policy", "42) delayacct_blkio_ticks", "43) guest_time", "44) cguest_time", "45) start_data",
    "46) end_data", "47) start_brk", "48) arg_start", "49) arg_end", "50) env_start",
    "51) env_end", "52) exit_code"
};

#define BUF_SIZE 0x200
#define FILE_OPEN_ERROR -1

int output_info_from_file(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (!f)
    {
        printf("Can't open file!");
        return FILE_OPEN_ERROR;
    }

    size_t len = 0;
    char buf[BUF_SIZE];

    while ((len = fread(buf, 1, BUF_SIZE, f)) > 0)
    {
        for (int i = 0; i < len; i++)
            if (buf[i] == 0)
                buf[i] = 10;
        buf[len - 1] = 0;
        printf("%s", buf);
    }

    fclose(f);
    return 0;
}

void output_fd()
{
    printf("fd:\n");

    struct dirent *dirp;
    DIR *dp;
    char str[BUF_SIZE];
    char path[BUF_SIZE];

    dp = opendir("/proc/self/fd");
    while ((dirp = readdir(dp)) != NULL)
    {
        if ((strcmp(dirp->d_name, ".") != 0) &&
            (strcmp(dirp->d_name, "..") != 0))
        {
            sprintf(path, "%s%s", "/proc/self/fd/", dirp->d_name);
            readlink(path, str, BUF_SIZE);
            printf("%s -> %s\n", dirp->d_name, str);
        }
    }

    closedir(dp);
}

void output_statement()
{
    char buf[BUF_SIZE];
    FILE* f = fopen("/proc/self/stat", "r");
    fread(buf, 1, BUF_SIZE, f);
    char *pch = strtok(buf, " ");
    printf("stat: \n");
    int i = 0;
    while (i < NUMBER_OF_COMMENTS && pch != NULL)
    {
        printf("%s: ", comments_for_stat[i]);
        printf("%s\n", pch);
        pch = strtok(NULL, " ");
        i++;
    }
    fclose(f);
}

#define BOARD_SIZE 80
void print_board()
{
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("~");
    }
    printf("\n");
}

int main(int argc, const char* argv[])
{
    print_board();
    printf("environ: \n");
    output_info_from_file("/proc/self/environ");

    print_board();
    output_statement();
    print_board();

    printf("cmdline: \n");
    output_info_from_file("/proc/self/cmdline");

    print_board();
    output_fd();
    return 0;
}
