DEBUG=-g
protect: protect.c Makefile
	gcc -o protect protect.c $(DEBUG)
