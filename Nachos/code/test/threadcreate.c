#include "syscall.h"


void print(int c){
    //PutString(c);
    PutInt(c);
    ThreadExit();
}

int main(){
    //char test = "Hello\n";
    int test = 10;
    ThreadCreate(print, test);
    ThreadCreate(print, test);
    ThreadCreate(print, test);
    ThreadCreate(print, test);
    print(test);
    return 0;
}
