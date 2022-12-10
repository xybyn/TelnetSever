#include "dump.h"
#include <stdio.h>
void print_dump_row(struct printer out, byte *p, int count)
{
    for (int i = 0; i < count; i++)
    {
        print_byte_hex(out, p[i]);
        out.out(", ", out.args);
    }
    out.out("//", out.args);

    for (int i = 0; i < count; i++)
    {
        print_byte_character(out, p[i]);
    }
    out.out("\n", out.args);
}

void print_dump_table(struct printer out, void *pointer, int count)
{
    int columns_in_row = 8;
    int count_of_rows = count / columns_in_row;
    if (count % columns_in_row != 0)
        count_of_rows++;
    int offset = 0;
    int bytes_left = count;
    byte *p = (byte *)pointer;

    for (int i = 0; i < count_of_rows; i++)
    {
        if (bytes_left < columns_in_row)
        {
            print_dump_row(out, p + offset, bytes_left);
        }
        else
        {
            print_dump_row(out, p + offset, columns_in_row);
            bytes_left -= columns_in_row;
            offset += columns_in_row;
        }
    }
}

int dump_command(struct printer out, const char *arg)
{
    char args_buffer[256];
    strcpy(args_buffer, arg);
    char *save;
    char *token = strtok_r(args_buffer, DELIMITERS, &save);
    if (strcmp(token, "mem") != 0)
        return ERR;

    token = strtok_r(NULL, DELIMITERS, &save);
    if (strcmp(token, "dump") == 0)
    {
        token = strtok_r(NULL, DELIMITERS, &save);
        u32 address = parse_u32(token);
        token = strtok_r(NULL, DELIMITERS, &save);
        u32 size = parse_u32(token);
        token = NULL;
        print_dump_table(out, get_ptr_by_address(address), size);
    }
    else if (strcmp(token, "write") == 0)
    {
        token = strtok_r(NULL, DELIMITERS, &save);
        u32 address = parse_u32(token);

        void *pointer = get_ptr_by_address(address);

        byte bytes[256];
        int size = 0;
        while (1)
        {
            token = strtok_r(NULL, DELIMITERS, &save);
            if (!token)
                break;
            u8 v = hex_to_u8(token);
            bytes[size] = v;
            size++;
        }

        write_memory(pointer, bytes, size);
        char buffer[256];
        sprintf(buffer, "wrote %d bytes to address %p\n", size, pointer);
        out.out(buffer, out.args);
    }
    else
        return ERR;

    return OK;
}
