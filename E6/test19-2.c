// test19-2: main vazia + 3 declaracoes locais a/b/c + WHILE (1)
// diff test5.s test19-2.s:
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
    while (1){a = 1;};
}