	.file	"overhead.cpp"
	.text
	.p2align 4
	.globl	raw_speed
	.type	raw_speed, @function
raw_speed:
.LFB150:
	.cfi_startproc
	divsd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE150:
	.size	raw_speed, .-raw_speed
	.p2align 4
	.globl	typed_speed
	.type	typed_speed, @function
typed_speed:
.LFB151:
	.cfi_startproc
	divsd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE151:
	.size	typed_speed, .-typed_speed
	.p2align 4
	.globl	raw_force
	.type	raw_force, @function
raw_force:
.LFB153:
	.cfi_startproc
	divsd	%xmm2, %xmm1
	mulsd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE153:
	.size	raw_force, .-raw_force
	.p2align 4
	.globl	typed_force
	.type	typed_force, @function
typed_force:
.LFB154:
	.cfi_startproc
	divsd	%xmm2, %xmm1
	mulsd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE154:
	.size	typed_force, .-typed_force
	.p2align 4
	.globl	raw_feet_to_metres
	.type	raw_feet_to_metres, @function
raw_feet_to_metres:
.LFB155:
	.cfi_startproc
	mulsd	.LC0(%rip), %xmm0
	ret
	.cfi_endproc
.LFE155:
	.size	raw_feet_to_metres, .-raw_feet_to_metres
	.p2align 4
	.globl	typed_feet_to_metres
	.type	typed_feet_to_metres, @function
typed_feet_to_metres:
.LFB156:
	.cfi_startproc
	mulsd	.LC0(%rip), %xmm0
	ret
	.cfi_endproc
.LFE156:
	.size	typed_feet_to_metres, .-typed_feet_to_metres
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	-604731395
	.long	1070825943
	.ident	"GCC: (Debian 14.2.0-19) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
