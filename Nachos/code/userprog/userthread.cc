#include "thread.h"
#include "system.h"
#include "synch.h"

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

    int next_free_slot = currentThread->space->AllocateUserStack();
    currentThread->bitmap_index=next_free_slot;
    int allocatedUserStack=currentThread->space->NumPages()*PageSize-(next_free_slot*256);
    if (allocatedUserStack == -1)
        return;

    machine->WriteRegister (StackReg, allocatedUserStack);
    currentThread->space->thread_count++;
    DEBUG ('a', "Initializing stack register to 0x%x\n",
           currentThread->space->AllocateUserStack());
    machine->WriteRegister (4, args[1]);
    printf("Writing %d in Register 4\n", args[1]);
    delete [] args;
    machine->Run();
}

int do_ThreadCreate(int f, int arg){
    currentThread->space->thread_waiting_room->P();
    if (currentThread->space->user_stack_slots->Find() == -1) {
        currentThread->space->thread_waiting_room->V();
        return -1;
    }
    printf("CURRENT THREADS BEFORE THREAD CREATION: %d\n",currentThread->space->thread_count);
    int *args{ new int[2]{f,arg} };
    Thread *t = new Thread ("forked thread");
    t->space=currentThread->space;
    t->Start (StartUserThread, args);
    currentThread->space->thread_waiting_room->V();
    return 0;
}



void do_ThreadExit(){
    currentThread->space->thread_waiting_room->P();
    currentThread->space->thread_count--;
    int current_thread_count=currentThread->space->thread_count;
    printf("BITMAP INDEX OF LEAVING THREAD: %d\n", currentThread->bitmap_index);
    currentThread->space->DeallocateUserStack(currentThread->bitmap_index);
    printf("CURRENT THREADS AFTER THREAD KILLING: %d\n",current_thread_count);
    if (current_thread_count <=0)
        interrupt->Halt();
    currentThread->Finish();
    currentThread->space->thread_waiting_room->V();
}