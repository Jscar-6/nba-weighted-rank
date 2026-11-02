#ifndef NBA_WEIGHTED_RANK_MAXHEAP_H
#define NBA_WEIGHTED_RANK_MAXHEAP_H
#include <vector>
using namespace std;

class MaxHeap {
    vector<int> heap;
    int size;
    void heapifyDown(int index);
    void buildHeap();
public:
    MaxHeap(vector<int>& array) {
        heap = array;
        size = heap.size();
        buildHeap();
    }
    int extractMax();
};


#endif //NBA_WEIGHTED_RANK_MAXHEAP_H
