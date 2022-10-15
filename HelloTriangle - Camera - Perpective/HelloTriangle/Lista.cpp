#include "Lista.h"
#include <iostream> 
#include <fstream>
#include <string>
// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"

using namespace std;

Lista::Lista()
{
	inicio_lista = fim_lista = aux = nullptr;
	temp = new int;
	qtdNo = 0;
	cursor = 0; //Inicia o cursor na primeira posi��o
}

void Lista::inserirLista(int index)
{
	No* n = new No; //Define o n� e insere o elemento no n�
	n->setDado(index); //Encapsula o dado no n�
	if (listaVazia()) { //Caso a lista esteja vazia
		inicio_lista = n; //Insere o n� na lista
	}
	else {//Caso n�o esteja vazia
		percorrerLista(); //Encontrar o �ltimo n� da lista
		aux->setProximo(n); //Insere o n� na lista
		n->setAnterior(aux); //Seta o anterior
	}
	fim_lista = n; //Configura o novo n� como sendo o �ltimo da lista
	qtdNo++;
}

void Lista::removerLista()
{
	qtdNo--;
	percorrerLista(); //Encontrar o pen�ltimo n� da lista
	delete fim_lista; //Desaloca a �ltima posi��o
	if (fim_lista == inicio_lista) {
		inicio_lista = fim_lista = aux = nullptr;
	}
	else {
		aux->setProximo(nullptr); //Configura valor nulo para o ponteiro proximoNo do pen�ltimo n�
		fim_lista = aux; //Corrige qual o �ltimo n� da lista
	}
}

int Lista::buscarLista(int pos)
{
	aux = inicio_lista;
	for (int i = 0; i < pos; i++) { //Percorre a lista at� encontrar o n� na posi��o inserida
		aux = aux->getProximo();
	}
	return aux->getDado(); //Retorna o dado desejado
}

bool Lista::listaVazia()
{
	if (inicio_lista == nullptr)
		return true;
	else
		return false;
}

void Lista::percorrerLista()
{
	aux = inicio_lista;
	for (int i = 0; i < qtdNo - 1; i++) {
		aux = aux->getProximo();
	}
}

int Lista::quantidadeElementos()
{
	return qtdNo;
}

/*
int Lista::obterPrimeiroElementoObsoleto()
{
	return inicio_lista->getDado();
}

int Lista::obterUltimoElementoObsoleto()
{
	return fim_lista->getDado();
}
*/

void Lista::insereFimLista(int* index)
{
	No* n = new No; //Define o n� e insere o elemento no n�
	n->setDado(*index);//Encapsula o dado no n�
	if (listaVazia()) { //Caso a lista esteja vazia
		inicio_lista = n; //Insere o n� na lista
		n->setAnterior(nullptr);
	}
	else {//Caso n�o esteja vazia
		percorrerLista(); //Encontrar o �ltimo n� da lista
		aux->setProximo(n); //Insere o n� na lista
		n->setAnterior(aux);
	}
	fim_lista = n; //Configura o novo n� como sendo o �ltimo da lista
	qtdNo++;
}

void Lista::insereInicioLista(int* index)
{
	No* n = new No; //Define o n� e insere o elemento no n�
	n->setDado(*index); //Encapsula o dado no n�
	aux = inicio_lista;
	if (listaVazia()) { //Caso a lista esteja vazia
		inicio_lista = fim_lista = n; //Insere o n� na lista
	}
	else {//Caso n�o esteja vazia
		aux->setAnterior(n); //Insere o n� na lista
		n->setProximo(aux);
		inicio_lista = n;
	}
	n->setAnterior(nullptr);
	qtdNo++;
}

void Lista::insereEm(int posicao, int* index)
{
	if (posicao == 0)
	{
		insereInicioLista(index);
	}
	else if (posicao == qtdNo)
	{
		insereFimLista(index);
	}
	else
	{
		// 0 * 1
		// aux = 0  proximoNo(aux) = * anteriorNo(auxiliar) = * auxiliar = 1
		No* n = new No; //Define o n� e insere o elemento no n�
		n->setDado(*index); //Encapsula o dado no n�

		No* auxiliar; //Define um ponteiro de n� como auxiliar para o m�todo

		aux = inicio_lista;
		for (int i = 0; i < posicao - 1; i++) {//Atinge a posi��o anterior a dejada
			aux = aux->getProximo();
		}
		auxiliar = aux->getProximo(); //Define o ponteiro auxiliar do m�todo como sendo o antigo n� na posi��o que ser� alterada
		n->setAnterior(aux);
		n->setProximo(auxiliar);
		aux->setProximo(n); //Define o endere�o da pr�xima posi��o do n� anterior ao que foi "afastado"
		auxiliar->setAnterior(n);//Define o endere�o da posi��o anterior ao n� que estava na posi��o do novo como o novo n�

		qtdNo++;
	}
}

void Lista::alterarIndex(int posicao, int index)
{
	aux = inicio_lista; //Coloca o aux na posi��o inicial
	for (int i = 0; i < posicao; i++) { //Atinge a posi��o dejada
		aux = aux->getProximo();
	}
	aux->setDado(index); //Muda o dado do index para o desejado
}

void Lista::removeUltimoNo()
{
	qtdNo--;
	percorrerLista(); //Encontrar o pen�ltimo n� da lista
	delete fim_lista; //Desaloca a �ltima posi��o
	if (fim_lista == inicio_lista) {
		inicio_lista = fim_lista = aux = nullptr;
	}
	else {
		aux->setProximo(nullptr); //Configura valor nulo para o ponteiro proximoNo do pen�ltimo n�
		fim_lista = aux; //Corrige qual o �ltimo n� da lista
	}
}

void Lista::removePrimeiroNo()
{
	qtdNo--;
	aux = inicio_lista;//Seta o Aux na posi��o Inicial
	aux = aux->getProximo();//Se desloca para a segunda posi��o
	if (inicio_lista->getProximo() == nullptr) {
		delete inicio_lista;//Desaloca a primeira posi��o
		inicio_lista = fim_lista = aux = nullptr;
	}
	else {
		delete inicio_lista;//Desaloca a primeira posi��o
		inicio_lista = aux; //Corrige qual o primeiro n� da lista
		aux->setAnterior(nullptr);
	}
}

void Lista::esvaziaLista()
{
	if (!listaVazia()) {
		aux = inicio_lista;
		while (aux->getProximo() != nullptr) removePrimeiroNo();//Esvazia at� o pen�ltimo N�
		removePrimeiroNo();//Esvazia o �ltimo N�
	}
}

void Lista::salvaLista()
{
	ofstream saveLista;
	saveLista.open("lista.txt");
	
	aux = inicio_lista;
	saveLista << aux->getDado() << endl;
	for (int i = 0; i < qtdNo - 1; i++) {
		aux = aux->getProximo();
		saveLista << aux->getDado() << endl;
	}
	saveLista.close();
}

void Lista::carregaLista()
{
	string line; //string para extrair do arquivo
	int num = -1, i = 0; //int para o index da lista, i para contagem da posi��o
	ifstream saveLista("lista.txt");

	esvaziaLista(); //limpa a lista atual
	if (saveLista.is_open()) { //carrega do arquivo
		while (getline(saveLista, line)) {
			num = stoi(line); //converte string em int
			inserirLista(num); //insere na lista
		}
		saveLista.close();
	}
	if (listaVazia()) { //caso o arquivo de salvamento estiver vazio, cria uma lista de -1
		for (int i = 0; i < 1000; i++) {
			inserirLista(-1); //-1 para todos os voxels n�o preenchidos
		}
	}
}


int* Lista::obterUltimoIndex()
{
	temporaria = fim_lista->getDado();
	return &temporaria;
}

int* Lista::obterPrimeiroIndex()
{
	*temp = inicio_lista->getDado();
	return temp;
}


int* Lista::obterIndexEm(int posicao)
{
	buscarLista(posicao);
	*temp = aux->getDado();
	return temp;
}

void Lista::moverCursor(int direc) //Move o cursor
{
	if (cursor + direc >= 0 && cursor + direc < 1000) { //Verifica se n�o est� na "extremidade" da grid
		cursor = cursor + direc;
	}
}

int Lista::obterPosCursor()
{
	return cursor;
}


void Lista::imprimirLista() //OBSOLETO
{
		cout << "Posi��o 0: " << aux->getDado() << endl;
		for (int i = 0; i < qtdNo - 1; i++) {
			aux = aux->getProximo();
			cout << "Posi��o " << i + 1 << ": " << aux->getDado() << endl;
		}
}