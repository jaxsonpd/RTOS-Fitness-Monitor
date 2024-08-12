#ifdef POT_MANAGER_H_
#define POT_MANAGER_H_

#include <stdint.h>
#include <stdbool.h>

bool pot_read_init(void);

void pot_read_thread(void* args);

#endif // POT_MANGER_H_