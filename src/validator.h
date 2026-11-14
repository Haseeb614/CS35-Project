#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include "list.h"

// Hand-built validator for dataset integrity checks.
// Ensures zero duplicates before insertion and validates graph edge references.
// Zero STL containers used.

// Checks if paper ID already exists in the list (to reject duplicates before insert)
bool isDuplicate(PaperList* list, int id);
bool isDuplicate(PaperNode* head, int id);

// Checks if both source and target papers exist in list, and source != target (no self-citations)
bool edgeIsValid(PaperList* list, int srcId, int tgtId);
bool edgeIsValid(PaperNode* head, int srcId, int tgtId);

// Validates all edges in an edges CSV file against the paper list
// Checks that endpoints exist, weight is between 1 and 10, and no self-citations
// Prints each invalid edge found and returns the total count of invalid edges
int checkEdgesFile(const char* edgesFilePath, PaperList* list);
int checkEdgesFile(const std::string& edgesFilePath, PaperList* list);

#endif // VALIDATOR_H
