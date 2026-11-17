#ifndef FEATURE_H
#define FEATURE_H

#include "list.h"

// Max-Heap data structure storing PaperNode pointers (no STL containers)
struct PaperHeap {
  PaperNode** data;
  int size;
  int capacity;
};

// Heap lifecycle
PaperHeap* heapCreate(int capacity = 16);
void heapFree(PaperHeap* heap);

// Core heap operations
bool heapInsert(PaperHeap* heap, PaperNode* node);
PaperNode* extractMax(PaperHeap* heap);
int heapSize(const PaperHeap* heap);

// Week 11: Real List API Integration
// Walks the PaperList and inserts every PaperNode into a freshly created heap
PaperHeap* buildHeapFromList(PaperList* list);

// Sort track (merge sort on pointer array)
void mergeSort(PaperNode** arr, PaperNode** tmp, int left, int right);

#endif // FEATURE_H
