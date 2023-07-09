#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int ac, char *av[])
{
    // setbuf(stdout, NULL);
    // 自分のinode番号をfstatで手に入れる→catdirのやつでde.inumが一致するのを探す、de.nameを出力
    int here;
    // int up;
    int cc;
    struct dirent de;
    struct stat st;
    char dirs[20][20];
    char lookat[500];
   
    here = open(".", 0);
    fstat(here, &st);
    int up;
    int meIno = st.ino;
    int counter = 0;
    lookat[0] = '.';
    lookat[1] = '.';
    while ((up = open(lookat, 0)) > 0 && meIno != 1) {
        counter++;
        // printf("%d\n", counter);
        fstat(up, &st);
        while ((cc = read(up, &de, 16)) == 16)
        {
            if (de.inum==meIno) strcpy(dirs[counter-1], de.name);
        }
        here = open(lookat, 0);
        fstat(here, &st);
        meIno = st.ino;
        lookat[counter * 3-1] = '/';
        lookat[counter * 3] = '.';
        lookat[counter * 3+1] = '.';
        // printf("nextlook:%s\n", lookat);
    }

    for (int i = counter-1; i >= 0; i--) {
        printf("/%s", dirs[i]);
    }
    printf("\n");

   
    exit(0);
}