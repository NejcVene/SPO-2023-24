#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");

struct timer_list timer1;

void timer_callback(struct timer_list *timer) {

    static int perioda = 50; // na koliko časa se preklaplja (vedno manj pogosto)
    printk("timer_callback (%ld).\n", jiffies);
    mod_timer( &timer1, jiffies + msecs_to_jiffies(perioda));
    perioda *= 2; // zaradi tega bo vedno manjše
    
}

int init_module(void) {

    printk("Timer module inserted\n");
    timer_setup( &timer1, timer_callback, 0);
    mod_timer( &timer1, jiffies + msecs_to_jiffies(2000));
    return 0;

}

void cleanup_module(void) {

    if ( del_timer( &timer1) )
    printk("Timer module removed\n");
    return;

}
