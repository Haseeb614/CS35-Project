#include "test_utils.h"
#include "validator.h"
#include "list.h"
#include "record.h"

#include <iostream>
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

}  // namespace

int main() {
    // ==========================================
    // Setup: Populate a PaperList for testing
    // ==========================================
    PaperList* list = list_create();
    CHECK(list != nullptr, "Created list successfully");

    CHECK(list_insert_front(list, make_paper(101, "Paper 101", 2020, 15)), "Inserted paper 101");
    CHECK(list_insert_front(list, make_paper(102, "Paper 102", 2021, 8)), "Inserted paper 102");
    CHECK(list_insert_front(list, make_paper(103, "Paper 103", 2019, 23)), "Inserted paper 103");
    CHECK(list_insert_front(list, make_paper(104, "Paper 104", 2022, 4)), "Inserted paper 104");
    CHECK(list_insert_front(list, make_paper(105, "Paper 105", 2023, 10)), "Inserted paper 105");

    // ==========================================
    // Scenario 1: Duplicate ID Detection (isDuplicate)
    // ==========================================
    CHECK(isDuplicate(list, 101), "Finds duplicate ID 101");
    CHECK(isDuplicate(list, 102), "Finds duplicate ID 102");
    CHECK(isDuplicate(list, 103), "Finds duplicate ID 103");
    CHECK(isDuplicate(list, 104), "Finds duplicate ID 104");
    CHECK(isDuplicate(list, 105), "Finds duplicate ID 105");

    // Non-existent IDs
    CHECK(!isDuplicate(list, 999), "Non-existent ID 999 is not duplicate");
    CHECK(!isDuplicate(list, 0), "ID 0 is not duplicate");
    CHECK(!isDuplicate(list, -5), "Negative ID -5 is not duplicate");

    // Raw node overload
    CHECK(isDuplicate(list->head, 102), "Finds duplicate ID 102 using node overload");
    CHECK(!isDuplicate(list->head, 999), "Non-existent ID 999 returns false using node overload");

    // Null safety
    CHECK(!isDuplicate(static_cast<PaperList*>(nullptr), 101), "Null list safely returns false");
    CHECK(!isDuplicate(static_cast<PaperNode*>(nullptr), 101), "Null head safely returns false");

    // ==========================================
    // Scenario 2: Edge Validity Checks (edgeIsValid)
    // ==========================================
    // Valid edges between existing papers
    CHECK(edgeIsValid(list, 101, 102), "Valid edge 101 -> 102");
    CHECK(edgeIsValid(list, 105, 101), "Valid edge 105 -> 101");
    CHECK(edgeIsValid(list, 102, 105), "Valid edge 102 -> 105");
    CHECK(edgeIsValid(list, 103, 104), "Valid edge 103 -> 104");

    // Invalid edges: missing source or target
    CHECK(!edgeIsValid(list, 999, 101), "Rejects missing source 999");
    CHECK(!edgeIsValid(list, 101, 888), "Rejects missing target 888");
    CHECK(!edgeIsValid(list, 999, 888), "Rejects both missing source and target");

    // Invalid edges: self citation
    CHECK(!edgeIsValid(list, 102, 102), "Rejects self-citation 102 -> 102");
    CHECK(!edgeIsValid(list, 101, 101), "Rejects self-citation 101 -> 101");

    // Invalid edges: non-positive IDs
    CHECK(!edgeIsValid(list, -1, 102), "Rejects negative source ID");
    CHECK(!edgeIsValid(list, 101, -5), "Rejects negative target ID");
    CHECK(!edgeIsValid(list, 0, 102), "Rejects zero source ID");
    CHECK(!edgeIsValid(list, 101, 0), "Rejects zero target ID");

    // Raw node overload
    CHECK(edgeIsValid(list->head, 101, 102), "Valid edge 101 -> 102 using node overload");
    CHECK(!edgeIsValid(list->head, 102, 102), "Rejects self-citation using node overload");
    CHECK(!edgeIsValid(list->head, 999, 101), "Rejects missing source using node overload");

    // Null safety
    CHECK(!edgeIsValid(static_cast<PaperList*>(nullptr), 101, 102), "Null list safely returns false");
    CHECK(!edgeIsValid(static_cast<PaperNode*>(nullptr), 101, 102), "Null head safely returns false");

    // ==========================================
    // Scenario 3: File-Based Edge Validation (checkEdgesFile)
    // ==========================================
    // data/bad_edge.csv has 7 edges total:
    // 102,101,8  -> valid
    // 999,101,5  -> invalid (source missing)
    // 101,888,4  -> invalid (target missing)
    // 102,102,6  -> invalid (self citation)
    // 105,102,3  -> valid
    // 103,104,0  -> invalid (weight 0 out of bounds [1-10])
    // 105,101,15 -> invalid (weight 15 out of bounds [1-10])
    // Expected bad count: 5
    int badCount = checkEdgesFile("data/bad_edge.csv", list);
    CHECK(badCount == 5, "Detected exactly 5 invalid edges in bad_edge.csv");

    // std::string overload
    std::string badEdgePath = "data/bad_edge.csv";
    CHECK(checkEdgesFile(badEdgePath, list) == 5, "std::string overload works identically");

    // Non-existent file
    CHECK(checkEdgesFile("data/does_not_exist.csv", list) == -1, "Returns -1 for non-existent file");

    // Null path
    CHECK(checkEdgesFile(static_cast<const char*>(nullptr), list) == -1, "Returns -1 for nullptr path");

    // ==========================================
    // Cleanup
    // ==========================================
    list_destroy(list);

    // Summary and return
    TEST_SUMMARY();
}
