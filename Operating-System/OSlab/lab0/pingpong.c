#include"types.h"
#include"user.h"

int
main(int argc, char* argv[])
{
    int par_to_chil[2], chil_to_par[2];
    pipe(par_to_chil);
    pipe(chil_to_par);
    char buf[8];
    //fork()=0: child process
    //fork()>0: parent process
    //fork()<0: make child process failed
    if(fork() == 0) //child
    {
        close(par_to_chil[1]);
        close(chil_to_par[0]);
        read(par_to_chil[0], buf, 4);
        printf(1, "%d: received %s\n", getpid(), buf);
        write(chil_to_par[1], "pong", 4);
    }
    else //parent
    {
        close(par_to_chil[0]);
        close(chil_to_par[1]);
        write(par_to_chil[1], "ping", 4);
        read(chil_to_par[0], buf, 4);
        printf(1, "%d: received %s\n", getpid(), buf);
        //if delete wait(),child will turn to zombie!
        //the purpose of wait() is to kill child process when finished
    }
    exit();
}