#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#define CNAME "mycdev"
#define COUNT 1
int major = 0;
int minor = 0;
struct cdev mycdev;
struct class *cls;
struct device *dev;
char *kbuf = NULL;
size_t kbuf_size = 0;

int mycdev_open(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

ssize_t mycdev_read(struct file *file, char __user *ubuf, size_t size, loff_t *offs)
{
    int ret;
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    if (size > kbuf_size)
        size = kbuf_size;

    ret = copy_to_user(ubuf, kbuf, size);
    if (ret)
    {
        printk("copy_to_user error\n");
        return -EIO;
    }
    return size;
}

ssize_t mycdev_write(struct file *file, const char __user *ubuf, size_t size, loff_t *offs)
{
    int ret;
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);

    if (kbuf)
        kfree(kbuf);

    kbuf = kzalloc(size, GFP_KERNEL);
    if (!kbuf)
    {
        printk("kzalloc error\n");
        return -ENOMEM;
    }
    kbuf_size = size;

    ret = copy_from_user(kbuf, ubuf, size);
    if (ret)
    {
        printk("copy_from_user error\n");
        kfree(kbuf);
        kbuf = NULL;
        kbuf_size = 0;
        return -EIO;
    }
    return size;
}

int mycdev_close(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

struct file_operations fops = {
    .open = mycdev_open,
    .read = mycdev_read,
    .write = mycdev_write,
    .release = mycdev_close,
};

static int __init mycdev_init(void)
{
    int ret;
    dev_t devno;

    ret = alloc_chrdev_region(&devno, minor, COUNT, CNAME);
    if (ret)
    {
        printk("alloc_chrdev_region error\n");
        return ret;
    }

    major = MAJOR(devno);
    cdev_init(&mycdev, &fops);
    ret = cdev_add(&mycdev, devno, COUNT);
    if (ret)
    {
        printk("cdev_add error\n");
        unregister_chrdev_region(devno, COUNT);
        return ret;
    }

    cls = class_create(THIS_MODULE, CNAME);
    if (IS_ERR(cls))
    {
        printk("class_create error\n");
        cdev_del(&mycdev);
        unregister_chrdev_region(devno, COUNT);
        return PTR_ERR(cls);
    }

    dev = device_create(cls, NULL, devno, NULL, "%s", CNAME);
    if (IS_ERR(dev))
    {
        printk("device_create error\n");
        class_destroy(cls);
        cdev_del(&mycdev);
        unregister_chrdev_region(devno, COUNT);
        return PTR_ERR(dev);
    }

    printk("%s:%s:%d: mycdev driver initialized\n", __FILE__, __func__, __LINE__);
    return 0;
}

static void __exit mycdev_exit(void)
{
    device_destroy(cls, MKDEV(major, minor));
    class_destroy(cls);
    cdev_del(&mycdev);
    unregister_chrdev_region(MKDEV(major, minor), COUNT);

    if (kbuf)
        kfree(kbuf);

    printk("%s:%s:%d: mycdev driver removed\n", __FILE__, __func__, __LINE__);
}

module_init(mycdev_init);
module_exit(mycdev_exit);

MODULE_LICENSE("GPL");
