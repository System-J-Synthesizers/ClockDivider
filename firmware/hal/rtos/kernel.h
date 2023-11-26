#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

void os_pend_context_switch(void);
void os_init(void);

#endif /* KERNEL_H */