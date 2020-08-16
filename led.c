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
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/of_gpio.h>



#define MIN(a,b) (a < b ? a : b)

typedef struct lxs_key
{
	unsigned int irq;
	int gpio_num;
	
	
}key;
static key *kkey;

static irqreturn_t key_handler(int irq, void *dev)
{
	printk("**************test**************\n");
	return IRQ_RETVAL(IRQ_HANDLED);
}

	

static int key_probe(struct platform_device *key_dev)
{ 
	struct device_node	*of_node = key_dev->dev.of_node;
	enum of_gpio_flags flags;
	kkey = kzalloc(sizeof(*kkey), GFP_KERNEL);
	kkey->gpio_num = of_get_gpio_flags(of_node,0,&flags);
	kkey->irq = gpio_to_irq(kkey->gpio_num);
	request_irq(kkey->irq,key_handler,IRQF_TRIGGER_FALLING,"key_irq",kkey);
	return 0;
}
static int key_remove(struct platform_device *led_dev)
{
	
	free_irq(kkey->irq, kkey);
	return 0;
}



static const struct of_device_id key_id_table[]=
{
	
	{.compatible = "lxs_key"},
	{ },
};


static  struct platform_driver key_driver =
{
	.probe  = key_probe,
	.remove = key_remove,
	.driver = 
	{
		.name = "lxs_key",
		.of_match_table = key_id_table,
	},
};

static int __init key_init(void)
{
	platform_driver_register(&key_driver);
	return 0;
} 


static void __exit key_exit(void)
{
	platform_driver_unregister(&key_driver);
}


module_init(key_init);
module_exit(key_exit);
MODULE_LICENSE("GPL");


