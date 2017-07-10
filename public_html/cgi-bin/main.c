#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define qtTeams 16
#define abbrLength 4


typedef struct {
  char name[abbrLength];
  int status;
  int wins;
} Team;

int Power(int, int);
void InitializeTeams(Team team[], int _abbrLength, char array[][_abbrLength]);
int ArrayContains(int x, int array[], int length);
void ShuffleStructArray(Team array[], int length);
void InitializeIntArray(int array[], int length, int mode);
void PrintIntArray(int array[], int length);
void OrganizeMatches(int lenTeams, Team teams[], int round);
void WriteHTML();

int main() {

  int i = 0;
  Team teams[qtTeams];
  int bets[qtTeams];
  char teamNames[qtTeams][abbrLength] = {"TSM", "RNG", "SSG", "IMO", "FLW", "FNC", "TG2", "AHQ", "SLG", "PNG", "KBM", "KTR", "LIN", "RSS", "C9T", "TLK"};

  srand(time(NULL));

  InitializeTeams(teams, abbrLength, teamNames);
  InitializeIntArray(bets, qtTeams, 0);

  ShuffleStructArray(teams, qtTeams);

  OrganizeMatches(qtTeams, teams, 0);
  WriteHTML();


  return 0;
}

int Power(int a, int x) {
  int i = 0, result = 1;

  for(i = 0; i < x; i++) {
    result *= a;
  }
  return result;
}


//Initialize the array containing all Team structs, setting each team's name and wins count(also 0).
void InitializeTeams(Team* _teams, int columns, char _teamNames[][columns]) {
  int i = 0;
  for(i = 0; i < qtTeams; i++) {
    strcpy(_teams[i].name, _teamNames[i]);
    _teams[i].wins = 0;
  }

}

void InitializeIntArray(int _array[], int length, int mode) {
  int i = 0;
  switch (mode) {
    case -1:
      for(i = 0; i < length; i++) {
        _array[i] = -1;
      }
      break;
    case 0:
      for(i = 0; i < length; i++) {
        _array[i] = 0;
      }
      break;
    case 1:
      for(i = 0; i < length; i++) {
        _array[i] = i;
      }
      break;
    default: {
      for(i = 0; i < length; i++) {
        _array[i] = 0;
      }
    }
  }
}

int ArrayContains(int x, int _array[], int length) {
  int i = 0, contains = 0;

  for(i = 0; i < length; i++) {
    if(_array[i] == x) {
      contains += 1;
    }
  }

  return contains;
}

//Shuffle the given array using the Durstendfeld's version of the Fisher-Yates shuffling algorithm.
void ShuffleStructArray(Team _array[], int length) {

  int i = 0, j = 0, usedValues[length], n;
  Team temp;

  InitializeIntArray(usedValues, length, -1);

  for(i = length; i > 0; i--) {
    n = rand()%(i+1);
    usedValues[i] = n;

    if(ArrayContains(n, usedValues, length) == 1) {
      temp = _array[n];
      _array[n] = _array[i];
      _array[i] = temp;
    }

  }

}

void PrintIntArray(int _array[], int length) {
  int i = 0;
  for(i = 0; i < length; i++) {
    //printf("%d ", _array[i]);
  }
  //printf("\n");
}

void OrganizeMatches(int lenTeams, Team _teams[], int _round) {
  int i = 0, j = 0, k= 0, randBoolean, isFirstTeam = 1;

  //printf("Round %d\n", _round+1);

  for(k = 0; k < lenTeams; k++) {
    if(_round == 0) {
      //printf("%d %s X %s %d\n", _teams[k].wins, _teams[k].name, _teams[k+1].name, _teams[k+1].wins);
      k++;
    } else {
      if(_teams[k].wins == _round) {
        if(isFirstTeam) {
          //printf("%d %s X ", _teams[k].wins, _teams[k].name);
          isFirstTeam = !isFirstTeam;
        } else {
          //printf("%s %d\n", _teams[k].name, _teams[k].wins);
          isFirstTeam = !isFirstTeam;
        }
      }
    }
  }


    for(i = 0; i < lenTeams; i+=Power(2, _round+1)) {
      randBoolean = rand()%2;
      for(j = 0; j < Power(2, _round+1); j++) {
        if(_teams[i+j].wins == _round) {
          _teams[i+j].wins += randBoolean;
          randBoolean = !randBoolean;
        }
      }
    }
    //printf("\n\n");


    _round++;
    system("PAUSE");

}

void WriteHTML() {
  printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);

 printf("<!DOCTYPE html>");

 printf("<html lang=\"en\">");

 printf("<head>");

 printf("<meta charset=\"utf-8\">");
 printf("<title>Sum</title>");

 printf("</head>");

 printf("<body>");
 printf("<!-- page content -->");
 printf("%Hello World");

 printf("</body>");

 printf("</html>");
}
