all:
	flex lex.l
	cc -c lex.yy.c
	cc -c myshell.c
	cc -o myshell lex.yy.o myshell.o -lfl 

clean:
	rm *.o
	rm lex.yy.c
	rm myshell
