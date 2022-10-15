#pragma once
#include "No.h"
class Lista
{
public:
	Lista();
	~Lista() {};
	void inserirLista(int index); //Insere um N� com dado no pr�ximo espa�o vago
	void removerLista(); //Remove o �ltimo elemento da lista
	int buscarLista(int pos); //Retorna o dado de um N� na posi��o especificada
	int quantidadeElementos(); //Retorna quantos nos tem na lista
	//int obterPrimeiroElementoObsoleto(); //Retorna o dado do primeiro no da lista
	//int obterUltimoElementoObsoleto(); //Retorna o dado do ultimo elemento

	//Novos m�todos
	void insereFimLista(int* index); //Insere um n� com dado definido por par�metro no fim da lista
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
	bool listaVazia(); //Verifica se a lista est� vazia

	No* inicio_lista;
	No* fim_lista;
	No* aux;
	int* temp;
	int temporaria;
	int qtdNo;
	int cursor;
};

