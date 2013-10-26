#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

static char msg[100] = "hello userspace from kernel";
static short readPos = 0;
static int times = 0;
static dev_t first;
static struct class *cl;
static struct cdev c_dev;

int dev_open(struct inode *, struct file *filep);
int dev_rls(struct inode *, struct file *filep);
ssize_t dev_read(struct file *filep, char *buff, size_t len, loff_t *off);
ssize_t dev_write(struct file *filep, const char *buff, size_t len, loff_t *off);


static struct file_operations fops = {
  .open = dev_open,
  .read = dev_read,
  .write = dev_write,
  .release = dev_rls,
};

int init_module(void)
{
  printk(KERN_INFO "init_module() called\n");
  if (alloc_chrdev_region(&first, 0, 1, "cryptctl") < 0)
    {
      return -1;
    }
  if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
      unregister_chrdev_region(first, 1);
      return -1;
    }
  if (device_create(cl, NULL, first, NULL, "cryptctl") == NULL)
    {
      class_destroy(cl);
      unregister_chrdev_region(first, 1);
      return -1;
    }
  cdev_init(&c_dev, &fops);
  if(cdev_add(&c_dev, first, 1) == -1)
    {
      device_destroy(cl, first);
      class_destroy(cl);
      unregister_chrdev_region(first, 1);
      return -1;
    }

  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "cleanup_module() called\n");
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first,1);
  
  printk(KERN_INFO "chr dev unregistered\n");
}

int dev_open(struct inode *inode, struct file *filep)
{
  times++;
  printk(KERN_INFO"Device opened %d times\n", times);
  return 0;
}

ssize_t dev_read(struct file *filep, char *buff, size_t len, loff_t *off)
{
  printk(KERN_INFO "reading device\n");
  return 0;
}

ssize_t dev_write(struct file *filep, const char *buff, size_t len, loff_t *off)
{
  printk(KERN_INFO "Writing to device\n");
  return 0;
}

int dev_rls(struct inode *inode, struct file *filep)
{
  printk(KERN_INFO "Device closed\n");
  return 0;
}
