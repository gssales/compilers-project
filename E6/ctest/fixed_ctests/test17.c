// test17: main vazia + 3 declaracoes locais a/b/c + OR
// diff test5.s test17.s:
/*
    cmpl    $0, -8(%rbp)
    jne     .L2
    cmpl    $0, -4(%rbp)
    je      .L3
.L2:
    movl    $1, %eax
    jmp     .L4
.L3:
    movl    $0, %eax
.L4:
    movl    %eax, -12(%rbp)
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
    a = (b || c);
}