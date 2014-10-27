#define KBD_BUFF 0x60 /*Output buffer from KBC*/

.global _scan_asm

.text
_scan_asm:
	mov $0,%eax
	inb $KBD_BUFF,%al
	mov %eax,_codek
ret
