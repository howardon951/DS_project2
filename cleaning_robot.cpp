#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;
#define maxint 1000000

class node
{
public:
    int type;      // 0: floor  1:wall  2:cleaned floor 3:battery
    int dist = -1; //the needed steps to startpoint
    bool visit = 0;
    bool nearby = 0;
    int r, c;
    node *dir[4]; //*up = 0, *down = 1, *left = 2, *right = 3;
};
struct cleaning_robot
{
    int direction; // up: 0 down: 1 left: 2 right: 3
    int battery_remain;
    node *next, *prev, *now;
};
class QueueArrayCircular
{
private:
    int capacity, front, back;
    node **queue;
    void DoubleCapacity();

public:
    QueueArrayCircular() : capacity(maxint), front(0), back(0)
    { // 從0開始, 第一個位置放掉
        queue = new node *[capacity];
    }
    void Push(node *x);
    void Pop();
    bool IsEmpty();
    bool IsFull();
    node *getFront();
    node *getBack();
    int getSize();
    int getCapacity(); // 驗證用, 可有可無
};

int map_row, map_col, battery_capacity, floor_num = 0;
char **field;
node **map;
node *recharger;
//void check_battery(void);
void error(void);
void cal_dist(node *startnode);
void start_cleaning(node *startpoint);
void printmap(void);
void build_adj_list();

/*********************main start*******************************/
int main()
{

    ifstream infile;
    infile.open("input.data");
    infile >> map_row >> map_col >> battery_capacity;
    /*dynamically set my 2-d char map space*/

    map = new node *[map_row];
    for (int i = 0; i < map_row; i++)
        map[i] = new node[map_col];
    field = new char *[map_row];
    for (int i = 0; i < map_row; i++)
        field[i] = new char[map_col];

    for (int i = 0; i < map_row; i++)
    {
        for (int j = 0; j < map_col; j++)
        {
            infile >> field[i][j]; /*read char data into field*/
            /*and set a digital map*/
            if (field[i][j] == '0' || field[i][j] == '1')
            {
                map[i][j].type = field[i][j] - '0';
                map[i][j].r = i;
                map[i][j].c = j;
                if (map[i][j].type == 0)
                    floor_num++;
            }

            else if (field[i][j] == 'R')
            {
                map[i][j].type = 2;
                map[i][j].r = i;
                map[i][j].c = j;
                map[i][j].dist = 0;
                recharger = &map[i][j];
            }
            else
                error();
        }
    };
    infile.close();

    build_adj_list();
    cal_dist(recharger); //BFS
    printmap();
    start_cleaning(recharger);
    //check_battery();
    printmap();
    /*algorithm--while loop*/

    /*  write the result to output.final */
    ofstream outfile;
    outfile.open("output.final");
    //write format

    outfile.close();
    return 0;
}

void error(void)
{
    puts("error!");
    exit(0);
}

void cal_dist(node *startnode)
{
    startnode->visit = true;
    QueueArrayCircular Q;
    Q.Push(startnode);
    while (!Q.IsEmpty())
    {
        node *u = Q.getFront();
        Q.Pop();
        for (int i = 0; i < 4; i++) //for all adjacent node from u
        {
            if ((u->dir[i] != NULL) && !(u->dir[i]->visit))
            {
                u->dir[i]->visit = true;
                Q.Push(u->dir[i]); //mark the distence from startpoint
                u->dir[i]->dist = u->dist + 1;
            }
        }
    }

    //int x = recharger->r, y = recharger->c;
    /*if (row >= map_row || col >= map_col || row < 0 || col < 0)
        return;
    if (map[row][col].dist > x)
        map[row][col].dist = x;
    map[row][col].visit = 1;
    if (map[row - 1][col].type == 0 && map[row - 1][col].visit == 0) //up
        cal_dist(map, row - 1, col, x + 1);
    else if (map[row + 1][col].type == 0 && map[row + 1][col].visit == 0) //down
        cal_dist(map, row + 1, col, x + 1);
    else if (map[row][col - 1].type == 0 && map[row][col - 1].visit == 0) //left
        cal_dist(map, row + 1, col, x + 1);
    else if (map[row + 1][col + 1].type == 0 && map[row][col + 1].visit == 0) //right
        cal_dist(map, row + 1, col, x + 1);
    map[row][col].visit = 0;*/
    /*for (int i = 0; i < map_row; i++)
    {
        for (int j = 0; j < map_col; j++)
        {
            if (map[i][j].type != 1)
                map[i][j].dist = abs(map[i][j].r - x) + abs(map[i][j].c - y);
            else
                map[i][j].dist = 0;
        }
    };*/
}
void start_cleaning(node *startpoint)
{
    cleaning_robot doggy;
    cleaning_robot *godd = &doggy;
    //now all node's visit is 1
    //visit==1 means not yet visit, visit==0 means the node is visited
    while (floor_num) //one tour
    {
        godd->now = startpoint;
        godd->battery_remain = battery_capacity;
        //node *path[battery_capacity / 2 + 1];
        while (godd->battery_remain > 0) //one step
        {
            godd->now->visit = 0; //mark visited
            int next_dir, dirw = 0;
            for (int i = 0; i < 4; i++)
            {
                int tmp = 0;
                if (godd->now->dir[i] != NULL)
                {
                    if (godd->now->dir[i]->visit == 1) //unvisit
                        tmp += 10;
                    if (godd->now->dir[i]->nearby == 1) //is near
                        tmp += 5;
                    if (tmp >= dirw && godd->battery_remain - 1 - godd->now->dir[i]->dist >= 0) //15:unvisit&near 10:unvisit&!near 5:visit&near 0:visit&!near
                        next_dir = i;
                }
            }
            if (godd->now->dir[next_dir]->visit == 1)
                floor_num--;
            godd->prev = godd->now;
            godd->now = godd->now->dir[next_dir];
            godd->battery_remain--;
        }
    }
}

void printmap(void)
{
    for (int i = 0; i < map_row; i++)
    {
        for (int j = 0; j < map_col; j++)
        {
            cout << map[i][j].visit << " ";
        }
        cout << endl;
    };
}

void build_adj_list()
{
    for (int i = 0; i < map_row - 1; i++)
    {
        for (int j = 0; j < map_col - 1; j++)
        {
            if (map[i][j].type == 1) //I am a wall...QQ
            {
                map[i][j].dir[3] = NULL; //right
                map[i][j].dir[2] = NULL; //left
                map[i][j].dir[1] = NULL; //down
                map[i][j].dir[0] = NULL; //up
                //if right is not wall
                if (map[i][j + 1].type != 1)
                {
                    map[i][j + 1].dir[2] = NULL; //left
                }
                else //right is wall
                {
                    map[i][j + 1].dir[2] = NULL; //left
                }
                //if down is not wall
                if (map[i + 1][j].type != 1)
                {
                    map[i + 1][j].dir[0] = NULL; //up
                }
                else
                {
                    map[i + 1][j].dir[0] = NULL; //up
                }
            }
            else
            {
                //if right is not wall
                if (map[i][j + 1].type != 1)
                {
                    map[i][j].dir[3] = &map[i][j + 1]; //right
                    map[i][j + 1].dir[2] = &map[i][j]; //left
                }
                else
                {
                    map[i][j].dir[3] = NULL;     //right
                    map[i][j + 1].dir[2] = NULL; //left
                }
                //if down is not wall
                if (map[i + 1][j].type != 1)
                {
                    map[i][j].dir[1] = &map[i + 1][j]; //down
                    map[i + 1][j].dir[0] = &map[i][j]; //up
                }
                else
                {
                    map[i][j].dir[1] = NULL;     //down
                    map[i + 1][j].dir[0] = NULL; //up
                }
            }
        }
        int j = map_col - 1;
        if (map[i][j].type == 2) //I am a start
        {
            map[i][j].dir[3] = NULL;           //right
            map[i][j].dir[2] = &map[i][j - 1]; //left
            map[i][j].dir[1] = NULL;           //down
            map[i][j].dir[0] = NULL;           //up
        }
        else if (map[i][j].type == 1)
        {
            map[i][j].dir[3] = NULL; //right
            map[i][j].dir[2] = NULL; //left
            map[i][j].dir[1] = NULL; //down
            map[i][j].dir[0] = NULL; //up
        }
        else
            error();
    }
    int i = map_row - 1; //last row
    for (int j = 0; j < map_col - 1; j++)
    {
        if (map[i][j].type == 2)
        {                                      //I am a start
            map[i][j].dir[3] = NULL;           //right
            map[i][j].dir[2] = NULL;           //left
            map[i][j].dir[1] = NULL;           //down
            map[i][j].dir[0] = &map[i - 1][j]; //up
        }
        else if (map[i][j].type == 1)
        {
            map[i][j].dir[3] = NULL; //right
            map[i][j].dir[2] = NULL; //left
            map[i][j].dir[1] = NULL; //down
            map[i][j].dir[0] = NULL; //up
        }
        else
            error();
    }
}

void QueueArrayCircular::DoubleCapacity()
{

    node **newQueue = new node *[capacity * 2];
    int j = front, size = getSize();
    for (int i = 1; i <= size; i++)
    {
        newQueue[i] = queue[++j % capacity]; // j 要先加一, 因為 front 沒有東西
    }
    back = getSize(); // 要在更改 capacity 之前抓住 back
    front = 0;        // 改變 front 要在 getSize() 之後
    capacity *= 2;
    delete[] queue;
    queue = newQueue;
}
void QueueArrayCircular::Push(node *x)
{
    if (IsFull())
    {
        DoubleCapacity();
    }
    back = (back + 1) % capacity;
    queue[back] = x;
}

void QueueArrayCircular::Pop()
{

    if (IsEmpty())
    {
        std::cout << "Queue is empty.\n";
        return;
    }
    front = (front + 1) % capacity;
}

bool QueueArrayCircular::IsEmpty()
{
    return (front == back);
}
bool QueueArrayCircular::IsFull()
{
    return ((back + 1) % capacity == front);
}
node *QueueArrayCircular::getFront()
{
    if (IsEmpty())
    {
        std::cout << "Queue is empty.\n";
        return NULL; //-1
    }
    return queue[(front + 1) % capacity];
}
node *QueueArrayCircular::getBack()
{
    if (IsEmpty())
    {
        std::cout << "Queue is empty.\n";
        return NULL; //-1
    }
    return queue[back];
}
int QueueArrayCircular::getSize()
{
    int size;
    if (front < back)
    {
        size = back - front;
    }
    else
    {
        size = capacity - (front - back);
    }
    return size;
}
int QueueArrayCircular::getCapacity()
{
    return capacity;
}
/*
void printCircularQueue(QueueArrayCircular queue)
{
    cout << "front: " << queue.getFront() << "    back: " << queue.getBack() << "\n"
         << "capacity: " << queue.getCapacity() << "  number of elements: " << queue.getSize() << "\n\n";
}
*/