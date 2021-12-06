// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#ifdef CHANGED
#include "consoledriver.h"
#include "userthread.h"
#include "forkprocess.h"
#endif

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    int address = machine->registers[BadVAddrReg];

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Powerdown ();
		    break;
		  }
#ifdef CHANGED
        case SC_PutChar:
        {
            DEBUG('s',"PutChar\n ");
            char c= machine->ReadRegister(4);
            consoledriver->PutChar(c);
            break;
        }
        case SC_GetChar:
        {
            DEBUG('s',"GetChar\n");
            printf("On passe ici dans le case SC_GetChar\n");
            int c = consoledriver->GetChar();
            machine->WriteRegister(2, c);
            break;
        }
        case SC_PutString:
        {
            DEBUG('s',"PutString\n ");
            int from = machine->ReadRegister(4);
            char to[MAX_STRING_SIZE];
            int chars_read=machine->copyStringFromMachine(from, to, MAX_STRING_SIZE);
            while(chars_read>1){
                consoledriver->PutString(to);
                from+=MAX_STRING_SIZE-1;
                chars_read=machine->copyStringFromMachine(from, to, MAX_STRING_SIZE);
            }
            break;
        }
        case SC_GetString:
        {

            DEBUG('s',"GetString\n");
            printf("On passe ici dans le case SC_GetString\n");
            int to = machine->ReadRegister(4);
            int n = machine->ReadRegister(5);
            char* s = (char *) malloc (n*sizeof(char));
			char* s_temp = s;
            consoledriver->GetString(s, n);
            int chars_read = 2;
            while(chars_read>1){
                chars_read=machine->copyStringToMachine(s_temp, to, MAX_STRING_SIZE);
				s_temp+=MAX_STRING_SIZE-1;
            }
            free(s);
            break;
        }

        case SC_PutInt:
        {
            DEBUG('s',"PutInt\n");
            printf("On passe dans le PutInt\n");
            int n=machine->ReadRegister(4);
            consoledriver->PutInt(n);
            break;
        }

        case SC_Yield:
        {
            DEBUG('s',"Yield\n");
            printf("On passe dans le SC_Yield\n");
            currentThread->Yield();
            break;
        }
        case SC_ThreadCreate:
        {
            DEBUG('s',"ThreadCreate\n");
            printf("On passe dans le SC_CreateThread\n");
            //GET PARAMS
            int f= machine->ReadRegister(4);
            int arg= machine->ReadRegister(5);
            //LAUNCH FUNCTION
            //printf("This is f: %d . This is arg: %d \n", f,arg);
            do_ThreadCreate(f,arg);
            break;
        }

        case SC_ThreadExit:
        {
            DEBUG('s',"ThreadExit\n");
            printf("On passe dans le SC_ThreadExit\n");
            do_ThreadExit();
            break;
        }

		case SC_Exit:
		  {
        machine->process_wainting_room->P();
			  int exit = machine->ReadRegister (4);
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
			  printf("\n=====\nCode de retour = %d\n",exit);
			  printf("On passe ici dans le case SC_Exit\n");
        machine->numberProcesses--;
        if(machine->numberProcesses == 0)
		      interrupt->Powerdown ();
        machine->process_wainting_room->V();
		    break;
		  }

          case SC_ForkExec:
          {
              DEBUG('s',"ForkExec\n");
              printf("On passe dans le SC_ForkExec\n");
              int file = machine->ReadRegister(4);
              char filename[MAX_STRING_SIZE];
              machine->copyStringFromMachine(file, filename, MAX_STRING_SIZE);
              do_ForkProcess(filename);
              break;
          }
#endif
		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!address) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }
	  break;

	case ReadOnlyException:
	  printf ("Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case BusErrorException:
	  printf ("Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case AddressErrorException:
	  printf ("Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case OverflowException:
	  printf ("Overflow at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case IllegalInstrException:
	  printf ("Illegal instruction at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	default:
	  printf ("Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
	  ASSERT (FALSE);
	  break;
      }
}
