#include "thread.h"
#include "system.h"

typedef struct{
    int f,a;
} args_T;

void StartUserThread(void* kernel_args){
    int i;
    int* args = (int*) kernel_args;

    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, args[0]);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!

    machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack());
    DEBUG ('a', "Initializing stack register to 0x%x\n",
           currentThread->space->AllocateUserStack());
    machine->WriteRegister (4, args[1]);
    printf("Writing %d in Register 4\n", args[1]);
    delete [] args;
    machine->Run();
}

int do_ThreadCreate(int f, int arg){
    int *args{ new int[2]{f,arg} };
    Thread *t = new Thread ("forked thread");
    t->space=currentThread->space;
    t->Start (StartUserThread, args);
    return 0;
}



void do_ThreadExit(){
    currentThread->Finish();
}