#include "thread.h"
#include "system.h"

typedef struct{
    int f,a;
} args_T;

int do_ThreadCreate(int f, int arg){
    //int args = new int[5];
    int* function;
    int* args;
    machine->ReadMem(f, sizeof(void (*)(void*)), function);
    machine->ReadMem(arg, sizeof(void *), args);

    Thread* test_thread = new Thread("test_thread");
    test_thread->space=currentThread->space;
    test_thread->Start(f, args);
    return 0;
}

void StartUserThread(){

}

void do_ThreadExit(){

}