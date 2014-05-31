#include "Priority.h"

Priority::Priority(int l, int lr)
{
    this->level = l;
    this->lr = lr;
}

Priority::Priority(void)
{
    Priority(-1, -1);
}

Priority::~Priority(void)
{
}
