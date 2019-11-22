#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#include <iostream>
#include "node.cpp"

#endif

class StackArray
{
private:
    int top;               // index of top element
    int capacity;          // allocated memory space of array
    node **stack;          // array representing stack
    void DoubleCapacity(); // double the capacity of stack
public:
    StackArray() : top(-1), capacity(1)
    {                                          // constructor
        stack = (node **)new node *[capacity]; // initial state: top=-1, capacity=1
    }
    void stack_Push(node *x);
    void stack_Pop();
    bool stack_IsEmpty();
    node *stack_Top();
    int stack_getSize();
};
void StackArray::DoubleCapacity()
{
    capacity *= 2;                          // double capacity
    node **newStack = new node *[capacity]; // create newStack

    for (int i = 0; i < capacity / 2; i++)
    { // copy element to newStack
        newStack[i] = (node *)stack[i];
    }

    delete[] stack;   // release the memory of stack
    stack = newStack; // redirect stack to newStack
}

void StackArray::stack_Push(node *x)
{
    if (top == capacity - 1)
    { // if stack is full, double the capacity
        DoubleCapacity();
    }
    stack[++top] = x; // update top and put x into stack
}

void StackArray::stack_Pop()
{

    if (stack_IsEmpty())
    { // if stack is empty, there is nothing to pop
        std::cout << "Stack is empty.\n";
        return;
    }
    top--; // update top
    //    stack[top] = 0;         // (*1)
    //    stack[top].~T();        // (*2)
}

bool StackArray::stack_IsEmpty()
{

    //    if (top == -1) {
    //        return true;
    //    }
    //    else {
    //        return false;
    //    }
    return (top == -1);
}

node *StackArray::stack_Top()
{
    if (stack_IsEmpty())
    { // check if stack is empty
        std::cout << "Stack is empty.\n";
        return NULL;
    }
    return stack[top]; // return the top element
}

int StackArray::stack_getSize()
{

    return top + 1; // return the number of elements in stack
}
/*
int main()
{
    node *a = new node;
    node *b = new node;
    node *c = new node;
    a->dist = 100;
    b->dist = 200;
    c->dist = 50;
    StackArray s;
    s.stack_Pop();
    s.stack_Push(a);
    s.stack_Push(b);
    std::cout << "\ntop: " << s.stack_Top()->dist << "\nsize: " << s.stack_getSize() << std::endl;
    s.stack_Push(c);
    std::cout << "\ntop: " << s.stack_Top()->dist << "\nsize: " << s.stack_getSize() << std::endl;
    s.stack_Pop();
    s.stack_Pop();
    std::cout << "\ntop: " << s.stack_Top()->dist << "\nsize: " << s.stack_getSize() << std::endl;
    s.stack_Pop();
    std::cout << "\ntop: " << s.stack_Top()->dist << "\nsize: " << s.stack_getSize() << std::endl;
    std::cout << "123";

    return 0;
}
*/