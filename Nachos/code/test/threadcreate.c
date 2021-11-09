

void print(char c){
    PutChar(c);
    ThreadExit();
}

int main(){
    char test='B';
    ThreadCreate(print, test);
    print('A');
    while(1);
    return 0;
}
