// test20: main vazia + 3 declaracoes locais a/b/c + RETURN a
// diff test5.s test20.s:
/*
17c17
<       movl    $0, %eax
---
>       movl    -12(%rbp), %eax
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
    return a;
}