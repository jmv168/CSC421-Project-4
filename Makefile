#
# Makefile for project2 csc421.231
# last-update:
# 	20 sep 2022 -bjr:
#

V=
# comment out for not verbose
V= -v 

F= snail
G= snail-util

$F: $F.c $G.o $F.h
	cc -o $@ $< -pthread $G.o
	
$G.o: $G.c $F.h
	cc -c $< 

run: $F
	./$< $V

clean:
	-rm $F $G.o
