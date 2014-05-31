#include "Item.h"
#include <algorithm>

Item::Item(void)
{
	Item(-1);
}

Item::Item(int id)
{
	item_id = id;
    item_lines = vector<ItemLine>(0);
}

Item::~Item(void)
{
}

bool item_line_comp(ItemLine &i, ItemLine &j)
{
    if (i.pid != j.pid) {
        return i.pid < j.pid;
    }

    if (i.dot_pos != j.dot_pos) {
        return i.dot_pos < j.dot_pos;
    }

    return i.lookahead.value < j.lookahead.value;
}

bool Item::equal(const Item &other) const
{
    vector<ItemLine> fi = this->item_lines;
    vector<ItemLine> si = other.item_lines;
    
    if (fi.size() != si.size()) {
        return false;
    }

    std::sort(fi.begin(), fi.end(), item_line_comp);
    std::sort(si.begin(), si.end(), item_line_comp);

    size_t l = fi.size();
    for (size_t i = 0; i < l; i++) {
        if (!fi.at(i).equal(si.at(i))) {
            return false;
        }
    }

    return true;
}



void Item::insert_item_line(ItemLine &i)
{
    item_lines.push_back(i);
}

void Item::set_item_lines(const vector<ItemLine> &ils)
{
	item_lines = ils;
}