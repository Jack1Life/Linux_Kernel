#include "hw.h"
#include "kparens.h"
#include "kobj.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yevhenii Horbul");

static int __init hw_obj_init(void)
{ 
  printk(KERN_WARNING "lktm: obj: init\n");
  
  if(kparens_create("my_kset") < 0)
  {
    printk(KERN_ALERT "kset create - fail");
    return -1;
  }

  if(kparens_create_file() < 0)
  {
    printk(KERN_ALERT "kset file create - fail");
    return -1;
  }

  if( kobj_create(current_kset) < 0)
  {
    printk(KERN_ALERT "hw: kobj failed\n");
    return -1;
  }
  return 0;
}

static void __exit hw_obj_exit(void)
{
  kobj_delete_all(current_kset);
  kparens_remove_file();
  kset_unregister(current_kset);
  printk(KERN_WARNING "lktm: obj: exit\n");
}

module_exit(hw_obj_exit);
module_init(hw_obj_init);