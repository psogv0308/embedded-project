#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/ioctl.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/version.h>

#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <mach/gpio.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include "./fpga_dot_font.h"

#define IOM_MAJOR 242		// ioboard led device major number
#define IOM_NAME "dev_driver"		// ioboard led device name

#define IOM_FND_ADDRESS 0x08000004 
#define IOM_LED_ADDRESS 0x08000016
#define IOM_FPGA_TEXT_LCD_ADDRESS 0x08000090
#define IOM_FPGA_DOT_ADDRESS 0x08000210 // pysical address


//Global variable
static unsigned char *iom_fpga_fnd_addr;
static unsigned char *iom_fpga_dot_addr;
struct timer_list *my_timer;
int red_time,blue_time;
int turn;
int sleep;
// define functions...
void dot_controller();
void fnd_controller(unsigned char min,unsigned char sec);
int iom_fpga_open(struct inode *minode, struct file *mfile);
int iom_fpga_release(struct inode *minode, struct file *mfile);
long iom_fpga_ioctl(struct file *mfile, unsigned int cmd, unsigned long arg);
// define file_operations structure 
static int inter_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
irqreturn_t inter_handler(int irq, void* dev_id, struct pt_regs* reg);

wait_queue_head_t wq_write;
DECLARE_WAIT_QUEUE_HEAD(wq_write);

void timer_handler(unsigned long arg){
	int data;
	my_timer->expires = get_jiffies_64() + HZ; //next timer time
	if(turn==0){
		blue_time--;
		data=blue_time;
	}
	else{
		red_time--;
		data=red_time;
	}
	fnd_controller(data/60,data%60);
	add_timer(my_timer); 
}
void dot_controller(){
	int i;
        unsigned short int value;
	for(i=0;i<10;i++){
		value=fpga_number[turn][i];
		outw(value,(unsigned int)iom_fpga_dot_addr+i*2);
	}
}	
void fnd_controller(unsigned char min,unsigned char sec){
        unsigned short int value_short;
        value_short=((min/10)<<12)|((min%10)<<8)|((sec/10)<<4)|(sec%10);
        outw(value_short,(unsigned int)iom_fpga_fnd_addr);
}
struct file_operations iom_fops =
{
	.owner=		THIS_MODULE,
	.unlocked_ioctl=iom_fpga_ioctl,
	.open=		iom_fpga_open,
	.release=	iom_fpga_release,
	.write = inter_write,
};
irqreturn_t inter_handler(int irq, void* dev_id, struct pt_regs* reg) {
        __wake_up(&wq_write,1,1,NULL);
	return IRQ_HANDLED;
}

//ioctl
long iom_fpga_ioctl(struct file *mfile, unsigned int cmd, unsigned long arg){
	//in case our timer is running now
	if(cmd==0){
		init_timer(my_timer);
		red_time=blue_time=1200;
		turn=0;
		my_timer->expires = get_jiffies_64() + HZ;
		my_timer->data=blue_time;
		my_timer->function = timer_handler;
		fnd_controller(20,0);
		dot_controller();
		add_timer(my_timer);
	}
	else if(cmd==1){
		turn=1-turn;
		if(turn==0)
			fnd_controller(blue_time/60,blue_time%60);
		else
			fnd_controller(red_time/60,red_time%60);
		dot_controller();
	}
	else{
		turn=2;
		fnd_controller(0,0);
		dot_controller();
		del_timer(my_timer);
	}
	return 0;
}

int iom_fpga_open(struct inode *minode, struct file *mfile) 
{	
	int irq,ret;
	sleep=0;
	gpio_direction_input(IMX_GPIO_NR(5,14));
        irq = gpio_to_irq(IMX_GPIO_NR(5,14));
        printk(KERN_ALERT "IRQ Number : %d\n",irq);
        ret=request_irq(irq, inter_handler, IRQF_TRIGGER_FALLING, "voldown", 0);
	return 0;
}

int iom_fpga_release(struct inode *minode, struct file *mfile) 
{
	free_irq(gpio_to_irq(IMX_GPIO_NR(5, 14)), NULL);
	return 0;
}

static int inter_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos ){
        printk("sleep on\n");
        interruptible_sleep_on(&wq_write); //sleep the process

        printk("write\n");
        return 0;
}


int __init iom_init(void)
{
	int result;

	result = register_chrdev(IOM_MAJOR, IOM_NAME, &iom_fops);
	if(result < 0) {
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}
	//memory mapping to kernel space
	iom_fpga_dot_addr = ioremap(IOM_FPGA_DOT_ADDRESS, 0x10);
	iom_fpga_fnd_addr = ioremap(IOM_FND_ADDRESS, 0x4);
	printk("init module, %s major number : %d\n", IOM_NAME, IOM_MAJOR);

	my_timer = kmalloc(sizeof(struct timer_list),GFP_KERNEL);
	if(my_timer==NULL) return -1;
	fnd_controller(0,0);

	return 0;
}

void __exit iom_exit(void) 
{
	//memory unmapping
	iounmap(iom_fpga_dot_addr);
	iounmap(iom_fpga_fnd_addr);
	if(my_timer!=NULL){
		kfree(my_timer);
	}
	unregister_chrdev(IOM_MAJOR, IOM_NAME);
}

module_init(iom_init);
module_exit(iom_exit);
MODULE_LICENSE("GPL");
