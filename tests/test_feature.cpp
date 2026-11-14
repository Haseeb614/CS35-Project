#include "feature.h"
#include "list.h"
#include "test_utils.h"

// ---------------------------------------------------------------------------
// Week 10: 5 hand-made records, check the order.
// ---------------------------------------------------------------------------
void testFiveHandMadeRecordsHeap() {
  Paper papers[5] = {
    {1, "Paper A", 2020, 15},
    {2, "Paper B", 2018, 50},
    {3, "Paper C", 2021, 5},
    {4, "Paper D", 2019, 120},
    {5, "Paper E", 2017, 30}
  };

  PaperNode nodes[5];
  for (int i = 0; i < 5; ++i) {
    nodes[i].data = papers[i];
    nodes[i].next = nullptr;
  }

  PaperHeap* heap = heapCreate(5);
  CHECK(heap != nullptr, "Heap creation succeeds");

  for (int i = 0; i < 5; ++i) {
    bool ok = heapInsert(heap, &nodes[i]);
    CHECK(ok, "Inserted node into heap");
  }

  CHECK(heapSize(heap) == 5, "Heap size is 5 after insertions");

  // Expected order: 120 (id 4), 50 (id 2), 30 (id 5), 15 (id 1), 5 (id 3)
  PaperNode* m1 = extractMax(heap);
  CHECK(m1 != nullptr && m1->data.id == 4 && m1->data.citation_count == 120, "1st max is 120");

  PaperNode* m2 = extractMax(heap);
  CHECK(m2 != nullptr && m2->data.id == 2 && m2->data.citation_count == 50, "2nd max is 50");

  PaperNode* m3 = extractMax(heap);
  CHECK(m3 != nullptr && m3->data.id == 5 && m3->data.citation_count == 30, "3rd max is 30");

  PaperNode* m4 = extractMax(heap);
  CHECK(m4 != nullptr && m4->data.id == 1 && m4->data.citation_count == 15, "4th max is 15");

  PaperNode* m5 = extractMax(heap);
  CHECK(m5 != nullptr && m5->data.id == 3 && m5->data.citation_count == 5, "5th max is 5");

  CHECK(heapSize(heap) == 0, "Heap is empty after extracting all");
  CHECK(extractMax(heap) == nullptr, "Extracting from empty heap returns nullptr");

  heapFree(heap);
}

void testFiveHandMadeRecordsMergeSort() {
  Paper papers[5] = {
    {1, "Paper A", 2020, 15},
    {2, "Paper B", 2018, 50},
    {3, "Paper C", 2021, 5},
    {4, "Paper D", 2019, 120},
    {5, "Paper E", 2017, 30}
  };

  PaperNode nodes[5];
  PaperNode* ptrs[5];
  PaperNode* tmp[5];
  for (int i = 0; i < 5; ++i) {
    nodes[i].data = papers[i];
    nodes[i].next = nullptr;
    ptrs[i] = &nodes[i];
  }

  mergeSort(ptrs, tmp, 0, 4);

  CHECK(ptrs[0]->data.citation_count == 120, "MergeSort 1st is 120");
  CHECK(ptrs[1]->data.citation_count == 50, "MergeSort 2nd is 50");
  CHECK(ptrs[2]->data.citation_count == 30, "MergeSort 3rd is 30");
  CHECK(ptrs[3]->data.citation_count == 15, "MergeSort 4th is 15");
  CHECK(ptrs[4]->data.citation_count == 5, "MergeSort 5th is 5");
}

// ---------------------------------------------------------------------------
// Week 11: Real List API Integration (walk list, insert nodes, test on hand-built list)
// ---------------------------------------------------------------------------
void testRealListHeapIntegration() {
  PaperList* list = list_create();
  CHECK(list != nullptr, "Created real PaperList");

  Paper p1 = {10, "Intro to AI", 2015, 80};
  Paper p2 = {20, "Deep Learning", 2017, 300};
  Paper p3 = {30, "Transformers", 2020, 1500};
  Paper p4 = {40, "Reinforcement Learning", 2019, 450};

  // Insert nodes using Component A's real list API
  list_insert_front(list, p1);
  list_insert_front(list, p2);
  list_insert_front(list, p3);
  list_insert_front(list, p4);

  CHECK(list_size(list) == 4, "PaperList has 4 elements");

  // Build heap by walking real list
  PaperHeap* heap = buildHeapFromList(list);
  CHECK(heap != nullptr, "Heap successfully built from real PaperList");
  CHECK(heapSize(heap) == 4, "Heap contains all 4 nodes from list");

  // Verify extraction order: 1500 -> 450 -> 300 -> 80
  PaperNode* top1 = extractMax(heap);
  CHECK(top1 != nullptr && top1->data.citation_count == 1500, "1st extraction is Transformers (1500)");

  PaperNode* top2 = extractMax(heap);
  CHECK(top2 != nullptr && top2->data.citation_count == 450, "2nd extraction is RL (450)");

  PaperNode* top3 = extractMax(heap);
  CHECK(top3 != nullptr && top3->data.citation_count == 300, "3rd extraction is DL (300)");

  PaperNode* top4 = extractMax(heap);
  CHECK(top4 != nullptr && top4->data.citation_count == 80, "4th extraction is AI (80)");

  CHECK(heapSize(heap) == 0, "Heap exhausted");

  heapFree(heap);
  list_destroy(list);
}

void testRealListEmptyAndSingleNode() {
  // Empty list
  PaperList* emptyList = list_create();
  PaperHeap* emptyHeap = buildHeapFromList(emptyList);
  CHECK(emptyHeap != nullptr, "buildHeapFromList on empty list returns valid heap");
  CHECK(heapSize(emptyHeap) == 0, "Heap from empty list has size 0");
  CHECK(extractMax(emptyHeap) == nullptr, "Extracting from empty list heap gives nullptr");
  heapFree(emptyHeap);
  list_destroy(emptyList);

  // Single node list
  PaperList* singleList = list_create();
  Paper solo = {1, "Solo Paper", 2022, 42};
  list_insert_front(singleList, solo);

  PaperHeap* singleHeap = buildHeapFromList(singleList);
  CHECK(singleHeap != nullptr, "Single node list heap created");
  CHECK(heapSize(singleHeap) == 1, "Single node heap size is 1");

  PaperNode* node = extractMax(singleHeap);
  CHECK(node != nullptr && node->data.id == 1 && node->data.citation_count == 42, "Extracted solo node correctly");
  CHECK(heapSize(singleHeap) == 0, "Heap empty after extracting single node");

  heapFree(singleHeap);
  list_destroy(singleList);
}

int main() {
  cout << "=== Running Component C (Khizar - Week 10 & 11) Tests ===" << endl;
  testFiveHandMadeRecordsHeap();
  testFiveHandMadeRecordsMergeSort();
  testRealListHeapIntegration();
  testRealListEmptyAndSingleNode();
  TEST_SUMMARY();
}
