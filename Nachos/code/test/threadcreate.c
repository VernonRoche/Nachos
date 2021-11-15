

void print(char c){
    PutChar(c);
    ThreadExit();
}

int main(){
    char test='B';
    ThreadCreate(print, test);
    ThreadCreate(print, test);
    print('A');
    return 0;
}
