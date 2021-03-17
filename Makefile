test: clean main.o

# 1o arg == 1 roda a parte de testes embutido no código do aluno
# 2o arg é o número de colunas para impressão do baralho
# 3o arg é a quantidade de cartas a serem distribuídas na mão
# 4o arg é o arquivo com a sequencia de cartas pré embaralhadas
# 5o arg é o arquivo de impressão do deck pré embaralhado

	bash test.sh "1" "4" "8" "cardseq.dat" "cardprint4.txt"
	
main.o:
	gcc main.c -o main.o
	
clean:
	rm -rf *.o *~ main
