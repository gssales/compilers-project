// test18-2: main vazia + 3 declaracoes locais a/b/c + IF (a == b) SEM ELSE
// diff test5.s test18-2.s:
/*
    movl    -12(%rbp), %eax
    cmpl    -8(%rbp), %eax
    jne     .L2
    movl    $1, -8(%rbp)
.L2:
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
    if (a == b) then {b = 1;};
}