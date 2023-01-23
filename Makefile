all:	game delay acm

game:	game.c
	gcc -O2 -lpthread -o game game.c

delay:	delay.c
	gcc -O2 -o delay delay.c

acm:	acm.c
	gcc -O2 -lpthread -o acm acm.c
