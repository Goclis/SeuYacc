#pragma once

class Priority
{
public:
    int level; // 等级，越大越高
    int lr; // 左结合还是右结合，左0，右1

    Priority(void);
    Priority(int l, int lr);
    ~Priority(void);
};
