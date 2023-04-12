// test6: main vazia + 3 declaracoes locais a/b/c + ADD
// diff test5.s test6.s:
/*
    movl    -8(%rbp), %edx
    movl    -4(%rbp), %eax
    addl    %edx, %eax
    movl    %eax, -12(%rbp)
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
    a = b + c;
}