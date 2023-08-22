.section .text
	nop
.global __mlibc_signal_restore
.type __mlibc_signal_restore, @function
__mlibc_signal_restore:
	mov $173, %eax
	; mov $119, %eax
	int $0x80
	ud2

.section .note.GNU-stack,"",%progbits
