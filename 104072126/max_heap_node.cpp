// A C++ program to demonstrate common Binary Heap Operations
#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#include <iostream>
#include "node.cpp"

#endif
#include <climits>

using namespace std;

// Prototype of a utility function to swap two integers
void swap(node **x, node **y);

// A class for Min Heap
class MaxHeap
{
    node **harr;   // pointer to array of elements in heap
    int capacity;  // maximum possible size of min heap
    int heap_size; // Current number of elements in min heap
public:
    // Constructor
    MaxHeap(int capacity);

    // to heapify a subtree with the root at given index
    void MaxHeapify(int);

    int parent(int i) { return (i - 1) / 2; }

    // to get index of left child of node at index i
    int left(int i) { return (2 * i + 1); }

    // to get index of right child of node at index i
    int right(int i) { return (2 * i + 2); }

    // to extract the root which is the minimum element
    node *extractMax();

    // Decreases key value of key at index i to new_val
    void decreaseKey(int i, int new_val);

    // Returns the minimum key (key at root) from min heap
    node *getMax() { return harr[0]; }

    // Deletes a key stored at index i
    void deleteKey(int i);

    // Inserts a new key 'k'
    void insertKey(node *k);
};

// Constructor: Builds a heap from a given array a[] of given size
MaxHeap::MaxHeap(int cap)
{
    heap_size = 0;
    capacity = cap;
    harr = new node *[cap];
}

// Inserts a new key 'k'
void MaxHeap::insertKey(node *k)
{
    if (heap_size == capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }

    // First insert the new key at the end
    heap_size++;
    int i = heap_size - 1;
    harr[i] = k;

    // Fix the min heap property if it is violated
    while (i != 0 && harr[parent(i)]->dist < harr[i]->dist)
    {
        swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
}

// Decreases value of key at index 'i' to new_val. It is assumed that
// new_val is smaller than harr[i].
void MaxHeap::decreaseKey(int i, int new_val)
{
    node *new_node = new node;
    new_node->dist = new_val;
    harr[i] = new_node;
    while (i != 0 && harr[parent(i)]->dist < harr[i]->dist)
    {
        swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
}

// Method to remove minimum element (or root) from min heap
node *MaxHeap::extractMax()
{
    if (heap_size <= 0)
        return NULL;
    if (heap_size == 1)
    {
        heap_size--;
        return harr[0];
    }

    // Store the minimum value, and remove it from heap
    node *root = harr[0];
    harr[0] = harr[heap_size - 1];
    heap_size--;
    MaxHeapify(0);

    return root;
}

// This function deletes key at index i. It first reduced value to minus
// infinite, then calls extractMin()
void MaxHeap::deleteKey(int i)
{
    decreaseKey(i, INT_MAX);
    extractMax();
}

// A recursive method to heapify a subtree with the root at given index
//// This method assumes that the subtrees are already heapified///
void MaxHeap::MaxHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l]->dist > harr[i]->dist)
        smallest = l;
    if (r < heap_size && harr[r]->dist > harr[smallest]->dist)
        smallest = r;
    if (smallest != i)
    {
        swap(&harr[i], &harr[smallest]);
        MaxHeapify(smallest);
    }
}

// A utility function to swap two elements
void swap(node **x, node **y)
{
    node *temp = *x;
    *x = *y;
    *y = temp;
}
/*
// Driver program to test above functions
int main()
{
    MaxHeap h(11);
    h.insertKey(3);
    h.insertKey(2);
    h.deleteKey(1);
    h.insertKey(15);
    h.insertKey(5);
    h.insertKey(4);
    h.insertKey(45);
    cout << h.extractMax() << " ";
    cout << h.getMax() << " ";
    h.decreaseKey(0, 2);
    cout << h.getMax();
    return 0;
}
*/
/*MaxHeap h(11);
    h.insertKey(&map[2][3]);
    h.insertKey(&map[1][1]);

    h.insertKey(&map[1][2]);
    h.insertKey(&map[1][3]);
    h.insertKey(&map[1][5]);
    h.insertKey(&map[5][5]);
    h.deleteKey(0);
    cout << h.extractMax()->dist << " ";
    cout << h.getMax()->dist << " ";

    //h.decreaseKey(0, 2);
    cout << h.getMax()->dist;
    return 0;*/
