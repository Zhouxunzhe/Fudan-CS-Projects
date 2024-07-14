#include"types.h"
#include"user.h"

int
main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf(1, "usage: sleep time(s)\n");
        exit();
    }
    int time = atoi(argv[1]);
    printf(1, "(nothing happens for a little while)\n");
    sleep(time);
    exit();
}
