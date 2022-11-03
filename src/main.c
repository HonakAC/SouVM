#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "memory.h"

void run(uint8_t *code, size_t size){
    uint8_t mode = 0x00; // 0xf0 - memory, 0xf1 - minimal sasm code
    int memorymode = 0; // 0 - name, 1 - value
    size_t startcode = 0;

    char *memoryname = cmalloc(0 * sizeof(char));
    size_t namesize = 0;
    char *memoryvalue = cmalloc(0 * sizeof(char));
    size_t valuesize = 0;

    char **memorynames = cmalloc(0 * sizeof(char*));
    size_t namessize = 0;
    char **memoryvalues = cmalloc(0 * sizeof(char*));
    size_t valuessize = 0;

    char **externalmemory = cmalloc(50 * sizeof(char*));

    for(size_t pos = 0; pos < size; pos++){
        uint8_t ch = code[pos];
        if(ch == 0xf1)
            startcode = pos+1;
        if(ch == 0xf0 || ch == 0xf1)
            mode = ch;  
        else if(mode == 0xf0){
            if(ch == 0xfe){
                memorymode = !memorymode;
                if(memorymode == 0){
                    memorynames = crealloc(memorynames, (++namessize) * sizeof(char*));
                    memorynames[namessize-1] = memoryname;
                    memoryvalues = crealloc(memoryvalues, (++valuessize) * sizeof(char*));
                    memoryvalues[valuessize-1] = memoryvalue;

                    memoryname = cmalloc(1 * sizeof(char));
                    namesize = 0;
                    memoryvalue = cmalloc(1 * sizeof(char));
                    valuesize = 0;
                }
            }
            else if(memorymode == 0){
                memoryname = crealloc(memoryname, (++namesize) * sizeof(char));
                memoryname[namesize-1] = (char) ch;
            } else if(memorymode == 1){
                memoryvalue = crealloc(memoryvalue, (++valuesize) * sizeof(char));
                memoryvalue[valuesize-1] = (char) ch;
            }
        } else if(mode == 0xf1){
            if(ch == 0x10){
                pos++;
                int eaddr = code[pos++];
                char *name = cmalloc(0 * sizeof(char));
                size_t namesize2 = 0;
                for(; code[pos] != 0xfe; pos++){
                    name = crealloc(name, (++namesize2) * sizeof(char));
                    name[namesize2-1] = (char) code[pos];
                }
                for(size_t pos2 = 0; pos2 < namessize; pos2++){
                    int equal = 1;
                    for(size_t pos3 = 0; pos3 < namesize2-1; pos3++){
                        if(name[pos3] != memorynames[pos2][pos3])
                            equal = 0;
                    }
                    if(equal)
                        externalmemory[eaddr] = memoryvalues[pos2];
                }
                continue;
            } else if(ch == 0x20){
                pos++;
                if(code[pos] == 0x0f){
                    printf("%s", externalmemory[1]);
                }
            }
        }
    }
    free(memoryname);
    free(memorynames);
    free(memoryvalues);
    free(memoryvalue);
    free(externalmemory);
}

int main(int argc, char **argv){
    FILE *file = fopen(argv[1], "r");
    uint8_t *content = cmalloc(0 * sizeof(uint8_t));
    size_t size = 0;
    uint8_t ch;
    while((ch = fgetc(file)) != (uint8_t) EOF){
        content = crealloc(content, (++size) * sizeof(uint8_t));
        content[size-1] = ch;
    }
    run(content, size);
    free(content);
}