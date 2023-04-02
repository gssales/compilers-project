// test6-2: main vazia + 3 declaracoes locais a/b/c + ADD IMMEDIATE
// diff test5.s test6-2.s:
/*
    movl    -8(%rbp), %eax
    addl    $500, %eax
    movl    %eax, -12(%rbp)
*/
int main() {
    int a = 100;
    int b = 200;
    int c = 300;
    a = b + 500;
}