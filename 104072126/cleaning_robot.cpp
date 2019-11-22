#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
//#include "node.cpp"
#include "max_heap_node.cpp"
#include "stack_node.cpp"
#include "QueueArrayCircular.cpp"
using namespace std;
#define maxint 1000000
/*
class node
{
public:
    int type;      // 0: floor  1:wall  2:cleaned floor 3:battery
    int dist = -1; //the needed steps to startpoint
    bool visit = 0;
    int r, c;
    node *dir[4]; //*up = NULL, *down = NULL, *left = NULL, *right = NULL;
    //u = 0, d = 1, l = 2, r = 3;
};
*/

struct cleaning_robot
{
    int direction; //up:-1 down: 1 left:-2 right:2
    int battery_remain;
    node *next;
};

int floor_number = 0;
int map_row, map_col, battery_capacity;
char **field;
node **map;
node *recharger;
//void check_battery(void);
void error(void);
void cal_dist(node *startnode);
void start_cleaning(node *startpoint, MaxHeap *heap_node);
void printmap(void);
void build_adj_list();
void clear_visit(node **map);
node *go_smaller(node *now);

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
    int id = 0;
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
                {
                    floor_number++;
                    map[i][j].idx = id++;
                }
            }

            else if (field[i][j] == 'R')
            {
                map[i][j].type = 2;
                map[i][j].r = i;
                map[i][j].c = j;
                map[i][j].dist = 0;
                recharger = &map[i][j];
                //map[i][j].idx=id++;
            }
            else
                error();
        }
    };
    infile.close();

    build_adj_list();
    cal_dist(recharger); //BFS

    //printmap(); //check_map

    MaxHeap heap_node(floor_number);
    for (int i = 0; i < map_row; i++)
    {
        for (int j = 0; j < map_col; j++)
        {
            if (map[i][j].type == 0)
                heap_node.insertKey(&map[i][j]);
        }
    };

    /*  write the result to output.final */
    ofstream outfile;
    outfile.open("output.final");

    /*algorithm--while loop*/
    //start_cleaning(recharger, &heap_node);
    //void clear_visit( map);
    printmap();
    node *des_point;
    while (des_point = heap_node.extractMax())
    {
        if (des_point == NULL)
            break;
        if (des_point->visit == false)
            continue; //had visited
        des_point->visit = false;
        //printmap();

        node *now = des_point;
        StackArray s;
        while (now != recharger) //go path
        {
            static int count = 0;
            cout << endl
                 << count++;
            now = go_smaller(now);
            s.stack_Push(now);
            if (now->visit == true)
                now->visit = false;
        }
        now = des_point;
        // cout stack path
        while (!s.stack_IsEmpty())
        {
            outfile << s.stack_Top()->r << " " << s.stack_Top()->c << endl;
            s.stack_Pop();
        }

        while (now != recharger) //back path
        {
            static int count2 = 0;
            cout << endl
                 << count2++ << "***";
            // cout path
            outfile << now->r << " " << now->c << endl;
            now = go_smaller(now);
            s.stack_Push(now);
            if (now->visit == true)
                now->visit = false;
        }
    }
    outfile << recharger->r << " " << recharger->c << endl;
    printmap();
    outfile.close();
    return 0;
}

/*********************main end*********************************/

void start_cleaning(node *startpoint, MaxHeap *heap_node)
{
    node *des_point;
    while (des_point = heap_node->extractMax())
    {
        if (des_point == NULL)
            break;
        if (des_point->visit == true)
            continue; //had visited
        des_point->visit = true;

        node *now = des_point;
        StackArray s;
        while (now != startpoint) //go path
        {
            now = go_smaller(now);
            s.stack_Push(now);
            if (now->visit = false)
                now->visit = true;
        }
        now = des_point;
        // cout stack path
        while (!s.stack_IsEmpty())
        {
            cout << s.stack_Top()->r << " " << s.stack_Top()->r << endl;
            s.stack_Pop();
        }

        while (now != startpoint) //back path
        {
            // cout path
            cout << now->r << " " << now->c << endl;
            now = go_smaller(now);
            s.stack_Push(now);
            if (now->visit = false)
                now->visit = true;
        }
    }
}

node *go_smaller(node *now)
{
    for (int i = 0; i < 4; i++)
    {
        if (now->dir[i] != NULL && now->dir[i]->dist < now->dist)
            return now->dir[i];
    }
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
    Q.queue_Push(startnode);
    while (!Q.queue_IsEmpty())
    {
        node *u = Q.queue_getFront();
        Q.queue_Pop();
        for (int i = 0; i < 4; i++) //for all adjacent node from u
        {
            if ((u->dir[i] != NULL) && !(u->dir[i]->visit))
            {
                u->dir[i]->visit = true;
                Q.queue_Push(u->dir[i]); //mark the distence from startpoint
                u->dir[i]->dist = u->dist + 1;
            }
        }
    }
}

void printmap(void)
{
    for (int i = 0; i < map_row; i++)
    {
        for (int j = 0; j < map_col; j++)
        {
            if (map[i][j].type == 0)
                cout << map[i][j].visit << " ";
            else
                cout << "  ";
        }
        cout << endl;
    };
    for (int i = 0; i < map_row; i++)
    {
        for (int j = 0; j < map_col; j++)
        {
            if (map[i][j].type == 0)
                cout << map[i][j].dist << " ";
            else
                cout << "  ";
        }
        cout << endl;
    };
}

void clear_visit(node **map)
{
    for (int i = 0; i < map_row; i++)
    {
        for (int j = 0; j < map_col; j++)
        {
            if (map[i][j].type != 1)
                map[i][j].visit = 0;
        }
    }
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