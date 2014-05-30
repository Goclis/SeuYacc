#include "ItemLine.h"

ItemLine::ItemLine(void)
{
	ItemLine(0, 0, Symbol("", 0));
}

ItemLine::ItemLine(int p, int d, Symbol &look)
{
	this->pid = p;
	this->dot_pos = d;
	this->lookahead = look;
}

ItemLine::~ItemLine(void)
{
}

bool ItemLine::equal(const ItemLine &other) const
{
	return this->pid == other.pid
		&& this->dot_pos == other.dot_pos
		&& this->lookahead.equal(other.lookahead);
}