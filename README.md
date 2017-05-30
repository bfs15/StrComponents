# PathCounter

Trabalho de Implementação 2

CI065 / CI755 - Algoritmos e Teoria dos Grafos

Bruno Freitas Serbena GRR20151343
Michel Melo de Souza GRR20159342

O algoritmo criado pelo grupo funciona da seguinte maneira:

Existe uma classe chamada Path.

Objetos Path armazenam o valor de atributos.

A classe possui uma fila de nodos (estática);

O algoritmo percorre os nodos do grafo pegando todos que tem atributo (sumidouros), coloca eles em uma fila de nodos "Q";

	Enquanto a fila não for vazia =>
		Retira o primeiro nodo dela, "X";
		Itera os pais de X =>
			Chama "Vai para cima" nesse pai, "P" {
				Caso esse pai já tenha sido processado {
					retorna; ou seja "vai para cima" no proximo P;
				} Do contrário {
					Um Path é criado em P; ou seja é marcado como processado;
					Para calcular path de P:
					Itera todos os filhos de P, "F" =>
					(um deles será X, que já foi processado)
						Chama P.path."Processo de Soma"(F) {
							Se F foi processado {
								adiciona path de F em path que chamou;
							} Senão {
								F é marcado como processado;
								Itera nos filhos de F =>
									Chama F.path."Processo de Soma" em filhos de F para calcular path de F;
									(loop recursivo)
								Se nodo filho possuir algum outro pai que não o pai que chamou a função {
									F é adicionado a fila de nodos;
								}
								adiciona path de F em path que chamou;
							}
						}
					loop;
				}
				chama "Vai para cima" nos pais desse pai;
			}
		loop;
	loop;

	Imprime o resultado =>
		Para cada nodo:
			Setta o valor de seus atributos de maneira a eles ficarem iguais ao do seu path;
			Imprime seu nome e os atributos que tem valor maior que 0;
			Imprime as arestas que saem do nodo;
