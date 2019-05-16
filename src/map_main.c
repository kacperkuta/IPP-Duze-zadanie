#include "map.h"
#include "OperationsLibrary.h"
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <stdio.h>

int main() {

  Map* map = newMap();

  realizeAllLines(map);

  deleteMap(map);

  return 0;

}
