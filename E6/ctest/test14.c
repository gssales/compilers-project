// test14: main vazia + 3 declaracoes locais a/b/c + CMP_GT
// diff test5.s test14.s:
/*
    movl    -8(%rbp), %eax
    cmpl    -4(%rbp), %eax
    setg    %al
    movzbl  %al, %eax
    movl    %eax, -12(%rbp)
*/
int main() {
    int a = 100;
    int b = 200;
    int c = 300;
    a = (b > c);
}