// test9: main vazia + 3 declaracoes locais a/b/c + DIV
// diff test5.s test9.s:
/*
    movl    -8(%rbp), %eax
    cltd
    idivl   -4(%rbp)
    movl    %eax, -12(%rbp)
*/
int main() {
    int a = 100;
    int b = 200;
    int c = 300;
    a = b / c;
}