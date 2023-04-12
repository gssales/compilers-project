// test15: main vazia + 3 declaracoes locais a/b/c + CMP_GE
// diff test5.s test15.s:
/*
    movl    -8(%rbp), %eax
    cmpl    -4(%rbp), %eax
    setge   %al
    movzbl  %al, %eax
    movl    %eax, -12(%rbp)
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
    a = (b >= c);
}