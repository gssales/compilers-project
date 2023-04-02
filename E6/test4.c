// test4: main vazia + declaracao local com inicializacao
// diff test1.s test4.s:
/*
    movl    $100, -4(%rbp)
*/
int main() {
    int a = 100;
}