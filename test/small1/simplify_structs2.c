#include "testharness.h"

struct list {
  struct list* back;
  int i;
  struct list* next;
};

#define NULL 0
struct list node1 = {NULL, 29, NULL};
struct list node2 = {&node1, 30, &node1};

void foo() {
  struct list local = node2;
  if (local.next == 0 || local.i != 30) E(1);
  
  //Test that this is an "atomic" operation.
  //If we naively split this into two assignments, 
  // local.next will change and *(local.next) will get the wrong value for
  // the second half of the assigment.
  local = *(local.next);
  if (local.next != 0 || local.i != 29) E(2);

  //Likewise, check that writing local.back (the first field of the struct)
  //doesn't cause problems.
  local = node2;
  local = *(local.back);
  if (local.next != 0 || local.i != 29) E(3);

}

int main() {
  foo();
  SUCCESS;
}
