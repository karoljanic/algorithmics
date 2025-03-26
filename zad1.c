#include <stdio.h>

int main() {
    const char* source = "#include <stdio.h>%c%cint main() {%c    const char* source = %c%s%c;%c    printf(source, 10, 10, 10, 34, source, 34, 10, 10, 10, 10);%c    return 0;%c}%c";
    printf(source, 10, 10, 10, 34, source, 34, 10, 10, 10, 10);
    return 0;
}
