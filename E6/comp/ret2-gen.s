	.bss
	.text
	.globl	moz
	.type	moz, @function
moz:
	pushq	%rbp
	movl	$100, %eax
	popq	%rbp
	ret
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movl	$50, %eax
	popq	%rbp
	ret
	movl	$100, %eax
	popq	%rbp
	ret
