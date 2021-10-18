

void print(char c){
    PutChar(c);
}

void print_void(){
    PutChar("A");
}

int main(){
    char test=10;
    int return_thread_code = ThreadCreate(print, &test);
    while(1);
    return 0;
}
