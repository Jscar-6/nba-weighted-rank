#include "MaxHeap.h"
#include <vector>

#include "Performance.h"
using namespace std;

void MaxHeap::buildHeap() {
    for (int i = size / 2; i >= 0; i--) {
        heapifyDown(i);
    }
}

Performance MaxHeap::extractMax() {
    Performance max = heap[0];
    heap[0] = heap[--size];
    heapifyDown(0);
    return max;
}

void MaxHeap::heapifyDown(int index) {
    int largest = index;
    int left = 2*index + 1;
    int right = 2*index + 2;

    if (left < size && heap[left].score > heap[largest].score) {
        largest = left;
    }
    if (right < size && heap[right].score > heap[largest].score) {
        largest = right;
    }

    if (largest != index) {
        Performance temp = heap[index];
        heap[index] = heap[largest];
        heap[largest] = temp;
        heapifyDown(largest);
    }
}
