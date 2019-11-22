#include <iostream>
struct node
{
    int type;      // 0: floor  1:wall  2:cleaned floor 3:battery
    int dist = -1; //the needed steps to startpoint
    bool visit = 0;
    int r, c;
    node *dir[4]; //u = 0, d = 1, l = 2, r = 3;
    int idx;
};

/*
class node
{
public:
    int type;      // 0: floor  1:wall  2:cleaned floor 3:battery
    int dist = -1; //the needed steps to startpoint
    bool visit = 0;
    int r, c;
    node *dir[4]; //u = 0, d = 1, l = 2, r = 3;
    int idx;
};
*/