// userRR
#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

# define DEBUG_PRINT(x) printf x

int
main(int argc, char *argv[]) {
    if (argc != 5) return -1;
    int error = 0;
    char *args[1];
    args[0] = "loop";
    int slice = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    char *job = argv[3];
    int jobcount = atoi(argv[4]);
    int pids[jobcount];
    for (int i = 0; i < jobcount; i++) {
        int pid = fork2(1);
        if (pid == 0) {
            error = exec(job, args);
            if (error == -1) {
                printf(1, "Loop exec error\n");
            }
        } else {
            pids[i] = pid;
        }
    }
    for (int j = 0; j < iterations; j++) {
        for (int i = 0; i < jobcount; i++) {
            setpri(pids[i], 2);
            sleep(slice);
            setpri(pids[i], 1);
        }
    }

    struct pstat st;
    if (getpinfo(&st) != 0) {
        printf(1, "%s:%d check (" "getpinfo(&st) == 0" ") failed: %s\n", "_file_name_", 61, "getpinfo");;
    }
    for (int i = 0; i < jobcount; i++) {
        kill(pids[i]);
        wait();
    }
    for (int j = 0; j < NPROC; j++) {
        if (st.inuse[j] && st.pid[j] >= 3 && st.pid[j] != getpid()) {
            DEBUG_PRINT((1, "XV6_SCHEDULER\t CHILD\n"));
            DEBUG_PRINT((1, "pid: %d\n", st.pid[j]));
            for (int k = 3; k >= 0; k--) {
                DEBUG_PRINT((1, "XV6_SCHEDULER\t \t level %d ticks used %d\n", k, st.ticks[j][k]));
                DEBUG_PRINT((1, "XV6_SCHEDULER\t \t level %d qtail %d\n", k, st.qtail[j][k]));
            }
        }
    }

    exit();

}

