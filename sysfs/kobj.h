#ifndef KOBJ
#define KOBJ

#include "hw.h"

void kobj_delete_all(struct kset* parens);
u32 kobj_create(struct kset* parens);

#endif