#ifndef KPARENS
#define KPARENS

int kparens_create(char* name);
int kparens_create_file(void);
int kparens_remove_file(void);

extern struct kset *current_kset;
#endif