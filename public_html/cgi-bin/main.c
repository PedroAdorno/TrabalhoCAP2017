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

typedef struct {
  int _round;
  Team teams[qtTeams];
  int bets[qtTeams];
  int cash;
} Round;

int Power(int, int);
void UpdateSaveFile(int _userId, int _round, Team _teams[], int _bets[], int _cash);
int CheckSaveFile(int _userId);
FILE *GetSaveFile(int _userId);
int GetCash(int _userId, int _cash);
void InitializeTeams(Team team[], int _abbrLength, char array[][_abbrLength]);
int ArrayContains(int x, int array[], int length);
void ShuffleStructArray(Team array[], int length);
void InitializeIntArray(int array[], int length, int mode);
void PrintIntArray(int array[], int length);
void OrganizeMatches(int _userId);
void StartHTML(int _userId);
void EndHTML();

int main() {

  int i = 0, _round = 0, cash = 0, userId;
  Team teams[qtTeams];
  int bets[qtTeams];
  char teamNames[qtTeams][abbrLength] = {"TSM", "RNG", "SSG", "IMO", "FLW", "FNC", "TG2", "AHQ", "SLG", "PNG", "KBM", "KTR", "LIN", "RSS", "C9T", "TLK"};
  char *data = NULL;
  Round level;

  srand(time(NULL));

  data = getenv("QUERY_STRING");

  if(data != NULL) {
    sscanf(data, "userId=%d&Bet0=%d&Bet1=%d&Bet2=%d&Bet3=%d&Bet4=%d&Bet5=%d&Bet6=%d&Bet7=%d&Bet8=%d&Bet9=%d&Bet10=%d&Bet11=%d&Bet12=%d&Bet13=%d&Bet14=%d&Bet15=%d", &userId, &bets[0], &bets[1], &bets[2], &bets[3], &bets[4], &bets[5], &bets[6], &bets[7], &bets[8], &bets[9], &bets[10], &bets[11], &bets[12], &bets[13], &bets[14], &bets[15]);
  }


  // if(data != NULL) {
  //   sscanf(data, "userId=%s", userId);
  // }

  if(!CheckSaveFile(userId)) {
    InitializeTeams(teams, abbrLength, teamNames);
    InitializeIntArray(bets, qtTeams, 0);
    ShuffleStructArray(teams, qtTeams);
    UpdateSaveFile(userId, 0, teams, bets, 1000);
  }

  StartHTML(userId);
  OrganizeMatches(userId);
  EndHTML();


  return 0;
}

int Power(int a, int x) {
  int i = 0, result = 1;

  for(i = 0; i < x; i++) {
    result *= a;
  }
  return result;
}

int CheckSaveFile(int _userId) {
  char filename[11];
  sprintf(filename, "%d", _userId);
  strcat(filename, ".bin");

  FILE *file = fopen(filename, "rb");

  return file != NULL;
}

void UpdateSaveFile(int _userId, int _round, Team _teams[], int _bets[], int _cash) {
  int i = 0;
  char filename[12];
  Round _level;

  sprintf(filename, "%d", _userId);
  strcat(filename, ".bin");

  FILE *file = fopen(filename, "wb");
  _level._round = _round;
  _level.cash = _cash;

  for(i = 0; i < qtTeams; i++) {
    _level.teams[i] = _teams[i];
    _level.bets[i] = _bets[i];
  }

  fwrite(&_level, sizeof(Round), 1, file);
  fclose(file);
  file = NULL;


}

FILE *GetSaveFile(int _userId) {

  char filename[11];
  sprintf(filename, "%d", _userId);
  strcat(filename, ".bin");

  FILE *file = fopen(filename, "rb");

  return file;
}

int GetCash(int _userId, int _cash) {
  Round _level;

  FILE *file = GetSaveFile(_userId);

  fread(&_level, sizeof(Round), 1, file);

  return _level.cash;
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

void OrganizeMatches(int _userId) {
  int i = 0, j = 0, k= 0, l = 0, randBoolean, isFirstTeam = 1, _round, _bets[qtTeams], cash;
  Round level;
  Team _teams[qtTeams];

  FILE *file = GetSaveFile(_userId);

  fread(&level, sizeof(Round), 1, file);

  _round = level._round;
  cash = level.cash;

  for(l = 0; l < qtTeams; l++) {
    _teams[l] = level.teams[l];
    _bets[l] = level.bets[l];
  }
  fclose(file);
  file = NULL;

  printf("Round %d\n", _round+1);

  for(k = 0; k < qtTeams; k++) {
    if(_round == 0) {
      //printf("%d %s X %s %d\n", _teams[k].wins, _teams[k].name, _teams[k+1].name, _teams[k+1].wins);
      printf("<tr> <th> <label>Sua aposta: <input name=\"Bet%d\" type=\"number\" placeholder=\"0-%d\" min=\"0\" max=\"%d\" step=\"1\" value=\"0\"class=\"numberInput\"></label> </th> <th> %s </th> <th> %s </th> <th><label>Sua aposta: <input name=\"Bet%d\" type=\"number\" placeholder=\"0-%d\" min=\"0\" max=\"%d\" step=\"1\" value=\"0\" class=\"numberInput\"> </th> </tr>", k, cash, cash, _teams[k].name, _teams[k+1].name, k+1, cash, cash);
      k++;
    } else {
      cash += _bets[k]*Power(-1, _teams[k].wins == _round);
      if(_teams[k].wins == _round) {
        if(isFirstTeam) {
          printf("<tr> <th> <label>Sua aposta: <input name=\"Bet%d\" type=\"number\" placeholder=\"0-%d\"  min=\"0\" max=\"%d\" step=\"1\" value=\"0\" class=\"numberInput\"></label> </th> <th> %s </th>", k, cash, cash, _teams[k].name);
          isFirstTeam = !isFirstTeam;
        } else {
          printf(" <th> %s </th> <th><label>Sua aposta: <input name=\"Bet%d\" type=\"number\" placeholder=\"0-%d\" min=\"0\" max=\"%d\" step=\"1\" value=\"0\" class=\"numberInput\"> </th> </tr>", _teams[k].name, k, cash, cash);
          isFirstTeam = !isFirstTeam;
        }
      } else {
        printf("<input name=\"Bet%d\" type=\"hidden\" value=\"0\" />", k);
      }
    }
  }


    for(i = 0; i < qtTeams; i+=Power(2, _round+1)) {
      randBoolean = rand()%2;
      for(j = 0; j < Power(2, _round+1); j++) {
        if(_teams[i+j].wins == _round) {
          _teams[i+j].wins += randBoolean;
          randBoolean = !randBoolean;
        }
      }
    }

    _round++;

    UpdateSaveFile(_userId, _round, _teams, _bets, cash);


    system("PAUSE");

}

void StartHTML(int _userId) {
  printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);

  printf("<!DOCTYPE html>");

  printf("<html lang=\"pt-BR\">");

  printf("<head>");

  printf("<meta charset=\"utf-8\">");
  printf("<title>Game</title>");
  printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"/~743586/game_style.css\">");
  printf("<link href=\"https://fonts.googleapis.com/css?family=Dosis\" rel=\"stylesheet\">");

  printf("</head>");

  printf("<body>");
  printf("<img src=\"https://upload.wikimedia.org/wikipedia/en/6/62/League_of_Legends_Champion_Series_Logo.jpg.png\">");
  printf("%d", GetCash(_userId));
  printf("<form method=\"GET\">");
  printf("<input name=\"userId\" type=\"hidden\" value=\"%d\"/>", _userId);
  printf("<table border=\"1\">");
  printf("<thead>");
  printf("<tr><th>Valor R$</th><th>Time</th><th>Time</th><th>Valor R$</th></tr>");
  printf("</thead>");
  printf("<tbody>");

}

void EndHTML() {
  printf("</tbody>");
  printf("</table>");
  printf("<div id=\"botao\">");
  printf("<input type=\"submit\" value=\"GO!\" class=\"botaoEnviar\" />");
  printf("</div>");
  printf("</form>");

  printf("</body>");

  printf("</html>");
}
