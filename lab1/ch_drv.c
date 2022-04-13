#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#define BUFF_SIZE 1024
static dev_t first;
static struct cdev c_dev;
static struct class *cl;

ssize_t input_char_len = 0;
char *input_buff;
static struct proc_dir_entry *entry;


char out_result[100];


static double my_read(struct file *f, char __user *buf, size_t len,
                         loff_t *off) {
  int i = 0;
  int a = 0;
  int b = 0;
  int operation = 0;
  while(input_buff[i] != '\0'){
    if((input_buff[i] < 58) and (input_buff[i] > 47)){
    	if(!operation){
    		a = a * 10;
    		a = a + input_buff[i] - 48;
    	}else{
    		b = b * 10;
    		b = b + input_buff[i] - 48;
    	}
    }else if((input_buff[i] == '-') or (input_buff[i] == '+') or (input_buff[i] == '/') or (input_buff[i] == '*')){
	operation = input_buff[i];
    }else{
    	operation = 0;
    	break;
    }
    
    i = i+1;
  } 
  if(operation == 0){
  	printk(KERN_ERR "Unable to read line. Arguments persed as: %d and %d\n", a, b);
  	return NULL;
  }
  if((b == 0) and (operation == '/')){
  	printk(KERN_ERR "Dividing by zero");
  	return NULL;
  }
  double result = 0;
  switch(operation){
  	case '-':
  		result = a - b;
  		break;
  	case '+':
  		result = a + b;
  		break;
  	case '*':
  		result = a * b;
  		break;
  	case '/':
  		result = a / b;
  		break;
  }
  
  sprintf(out_result, "%d", result);

  if (copy_to_user(buf, out_result, sizeof(out_result)) != 0) {
    return -EFAULT;
  }


  return result;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len,
                          loff_t *off) {
    if(len>BUFF_SIZE){
        size_t new_len = len - BUFF_SIZE ;
        if (copy_from_user(input_buff, buf+BUFF_SIZE, new_len) != 0) {
            return -EFAULT;
        }
        if (copy_from_user(input_buff + new_len, buf, BUFF_SIZE) != 0) {
        return -EFAULT;
        }
    }

  if (copy_from_user(input_buff, buf, len) != 0) {
    return -EFAULT;
  }
  input_char_len += len - 1;
  printk(KERN_DEBUG "Input characters amount: %ld\n", input_char_len);

  return len;
}

static struct file_operations proc_fops = {
    .owner = THIS_MODULE, .read = my_read};

static struct file_operations dev_fops = {
    .owner = THIS_MODULE, .read = my_read, .write = my_write};

static int mychardev_uevent(struct device *dev, struct kobj_uevent_env *env) {
  add_uevent_var(env, "DEVMODE=%#o", 0666);
  return 0;
}

static int __init ch_drv_init(void) {
  input_buff = (char *)kmalloc(1024, GFP_KERNEL);
  if (!input_buff) {
    printk(KERN_ERR "%s:impossible to allocate memory\n", THIS_MODULE->name);
    return -1;
  }
  input_char_len = 0;
  if (!memset(input_buff, 0, BUFF_SIZE)) {
    kfree(input_buff);
    printk(KERN_ERR "%s:something went wrong\n", THIS_MODULE->name);
    return -1;
  }

  if (alloc_chrdev_region(&first, 0, 1, "ch_dev") < 0) {
    kfree(input_buff);
    return -1;
  }

  if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL) {
    kfree(input_buff);
    unregister_chrdev_region(first, 1);
    return -1;
  }

  cl->dev_uevent = mychardev_uevent;
  if (device_create(cl, NULL, first, NULL, "var2") == NULL) {
    kfree(input_buff);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }

  cdev_init(&c_dev, &dev_fops);
  if (cdev_add(&c_dev, first, 1) == -1) {
    kfree(input_buff);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }

  entry = proc_create("var2", 8675, NULL, &proc_fops);
  if (entry == NULL) {
    kfree(input_buff);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }

  printk(KERN_INFO "%s: dev file is created\n", THIS_MODULE->name);
  printk(KERN_INFO "%s: proc file is created\n", THIS_MODULE->name);

  return 0;
}

static void __exit ch_drv_exit(void) {
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first, 1);
  printk(KERN_INFO "%s: dev file is deleted\n", THIS_MODULE->name);

  kfree(input_buff);
  proc_remove(entry);
  printk(KERN_INFO "%s: proc file is deleted\n", THIS_MODULE->name);
  printk(KERN_INFO "Bye!!!\n");
}

module_init(ch_drv_init);
module_exit(ch_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Danila Gorelko, Boris Novoselov");
MODULE_DESCRIPTION("The first kernel module");
