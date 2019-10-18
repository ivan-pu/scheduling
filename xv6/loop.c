#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void) {
    for (;;) {
        sleep(10);
        printf(1, "haha %d\n", getpid());
    }
}