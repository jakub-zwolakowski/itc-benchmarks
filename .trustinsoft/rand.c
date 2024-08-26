#include <stdlib.h>
#include "tis_builtin.h"

int rand(void) {
    int any_int = 0;
    tis_make_unknown(&any_int, sizeof(any_int));
    return any_int;
}
