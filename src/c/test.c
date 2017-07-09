#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int rows = 3, columns = 4;

void Imprima(int, char haduken[][columns]);

int main() {
  char array[rows][columns];

  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < columns; j++) {
      array[i][j] = i+j;
    }
  }
  Imprima(columns, array);

}

void Imprima(int _columns, char _array[][_columns]) {
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < _columns; j++) {
      printf("%s ", _array[i]);
    }
  }
}
