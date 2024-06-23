#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/mm.h>
#define CNAME "cdev"

void *ptr;
int major;
struct class *cls;
struct device *dev;
int ret;

int myopen(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

int myclose(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}
// vm_struct结构体和vm_area_struct结构体的区别
int mmap(struct file *file, struct vm_area_struct *vma)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    // 1.通过虚拟地址得到对应的物理地址
    unsigned long pfn = virt_to_phys(ptr)>>PAGE_SHIFT;
    unsigned long size = vma->vm_end - vma->vm_start;
    // 2.把物理地址通过vm_area_struct结构体映射到用户空间
    ret=remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot);
    if(ret){
        printk("remap_pfn_range err\n");
        return -EAGAIN;
    }
    return 0;
}

struct file_operations fop = {
    .open = myopen,
    .release = myclose,
    .mmap = mmap,
};

static int __init mycdev_init(void)
{
    major = register_chrdev(0, CNAME, &fop);
    if (major < 0)
    {
        printk("register err\n");
        return major;
    }

    cls = class_create(THIS_MODULE, CNAME);
    if (IS_ERR(cls))
    {
        unregister_chrdev(major, CNAME);
        return PTR_ERR(cls);
    }

    dev = device_create(cls, NULL, MKDEV(major, 0), NULL, "cdev");
    if (IS_ERR(dev))
    {
        class_destroy(cls);
        unregister_chrdev(major, CNAME);
        return PTR_ERR(dev);
    }

    ptr = kzalloc(PAGE_SIZE, GFP_ATOMIC);
    if (!ptr)
    {
        device_destroy(cls, MKDEV(major, 0));
        class_destroy(cls);
        unregister_chrdev(major, CNAME);
        return -ENOMEM;
    }

    return 0;
}

static void __exit mycdev_exit(void)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
    unregister_chrdev(major, CNAME);
    kfree(ptr);
}

module_init(mycdev_init);
module_exit(mycdev_exit);

MODULE_LICENSE("GPL");
