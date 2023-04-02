// test22: main vazia + 3 declaracoes locais a/b/c + chamada de funcao COM ARGUMENTOS
// diff test5.s test22.s:
/*
@2a3,23
>       .globl  foo
>       .type   foo, @function
> foo:
> .LFB0:
>       .cfi_startproc
>       endbr64
>       pushq   %rbp
>       .cfi_def_cfa_offset 16
>       .cfi_offset 6, -16
>       movq    %rsp, %rbp
>       .cfi_def_cfa_register 6
>       movl    %edi, -4(%rbp)
>       movl    %esi, -8(%rbp)
>       movl    %edx, -12(%rbp)
>       nop
>       popq    %rbp
>       .cfi_def_cfa 7, 8
>       ret
>       .cfi_endproc
> .LFE0:
>       .size   foo, .-foo
@6c27
< .LFB0:
---
> .LFB1:
@13a35
>       subq    $16, %rsp
@16a39,44
>       movl    -4(%rbp), %edx
>       movl    -8(%rbp), %ecx
>       movl    -12(%rbp), %eax
>       movl    %ecx, %esi
>       movl    %eax, %edi
>       call    foo
@18c46
<       popq    %rbp
---
>       leave
@22c50
< .LFE0:
---
> .LFE1:
*/
int foo(int d, int e, int f) {
}
int main() {
    int a = 100;
    int b = 200;
    int c = 300;
    foo(a, b, c);
}