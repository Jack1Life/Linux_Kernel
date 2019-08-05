
#include "kobj.h"

static ssize_t hw_show(struct kobject *kobj, struct attribute *attr, char *buf);
static ssize_t hw_store(struct kobject *kobj, struct attribute *attr, const char *buf, size_t count);
static void    hw_release(struct kobject *kobj);

static struct attribute hw_attr1 = {
  .name = "home_work1",
  .mode = S_IWOTH | S_IROTH  /*| S_IXOTH | S_ISVTX S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP */
};

static struct attribute hw_attr2 = {
  .name = "home_work2",
  .mode =   S_IWOTH | S_IROTH  /*| S_IXOTH | S_ISVTX S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP*/
};

static struct attribute *hw_def_attrs[] = {
  &hw_attr1,
  &hw_attr2,
  NULL
};

static struct sysfs_ops hw_def_ops = {
  .show          = hw_show,
  .store         = hw_store
};

static struct kobj_type hw_ktype = {
  .release       = hw_release,
  .sysfs_ops     = &hw_def_ops,
  .default_attrs = hw_def_attrs
};

static ssize_t hw_show(struct kobject *kobj, struct attribute *attr, char *buf)
{
    u32 num_bytes = 0;
    struct hw_kobj *child = container_of(kobj, struct hw_kobj, kobj);
    struct hw_kobj *entry = NULL;
    struct list_head *list;
    printk(KERN_DEBUG "hw obj: show value\n");

     for(list = &child->list; ;list = list->next)   //list_for_each_entry(entry, &child->list, list)//
    {
      entry =  container_of(list, struct hw_kobj, list);
      strcat(buf, entry->data);
      num_bytes +=  strlen(entry->data);
      if(list->next == &child->list)
        break;
    }
    return num_bytes; 
}

static ssize_t hw_store(struct kobject *kobj, struct attribute *attr, const char *buf, size_t count)
{
    struct hw_kobj *child = container_of(kobj, struct hw_kobj, kobj);
    struct hw_kobj *last_member = NULL;       //list_first_entry(&child->list, struct hw_kobj, list);
    struct hw_kobj *new_member = NULL;
    struct list_head *last;

    new_member = kzalloc(sizeof(*new_member), GFP_KERNEL);
    if(new_member == NULL)
    {
      printk(KERN_DEBUG "hw obj: store value failed\n");
      return -1;
    } 
    for(last = &child->list; ; last = last->next)
    {
      if(last->next == &child->list)
      {
        last_member = container_of(last, struct hw_kobj, list);
        break;
      }
    }
    INIT_LIST_HEAD(&new_member->list);
    new_member->list.next = &child->list;
    new_member->list.prev = &last_member->list;
    last_member->list.next = &new_member->list;

    strcat(new_member->data, buf);
    strcat(new_member->data, "\n");
    printk(KERN_DEBUG "hw obj: store value success. Last member: %s\n", last_member->data);
    return count;
}

static void    hw_release(struct kobject *kobj)
{
    struct hw_kobj *child = container_of(kobj, struct hw_kobj, kobj);
    struct list_head *list;
    struct hw_kobj *entry = NULL;
    printk(KERN_DEBUG "hw obj: release %s\n", kobject_name(kobj));
  
    for(list = &child->list; ;list = list->next)      //list_for_each_entry(entry, &child->list, list)//
    {
      entry =  container_of(list, struct hw_kobj, list);
      kfree(entry);
      if(list->next == &child->list)
        break;
    }
}

u32 kobj_create(struct kset* parens)
{
    struct hw_kobj *child = NULL;
    int result = -1;
    static u32 index;

    child = kzalloc(sizeof(*child), GFP_KERNEL);
    if(child == NULL)
    {
        printk(KERN_WARNING "Home work module: fail to alocate memory\n");
        return -1;
    }
    INIT_LIST_HEAD(&child->list);
    sprintf(child->data, "%s\n", "First string in head");
    child->kobj.kset = parens;

    result = kobject_init_and_add(&child->kobj, &hw_ktype, NULL, "hw%d", index);
    if(result < 0)
    {
        printk(KERN_WARNING "Home work module: kobject init and add - fail. Result %d\n", result);
        kfree(child);
        return -1;
    }
    return 0;
}

void kobj_delete_all(struct kset* parens)
{
  struct kobject *k = NULL;
  struct kobject *tmp = NULL;

  list_for_each_entry_safe(k, tmp, &parens->list, entry) 
  {
      kobject_put(k);
  }
}