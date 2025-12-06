#include "headers/Symbol.h"

Symbol::Symbol(/* args */)
{
}

Symbol::Symbol(Token token)
{
    this->type = terminal;    
    this->value.token = token;
}

Symbol::Symbol(NoTerm noTerm, SymType type)
{
    this->type = type;    
    this->value.noTerm = noTerm;
}

Symbol::~Symbol()
{
}

SymType Symbol::getType()
{
    return type;
}

Token Symbol::getToken()
{
    return value.token;
}

NoTerm Symbol::getNoTerm()
{
    return value.noTerm;
}
