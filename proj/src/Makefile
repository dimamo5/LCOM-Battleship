
COMPILER_TYPE= gnu

CC= gcc

PROG = battleship
SRCS = main.c bitmap.c vbe.c graphics.c rtc.c keyboard_mouse.c timer.c handler.S battleship.c MainMenu.c button.c game.c setint.S clearint.S
CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys 

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
