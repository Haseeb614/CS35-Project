#include "list.h"

#include <cstring>
#include <new>

namespace {

void copy_paper(Paper& dest, const Paper& src) {
  dest.id = src.id;
  dest.year = src.year;
  dest.citation_count = src.citation_count;
  std::strncpy(dest.title, src.title, sizeof(dest.title) - 1);
  dest.title[sizeof(dest.title) - 1] = '\0';
}

}  // namespace

PaperList* list_create() {
  PaperList* list = new (std::nothrow) PaperList;
  if (list == nullptr) {
    return nullptr;
  }
  list->head = nullptr;
  list->count = 0;
  return list;
}

void list_clear(PaperList* list) {
  if (list == nullptr) {
    return;
  }
  PaperNode* cur = list->head;
  while (cur != nullptr) {
    PaperNode* next = cur->next;
    delete cur;
    cur = next;
  }
  list->head = nullptr;
  list->count = 0;
}

void list_destroy(PaperList* list) {
  if (list == nullptr) {
    return;
  }
  list_clear(list);
  delete list;
}

bool list_insert_front(PaperList* list, const Paper& paper) {
  if (list == nullptr) {
    return false;
  }
  PaperNode* node = new (std::nothrow) PaperNode;
  if (node == nullptr) {
    return false;
  }
  copy_paper(node->data, paper);
  node->next = list->head;
  list->head = node;
  list->count += 1;
  return true;
}

PaperNode* list_find(PaperList* list, int id) {
  if (list == nullptr) {
    return nullptr;
  }
  for (PaperNode* cur = list->head; cur != nullptr; cur = cur->next) {
    if (cur->data.id == id) {
      return cur;
    }
  }
  return nullptr;
}

bool list_remove(PaperList* list, int id) {
  if (list == nullptr || list->head == nullptr) {
    return false;
  }

  PaperNode* cur = list->head;
  PaperNode* prev = nullptr;
  while (cur != nullptr) {
    if (cur->data.id == id) {
      if (prev == nullptr) {
        list->head = cur->next;
      } else {
        prev->next = cur->next;
      }
      delete cur;
      list->count -= 1;
      return true;
    }
    prev = cur;
    cur = cur->next;
  }
  return false;
}

int list_size(const PaperList* list) {
  if (list == nullptr) {
    return 0;
  }
  return list->count;
}

PaperNode* list_head(PaperList* list) {
  if (list == nullptr) {
    return nullptr;
  }
  return list->head;
}

void list_foreach(PaperList* list, void (*fn)(Paper&, void*), void* ctx) {
  if (list == nullptr || fn == nullptr) {
    return;
  }
  for (PaperNode* cur = list->head; cur != nullptr; cur = cur->next) {
    fn(cur->data, ctx);
  }
}
