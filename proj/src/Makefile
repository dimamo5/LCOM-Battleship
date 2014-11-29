
COMPILER_TYPE= gnu

CC= gcc

PROG = battleship
SRCS = main.c bitmap.c vbe.c graphics.c rtc.c keyboard.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys 

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
