// test7: main vazia + 3 declaracoes locais a/b/c + SUB
// diff test5.s test7.s:
/*
    movl    -8(%rbp), %eax
    subl    -4(%rbp), %eax
    movl    %eax, -12(%rbp)
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
    a = b - c;
}