// test19-3: main vazia + 3 declaracoes locais a/b/c + WHILE (a == b)
// diff test5.s test19-3.s:
/*
    jmp     .L2
.L3:
    movl    $1, -12(%rbp)
.L2:
    movl    -12(%rbp), %eax
    cmpl    -8(%rbp), %eax
    je      .L3
*/
int main() {
    int a = 100;
    int b = 200;
    int c = 300;
    while (a == b){a = 1;};
}