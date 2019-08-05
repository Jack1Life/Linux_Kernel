#ifndef HOMEWORK
#define HOMEWORK

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/uaccess.h> /* to access to user space*/
#include <linux/slab.h>

struct hw_kobj
{
  struct kobject        kobj;  
  struct list_head      list;
  char                  data[];
};
#endif