// test16: main vazia + 3 declaracoes locais a/b/c + AND
// diff test5.s test16.s:
/*
    cmpl    $0, -8(%rbp)
    je      .L2
    cmpl    $0, -4(%rbp)
    je      .L2
    movl    $1, %eax
    jmp     .L3
.L2:
    movl    $0, %eax
.L3:
    movl    %eax, -12(%rbp)
*/
int main() {
    int a = 100;
    int b = 200;
    int c = 300;
    a = (b && c);
}