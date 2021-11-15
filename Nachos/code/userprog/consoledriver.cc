#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"


static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *readToken;
static Semaphore *writeToken;
static Semaphore *waitingRoom;
static Semaphore *stringWriteToken;
static int semaphore_number;
static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }
ConsoleDriver::ConsoleDriver(const char *in, const char *out)
{
readAvail = new Semaphore("read avail", 0);
writeDone = new Semaphore("write done", 0);
readToken = new Semaphore("read token", 1);
writeToken = new Semaphore("write token", 1);
waitingRoom = new Semaphore("waiting room", 1);
stringWriteToken = new Semaphore("string write token", 1);
semaphore_number=0;
console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}
ConsoleDriver::~ConsoleDriver()
{
delete console;
delete writeDone;
delete readAvail;
delete writeToken;
delete readToken;
delete stringWriteToken;
delete waitingRoom;
}
void ConsoleDriver::PutChar(int ch)
{
   #ifdef CHANGED
    waitingRoom->P();
    writeToken->P ();
    waitingRoom->V();
    console->TX (ch);
    writeDone->P();
    writeToken->V();
   #endif
}
int ConsoleDriver::GetChar()
{
#ifdef CHANGED
    waitingRoom->P();
    readToken->P ();
    if (++semaphore_number == 1) //pathfinder
        writeToken->P();
    readToken->V();
    waitingRoom->V();
    readAvail->P();
    int read_results=console->RX ();
    readToken->P();
    if (--semaphore_number == 0) //last to leave
        writeToken->V();
    readToken->V();
    return read_results;
#endif

}
#ifdef CHANGED
void ConsoleDriver::PutString(const char s[])
{
    stringWriteToken->P();
    for(int i=0; i<MAX_STRING_SIZE ; i++){
        this->PutChar( (int) s[i]);
        if (s[i]=='\0'){
            break;
        }
    }
    stringWriteToken->V();
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

#ifdef CHANGED
void ConsoleDriver::PutInt(int n){
    int buffer_size=15;
    char buffer[buffer_size];
    snprintf(buffer, buffer_size, "%d.0\n", n);
    this->PutString(buffer);
}

#endif
#endif // CHANGED
