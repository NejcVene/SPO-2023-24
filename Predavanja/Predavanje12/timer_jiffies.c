#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>

#define DEVICE_NAME "timer_jiffies"

MODULE_LICENSE("GPL");

int fInit(void);
void fExit(void);

int fInit() {

    unsigned long j1, j2, stamp1s, stamp100Ms, stamp5S;
    j1 = jiffies;

    stamp1s = j1 + HZ;
    stamp100Ms = j1 + 100 * HZ / 1000;

    stamp5S = j1 + 5 * HZ;

    j2 = jiffies;
    printk(KERN_INFO "j1: %lu\nj2: %lu\nStamp 1s: %ld\nStamp 5s: %ld\n", j1, j2, stamp1s, stamp5S);

    return 0;

}

void fExit() {

    return;

}

module_init(fInit);
module_exit(fExit);

