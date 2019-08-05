#include "hw.h"
#include "kparens.h"

static ssize_t kparens_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t kparens_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

struct kset *current_kset = NULL;
struct kobj_attribute create = __ATTR(kset_file, S_IRUSR | S_IWUSR, kparens_show, kparens_store);


ssize_t kparens_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    static u8 i;
    i++;
    return sprintf(buf, "%u\n", i);
}

ssize_t kparens_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
  return 0;
}

int kparens_create(char* name)
{
    current_kset = kset_create_and_add(name, NULL, kernel_kobj);
  
    if(current_kset == NULL)
        return -1;
    else
        return 0;
    
}

int kparens_create_file()
{
    if(sysfs_create_file(&current_kset->kobj, &create.attr) < 0)
        return -1;
    else
        return 0;
}

int kparens_remove_file(void)
{
    sysfs_remove_file(&current_kset->kobj, &create.attr);
    return 0;
}
