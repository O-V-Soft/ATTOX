#ifndef USER_H
#define USER_H
#include <stdint.h>

void switch_to_user_mode(void (*user_func)());

#endif