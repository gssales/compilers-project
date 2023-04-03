// test18: main vazia + 3 declaracoes locais a/b/c + IF (a) SEM ELSE
// diff test5.s test18.s:
/*
    cmpl    $0, -12(%rbp)
    je      .L2
    movl    $1, -8(%rbp)
.L2:
*/
int main() {
    int a = 100;
    int b = 200;
    int c = 300;
    if (a){b = 1;};
}