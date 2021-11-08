

void print(char c){
    PutChar(c);
    ThreadExit();
}

int main(){
    char test='B';
    ThreadCreate(print, test);
    print('A');
    ThreadExit();
    return 0;
}
