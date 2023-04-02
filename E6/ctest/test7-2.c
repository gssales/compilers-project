// test7-2: main vazia + 3 declaracoes locais a/b/c + SUB IMMEDIATE
// diff test5.s test7-2.s:
/*
    movl    -8(%rbp), %eax
    subl    $500, %eax
    movl    %eax, -12(%rbp)
*/
int main() {
    int a = 100;
    int b = 200;
    int c = 300;
    a = b - 500;
}