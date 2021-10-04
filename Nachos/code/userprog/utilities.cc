#ifdef CHANGED

#include "utilities.h"

int copyStringFromMachine(int from, char *to, unsigned size){
    int c;
    for (unsigned int i=0; i<size ; i++, from++){
        machine->ReadMem(from, 1, &c);
        to[i] = (char) c;
        if (to[i]=='\0'){
            return i+1;
        }
    }
    if (to[size-1]!='\0'){
        to[size-1]='\0';
    }
    return size;
}


#endif