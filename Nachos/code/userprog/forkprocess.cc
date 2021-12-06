#include "system.h"
#include "forkprocess.h"

void StartUserProc(void* arg){
    currentThread->space->InitRegisters ();	// set the initial register values
    currentThread->space->RestoreState ();	// load page table register
    machine->DumpMem ("memory.svg");
    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
}

int do_ForkProcess(char* filename){
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL)
    {
        SetColor (stdout, ColorRed);
        SetBold (stdout);
        printf ("Unable to open file %s\n", filename);
        ClearColor (stdout);
        return -1;
    }
    space = new AddrSpace (executable);
    delete executable;		// close file
    Thread *t = new Thread ("forked process thread");
    t->space=space;

    t->Start (StartUserProc, filename);
    return -1;
    // the address space exits
    // by doing the syscall "exit"

}
