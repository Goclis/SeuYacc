#include "Symbol.h"

Symbol::Symbol()
{
    Symbol("", 0);
}

Symbol::~Symbol(void)
{
}

Symbol::Symbol(string v, int t)
{
    value = v;
    type = t;
}

Symbol::Symbol(const Symbol &s)
{
    this->value = s.value;
    this->type = s.type;
}

bool Symbol::equal(const Symbol &other) const
{
    return this->value == other.value
        && this->type == other.type;
}