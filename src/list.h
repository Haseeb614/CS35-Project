#ifndef LIST_H
#define LIST_H

#include "record.h"

// Hand-built singly linked list of Paper records.
// No STL containers. Duplicate IDs are allowed here; the validator
// must prevent duplicates before insert. find/remove use first match.
// Insert is always O(1) prepend (list_insert_front).

struct PaperNode {
  Paper data;
  PaperNode* next;
};

struct PaperList {
  PaperNode* head;
  int count;
};

// Allocates an empty list. Returns nullptr on allocation failure.
PaperList* list_create();

// Frees all nodes, then frees the list itself. Safe on nullptr.
void list_destroy(PaperList* list);

// Frees all nodes; leaves list empty and reusable. Safe on nullptr.
void list_clear(PaperList* list);

// Prepends a copy of paper. Returns false on null list or alloc failure.
bool list_insert_front(PaperList* list, const Paper& paper);

// First node with matching id, or nullptr.
PaperNode* list_find(PaperList* list, int id);

// Removes first node with matching id. Returns true if a node was removed.
bool list_remove(PaperList* list, int id);

// Number of nodes (0 if list is nullptr).
int list_size(const PaperList* list);

// Head node, or nullptr.
PaperNode* list_head(PaperList* list);

// Calls fn(paper, ctx) for each paper from head to tail. No-op if list or fn is null.
void list_foreach(PaperList* list, void (*fn)(Paper&, void*), void* ctx);

#endif  // LIST_H
