// test21: main vazia + 3 declaracoes locais a/b/c + declaracao de funcao
// diff test5.s test21.s:
/*
@2a3,20
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
>       nop
>       popq    %rbp
>       .cfi_def_cfa 7, 8
>       ret
>       .cfi_endproc
> .LFE0:
>       .size   foo, .-foo
@6c24
< .LFB0:
---
> .LFB1:
@22c40
< .LFE0:
---
> .LFE1:
*/
int foo() {
}
int main() {
    int a <= 100;
    int b <= 200;
    int c <= 300;
}