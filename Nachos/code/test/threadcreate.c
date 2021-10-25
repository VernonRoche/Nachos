

void print(char c){
    PutChar(c);
    ThreadExit();
}

void print_void(){
    PutChar('A');
    ThreadExit();
}

int main(){
    char test='B';
    ThreadCreate(print, test);
    Yield();
    while(1);
    return 0;
}
