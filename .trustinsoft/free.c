#include <stdlib.h>
#include "tis_builtin.h"

/*@ requires UB_free_null_pointer: ptr != \null; */
void free(void *ptr) {
    tis_free(ptr);
}
