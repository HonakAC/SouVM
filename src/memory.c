#include <stdio.h>
#include <stdlib.h>

void memerr(){
    fprintf(stderr, "Ошибка выделении памяти!\n");
    exit(EXIT_FAILURE);
}

void *cmalloc(size_t size){
    void *out = malloc(size);
    if(!out)
        memerr();
    return out;
}
void *crealloc(void *ptr, size_t newsize){
    void *out = realloc(ptr, newsize);
    if(!out)
        memerr();
    return out;
}