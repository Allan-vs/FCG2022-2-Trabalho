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
	cursor = 0; //Inicia o cursor na primeira posição
}

void Lista::inserirLista(int index)
{
	No* n = new No; //Define o nó e insere o elemento no nó
	n->setDado(index); //Encapsula o dado no nó
	if (listaVazia()) { //Caso a lista esteja vazia
		inicio_lista = n; //Insere o nó na lista
	}
	else {//Caso não esteja vazia
		percorrerLista(); //Encontrar o último nó da lista
		aux->setProximo(n); //Insere o nó na lista
		n->setAnterior(aux); //Seta o anterior
	}
	fim_lista = n; //Configura o novo nó como sendo o último da lista
	qtdNo++;
}

void Lista::removerLista()
{
	qtdNo--;
	percorrerLista(); //Encontrar o penúltimo nó da lista
	delete fim_lista; //Desaloca a última posição
	if (fim_lista == inicio_lista) {
		inicio_lista = fim_lista = aux = nullptr;
	}
	else {
		aux->setProximo(nullptr); //Configura valor nulo para o ponteiro proximoNo do penúltimo nó
		fim_lista = aux; //Corrige qual o último nó da lista
	}
}

int Lista::buscarLista(int pos)
{
	aux = inicio_lista;
	for (int i = 0; i < pos; i++) { //Percorre a lista até encontrar o nó na posição inserida
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
	No* n = new No; //Define o nó e insere o elemento no nó
	n->setDado(*index);//Encapsula o dado no nó
	if (listaVazia()) { //Caso a lista esteja vazia
		inicio_lista = n; //Insere o nó na lista
		n->setAnterior(nullptr);
	}
	else {//Caso não esteja vazia
		percorrerLista(); //Encontrar o último nó da lista
		aux->setProximo(n); //Insere o nó na lista
		n->setAnterior(aux);
	}
	fim_lista = n; //Configura o novo nó como sendo o último da lista
	qtdNo++;
}

void Lista::insereInicioLista(int* index)
{
	No* n = new No; //Define o nó e insere o elemento no nó
	n->setDado(*index); //Encapsula o dado no nó
	aux = inicio_lista;
	if (listaVazia()) { //Caso a lista esteja vazia
		inicio_lista = fim_lista = n; //Insere o nó na lista
	}
	else {//Caso não esteja vazia
		aux->setAnterior(n); //Insere o nó na lista
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
		No* n = new No; //Define o nó e insere o elemento no nó
		n->setDado(*index); //Encapsula o dado no nó

		No* auxiliar; //Define um ponteiro de nó como auxiliar para o método

		aux = inicio_lista;
		for (int i = 0; i < posicao - 1; i++) {//Atinge a posição anterior a dejada
			aux = aux->getProximo();
		}
		auxiliar = aux->getProximo(); //Define o ponteiro auxiliar do método como sendo o antigo nó na posição que será alterada
		n->setAnterior(aux);
		n->setProximo(auxiliar);
		aux->setProximo(n); //Define o endereço da próxima posição do nó anterior ao que foi "afastado"
		auxiliar->setAnterior(n);//Define o endereço da posição anterior ao nó que estava na posição do novo como o novo nó

		qtdNo++;
	}
}

void Lista::alterarIndex(int posicao, int index)
{
	aux = inicio_lista; //Coloca o aux na posição inicial
	for (int i = 0; i < posicao; i++) { //Atinge a posição dejada
		aux = aux->getProximo();
	}
	aux->setDado(index); //Muda o dado do index para o desejado
}

void Lista::removeUltimoNo()
{
	qtdNo--;
	percorrerLista(); //Encontrar o penúltimo nó da lista
	delete fim_lista; //Desaloca a última posição
	if (fim_lista == inicio_lista) {
		inicio_lista = fim_lista = aux = nullptr;
	}
	else {
		aux->setProximo(nullptr); //Configura valor nulo para o ponteiro proximoNo do penúltimo nó
		fim_lista = aux; //Corrige qual o último nó da lista
	}
}

void Lista::removePrimeiroNo()
{
	qtdNo--;
	aux = inicio_lista;//Seta o Aux na posição Inicial
	aux = aux->getProximo();//Se desloca para a segunda posição
	if (inicio_lista->getProximo() == nullptr) {
		delete inicio_lista;//Desaloca a primeira posição
		inicio_lista = fim_lista = aux = nullptr;
	}
	else {
		delete inicio_lista;//Desaloca a primeira posição
		inicio_lista = aux; //Corrige qual o primeiro nó da lista
		aux->setAnterior(nullptr);
	}
}

void Lista::esvaziaLista()
{
	if (!listaVazia()) {
		aux = inicio_lista;
		while (aux->getProximo() != nullptr) removePrimeiroNo();//Esvazia até o penúltimo Nó
		removePrimeiroNo();//Esvazia o último Nó
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
	int num = -1, i = 0; //int para o index da lista, i para contagem da posição
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
			inserirLista(-1); //-1 para todos os voxels não preenchidos
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
	if (cursor + direc >= 0 && cursor + direc < 1000) { //Verifica se não está na "extremidade" da grid
		cursor = cursor + direc;
	}
}

int Lista::obterPosCursor()
{
	return cursor;
}


void Lista::imprimirLista() //OBSOLETO
{
		cout << "Posição 0: " << aux->getDado() << endl;
		for (int i = 0; i < qtdNo - 1; i++) {
			aux = aux->getProximo();
			cout << "Posição " << i + 1 << ": " << aux->getDado() << endl;
		}
}