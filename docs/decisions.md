# Design Decisions – Component C (Smart Feature / Ranking)

**Author:** Khizar (Role C)  
**Branch:** `c-feature`

---

## 1. Domain
- **Domain B:** Citation Network
- **Entities:** Academic papers and directed citation links.
- **Attributes:** `id`, `title`, `year`, `citations` (`citation_count`).

---

## 2. Track & Ranked Field
- **Track Choice:** Max-Heap (Primary Track) with Merge-Sort on pointer array (Secondary / Verification Track).
- **Ranked Field:** `citations` (`citation_count` in `struct Paper`).
  - Higher citation count indicates greater scientific impact, ranking higher.

---

## 3. Data Structure & Algorithmic Trade-offs
- **Max-Heap:**
  - Maintains heap property using citation count.
  - Array of pointers (`PaperNode**`) avoiding deep copying of paper records.
  - Insert: $O(\log n)$
  - Extract-Max: $O(\log n)$
  - Building Top-$K$: $O(n + k \log n)$
- **Merge-Sort Alternative:**
  - Stable $O(n \log n)$ sort on array of node pointers.
  - Provides deterministic ranking order for ties.

---

## 4. Integration Plan
- Built on top of Paper singly-linked list (`PaperList` / `PaperNode`).
- Walk the linked list nodes and insert their references into the heap.
- Zero STL containers used in compliance with project requirements.
