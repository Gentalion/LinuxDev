#include <stdio.h>
#include "outlib.h"
void output(char *str) {
        printf("%d: %s\012", Count++, str);
}