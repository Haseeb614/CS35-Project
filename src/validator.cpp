#include "validator.h"
#include "list.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Checks if paper ID already exists in the list (to reject duplicates before insert)
bool isDuplicate(PaperList* list, int id) {
    if (list == nullptr) {
        return false;
    }
    return list_find(list, id) != nullptr;
}

bool isDuplicate(PaperNode* head, int id) {
    for (PaperNode* curr = head; curr != nullptr; curr = curr->next) {
        if (curr->data.id == id) {
            return true;
        }
    }
    return false;
}

// Checks if both source and target papers exist in list, and source != target (no self-citations)
bool edgeIsValid(PaperList* list, int srcId, int tgtId) {
    if (list == nullptr) {
        return false;
    }
    if (srcId <= 0 || tgtId <= 0) {
        return false;
    }
    if (srcId == tgtId) {
        return false; // Reject self citations
    }
    return (list_find(list, srcId) != nullptr && list_find(list, tgtId) != nullptr);
}

bool edgeIsValid(PaperNode* head, int srcId, int tgtId) {
    if (srcId <= 0 || tgtId <= 0 || srcId == tgtId) {
        return false;
    }
    bool foundSrc = false;
    bool foundTgt = false;
    for (PaperNode* curr = head; curr != nullptr; curr = curr->next) {
        if (curr->data.id == srcId) {
            foundSrc = true;
        }
        if (curr->data.id == tgtId) {
            foundTgt = true;
        }
        if (foundSrc && foundTgt) {
            return true;
        }
    }
    return false;
}

// Validates all edges in an edges CSV file against the paper list
// Checks that endpoints exist, weight is between 1 and 10, and no self-citations
// Prints each invalid edge found and returns the total count of invalid edges
int checkEdgesFile(const char* edgesFilePath, PaperList* list) {
    if (edgesFilePath == nullptr) {
        return -1;
    }

    std::ifstream file(edgesFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open edges file: " << edgesFilePath << std::endl;
        return -1;
    }

    int badEdgesCount = 0;
    std::string line;
    bool isFirstLine = true;

    auto trim = [](std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            s.clear();
            return;
        }
        size_t end = s.find_last_not_of(" \t\r\n");
        s = s.substr(start, end - start + 1);
    };

    while (std::getline(file, line)) {
        // Strip trailing CR if present (\r\n handling)
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ' || line.back() == '\t')) {
            line.pop_back();
        }

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Header detection: check first line for non-numeric start or "source_id"
        if (isFirstLine) {
            isFirstLine = false;
            if (line.find("source_id") != std::string::npos ||
                (!line.empty() && !std::isdigit(static_cast<unsigned char>(line[0])))) {
                continue;
            }
        }

        std::stringstream ss(line);
        std::string srcStr, tgtStr, weightStr;

        if (!std::getline(ss, srcStr, ',') ||
            !std::getline(ss, tgtStr, ',') ||
            !std::getline(ss, weightStr, ',')) {
            std::cout << "Bad edge: line \"" << line << "\" (malformed CSV format)" << std::endl;
            badEdgesCount++;
            continue;
        }

        trim(srcStr);
        trim(tgtStr);
        trim(weightStr);

        int srcId = 0;
        int tgtId = 0;
        int weight = 0;

        try {
            srcId = std::stoi(srcStr);
            tgtId = std::stoi(tgtStr);
            weight = std::stoi(weightStr);
        } catch (...) {
            std::cout << "Bad edge: line \"" << line << "\" (non-numeric field)" << std::endl;
            badEdgesCount++;
            continue;
        }

        // Validate edge constraints
        if (srcId <= 0 || tgtId <= 0) {
            std::cout << "Bad edge: " << srcId << " -> " << tgtId << " with weight " << weight
                      << " (invalid paper id)" << std::endl;
            badEdgesCount++;
        } else if (srcId == tgtId) {
            std::cout << "Bad edge: " << srcId << " -> " << tgtId << " with weight " << weight
                      << " (self citation)" << std::endl;
            badEdgesCount++;
        } else if (weight < 1 || weight > 10) {
            std::cout << "Bad edge: " << srcId << " -> " << tgtId << " with weight " << weight
                      << " (weight out of bounds [1-10])" << std::endl;
            badEdgesCount++;
        } else {
            bool srcFound = (list != nullptr && list_find(list, srcId) != nullptr);
            bool tgtFound = (list != nullptr && list_find(list, tgtId) != nullptr);

            if (!srcFound && !tgtFound) {
                std::cout << "Bad edge: " << srcId << " -> " << tgtId << " with weight " << weight
                          << " (both source and target papers not found)" << std::endl;
                badEdgesCount++;
            } else if (!srcFound) {
                std::cout << "Bad edge: " << srcId << " -> " << tgtId << " with weight " << weight
                          << " (source paper not found)" << std::endl;
                badEdgesCount++;
            } else if (!tgtFound) {
                std::cout << "Bad edge: " << srcId << " -> " << tgtId << " with weight " << weight
                          << " (target paper not found)" << std::endl;
                badEdgesCount++;
            }
        }
    }

    file.close();
    return badEdgesCount;
}

int checkEdgesFile(const std::string& edgesFilePath, PaperList* list) {
    return checkEdgesFile(edgesFilePath.c_str(), list);
}
