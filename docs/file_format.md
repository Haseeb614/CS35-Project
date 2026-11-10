# Dataset File Formats & Specification

This document defines the schema and validation rules for CSV data files used in the CS-250 Citation Network project.

---

## 1. Papers Dataset (`records.csv` / `records_sample.csv`)

Stores the academic papers loaded into the singly linked list.

### Format
- **Header:** `id,title,year,citations`
- **Delimiter:** Comma (`,`)

### Fields
| Column | Type | Constraints / Rules |
| :--- | :--- | :--- |
| `id` | Integer | Unique identifier for each paper. Cannot be duplicated. Positive integer. |
| `title` | String | Title of the research paper. If title contains a comma, it should be enclosed in double quotes. |
| `year` | Integer | Publication year (e.g., 2010 - 2024). |
| `citations` | Integer | Total citations received. Non-negative integer (`>= 0`). Used by the ranking feature. |

### Example
```csv
id,title,year,citations
101,Deep Residual Learning for Image Recognition,2016,150000
102,Attention Is All You Need,2017,120000
103,Adam: A Method for Stochastic Optimization,2014,140000
104,Generative Adversarial Nets,2014,65000
105,BERT: Pre-training of Deep Bidirectional Transformers,2018,85000
```

---

## 2. Citation Edges Dataset (`edges.csv` / `edges_sample.csv`)

Stores directed citation connections between papers.

### Format
- **Header:** `source_id,target_id,weight`
- **Delimiter:** Comma (`,`)

### Fields
| Column | Type | Constraints / Rules |
| :--- | :--- | :--- |
| `source_id` | Integer | ID of the citing paper (must exist in loaded records). |
| `target_id` | Integer | ID of the paper being cited (must exist in loaded records). |
| `weight` | Integer | Strength / relevance of citation. Allowed integer range: `1` to `10`. |

### Directionality & Rules
- An edge `source_id -> target_id` means **paper `source_id` cites paper `target_id`**.
- **No Self-Citations:** `source_id` should not equal `target_id`.
- **Temporal Validity:** Citing paper year should be greater than or equal to cited paper year (`source.year >= target.year`).
- **Referential Integrity:** If either `source_id` or `target_id` is missing in the papers list, the edge must be rejected by `edgeIsValid()`.

### Example
```csv
source_id,target_id,weight
102,101,8
105,102,9
105,103,4
104,103,6
```

---

## 3. Test & Edge Case Datasets

These datasets are used specifically for automated testing and validation:

1. **`data/bad_duplicate.csv`**:
   - Contains intentionally duplicated paper IDs (e.g. two rows with `id = 101`).
   - Expected behavior: Loader/Validator rejects duplicate lines and continues parsing valid rows.

2. **`data/bad_edge.csv`**:
   - Contains edges where either `source_id` or `target_id` does not exist in `records.csv`, or where `weight` is out of bounds (e.g., `< 1` or `> 10`).
   - Expected behavior: `checkEdgesFile()` reports each bad edge, increments error count, and skips adding them to the graph.

3. **`data/cyclic.csv`**:
   - Small directed cycle (e.g. Paper 1 cites Paper 2, Paper 2 cites Paper 3, Paper 3 cites Paper 1).
   - Expected behavior: Used by Ahsan's graph BFS/DFS to verify that the `visited` array prevents infinite loops.

---

## 4. Parser Guidelines
- The first line is always the header line and should be skipped by the parser.
- Blank lines and trailing empty lines should be ignored safely.
- Parsers must handle both Windows (`\r\n`) and Unix (`\n`) line endings.
