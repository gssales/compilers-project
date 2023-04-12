// test11: main vazia + 3 declaracoes locais a/b/c + CMP_NE
// diff test5.s test11.s:
/*
    movl    -8(%rbp), %eax
    cmpl    -4(%rbp), %eax
    setne   %al
    movzbl  %al, %eax
    movl    %eax, -12(%rbp)
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
    a = (b != c);
}