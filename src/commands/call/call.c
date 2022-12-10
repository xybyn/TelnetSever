#include "call.h"

int call_command(struct printer out, const char *args)
{
    char *save;
    char args_buffer[256];
    strcpy(args_buffer, args);

    char *token = strtok_r(args_buffer, DELIMITERS, &save);
    if (strcmp(token, "c") != 0)
        return ERR;

    token = strtok_r(NULL, DELIMITERS, &save);

    char *open_bracket = strchr(token, '(');
    char *function_params_buffer = (char *)malloc(strlen(open_bracket));
    function_params_buffer[strlen(open_bracket)] = '\0';
    memcpy(function_params_buffer, open_bracket + 1, strlen(open_bracket) - 2);

    int name_len = open_bracket - token;
    char *name = (char *)malloc(name_len + 1);
    memcpy(name, token, name_len);
    name[name_len] = '\0';

    typedef u32 (*f_ptr)();
    f_ptr fun = (f_ptr)dlsym(NULL, name);
    if (!fun)
    {
        out.out("cant find that function\n", out.args);
        return ERR;
    }

    int args_count = 0;

    char *function_arg = strtok_r(function_params_buffer, ",", &save);

    char *b[5];
    int str_indices[5];
    int str_indices_size = 0;
    while (1)
    {
        if (function_arg[0] == '\"')
        {
            int s = strlen(function_arg) - 1;
            char *function_arg_buffer = (char *)malloc(sizeof(char) * (s));
            memcpy(function_arg_buffer, function_arg + 1, s - 1);
            function_arg_buffer[s] = '\0';
            b[args_count] = function_arg_buffer;
            str_indices[str_indices_size] = args_count;
            str_indices_size++;
        }
        else
        {
            b[args_count] = (u32)dec_to_u32(function_arg);
        }
        args_count++;

        function_arg = strtok_r(NULL, ",", &save);
        if (!function_arg)
            break;
    }

    u32 result;
    if (args_count == 0)
        result = fun();
    else if (args_count == 1)
        result = fun(b[0]);
    else if (args_count == 2)
        result = fun(b[0], b[1]);
    else if (args_count == 3)
        result = fun(b[0], b[1], b[2]);
    else if (args_count == 4)
        result = fun(b[0], b[1], b[2], b[3]);
    else if (args_count == 5)
        result = fun(b[0], b[1], b[2], b[3], b[4]);

    char buffer[256];
    sprintf(buffer, "func %s at %p returned 0x%lX\n", name, fun, result);
    out.out(buffer, out.args);
    free(name);
    free(function_params_buffer);

    for (int i = 0; i < str_indices_size; i++)
        free(b[str_indices[i]]);

    return OK;
}