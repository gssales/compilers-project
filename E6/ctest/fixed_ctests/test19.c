// test19: main vazia + 3 declaracoes locais a/b/c + WHILE (0)
// diff test5.s test19.s:
/*
    nop
*/
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
    while (0){a = 1;};
}