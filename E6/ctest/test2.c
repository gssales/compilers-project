// test2: main vazia + declaracao GLOBAL
// diff test0.s test2.s:
/* 	
	.text
	.globl  a
	.bss
	.align 4
	.type   a, @object
	.size   a, 4
a:
	.zero   4
*/
int a;
int main() {
}