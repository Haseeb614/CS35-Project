# Build Nouman’s list tests only

From the store root (or after copying `src/` and `tests/` into the team repo):

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O0 -g \
  -I src tests/test_list.cpp src/list.cpp -o test_list && ./test_list
```

No team Makefile here — that is Haseeb’s job.
