/*
 ============================================================================
 Name : Lab6.c
 Author : Austin Tian
 Revised by : Yunus Cheentavida
 Version :
 Copyright : Copyright 2023
 Description : Lab 6 in C, ANSI-C Style
 ============================================================================
 */
#include "header.h"
#include <time.h>

// Menu for the testing.
char *menu = "\n" \
" ***********Please select the following options**********************\n" \
" * This is the memory operation menu (Lab 6) *\n" \
" ********************************************************************\n" \
" * 1. Write a double-word (32-bit) to the memory *\n" \
" ********************************************************************\n" \
" * 2. Read a byte (8-bit) data from the memory *\n" \
" * 3. Read a double-word (32-bit) data from the memory *\n" \
" ********************************************************************\n" \
" * 4. Generate a memory dump from any memory location *\n" \
" ********************************************************************\n" \
" * e. To Exit, Type 'e' or 'E' *\n" \
" ********************************************************************\n";
//---------------------------------------------------------------
//
void write_byte(const char *base_address, const int offset, const unsigned char byte_data)
{
    *(unsigned char *)(base_address + offset) = byte_data;
}
//---------------------------------------------------------------
// Generate a random number between 0x00 and 0xFF.
unsigned char rand_generator()
{
    return rand() % 256;
}

//---------------------------------------------------------------
void free_memory(char *base_address)
{
    free(base_address);
}

//---------------------------------------------------------------
char *init_memory()
{
    unsigned int i;
    char *mem = malloc(MEM_SIZE); // allocate the memory
    srand((unsigned int)time(NULL));

    for (i = 0; i < MEM_SIZE; i++)
    {
        mem[i] = rand_generator();
    }

    return mem;
}

//---------------------------------------------------------------
void write_dword(const char *base_address, const int offset, const unsigned int dword_data)
{
    *(unsigned char *)(base_address + offset) = dword_data & 0xFF;
    *(unsigned char *)(base_address + offset + 1) = (dword_data >> 8) & 0xFF;
    *(unsigned char *)(base_address + offset + 2) = (dword_data >> 16) & 0xFF;
    *(unsigned char *)(base_address + offset + 3) = (dword_data >> 24) & 0xFF;
}

//---------------------------------------------------------------
unsigned char read_byte(const char *base_address, const int offset)
{
    unsigned char value;
    value = *(unsigned char *)(base_address + offset);
    printf("Byte at address base + 0x%X = 0x%02X\n", offset, value);
    return value;
}

//---------------------------------------------------------------
unsigned int read_dword(const char *base_address, const int offset)
{
    unsigned int value;
    value = (unsigned int)(unsigned char)*(base_address + offset);
    value |= (unsigned int)(unsigned char)*(base_address + offset + 1) << 8;
    value |= (unsigned int)(unsigned char)*(base_address + offset + 2) << 16;
    value |= (unsigned int)(unsigned char)*(base_address + offset + 3) << 24;
    printf("Double-word at address base + 0x%X = 0x%08X\n", offset, value);
    return value;
}

//---------------------------------------------------------------
void memory_dump(const char *base_address, const int offset, unsigned int dumpsize)
{
    unsigned int i, j;
    unsigned char value;

    if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE)
        dumpsize = MIN_DUMP_SIZE;

    puts("\nAddress +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F ASCII");
    puts("--------------------------------------------------------------------------------------");

    for (i = offset; i < offset + dumpsize; i += DUMP_LINE)
    {
        printf("0x%08X ", i);

        for (j = 0; j < DUMP_LINE; j++)
        {
            printf("%02X ", (unsigned char)base_address[i + j]);
        }

        printf(" ");

        for (j = 0; j < DUMP_LINE; j++)
        {
            value = (unsigned char)base_address[i + j];
            if (value >= 0x20 && value <= 0x7E)
                printf("%c", value);
            else
                printf(".");
        }

        printf("\n");
    }
}

//---------------------------------------------------------------
void setup_memory()
{
    char *mem = init_memory();
    char options = 0;
    unsigned int offset, dumpsize;
    char tempchar;
    unsigned int dword_data;

    do
    {
        if (options != 0x0a)
        {
            puts(menu);
            printf("\nThe base address of your memory is: %I64Xh (HEX)\n", (long long unsigned int)(mem));
            puts("Please make a selection:");
        }

        options = getchar();

        switch (options)
        {
            case '1':
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", (unsigned int*)&offset);
                puts("Please input your DOUBLE WORD data to be written (in HEX):");
                scanf("%x", (unsigned int*)&dword_data);
                write_dword(mem, offset, dword_data);
                continue;

            case '2':
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset);
                read_byte(mem, offset);
                continue;

            case '3':
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset);
                read_dword(mem, offset);
                continue;

            case '4':
                puts("Please input your memory's offset address (in HEX, should be a multiple of 0x10h):");
                scanf("%x", &offset);
                puts("Please input the size of the memory to be dumped (a number between 256 and 1024 ):");
                scanf("%d", &dumpsize);
                memory_dump(mem, offset, dumpsize);
                continue;

            case 'e':
            case 'E':
                puts("Code finished, press any key to exit");
                free_memory(mem);
                while ((tempchar = getchar()) != '\n' && tempchar != EOF);
                tempchar = getchar();
                return;

            default:
                continue;
        }
    } while (1);
}
