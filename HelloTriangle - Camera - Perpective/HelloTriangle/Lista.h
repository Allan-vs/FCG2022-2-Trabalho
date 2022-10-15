#pragma once
#include "No.h"
class Lista
{
public:
	Lista();
	~Lista() {};
	void inserirLista(int index); //Insere um Nó com dado no próximo espaço vago
	void removerLista(); //Remove o último elemento da lista
	int buscarLista(int pos); //Retorna o dado de um Nó na posição especificada
	int quantidadeElementos(); //Retorna quantos nos tem na lista
	//int obterPrimeiroElementoObsoleto(); //Retorna o dado do primeiro no da lista
	//int obterUltimoElementoObsoleto(); //Retorna o dado do ultimo elemento

	//Novos métodos
	void insereFimLista(int* index); //Insere um nó com dado definido por parâmetro no fim da lista
	void insereInicioLista(int* index);
	void insereEm(int posicao, int* index);
	void alterarIndex(int posicao, int index);
	void removeUltimoNo();
	void removePrimeiroNo();
	void esvaziaLista();
	void salvaLista();
	void carregaLista();
	int* obterUltimoIndex();
	int* obterPrimeiroIndex();
	int* obterIndexEm(int posicao);
	void moverCursor(int direc);
	int obterPosCursor();

	void imprimirLista(); //OBSOLETO

private:
	void percorrerLista(); //Percorre todos elementos da lista
	bool listaVazia(); //Verifica se a lista está vazia

	No* inicio_lista;
	No* fim_lista;
	No* aux;
	int* temp;
	int temporaria;
	int qtdNo;
	int cursor;
};

