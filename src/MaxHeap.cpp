#include "MaxHeap.h"
#include <vector>
using namespace std;

void MaxHeap::buildHeap() {
    for (int i = size / 2; i >= 0; i--) {
        heapifyDown(i);
    }
}

int MaxHeap::extractMax() {
    int max = heap[0];
    heap[0] = heap[--size];
    heapifyDown(0);
    return max;
}

void MaxHeap::heapifyDown(int index) {
    int largest = 0;
    if (2*index + 1 >= size || heap[index] < heap[2*index + 1] || heap[index] > heap[2*index + 2]) {
        return;
    }
    if (2*index + 2 < size && heap[2*index + 1] < heap[2*index + 2]) {
        largest = heap[2*index + 2];
    }
    else if (2*index + 2 < size && heap[2*index + 1] > heap[2*index + 2]) {
        largest = heap[2*index + 1];
    }
    int temp = heap[index];
    heap[index] = largest;
    largest = temp;
    heapifyDown(largest);
}
