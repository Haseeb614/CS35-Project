# Paper list API (Role A — Nouman Shakeel)

Locked decisions for hash table (Aarish), graph (Ahsan), ranking (Khizar), and loader teammates. Copy `src/record.h`, `src/list.h`, `src/list.cpp` into the team repo when GitHub access works.

## `Paper` (metadata only)

```cpp
struct Paper {
  int id;                 // unique paper id (int); uniqueness enforced by validator, not list
  char title[256];        // fixed C string; max 255 chars + '\0'
  int year;
  int citation_count;     // inbound citation count for ranking; graph owns edges
};
```

| Field | Rule |
|---|---|
| `id` | `int`. List does **not** reject duplicate IDs. |
| `title` | `char[256]`. Callers must null-terminate. Titles longer than 255 chars must be truncated by the loader before insert. **No `std::string`.** |
| `year` | Publication year as `int`. |
| `citation_count` | Inbound count for Khizar’s ranking. **Not** an adjacency list. Ahsan’s graph owns citation edges. |

`Paper` holds **no** outgoing cite pointers/arrays. Edges live only in the graph layer.

## List contract (singly linked, raw pointers)

Hand-built singly linked list. **No STL containers** (`vector`, `map`, `list`, `string`, etc.).

| Op | Behavior |
|---|---|
| Create / destroy | `list_create` allocates empty list; `list_destroy` frees all nodes then the list. |
| Insert | **`list_insert_front`** — O(1) prepend. Always inserts; does not check IDs. |
| Find | `list_find` — first node with matching `id`, or `nullptr`. |
| Remove | `list_remove` — removes first match by `id`; returns `true` if removed. |
| Size | `list_size` — node count. |
| Clear | `list_clear` — frees all nodes; list reusable empty. |
| Access | `list_head` — first node (or `nullptr`); `list_foreach` — walk with callback. |

### Duplicate-ID policy

**List layer allows duplicates.** Insert always prepends. Find/remove use the **first** match. **Aarish’s validator must reject duplicate IDs before insert.** Cleaner for a student project than burying uniqueness in the list.

### Ownership

- List owns node memory (`PaperNode` + next pointers).
- `Paper` values are **copied** into nodes on insert.
- Destructor path: `list_destroy` / `list_clear` free every node. Callers must not use dangling node pointers after remove/clear/destroy.

### Intended consumers

- **Loader**: build `Paper`, call `list_insert_front`.
- **Hash table**: `list_find` / walk via `list_foreach` or `list_head` + `next`.
- **Ranking**: read `citation_count` on papers found via list or hash.
- **Graph**: uses paper `id`s only; does not store edges on `Paper`.

## Header surface (`list.h`)

```cpp
struct PaperNode {
  Paper data;
  PaperNode* next;
};

struct PaperList {
  PaperNode* head;
  int count;
};

PaperList* list_create();
void       list_destroy(PaperList* list);
void       list_clear(PaperList* list);
bool       list_insert_front(PaperList* list, const Paper& paper);
PaperNode* list_find(PaperList* list, int id);
bool       list_remove(PaperList* list, int id);
int        list_size(const PaperList* list);
PaperNode* list_head(PaperList* list);
void       list_foreach(PaperList* list, void (*fn)(Paper&, void*), void* ctx);
```

C++17. Include `record.h` for `Paper`.
