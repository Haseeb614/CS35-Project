#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

// Forward declaration of ListNode so validator header doesn't 
// strictly depend on list.h being merged yet
struct ListNode;

// Checks if a record with the given ID already exists in the linked list
// Returns true if duplicate is found, false if ID is unique
bool isDuplicate(ListNode* head, int id);

// Checks if both source and target IDs exist in the linked list
// Returns true if both exist (valid edge), false otherwise
bool edgeIsValid(ListNode* head, int srcId, int tgtId);

// Reads an edges CSV file and validates every edge against the paper list
// Prints an error message for each bad edge found
// Returns the total count of invalid edges
int checkEdgesFile(const std::string& edgesFilePath, ListNode* head);

#endif // VALIDATOR_H
