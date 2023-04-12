// test5: main vazia + 3 declaracoes locais a/b/c
// diff test4.s test5.s:
/*
    movl    $100, -12(%rbp)
    movl    $200, -8(%rbp)
    movl    $300, -4(%rbp)
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
}