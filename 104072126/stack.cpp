#include <iostream>

class StackArray
{
private:
    int top;               // index of top element
    int capacity;          // allocated memory space of array
    int *stack;            // array representing stack
    void DoubleCapacity(); // double the capacity of stack
public:
    StackArray() : top(-1), capacity(1)
    {                              // constructor
        stack = new int[capacity]; // initial state: top=-1, capacity=1
    }
    void Push(int x);
    void Pop();
    bool IsEmpty();
    int Top();
    int getSize();
};
void StackArray::DoubleCapacity()
{

    capacity *= 2;                     // double capacity
    int *newStack = new int[capacity]; // create newStack

    for (int i = 0; i < capacity / 2; i++)
    { // copy element to newStack
        newStack[i] = stack[i];
    }

    delete[] stack;   // release the memory of stack
    stack = newStack; // redirect stack to newStack
}

void StackArray::Push(int x)
{

    if (top == capacity - 1)
    { // if stack is full, double the capacity
        DoubleCapacity();
    }
    stack[++top] = x; // update top and put x into stack
}

void StackArray::Pop()
{

    if (IsEmpty())
    { // if stack is empty, there is nothing to pop
        std::cout << "Stack is empty.\n";
        return;
    }
    top--; // update top
    //    stack[top] = 0;         // (*1)
    //    stack[top].~T();        // (*2)
}

bool StackArray::IsEmpty()
{

    //    if (top == -1) {
    //        return true;
    //    }
    //    else {
    //        return false;
    //    }
    return (top == -1);
}

int StackArray::Top()
{

    if (IsEmpty())
    { // check if stack is empty
        std::cout << "Stack is empty.\n";
        return -1;
    }
    return stack[top]; // return the top element
}

int StackArray::getSize()
{

    return top + 1; // return the number of elements in stack
}

int main()
{

    StackArray s;
    s.Pop();
    s.Push(14);
    s.Push(9);
    std::cout << "\ntop: " << s.Top() << "\nsize: " << s.getSize() << std::endl;
    s.Push(7);
    std::cout << "\ntop: " << s.Top() << "\nsize: " << s.getSize() << std::endl;
    s.Pop();
    s.Pop();
    std::cout << "\ntop: " << s.Top() << "\nsize: " << s.getSize() << std::endl;
    s.Pop();
    std::cout << "\ntop: " << s.Top() << "\nsize: " << s.getSize() << std::endl;

    return 0;
}