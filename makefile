.PHONY: all clean runCompile

all: dire anaLe anaSi estru compile  runCompile

compile: Compilador/compilador.c
	gcc Compilador/compilador.c -o compile

dire: directivasDeProcessamento/Bibliotecas.c
	gcc directivasDeProcessamento/Bibliotecas.c -c
	@echo "Biblioteca está incluso"

anaLe: AnalisadorLexico/analisadorLexico.c
	gcc AnalisadorLexico/analisadorLexico.c -c
	@echo "As Funções do analisadorLexico estão inclusas"

anaSi: AnalisadorSintatico/analisadorSintatico.c
	gcc AnalisadorSintatico/analisadorSintatico.c -c
	@echo "As Funções do analisadorSintatico estão inclusas"

estru: estruturas/estruturas.c
	gcc estruturas/estruturas.c -c
	@echo "Estruturas estão inclusas"

runCompile: compile
	./compile

clean:
	rm -rf compile analisadorLexico.o analisadorSintatico.o estruturas.o
