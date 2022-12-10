#include "write_by_address.h"

#define handle_(type)                                         \
    next = strtok_r(NULL, " ", &save);                        \
                                                              \
    type *variable;                                           \
                                                              \
    if (isdigit(*next))                                       \
    {                                                         \
        variable = get_ptr_by_address(parse_##type(next));    \
    }                                                         \
    else                                                      \
    {                                                         \
        variable = (type *)dlsym(NULL, next);                 \
    }                                                         \
                                                              \
    next = strtok_r(NULL, " ", &save);                        \
    if (strcmp(next, "=") == 0)                               \
    {                                                         \
        next = strtok_r(NULL, " ", &save);                    \
        if (isdigit(*next))                                   \
        {                                                     \
            type v = parse_##type(next);                      \
            *variable = v;                                    \
            char buffer[256];                                 \
            sprintf(buffer, "wrote %d to %p\n", v, variable); \
            out.out(buffer, out.args);                        \
        }                                                     \
    }

int write_by_address_command(struct printer out, const char *args)
{
    char *save;
    char args_buffer[256];
    strcpy(args_buffer, args);
    char *token = strtok_r(args_buffer, " ", &save);
    if (strcmp(token, "w") != 0)
        return ERR;

    char *next = strtok_r(NULL, " ", &save);
    if (next)
    {
        char *type = next;

        if (strcmp(type, "u8") == 0)
        {
            handle_(u8);
        }
        else if (strcmp(type, "u16") == 0)
        {
            handle_(u16);
        }
        else if (strcmp(type, "u32") == 0)
        {
            handle_(u32);
        }
    }

    return OK;
}
