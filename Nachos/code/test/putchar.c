#include "syscall.h"

void print(char c,int n){

    for (int i = 0; i < n; i++) {
        PutChar(c + i);
    }
    PutChar('\n');

}

int main(){
    print('a',5);
    return 0;
}