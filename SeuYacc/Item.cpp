#include "Item.h"

Item::Item(void)
{
	Item(-1);
}

Item::Item(int id)
{
	item_id = id;
}

Item::~Item(void)
{
}

void Item::set_item_lines(const vector<ItemLine> &ils)
{
	item_lines = ils;
}