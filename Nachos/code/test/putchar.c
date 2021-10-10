#include "syscall.h"

void print(char c,int n){
    int i;
    
#if 1
    for (i = 0; i < n; i++) {
        PutChar(c + i);
    }
    PutChar('\n');
    //const char *test_string = "Petit test";
    //PutString(test_string);
    
#endif
}

int main(){
    print('a',6);
    return 8;
}