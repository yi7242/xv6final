#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int max = 1;
    int buf[256];
    buf[0] = 2;
    int now = 3;
    while (1) {
        int flag = 0;
        for (int i = 0; i < max; i++) {
            if (now%buf[i] == 0) {
                flag = 1;
            }
        }
        if (flag == 0) {
            fprintf(1, "%d\n", now);
            buf[max] = now;
            max++;
        }
        now++;
    }
    exit(0);
}