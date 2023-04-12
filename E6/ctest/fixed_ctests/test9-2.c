// test9-2: main vazia + 3 declaracoes locais a/b/c + DIV IMMEDIATE (aqui complicou um pouco)
// diff test5.s test9-2.s:
/*
    movl    -8(%rbp), %eax
    movslq  %eax, %rdx
    imulq   $274877907, %rdx, %rdx
    shrq    $32, %rdx
    sarl    $5, %edx
    sarl    $31, %eax
    movl    %eax, %ecx
    movl    %edx, %eax
    subl    %ecx, %eax
    movl    %eax, -12(%rbp)
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
    a = b / 500;
}