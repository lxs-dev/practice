#include<linux/fs.h>
#include<linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>



#define MIN(a,b) (a < b ? a : b)

typedef struct lxs_led
{
	unsigned int major;
	char *name;
	struct class *led_class;
	struct device *led_1;
	struct platform_device *dev;
	struct gpio_desc *led_index;
	
}led;
led lled;

char read_buf[100];
char write_buf[100];
char *buf = "1234567890";
char recvbuf[100];
ssize_t led_read (struct file *file, char __user *data, size_t size, loff_t *loff)
{
	int ret = 0;
	//printk("read()\n");
	memcpy(read_buf,buf,strlen(buf));
	printk("kernal:%d\n",strlen(buf));
	ret = copy_to_user(data, read_buf, strlen(buf));
	return strlen(buf) ;
}
ssize_t led_write (struct file *file, const char __user *data, size_t size, loff_t *loff)
{
	int ret;
	int led_state;
	//printk("write()\n");
	ret = copy_from_user(&led_state,data,sizeof(led_state));
	gpiod_set_value(lled.led_index,led_state);
	printk("state:%d\n",led_state);
	//printk("revc:%s\n",recvbuf);
	return 0;
}
int led_open (struct inode *inode, struct file *file)
{
	//printk("led_open()\n");
	
	return 0;
}
int led_close (struct inode *inode, struct file *file)
{
	//printk("led_close()\n");
	//gpiod_set_value(lled.led_index,1);
	return 0;
}


static struct file_operations led_op = {
	.owner = THIS_MODULE,
	.read = led_read,
	.write = led_write,
	.open = led_open,
	.release = led_close,
};

static int led_probe(struct platform_device *led_dev)
{
	lled.name = "lxs_led";
	lled.dev = led_dev;
	lled.led_index = gpiod_get_index(&led_dev->dev, "led",0,GPIOD_OUT_HIGH);
	lled.major = register_chrdev(0, lled.name, &led_op);
	if(lled.major < 0){
		printk("register_chrdev()\n");
		return -1;
	}
	lled.led_class = class_create(THIS_MODULE, "lxs_led");
	
	lled.led_1 = device_create(lled.led_class,NULL,
			     MKDEV(lled.major,0), NULL,"led%d",0);
	return 0;
}
static int led_remove(struct platform_device *led_dev)
{
	device_destroy(lled.led_class, MKDEV(lled.major,0));
	class_destroy(lled.led_class);
	unregister_chrdev(lled.major,lled.name);
	gpiod_put(lled.led_index);
	return 0;
}



static const struct of_device_id led_id_table[]=
{
	
	{.compatible = "100ask,leddrv"},
	{ },
};


static  struct platform_driver led_driver =
{
	.probe  = led_probe,
	.remove = led_remove,
	.driver = 
	{
		.name = "100ask,leddrv",
		.of_match_table = led_id_table,
	},
};

static int __init led_init(void)
{
	platform_driver_register(&led_driver);
	return 0;
} 


static void __exit led_exit(void)
{
	platform_driver_unregister(&led_driver);
}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");


