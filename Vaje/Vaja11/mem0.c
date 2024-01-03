#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "mem0"

MODULE_LICENSE("GPL");

int start_module(void);
void end_module(void);
int open(struct inode *, struct file *);
int release(struct inode *, struct file *);
ssize_t read(struct file *filp, char __user *buff, size_t len, loff_t *offset);
ssize_t write(struct file *filp, const char __user *buff, size_t len, loff_t *offset);
ssize_t write2(struct file *filp, const char __user *buff, size_t len, loff_t *offset);

int Major;
struct file_operations fops = {
	.open = open,
	.release = release,
	.read = read,
	.write = write2
};

struct mem0_dev {
	char *buffer;
	size_t bufferSize;
};

struct mem0_dev *mem0Dev;

module_init(start_module);
module_exit(end_module);

int start_module(void) {

	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Major < 0) {
		printk(KERN_ALERT "Registracija znakovne naprave spodletela.\n");
		return Major;
	}
	printk(KERN_INFO "Glavno stevilo je %d.\n", Major);

	mem0Dev = kmalloc(sizeof(struct mem0_dev), GFP_KERNEL);
	if (!mem0Dev) {
		return -ENOMEM;
	}
	memset(mem0Dev, 0, sizeof(struct mem0_dev));

	return 0;

}

void end_module(void) {

	unregister_chrdev(Major, DEVICE_NAME);

}

int open(struct inode *inode, struct file *file) { 

	return 0; 

}

int release(struct inode *inode, struct file *file) {

	return 0; 

}

ssize_t read(struct file *filp, char __user *buff, size_t len, loff_t *offset) {

	size_t size = mem0Dev->bufferSize;

	if (*offset >= size) { // all was read
		return 0;
	}

	if (len > size - *offset) {
		len = size - *offset; // how big a buffer is needed
	}

	if (copy_to_user(buff, mem0Dev->buffer, len) ) {
		return -EFAULT;
	}

	*offset += len; // move offset for len (where data is gonna be read next time)

	return len;

}

ssize_t write2(struct file *filp, const char __user *buff, size_t len, loff_t *offset) {

	char *tmpBuffer = kmalloc(len, GFP_KERNEL);
	if (!tmpBuffer) {
		return -ENOMEM;
	}

	if (copy_from_user(tmpBuffer, buff, len)) {
		kfree(tmpBuffer);
		return -EFAULT;
	}

	mem0Dev->buffer = tmpBuffer;
	mem0Dev->bufferSize = len;
	kfree(tmpBuffer);
	*offset += len;

	printk("Data read: %s of size: %ld\n", mem0Dev->data, mem0Dev->bufferSize);

	return len;

}

ssize_t write(struct file *filp, const char __user *buff, size_t len, loff_t *offset) {

	if (!mem0Dev) {
		return -ENOMEM;
	}

	mem0Dev->data = kmalloc(len, GFP_KERNEL);
	if (!mem0Dev->data) {
		return -ENOMEM;
	}

	if (copy_from_user(mem0Dev->data, buff, len)) {
		return -ENOMEM;
	}
	mem0Dev->bufferSize = len;
	*offset += len;

	printk("Data read: %s of size: %ld\n", mem0Dev->data, mem0Dev->bufferSize);

	return 0;

}