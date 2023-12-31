#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "mem0"

MODULE_LICENSE("GPL");

// struct which will have data about the inputed string
struct mem0_dev {
	char *buffer;
	size_t bufferSize;
};

int start_module(void);
void end_module(void);
int open(struct inode *, struct file *);
int release(struct inode *, struct file *);
ssize_t read(struct file *filp, char __user *buff, size_t len, loff_t *offset);
ssize_t write(struct file *filp, const char __user *buff, size_t len, loff_t *offset);
void mem0_clenup(struct mem0_dev *);

module_init(start_module);
module_exit(end_module);

struct file_operations fops = {
	.open = open,
	.release = release,
	.read = read,
	.write = write
};

int Major;
struct mem0_dev *mem0Dev;

int start_module(void) {

	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Major < 0) {
		printk(KERN_ALERT "Start: device could not be registered\n");
		return Major;
	}
	printk(KERN_INFO "Start: major is: %d.\n", Major);

	// allocate memory for mem0_dev
	mem0Dev = kmalloc(sizeof(struct mem0_dev), GFP_KERNEL);
	if (!mem0Dev) {
		printk(KERN_ALERT "Start: uld not allocate memory\n");
		return -ENOMEM;
	}
	// set it all to 0
	memset(mem0Dev, 0, sizeof(struct mem0_dev));

	return 0;

}

// when module ends free memory
void end_module(void) {

	mem0_clenup(mem0Dev);

}

int open(struct inode *inode, struct file *file) { 

	return 0; 

}

int release(struct inode *inode, struct file *file) {

	return 0; 

}

ssize_t read(struct file *filp, char __user *buff, size_t len, loff_t *offset) {

	size_t size = mem0Dev->bufferSize;

	if (*offset >= size) { // chehck if everything was read
		return 0;
	}

	if (len > size - *offset) {
		len = size - *offset; // calculate how big a buffer is needed
	}

	// send to user space
	if (copy_to_user(buff, mem0Dev->buffer, len) ) {
		// if it fails, then release memory
		mem0_clenup(mem0Dev);
		return -EFAULT;
	}

	*offset += len; // move offset for len (where data is gonna be read next time)

	printk("Data read: \'%s\' of size: %ld\n", mem0Dev->buffer, mem0Dev->bufferSize);

	return len;

}

ssize_t write(struct file *filp, const char __user *buff, size_t len, loff_t *offset) {

	// allocate len amount of memory
	mem0Dev->buffer = kmalloc(len, GFP_KERNEL);
	if (!mem0Dev->buffer) {
		// if it fails, then release memory
		printk(KERN_ALERT "Write: could not allocate memory\n");
		mem0_clenup(mem0Dev);
		return -ENOMEM;
	}
	mem0Dev->bufferSize = len;

	// to kernel space
	if (copy_from_user(mem0Dev->buffer, buff, len)) {
		// if it fails, then release memory
		mem0_clenup(mem0Dev);
		return -EFAULT;
	}

	*offset += len;

	printk("Data write: \'%s\' of size: %ld\n", mem0Dev->buffer, mem0Dev->bufferSize);

	return len;

}

void mem0_clenup(struct mem0_dev * mem0Dev) {

	// check if pointer has memory allocated
	if (mem0Dev) {
		// if it has, then free buffer for string
		kfree(mem0Dev->buffer);
	}

	// free memory allocated for struct
	kfree(mem0Dev);
	unregister_chrdev(Major, DEVICE_NAME);

}
