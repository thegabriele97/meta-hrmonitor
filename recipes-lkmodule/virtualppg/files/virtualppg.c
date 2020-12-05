#include "linux/module.h"
#include "linux/init.h"
#include "linux/kernel.h"
#include "linux/cdev.h"
#include "linux/device.h"
#include "linux/fs.h"
#include "linux/uaccess.h"
#include "data.h"

#define KBUILD_MODNAME "VirtPPGSensor"

#define PRINTK(level, msg, ...) printk(level KBUILD_MODNAME ": " msg, ##__VA_ARGS__)
#define PRINTK_0(level, msg) printk(level KBUILD_MODNAME ": " msg)
#define PRINTK_INF(msg) PRINTK_0(KERN_INFO, msg)
#define PRINTK_ERR(msg) PRINTK_0(KERN_ERR, msg)

static int __init mod_init(void);
static void mod_cleanup(void);
static ssize_t read(struct file *filp, char *buf, size_t count, loff_t *f_pos);

static dev_t mod_dev;
struct cdev mod_cdev;
struct class *mod_class;
static struct device *device;

struct file_operations f_ops = {
    .owner = THIS_MODULE,
    .read = read
};

static int __init mod_init() {
    int status;

    PRINTK_INF("Loading module...\n");

    // Trying to allocate a now character device region
    status = alloc_chrdev_region(&mod_dev, 0, 1, KBUILD_MODNAME);

    // Allocating a new cdev struct
    cdev_init(&mod_cdev, &f_ops);
    mod_cdev.owner = f_ops.owner;

    // Checking for errors
    if (status < 0 || (status = cdev_add(&mod_cdev, mod_dev, 1)) < 0) {
        PRINTK(KERN_ERR, "An error occurred while registering the new char device: %d\n", status);
        return (mod_cleanup(), status);
    }

    mod_class = class_create(THIS_MODULE, "virtppg");
    device = device_create(mod_class, NULL, mod_dev, NULL, "virtppg");

    PRINTK(KERN_INFO, "Module registered successfully with major number %d:%d\n", MAJOR(mod_dev), MINOR(mod_dev));

    return 0;
}

static void mod_cleanup() {

    PRINTK_INF("Unloading...\n");

    if (mod_class) {
        device_destroy(mod_class, mod_dev);
        class_destroy(mod_class);
    }

    cdev_del(&mod_cdev);
    unregister_chrdev_region(mod_dev, 1);

    PRINTK_INF("Module unloaded successfully. Bye!\n");
}

static ssize_t read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
    ssize_t bytes_read = 0;
    char *curr_ptr;

    PRINTK(KERN_DEBUG, "Request for reading %d bytes from %llu\n", count, *f_pos);

    while (count > 0) { 

        curr_ptr = ((char *)ppg) + *f_pos;
        PRINTK(KERN_DEBUG, "Copying to user %#x, %d bytes from %#x : %#x\n", (int)buf, 1, (int)curr_ptr, (char)(*curr_ptr));
        if (copy_to_user(buf++, curr_ptr, 1) != 0) {
            PRINTK_ERR("Something bad happened while copying data to user. Returning..\n");
            break;
        }
        
        *f_pos += 1;
        if (*f_pos >= (N_VALS << 2)) {
            *f_pos = 0;
        }
        
        count--;
        bytes_read++;
    }

    return bytes_read;
}

module_init(mod_init);
module_exit(mod_cleanup);

MODULE_AUTHOR("Gabriele La Greca @thegabriele97");
MODULE_LICENSE("GPL v2");