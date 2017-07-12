#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define abbrLength 4


typedef struct {
  char name[abbrLength];
  int status;
  int wins;
} Team;

int Power(int, int);
int ClosestMultipleOf(int, int);
void GenerateTeamNames(char **array, int _qtTeams);
//void InitializeTeams(Team team[], char **array, int qtTeams);
int ArrayContains(int x, int array[], int length);
void ShuffleStructArray(Team array[], int length);
void InitializeIntArray(int array[], int length, int mode);
void PrintIntArray(int array[], int length);
void OrganizeMatches(int lenTeams, Team teams[], int round);
void WriteHTML();

int main() {
  int i=0, qtTeams, *bets;
  Team *teams;
  char **teamNames;

  /*char *pData = NULL;
  char query[512] = {'\n'};

  pData = getenv("QUERY_STRING");

  if(pData != NULL) {
    sscanf(pData, "qt_teams=%d", &qtTeams);
  }*/
  printf("Type the quantity of teams: ");
  scanf("%d", &qtTeams);

  teams = (Team *) calloc(qtTeams, sizeof(Team));
  teamNames = (char **) calloc(qtTeams, sizeof(char));
  bets = (int *) calloc(qtTeams, sizeof(int));

  for(i = 0; i < abbrLength; i++) {
    teamNames[i] = (char *) calloc(abbrLength, sizeof(char));
  }


  srand(time(NULL));

  GenerateTeamNames(teamNames, qtTeams);
  //InitializeTeams(teams, abbrLength, teamNames, qtTeams);
  // InitializeIntArray(bets, qtTeams, 0);
  //
  // ShuffleStructArray(teams, qtTeams);
  //
  // OrganizeMatches(qtTeams, teams, 0);
  //WriteHTML();


  return 0;
}

int Power(int a, int x) {
  int i = 0, result = 1;

  for(i = 0; i < x; i++) {
    result *= a;
  }
  return result;
}

int ClosestMultipleOf(int m, int r) {
  m /= r;
  m*= r;

  return m;
}

int ClosestPowerOf2Above(int x) {
  int n = 0, temp = 0;

  temp = x;

  while(x > 1) {
    x = x >> 1;
    n++;
  }
  if(temp == Power(2, n)) {
    return Power(2, n);
  } else {
    return Power(2, n+1);
  }
}

void GenerateTeamNames(char **_teamNames, int _qtTeams) {
  int i = 0, j = 0;
  char randChar;
  printf("\n%d\n", _qtTeams);

  for(i = 0; i < _qtTeams; i++) {
    printf("a\n");
    for(j = 0; j < abbrLength-1; j++) {
       randChar = 'A';
    //   while(j > 0 && randChar != (int)teamNames[i][j-1]) {
    //     randChar = 65 + rand()%26;
    //   }
        _teamNames[i][j] = (char)randChar;
        printf("Team %d: %s  ", i, _teamNames[i]);
    }
  }
}


//Initialize the array containing all Team structs, setting each team's name and wins count(also 0).
void InitializeTeams(Team *_teams, int columns, char *_teamNames[columns], int qtTeams) {
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
    printf("%d ", _array[i]);
  }
  printf("\n");
}

//Sistema de disputas por eliminação simples.
void OrganizeMatches(int lenTeams, Team _teams[], int _round) {
  int i = 0, j = 0, k= 0, iStart, iCap, jCap, randBoolean, isFirstTeam = 1, tracker = 0, exempt = 0;

  exempt = ClosestPowerOf2Above(lenTeams) - lenTeams;

  printf("Round %d, full tree: \n", _round);

  if(_round == 0) {
    iStart = exempt/2;
    iCap = lenTeams-((exempt/2)+(exempt%2));
  } else {
    iStart = 0;
    iCap = lenTeams;
  }


  for(k = 0; k < lenTeams; k++) {
    if(_teams[k].wins == _round) {
      if(isFirstTeam) {
        printf("%d %s X ", _teams[k].wins, _teams[k].name);
        isFirstTeam = !isFirstTeam;
      } else {
        printf("%s %d\n", _teams[k].name, _teams[k].wins);
        isFirstTeam = !isFirstTeam;
      }
    }
  }

  if(_round == 0) {
    printf("\n\n Round %d, partial tree: \n", _round);

    for(k = iStart; k < iCap; k++) {
      if(_teams[k].wins == _round) {
        if(isFirstTeam) {
          printf("%d %s X ", _teams[k].wins, _teams[k].name);
          isFirstTeam = !isFirstTeam;
        } else {
          printf("%s %d\n", _teams[k].name, _teams[k].wins);
          isFirstTeam = !isFirstTeam;
        }
      }
    }
  }

  printf("\niStart = %d, iCap = %d, exempt = %d", iStart, iCap, exempt);

  for(i = 0; i < lenTeams; i+=Power(2, _round+1)) {
    jCap = Power(2, _round+1)-((lenTeams%Power(2, _round+1))*(i == ClosestMultipleOf(lenTeams, Power(2, _round+1))));
    // printf("i = %d ", i);
    randBoolean = 0;
    for(j = 0; j < jCap; j++) {
      if(_teams[i+j].wins == _round) {
        if(_round == 0) {
          if(i+j < iStart || i+j >= iCap) {
            _teams[i+j].wins += 1;
            //printf("\n%d", i+j);
          } else {
            _teams[i+j].wins += randBoolean;
            randBoolean = !randBoolean;
          }
        } else {
            if(tracker < 1) {
              _teams[i+j].wins += randBoolean;
              randBoolean = !randBoolean;
              tracker++;
            } else {
              tracker = 0;
              _teams[i+j].wins += randBoolean;
              i -= ((i+Power(2, _round+1))-((i+j)+1));
              printf("\n j = %d", j);
              break;
            }
          }
        }
      }
  }
  printf("\n\n");


  _round++;
  system("PAUSE");
  OrganizeMatches(lenTeams, _teams, _round);

}

void WriteHTML() {
  printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);

 printf("<!DOCTYPE html>");

 printf("<html lang=\"en\">");

 printf("<head>");

 printf("<meta charset=\"utf-8\">");
 printf("<title>Apostas LoL</title>");
 printf("<link href=\"home.css\" type=\"text/css\" rel=\"stylesheet\" />");
 printf("<link href=\"https://fonts.googleapis.com/css?family=Dosis\" rel=\"stylesheet\">");

 printf("</head>");

 printf("<body>");



 printf("</body>");

 printf("</html>");
}
