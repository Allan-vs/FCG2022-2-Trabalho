#include "No.h"

No::No()
{
	proximoNo = nullptr;
}

int No::getDado()
{
	return dado;
}


void No::setDado(int d)
{
	dado = d;
}


No* No::getProximo()
{
	return proximoNo;
}

No* No::getAnterior()
{
	return anteriorNo;
}


void No::setProximo(No* prox)
{
	proximoNo = prox;
}

void No::setAnterior(No* ant)
{
	anteriorNo = ant;
}


