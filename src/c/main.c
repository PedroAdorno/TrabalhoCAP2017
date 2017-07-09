#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define qtTeams 16
#define abbrLength 4


typedef struct {
  char name[abbrLength];
  int status;
  int wins;
} Team;

void InitializeTeams(Team team[], int _abbrLength, char array[][_abbrLength]);
int ArrayContains(int x, int array[], int length);
void ShuffleArray(int array[], int length);
void InitializeIntArray(int array[], int length, int mode);
void PrintIntArray(int array[], int length);
void OrganizeMatches(int lenMatches, int matches[], int lenTeams, Team teams[]);

int main() {

  int i = 0;
  Team teams[qtTeams];
  int results[qtTeams], bets[qtTeams], matches[qtTeams];
  char teamNames[qtTeams][abbrLength] = {"TSM", "RNG", "SSG", "IMO", "FLW", "FNC", "TG2", "AHQ", "SLG", "PNG", "KBM", "KTR", "LIN", "RSS", "C9T", "TLK"};


  InitializeTeams(teams, abbrLength, teamNames);
  InitializeIntArray(results, qtTeams, 0);
  InitializeIntArray(bets, qtTeams, 0);
  InitializeIntArray(matches, qtTeams, 1);

  ShuffleArray(matches, qtTeams);

  OrganizeMatches(qtTeams, matches, qtTeams, teams);


  return 0;
}


//Initialize the array containing all Team structs, setting each team's name, status to 0(lost) and wins count(also 0).
void InitializeTeams(Team* _teams, int columns, char _teamNames[][columns]) {
  for(int i = 0; i < qtTeams; i++) {
    strcpy(_teams[i].name, _teamNames[i]);
    _teams[i].status = 0;
    _teams[i].wins = 0;
  }

}

void InitializeIntArray(int _array[], int length, int mode) {
  switch (mode) {
    case -1:
      for(int i = 0; i < length; i++) {
        _array[i] = -1;
      }
      break;
    case 0:
      for(int i = 0; i < length; i++) {
        _array[i] = 0;
      }
      break;
    case 1:
      for(int i = 0; i < length; i++) {
        _array[i] = i;
      }
      break;
    default: {
      for(int i = 0; i < length; i++) {
        _array[i] = 0;
      }
    }
  }
}

int ArrayContains(int x, int _array[], int length) {
  int contains = 0;

  for(int i = 0; i < length; i++) {
    if(_array[i] == x) {
      contains += 1;
    }
  }

  return contains;
}

//Shuffle the given array using the Durstendfeld's version of the Fisher-Yates shuffling algorithm.
void ShuffleArray(int _array[], int length) {

  int i = 0, j = 0, usedValues[length], n, temp;

  InitializeIntArray(usedValues, length, -1);

  srand(time(NULL));

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
  for(int i = 0; i < length; i++) {
    printf("%d ", _array[i]);
  }
  printf("\n");
}

void OrganizeMatches(int lenMatches, int _matches[], int lenTeams, Team _teams[]) {
  int i = 0;
  for(i = 0; i < lenMatches; i+=2) {
    _teams[_matches[i]].status = rand()%2;
    _teams[_matches[i+1]].status = !_teams[_matches[i]].status;
    printf("%s X %s : %d X %d\n", &_teams[_matches[i]].name, &_teams[_matches[i+1]].name, _teams[_matches[i]].status, _teams[_matches[i+1]].status);
  }
  system("PAUSE");
}
