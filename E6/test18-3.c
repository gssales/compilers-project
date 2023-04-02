// test18-3: main vazia + 3 declaracoes locais a/b/c + IF (a == b) COM ELSE (c = 1)
// diff test5.s test18-3.s:
/*
    movl    -12(%rbp), %eax
    cmpl    -8(%rbp), %eax
    jne     .L2
    movl    $1, -8(%rbp)
    jmp     .L3
.L2:
    movl    $1, -4(%rbp)
.L3:
*/
int main() {
    int a = 100;
    int b = 200;
    int c = 300;
    if (a == b)
    {
        b = 1;
    } else
    {
        c = 1;
    };
}