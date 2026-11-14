#include "feature.h"
#include <new>

namespace {

void swapNodes(PaperNode** a, PaperNode** b) {
  PaperNode* temp = *a;
  *a = *b;
  *b = temp;
}

void siftUp(PaperHeap* heap, int idx) {
  while (idx > 0) {
    int parent = (idx - 1) / 2;
    if (heap->data[idx]->data.citation_count > heap->data[parent]->data.citation_count) {
      swapNodes(&heap->data[idx], &heap->data[parent]);
      idx = parent;
    } else {
      break;
    }
  }
}

void siftDown(PaperHeap* heap, int idx) {
  while (true) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size &&
        heap->data[left]->data.citation_count > heap->data[largest]->data.citation_count) {
      largest = left;
    }
    if (right < heap->size &&
        heap->data[right]->data.citation_count > heap->data[largest]->data.citation_count) {
      largest = right;
    }

    if (largest != idx) {
      swapNodes(&heap->data[idx], &heap->data[largest]);
      idx = largest;
    } else {
      break;
    }
  }
}

} // namespace

PaperHeap* heapCreate(int capacity) {
  if (capacity <= 0) capacity = 16;
  PaperHeap* heap = new (std::nothrow) PaperHeap;
  if (!heap) return nullptr;

  heap->data = new (std::nothrow) PaperNode*[capacity];
  if (!heap->data) {
    delete heap;
    return nullptr;
  }
  heap->size = 0;
  heap->capacity = capacity;
  return heap;
}

void heapFree(PaperHeap* heap) {
  if (!heap) return;
  delete[] heap->data;
  delete heap;
}

bool heapInsert(PaperHeap* heap, PaperNode* node) {
  if (!heap || !node) return false;

  if (heap->size >= heap->capacity) {
    int newCap = heap->capacity * 2;
    PaperNode** newData = new (std::nothrow) PaperNode*[newCap];
    if (!newData) return false;

    for (int i = 0; i < heap->size; ++i) {
      newData[i] = heap->data[i];
    }
    delete[] heap->data;
    heap->data = newData;
    heap->capacity = newCap;
  }

  heap->data[heap->size] = node;
  siftUp(heap, heap->size);
  heap->size++;
  return true;
}

PaperNode* extractMax(PaperHeap* heap) {
  if (!heap || heap->size == 0) return nullptr;

  PaperNode* maxNode = heap->data[0];
  heap->size--;

  if (heap->size > 0) {
    heap->data[0] = heap->data[heap->size];
    siftDown(heap, 0);
  }

  return maxNode;
}

int heapSize(const PaperHeap* heap) {
  return heap ? heap->size : 0;
}

PaperHeap* buildHeapFromList(PaperList* list) {
  if (!list) return nullptr;
  int n = list_size(list);
  if (n <= 0) n = 16;

  PaperHeap* heap = heapCreate(n);
  if (!heap) return nullptr;

  for (PaperNode* cur = list_head(list); cur != nullptr; cur = cur->next) {
    if (!heapInsert(heap, cur)) {
      heapFree(heap);
      return nullptr;
    }
  }

  return heap;
}

// Merge sort implementation
namespace {
void merge(PaperNode** arr, PaperNode** tmp, int left, int mid, int right) {
  int i = left;
  int j = mid + 1;
  int k = left;

  while (i <= mid && j <= right) {
    if (arr[i]->data.citation_count >= arr[j]->data.citation_count) {
      tmp[k++] = arr[i++];
    } else {
      tmp[k++] = arr[j++];
    }
  }

  while (i <= mid) {
    tmp[k++] = arr[i++];
  }
  while (j <= right) {
    tmp[k++] = arr[j++];
  }

  for (i = left; i <= right; ++i) {
    arr[i] = tmp[i];
  }
}
} // namespace

void mergeSort(PaperNode** arr, PaperNode** tmp, int left, int right) {
  if (!arr || !tmp || left >= right) return;
  int mid = left + (right - left) / 2;
  mergeSort(arr, tmp, left, mid);
  mergeSort(arr, tmp, mid + 1, right);
  merge(arr, tmp, left, mid, right);
}
