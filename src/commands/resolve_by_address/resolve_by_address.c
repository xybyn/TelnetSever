#include "resolve_by_address.h"

#define handle_(type)                                           \
const char *symbol_or_address = strtok_r(NULL, DELIMITERS, &save); \
                                        \
 if (isdigit(*symbol_or_address))\
{                                                               \
unsigned long v = parse_u32(symbol_or_address);              \
void *p = get_ptr_by_address(v);                                \
print_var_##type(out, p);\
}\
else\
{                                                               \
void *p = dlsym(NULL, symbol_or_address);                       \
print_var_##type(out, p);\
}

int resolve_by_address_command(struct printer out, const char *args) {
    char *save;
    char args_buffer[256];
    strcpy(args_buffer, args);
    char *token = strtok_r(args_buffer, DELIMITERS, &save);
    if (strcmp(token, "r") != 0)
        return ERR;
    char *next = strtok_r(NULL, DELIMITERS, &save);
    if (next) {
        char *type = next;
        if (strcmp(type, "u8") == 0) {
            handle_(u8);
        } else if (strcmp(type, "u16") == 0) {
            handle_(u16);
        } else if (strcmp(type, "u32") == 0) {
            printf("---%s\n", next);
            handle_(u32);
        } else {
            uint address = parse_u32(next);
            void *ptr = get_ptr_by_address(address);

            print_function(out, ptr);
        }
    }

    return OK;
}
