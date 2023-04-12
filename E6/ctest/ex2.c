// exemplo 2 da spec na secao A.1:
/* output ex1.s esperado:
    .file "ex1.c"
    .text
    .globl a
    .data
    .align 4
    .type a, @object
    .size a, 4
a:
    .long 3
    .text
    .globl main
    .type main, @function
main:
.LFB0 :
pushq %rbp
movq %rsp , %rbp
movl a(%rip), %eax
movl %eax, −4(%rbp)
movl $0, %eax
popq %rbp
ret
*/
int a = 3;
int main() {
    int b;
    b = a;
    return 0;
}