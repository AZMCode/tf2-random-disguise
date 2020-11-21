#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/random.h>
#include <linux/miscdevice.h>
#define DISGUISE_PATTERN "disguise 0 -1\nvoicemenu 2 1\n"


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
    .read = read_disguise,
    .open = NULL
};

struct miscdevice disg_dev_cfg = {
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &fops,
    .name = "tf2d",
    .mode = 444
};


static int __init init_disg_module(void){
    const int regResult = misc_register(&disg_dev_cfg);
    if (regResult != 0) {
        printk(KERN_INFO "Error %i registering TF2 disguise device",regResult);
        return -1;
    }
    return 0;
}

static void __exit exit_disg_module(void){
    misc_deregister(&disg_dev_cfg);
}
module_init(init_disg_module);
module_exit(exit_disg_module);