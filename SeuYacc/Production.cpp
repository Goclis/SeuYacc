#include "Production.h"

Production::Production(int pid, Symbol &left, vector<Symbol> &right)
{
    this->pid = pid;
    this->left = left;
    this->right = right;
}

Production::~Production(void)
{
}
