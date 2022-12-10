#include "common.h"

#define dec_to_(type)                            \
    type dec_to_##type(const char *dec_string)   \
    {                                            \
        return (type)strtoul(dec_string, 0, 10); \
    }
#define hex_to_(type)                            \
    type hex_to_##type(const char *hex_string)   \
    {                                            \
        return (type)strtoul(hex_string, 0, 16); \
    }

#define write_var_(type)                             \
    type write_var_##type(type *pointer, type value) \
    {                                                \
        *pointer = value;                            \
    }

#define parse_(type)                   \
    type parse_##type(const char *str) \
    {                                  \
        if (is_hex(str))               \
            return hex_to_##type(str); \
        return dec_to_##type(str);     \
    }

#define write_var_symbol_(type)                                  \
    void write_var_symbol_##type(const char *symbol, type value) \
    {                                                            \
        type *p = (type *)dlsym(NULL, symbol);                   \
        *p = value;                                              \
    }

#define print_var_(type)                                                          \
    void print_var_##type(struct printer out, type *pointer)                                  \
    {                                                                             \
        typedef struct                                                            \
        {                                                                         \
            const char *dli_fname;                                                \
            void *dli_fbase;                                                      \
            const char *dli_sname;                                                \
            void *dli_saddr;                                                      \
        } Dl_info;                                                                \
        Dl_info info;                                                             \
        dladdr(pointer, &info);                                                   \
        char buffer[256];                                                                              \
        sprintf(buffer, "*%s(%p)=0x"type##HEX_FORMAT"\n", info.dli_sname, info.dli_saddr, *pointer); \
        out.out(buffer, out.args);                                                                              \
    }

dec_to_(u32);

dec_to_(u16);

dec_to_(u8);

hex_to_(u32);

hex_to_(u16);

hex_to_(u8);

write_var_(u32);

write_var_(u16);

write_var_(u8);

parse_(u32);

parse_(u16);

parse_(u8);

write_var_symbol_(u32);

write_var_symbol_(u16);

write_var_symbol_(u8);

print_var_(u32);

print_var_(u16);

print_var_(u8);

int is_hex(const char *str) {
    return str[0] == '0' && str[1] == 'x';
}

void *get_ptr_by_address(u32 address) {
    void *ptr = (void *) address;
    return ptr;
}

int write_memory(void *pointer, byte *bytes, int bytes_size) {
    byte *p = (byte *) pointer;
    for (int i = 0; i < bytes_size; i++) {
        p[i] = bytes[i];
    }
    return bytes_size;
}

void print_function(struct printer out, void *ptr) {
    typedef struct {
        const char *dli_fname;
        void *dli_fbase;
        const char *dli_sname;
        void *dli_saddr;
    } Dl_info;

    Dl_info info;
    dladdr(ptr, &info);
    char buffer[256];
    sprintf(buffer, "Address '%p' located at %s within the program %s\n", ptr, info.dli_fname, info.dli_sname);
    out.out(buffer, out.args);
}

void print_var_symbol(struct printer out, const char *symbol) {
    u32 *p = (u32 *) dlsym(NULL, symbol);
    print_var_u32(out, p);
}


void print_resolve_symbol(struct printer out, const char *symbol) {
    void *p = dlsym(NULL, symbol);
    char buffer[256];
    sprintf(buffer, "symbol '%s' at %p\n", symbol, p);
    out.out(buffer, out.args);
}

void print_byte_hex(struct printer out, byte b) {
    char buff[256];
    sprintf(buff, "0x%02X", b);
    out.out(buff, out.args);
}

void print_byte_character(struct printer out, byte b) {
    char buffer[256];

    sprintf(buffer, "%c", b);
    out.out(buffer, out.args);
}
