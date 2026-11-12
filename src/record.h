#ifndef RECORD_H
#define RECORD_H

// Paper metadata for the citation network.
// Citation edges are owned by the graph layer (Ahsan), not by Paper.
// title holds at most 255 characters plus a terminating '\0'.

struct Paper {
  int id;
  char title[256];
  int year;
  int citation_count;  // inbound count for ranking; graph owns edges
};

#endif  // RECORD_H
