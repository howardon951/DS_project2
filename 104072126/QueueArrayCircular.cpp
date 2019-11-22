#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#include <iostream>
#include "node.cpp"

#endif
#define maxint 1000000

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
    void queue_Push(node *x);
    void queue_Pop();
    bool queue_IsEmpty();
    bool queue_IsFull();
    node *queue_getFront();
    node *queue_getBack();
    int queue_getSize();
    int queue_getCapacity(); // 驗證用, 可有可無
};

void QueueArrayCircular::DoubleCapacity()
{

    node **newQueue = new node *[capacity * 2];
    int j = front, size = queue_getSize();
    for (int i = 1; i <= size; i++)
    {
        newQueue[i] = queue[++j % capacity]; // j 要先加一, 因為 front 沒有東西
    }
    back = queue_getSize(); // 要在更改 capacity 之前抓住 back
    front = 0;              // 改變 front 要在 getSize() 之後
    capacity *= 2;
    delete[] queue;
    queue = newQueue;
}
void QueueArrayCircular::queue_Push(node *x)
{
    if (queue_IsFull())
    {
        DoubleCapacity();
    }
    back = (back + 1) % capacity;
    queue[back] = x;
}

void QueueArrayCircular::queue_Pop()
{

    if (queue_IsEmpty())
    {
        std::cout << "Queue is empty.\n";
        return;
    }
    front = (front + 1) % capacity;
}

bool QueueArrayCircular::queue_IsEmpty()
{
    return (front == back);
}
bool QueueArrayCircular::queue_IsFull()
{
    return ((back + 1) % capacity == front);
}
node *QueueArrayCircular::queue_getFront()
{
    if (queue_IsEmpty())
    {
        std::cout << "Queue is empty.\n";
        return NULL; //-1
    }
    return queue[(front + 1) % capacity];
}
node *QueueArrayCircular::queue_getBack()
{
    if (queue_IsEmpty())
    {
        std::cout << "Queue is empty.\n";
        return NULL; //-1
    }
    return queue[back];
}
int QueueArrayCircular::queue_getSize()
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
int QueueArrayCircular::queue_getCapacity()
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
