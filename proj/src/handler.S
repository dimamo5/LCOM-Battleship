#define KBD_BUFF 0x60 /*Output buffer from KBC*/
#define KBD_STATUS 0x64
#define OUTBUFF_FULL 0x01

.global _handler_kbd

.text
_handler_kbd:
// the next 4 lines verify if the output buffer is full and ready for reading

	inb $KBD_STATUS,%al		//Saving the status register to the AL register
	and $OUTBUFF_FULL,%al
	cmp $1,%al				// Verifying if the Status register has the output full bit active
	jne fim		//this is done above by doing a "And" with the status register and output_full bit


// the next 2 lines receive the code from the keyboard

	xor %eax,%eax        // initializing eax as 0 to foolproof and empty the remaining bits not used in the next instruction
	inb $KBD_BUFF,%al // sending the out buffer content to the AL register
fim:ret
