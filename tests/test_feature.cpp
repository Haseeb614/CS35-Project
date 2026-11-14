#include "feature.h"
#include "list.h"
#include "test_utils.h"

// Week 10: 5 hand-made records, check the order.
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

int main() {
  cout << "--- Running Component C (Week 10) Tests ---" << endl;
  testFiveHandMadeRecordsHeap();
  testFiveHandMadeRecordsMergeSort();
  TEST_SUMMARY();
}
