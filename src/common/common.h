#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ptr_size_t unsigned int
#define byte unsigned char
#define OK 1
#define ERR 0
#define u8 byte
#define u16 unsigned short
#define u32 unsigned long
#define u32HEX_FORMAT "%lX"
#define u16HEX_FORMAT "%X"
#define u8HEX_FORMAT "%X"
#define u32DEC_FORMAT "%u"
#define u16DEC_FORMAT "%u"
#define u8DEC_FORMAT "%u"
#define DELIMITERS " \r\n"


typedef void (*print)(const char* text, void* args);

struct printer{
    void* args;
    print out;
};


#define declare_dec_to_(type) type dec_to_##type(const char* dec_string);
#define declare_hex_to_(type) type hex_to_##type(const char* hex_string);
#define declare_write_var_(type) type write_var_##type(type* pointer, type value);
#define declare_parse_(type) type parse_##type(const char *str);
#define declare_write_var_symbol_(type) void write_var_symbol_##type(const char *symbol, type value);
#define declare_print_var_(type) void print_var_##type(struct printer out, type *pointer);

u32 dec_to_u32(const char* dec_string);
u32 hex_to_u32(const char* hex_string);
u32 write_var_u32(u32* pointer, u32 value);
u32 parse_u32(const char *str);
void write_var_symbol_u32(const char *symbol, u32 value);
declare_dec_to_(u32);
declare_dec_to_(u16);
declare_dec_to_(u8);
declare_hex_to_(u32);
declare_hex_to_(u16);
declare_hex_to_(u8);
declare_write_var_(u32);
declare_write_var_(u16);
declare_write_var_(u8);
declare_parse_(u32);
declare_parse_(u16);
declare_parse_(u8);
declare_write_var_symbol_(u32);
declare_write_var_symbol_(u16);
declare_write_var_symbol_(u8);
declare_print_var_(u32);
declare_print_var_(u16);
declare_print_var_(u8);

int is_hex(const char *str);
int write_memory(void *pointer, byte *bytes, int bytes_size);
void *get_ptr_by_address(u32 address);

void print_function(struct printer out, void *ptr);
void print_var_symbol(struct printer out, const char *symbol);

void print_byte_hex(struct printer out, byte b);

void print_byte_character(struct printer out, byte b);
#endif