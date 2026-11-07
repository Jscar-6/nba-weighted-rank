#ifndef NBA_WEIGHTED_RANK_MAXHEAP_H
#define NBA_WEIGHTED_RANK_MAXHEAP_H
#include <vector>
#include "Performance.h"
using namespace std;

class MaxHeap {
    vector<Performance> heap;
    int size;
    void heapifyDown(int index);
    void buildHeap();
public:
    MaxHeap(vector<Performance>& array) {
        heap = array;
        size = static_cast<int>(heap.size());
        buildHeap();
    }
    Performance extractMax();
    vector<Performance> top25();
};


#endif //NBA_WEIGHTED_RANK_MAXHEAP_H
