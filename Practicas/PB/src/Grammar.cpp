#include "headers/Grammar.h"

Grammar::Grammar(/* args */)
{
}

Grammar::~Grammar()
{
}

void Grammar::addSymbol(int i, Symbol s)
{
    syms[i] = s;
}

void Grammar::addProd(int i, Production p)
{
    prods[i] = p;
}

Symbol Grammar::getSym(int i)
{
    return syms[i];
}

Production Grammar::getProd(int i)
{
    return prods[i];
}

void Grammar::setNum(int term, int noTerm)
{
    this->term = term;
    this->noTerm = noTerm;
}
