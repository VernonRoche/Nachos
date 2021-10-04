#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }
ConsoleDriver::ConsoleDriver(const char *in, const char *out)
{
readAvail = new Semaphore("read avail", 0);
writeDone = new Semaphore("write done", 0);
console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}
ConsoleDriver::~ConsoleDriver()
{
delete console;
delete writeDone;
delete readAvail;
}
void ConsoleDriver::PutChar(int ch)
{
   #ifdef CHANGED
    console->TX (ch);
    writeDone->P ();
   #endif
}
int ConsoleDriver::GetChar()
{
#ifdef CHANGED
    readAvail->P ();
    return console->RX ();
#endif

}
#ifdef CHANGED
void ConsoleDriver::PutString(const char s[])
{
    int size=20;
    for(int i=0; i<size ; i++){
        this->PutChar( (int) s[i]);
        if (s[i]=='\0'){
            break;
        }
    }

}
#endif
void ConsoleDriver::GetString(char *s, int n)
{
// ...
}
#endif // CHANGED
