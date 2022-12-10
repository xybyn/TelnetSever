#include "resolve.h"

int resolve_command(struct printer out, const char *args)
{
    char args_buffer[256];
    strcpy(args_buffer, args);
    char *save;
    char *token = strtok_r(args_buffer, DELIMITERS, &save);
    if (strcmp(token, "s") != 0)
        return ERR;

    token = strtok_r(NULL, " ", &save);

    void *p = dlsym(NULL, token);
    char buffer[256];
    sprintf(buffer, "symbol '%s' at %p\n", token, p);
    out.out(buffer, out.args);
    return OK;
}