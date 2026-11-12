#include "list.h"

#include <cassert>
#include <cstdio>
#include <cstring>

namespace {

Paper make_paper(int id, const char* title, int year, int citation_count) {
  Paper p{};
  p.id = id;
  p.year = year;
  p.citation_count = citation_count;
  std::strncpy(p.title, title, sizeof(p.title) - 1);
  p.title[sizeof(p.title) - 1] = '\0';
  return p;
}

void count_cb(Paper&, void* ctx) {
  int* n = static_cast<int*>(ctx);
  *n += 1;
}

void sum_id_cb(Paper& paper, void* ctx) {
  int* sum = static_cast<int*>(ctx);
  *sum += paper.id;
}

}  // namespace

int main() {
  // create / empty
  PaperList* list = list_create();
  assert(list != nullptr);
  assert(list_size(list) == 0);
  assert(list_head(list) == nullptr);
  assert(list_find(list, 1) == nullptr);
  assert(list_remove(list, 1) == false);

  // insert front (prepend): order becomes 3, 2, 1
  assert(list_insert_front(list, make_paper(1, "Alpha", 2010, 0)));
  assert(list_insert_front(list, make_paper(2, "Beta", 2015, 3)));
  assert(list_insert_front(list, make_paper(3, "Gamma", 2020, 7)));
  assert(list_size(list) == 3);
  assert(list_head(list) != nullptr);
  assert(list_head(list)->data.id == 3);
  assert(list_head(list)->next->data.id == 2);
  assert(list_head(list)->next->next->data.id == 1);
  assert(list_head(list)->next->next->next == nullptr);

  // find + title copy
  PaperNode* found = list_find(list, 2);
  assert(found != nullptr);
  assert(found->data.year == 2015);
  assert(found->data.citation_count == 3);
  assert(std::strcmp(found->data.title, "Beta") == 0);
  assert(list_find(list, 99) == nullptr);

  // foreach
  int walked = 0;
  list_foreach(list, count_cb, &walked);
  assert(walked == 3);
  int id_sum = 0;
  list_foreach(list, sum_id_cb, &id_sum);
  assert(id_sum == 6);

  // remove middle, then head, then last
  assert(list_remove(list, 2) == true);
  assert(list_size(list) == 2);
  assert(list_find(list, 2) == nullptr);
  assert(list_head(list)->data.id == 3);
  assert(list_head(list)->next->data.id == 1);

  assert(list_remove(list, 3) == true);
  assert(list_size(list) == 1);
  assert(list_head(list)->data.id == 1);

  assert(list_remove(list, 1) == true);
  assert(list_size(list) == 0);
  assert(list_head(list) == nullptr);
  assert(list_remove(list, 1) == false);

  // duplicate IDs allowed; find returns first (newest / front)
  assert(list_insert_front(list, make_paper(10, "First", 2000, 1)));
  assert(list_insert_front(list, make_paper(10, "Second", 2001, 2)));
  assert(list_size(list) == 2);
  found = list_find(list, 10);
  assert(found != nullptr);
  assert(std::strcmp(found->data.title, "Second") == 0);
  assert(list_remove(list, 10) == true);  // removes first match only
  assert(list_size(list) == 1);
  found = list_find(list, 10);
  assert(found != nullptr);
  assert(std::strcmp(found->data.title, "First") == 0);

  // title truncation safety via insert copy (255 max usable chars)
  char long_title[300];
  for (int i = 0; i < 299; ++i) {
    long_title[i] = 'A';
  }
  long_title[299] = '\0';
  Paper long_paper = make_paper(42, long_title, 1999, 0);
  assert(list_insert_front(list, long_paper));
  found = list_find(list, 42);
  assert(found != nullptr);
  assert(std::strlen(found->data.title) == 255);
  assert(found->data.title[255] == '\0');

  // clear leaves reusable empty list
  list_clear(list);
  assert(list_size(list) == 0);
  assert(list_head(list) == nullptr);
  assert(list_insert_front(list, make_paper(7, "Reuse", 2024, 0)));
  assert(list_size(list) == 1);

  // null-safe helpers
  list_destroy(nullptr);
  list_clear(nullptr);
  assert(list_size(nullptr) == 0);
  assert(list_find(nullptr, 1) == nullptr);
  assert(list_remove(nullptr, 1) == false);
  assert(list_head(nullptr) == nullptr);
  assert(list_insert_front(nullptr, make_paper(1, "x", 1, 0)) == false);

  list_destroy(list);

  std::printf("test_list: all assertions passed\n");
  return 0;
}
