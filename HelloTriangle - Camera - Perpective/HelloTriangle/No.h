#pragma once
class No
{
public:
	No();
	~No() {};

	int getDado();
	void setDado(int d);
	No* getProximo();
	No* getAnterior();
	No* getAcima();
	No* getAbaixo();
	No* getFrente();
	No* getTras();
	void setProximo(No* prox);
	void setAnterior(No* ant);

private:
	int dado;
	No* proximoNo;
	No* anteriorNo;
};

