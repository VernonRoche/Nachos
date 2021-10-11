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
    for(int i=0; i<MAX_STRING_SIZE ; i++){
        this->PutChar( (int) s[i]);
        if (s[i]=='\0'){
            break;
        }
    }

}
#endif
void ConsoleDriver::GetString(char *s, int n)
{
#ifdef CHANGED
    for(int i=0; i<n-1 ; i++){
        s[i]=(char) this->GetChar();
        if (s[i]==-1 || s[i]=='\n' || s[i]=='\0'){
            break;
        }
    }
    if (s[n-2]!='\0'){
        s[n-1]='\0';
    }
#endif
}
#endif // CHANGED
