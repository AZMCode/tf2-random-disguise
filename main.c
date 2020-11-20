#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/random.h>
#define DISGUISE_PATTERN "disguise 0 -1\nvoicemenu 2 1"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adriano Zambrana Marchetti");
MODULE_DESCRIPTION("TF2 Random disguise device");
MODULE_VERSION("0.0.1");

ssize_t read_disguise(struct file* file_reading, char __user* user_buffer, size_t size_to_read, loff_t* offset){
    if(*offset >= strlen(DISGUISE_PATTERN)){
        return 0;
    } else {
        int i;
        int readedBytes;
        for(i=*offset;i<strlen(DISGUISE_PATTERN) && i<size_to_read;i++){
            if(i == 9){
                char randomClass = 0;
                while(randomClass < 1 || randomClass > 9){
                    get_random_bytes(&randomClass,sizeof(char));
                }
                randomClass = randomClass + 48;
                user_buffer[i] = randomClass;
            } else {
                user_buffer[i] = DISGUISE_PATTERN[i];
            }
        }
        readedBytes = i-*offset;
        *offset = i;
        return readedBytes;
    }
};

struct file_operations fops = {
    .read = read_disguise
};
struct cdev cdev_cfg = {
    .owner = THIS_MODULE,
    .ops = &fops
};

int majorNumber = -1;
struct class* cls;


static int __init init_disg_module(void){
    int regResult;
    printk(KERN_INFO "tf2-random-disguise kernel module loading");
    

    regResult = register_chrdev(0,"tf2d", &fops);
    if(regResult < 0){
        printk(KERN_ALERT "Registering device failed with %d.",regResult);
        return -1;
    }
    majorNumber = regResult;
    cls = class_create(THIS_MODULE,"tf2d");
    cdev_init(&cdev_cfg,&fops);
    cdev_add(&cdev_cfg,0,1);
    device_create(cls,NULL,MKDEV(majorNumber,0),NULL,"tf2d");
    printk(KERN_INFO "tf2-random-disguise kernel module loaded\n");
    return 0;
}

static void __exit exit_disg_module(void){
    if(majorNumber == -1){
        printk(KERN_ALERT "Module could not remember major number");
    }
    device_destroy(cls, MKDEV(majorNumber, 0));
    cdev_del(&cdev_cfg);
    class_destroy(cls);
    unregister_chrdev(majorNumber,"tf2d");
    printk(KERN_INFO "Module unloaded and removed\n");
}
module_init(init_disg_module);
module_exit(exit_disg_module);