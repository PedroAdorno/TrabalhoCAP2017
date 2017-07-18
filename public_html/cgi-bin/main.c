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
int GetFromSave(int _userId, char mode);
void InitializeTeams(Team team[], int _abbrLength, char array[][_abbrLength]);
int ArrayContains(int x, int array[], int length);
void ShuffleStructArray(Team array[], int length);
void InitializeIntArray(int array[], int length, int mode);
void PrintTeamsTable(int _userID, Team _teams[qtTeams], int _round);
void OrganizeMatches(int _userID, Team _teams[qtTeams], int _bets[qtTeams], int shouldPrint);
void StartHTML(int _userId, char mode);
void EndHTML(int _userId, int _cash);
void GreedyHTML();
void InvalidInputHTML();
void FinishHTML(int _userId, char winner[abbrLength]);

int main() {

  int i = 0, j = 0, _round = 0, cash = 0, userId, bets[qtTeams], sum = 0, winner = 0, validInputs = 0;
  Team teams[qtTeams];
  char teamNames[qtTeams][abbrLength] = {"TSM", "RNG", "SSG", "IMO", "FLW", "FNC", "TG2", "AHQ", "SLG", "PNG", "KBM", "KTR", "LIN", "RSS", "C9T", "TLK"};
  char *data = NULL, filename[11];
  Round level;

  srand(time(NULL));

  data = getenv("QUERY_STRING");

  validInputs = sscanf(data, "userId=%d&Bet0=%d&Bet1=%d&Bet2=%d&Bet3=%d&Bet4=%d&Bet5=%d&Bet6=%d&Bet7=%d&Bet8=%d&Bet9=%d&Bet10=%d&Bet11=%d&Bet12=%d&Bet13=%d&Bet14=%d&Bet15=%d", &userId, &bets[0], &bets[1], &bets[2], &bets[3], &bets[4], &bets[5], &bets[6], &bets[7], &bets[8], &bets[9], &bets[10], &bets[11], &bets[12], &bets[13], &bets[14], &bets[15]);

  // if(data == NULL || validInputs != qtTeams+1) {
  //   printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);
  //   printf("<!DOCTYPE html>");
  //   printf("<html lang=\"pt-BR\">");
  //   printf("<body>");
  //   printf("<h2> Numero de entradas inválido.<br>Certifique-se de que nenhum campo está vazio.</h2>");
  //   printf("</body>");
  //   printf("</html>");
  //   return 0;
  // }

  if(!CheckSaveFile(userId)) {
    InitializeTeams(teams, abbrLength, teamNames);
    InitializeIntArray(bets, qtTeams, 0);
    ShuffleStructArray(teams, qtTeams);
    UpdateSaveFile(userId, 0, teams, bets, 1000);
  }



  sprintf(filename, "%d", userId);
  strcat(filename, ".bin");

  FILE *save = fopen(filename, "rb");

  fread(&level, sizeof(Round), 1, save);
  fclose(save);
  save = NULL;


  for(i = 0; i < qtTeams; i++) {
    sum += bets[i];
    if(level.teams[winner].wins < level.teams[i].wins) {
      winner = i;
    }
  }

  cash = GetFromSave(userId, 'c');
  _round = GetFromSave(userId, 'r');
  int maxRounds = 4;

  if(sum <= cash) {
    if((_round < maxRounds) && (cash > 0)) {
      StartHTML(userId, 'n');
      OrganizeMatches(userId, level.teams, bets, 1);
      EndHTML(userId, GetFromSave(userId, 'c'));
    } else {
      OrganizeMatches(userId, level.teams, bets, 0);
      FinishHTML(userId, level.teams[winner].name);
    }
  } else {
    if(data != NULL && validInputs == qtTeams+1) {
      StartHTML(userId, 'g');
    } else {
      StartHTML(userId, 'i');
    }
    PrintTeamsTable(userId, level.teams, _round-1);
    EndHTML(userId, GetFromSave(userId, 'c'));
  }



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

int GetFromSave(int _userId, char mode) {
  char filename[11];
  Round _level;

  sprintf(filename, "%d", _userId);
  strcat(filename, ".bin");

  FILE *file = fopen(filename, "rb");

  fread(&_level, sizeof(Round), 1, file);

  fclose(file);
  file = NULL;

  switch(mode) {
    case 'c':
      return _level.cash;
      break;
    case 'r':
      return _level._round;
      break;
      default:
        return 0;
        break;
  }

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

void PrintTeamsTable(int _userId, Team _teams[qtTeams], int _round) {
  int k = 0, isFirstTeam = 1, cash;

  cash = GetFromSave(_userId, 'c');

  for(k = 0; k < qtTeams; k++) {
      if(_teams[k].wins == _round) {
        if(isFirstTeam) {
          printf("<tr> <th> <label>Sua aposta: <input name=\"Bet%d\" type=\"number\" placeholder=\"0-%d\"  min=\"0\" max=\"%d\" step=\"1\" value=\"0\" class=\"numberInput\"></label> </th> <th> %s  ", k, cash, cash, _teams[k].name);
          isFirstTeam = !isFirstTeam;
        } else {
          printf("X  %s </th> <th><label>Sua aposta: <input name=\"Bet%d\" type=\"number\" placeholder=\"0-%d\" min=\"0\" max=\"%d\" step=\"1\" value=\"0\" class=\"numberInput\"> </th> </tr>", _teams[k].name, k, cash, cash);
          isFirstTeam = !isFirstTeam;
        }
      } else {
        printf("<input name=\"Bet%d\" type=\"hidden\" value=\"0\" />", k);
      }
    }
}

void OrganizeMatches(int _userId, Team _teams[qtTeams], int _bets[qtTeams], int shouldPrint) {
  int i = 0, j = 0, k= 0, randBoolean, _round, cash;
  Round level;

  _round = GetFromSave(_userId, 'r');
  cash = GetFromSave(_userId, 'c');

  if(_round == 0) {
      PrintTeamsTable(_userId, _teams, _round);
  } else {
     for(i = 0; i < qtTeams; i+=Power(2, _round)) {
      randBoolean = rand()%2;
      for(j = 0; j < Power(2, _round); j++) {
        if(_teams[i+j].wins == _round-1) {
          _teams[i+j].wins += randBoolean;
          randBoolean = !randBoolean;
        }
      }
    }
    for(k = 0; k < qtTeams; k++) {
      cash -= _bets[k]*Power(-1, _teams[k].wins == _round);
    }
    UpdateSaveFile(_userId, _round, _teams, _bets, cash);
    if(shouldPrint) {
      PrintTeamsTable(_userId, _teams, _round);
    }
  }

    _round++;
    UpdateSaveFile(_userId, _round, _teams, _bets, cash);
}

void StartHTML(int _userId, char mode) {
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
  if(mode == 'g') {
    GreedyHTML();
  } else if(mode == 'i'){
    InvalidInputHTML();
  }
  printf("<form method=\"GET\">");
  printf("<input name=\"userId\" type=\"hidden\" value=\"%d\"/>", _userId);
  printf("<table border=\"1\">");
  printf("<thead>");
  printf("<tr><th>Valor R$</th><th>Time X Time</th><th>Valor R$</th></tr>");
  printf("</thead>");
  printf("<tbody>");
}

void EndHTML(int _userId, int _cash) {
  printf("</tbody>");
  printf("<tr><th>Round %d</th><th></th><th>Fundos: R$ %d</th></tr>", GetFromSave(_userId, 'r'), GetFromSave(_userId, 'c'));
  printf("</table>");
  printf("<div id=\"botao\">");
  if(_cash) {
    printf("<input type=\"submit\" value=\"GO!\" class=\"botaoEnviar\" />");
  } else {
    printf("<input type=\"submit\" value=\"Aceito a derrota\" class=\"botaoEnviar\" />");
  }
  printf("</div>");
  printf("</form>");
  printf("<div id=\"warning\">");
  printf("<p>Nenhum campo deve estar vazio. Caso não queira apostar<br>em algum time, preencha seu campo com um zero.</p>");
  printf("</div>");
  printf("</body>");
  printf("</html>");
}

void GreedyHTML() {
  printf("<div id=\"wboxg\">");
  printf("<p> <strong>Você está tentando apostar mais do que possui!</strong></p>");
  printf("</div>");
  printf("GREEDY");
}

void InvalidInputHTML() {
  printf("<div id=\"wboxi\">");
  printf("<p> <strong>Numero de entradas inválido.<br>Certifique-se de que nenhum campo está vazio.</strong></p>");
  printf("</div>");
}

void FinishHTML(int _userId, char winner[4]) {
  int cash = GetFromSave(_userId, 'c');
  int _round = GetFromSave(_userId, 'r');
  printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);
  printf("<html lang=\"pt-BR\">");
  printf("<!DOCTYPE html>");
  printf("<html>");
  printf("<head>");
  printf("<title>End</title>");
  printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"/~743586/end.css\">");
  printf("</head>");
  printf("<body>");
  printf("<img src=\"http://blackmarket.rotran.io/images/LOL_LOGOSmallTransparent.png\">");
  if(_round >= (log(qtTeams)/log(2))) {
    printf("<h2>O campeonato chegou ao seu fim, com o time vitorioso sendo %s!<br> Muito obrigado pela sua presença.</h2>", winner);
  } else {
    printf("<h2>Ora ora, alguém aqui perdeu todo seu dinheiro para apostas...<br> Muito obrigado pela sua presença.</h2>");
  }
  if(cash >= 1000) {
    printf("<h3>Seu lucro foi de incríveis %d reais! Mim çina!</h3>", cash-1000);
  } else {
    printf("<h3>Parabéns! Seu lucro foi de %d reais!..... Negativos.. :(</h3>", 1000-cash);
  }
  printf("<h1>Sua pontuação final foi de R$ %d</h4>", cash);
  printf("<img src=\"http://mypst.com.br/media/game/NPWR02631_00/full-icon.png\">");
  printf("</body>");
  printf("</html>");
}
