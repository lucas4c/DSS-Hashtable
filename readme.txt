Função Hash:
	A Função hash usada nesse caso foi bem simples, ela faz uma série de multiplicações de um número primo (no caso 83) + o código ASCII de cada letra da chave.
	E depois retorna esse número mod (%) TABLE_SIZE, TABLE_SIZE sendo o tamanho da hashtable, retornando assim um número entre 0 e o tamanho da tabela hash.
Colisões:
	Colisões foram lidadas com a maneira de endereçamento fechado, ou seja, cada par chave-valor está em seu respectivo índice, que, caso forem iguais para dois elementos com chaves distintas, 
	serão inseridos em uma lista encadeada dentro de seu próprio índice.

