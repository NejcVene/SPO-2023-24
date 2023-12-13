#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
// static funkcija je vidna samo v tej datoteki
static int hello_init(void) {

    printk(KERN_ALERT "Zdravo\n");
    return 0;

}
static void hello_exit(void) {

    printk(KERN_ALERT "Adijo!\n");

}

module_init(hello_init);
module_exit(hello_exit);