/* The driver is really basic with a couple of essentials:
 * init() and cleanup(), module_init() and module_exit()
 *
 * and a couple of extras:
 * *current object, showing the current process and
 * module_param(), allowing load-time modifications.
 */
#include <linux/module.h>
#include <linux/init.h>

/* for *current */
//#include <asm/current.h>    
#include <linux/sched.h> // defines struct task_struct

/* Parameters */
static char *whom = "world";
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);


static int __init myinit(void){
    int i;
    printk(KERN_INFO "My init!\n");
    printk(KERN_INFO "The process is \"%s\" (pid %i)\n", current->comm, current->pid);
    for (i = 0; i < howmany; ++i)
        printk(KERN_INFO "Hello, %s\n", whom);
    return 0;
}

static void __exit mycleanup(void){
    printk(KERN_INFO "My clean up!\n");
    return;
}

module_init(myinit);
module_exit(mycleanup);

MODULE_LICENSE("Dual GPL/BSD");
