

void print(char* c){
    PutString(c);
    ThreadExit();
}

int main(){
    char* test="Hello\n";
    ThreadCreate(print, test);
    ThreadCreate(print, test);
    ThreadCreate(print, test);
    ThreadCreate(print, test);
    print(test);
    return 0;
}
