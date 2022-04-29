#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define SET_CURSOR(i, j) printf("\033[%d;%dH", i + 1, j + 1)

int mo;
int cpu = 0;
int cpu_heuristics = 0;
int score = 0;
int disks[2] = {};

int cnd_count;
int **candidate_cells;

int board[8][8] = {{0}};
int cpu_board[8][8] = {{0}};
int val_table[8][8] = {
    {120, -20, 20, 5, 5, 20, -20, 120},   {-20, -40, -5, -5, -5, -5, -40, -20},
    {20, -5, 15, 3, 3, 15, -5, 20},       {5, -5, 3, 3, 3, 3, -5, 5},
    {5, -5, 3, 3, 3, 3, -5, 5},           {20, -5, 15, 3, 3, 15, -5, 20},
    {-20, -40, -5, -5, -5, -5, -40, -20}, {120, -20, 20, 5, 5, 20, -20, 120}};

void init();
int reverse(char, char);
int isPuttable(int);
void select_cell(char *, char *);
void set_board(int, int);
void result();

int main() {
  char r, c;

  init();

  while (isPuttable(0)) {
    SET_CURSOR(2, 1);
    if (cpu == 3) {
      printf("CPU%d", mo);
    } else if (cpu) {
      printf("%s", mo == cpu ? "CPU" : "YOU");
    } else {
      printf("%c", 'A' + mo - 1);
    }

    do {
      SET_CURSOR(3, 8);
      printf("                              \n");
      printf("                              \n\n");
      printf("                              ");
      SET_CURSOR(3, 8);
      if (cpu == 3) {
        select_cell(&r, &c);
      } else if (cpu) {
        if (mo == cpu) {
          select_cell(&r, &c);
        } else {
          c = getchar();
          r = getchar();
          while (getchar() != 10)
            ;
        }
      } else {
        c = getchar();
        r = getchar();
        while (getchar() != 10)
          ;
      }
    } while (reverse(r, c));

    mo = (mo & 1) + 1;
  }

  result();

  return 0;
}

void init() {
  int i, j, pm;

  printf("[1. PvP; 2. PvC; 3. CvC]\n");
  printf("Input number to select play mode: ");
  scanf("%d", &pm);
  if (pm < 1 || 3 <= pm) {
    cpu = 3;
  }
  if (pm != 1) {
    printf("\n[1. Random; 2. Greedy; 3. Clever]\n");
    printf("Input number to select CPU's heuristics: ");
    scanf("%d", &cpu_heuristics);
    if (cpu_heuristics < 1 || 3 < cpu_heuristics) {
      cpu_heuristics = 3;
    }
  }
  if (pm == 2) {
    printf("\n[1. First move; 2. Second move]\n");
    printf("Input number to select your moving order: ");
    scanf("%d", &mo);
    if (mo <= 1 || 2 < mo) {
      cpu = 2;
    } else {
      cpu = 1;
    }
  }

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      board[i][j] = 0;
    }
  }

  printf("\033[2J\033[0;4H");
  printf("OTHELLO GAME\n\n");
  if (cpu == 3) {
    printf("~CPU1~");
  } else if (cpu) {
    printf("~%s~", cpu == 1 ? "CPU" : "YOU");
  } else {
    printf("~A turn~");
  }
  SET_CURSOR(2, 20);
  printf("o:  2, x:  2\n");
  printf(" Cell : \n\n\n\n");

  printf("   a ");
  for (i = 1; i < 8; i++) {
    printf("  %c ", 'a' + i);
  }
  printf("\n +");
  for (i = 0; i < 8; i++) {
    printf("---+");
  }
  for (i = 1; i < 9; i++) {
    printf("\n%d|", i);
    for (j = 0; j < 8; j++) {
      printf("   |");
    }
    printf("\n +");
    for (j = 0; j < 8; j++) {
      printf("---+");
    }
  }
  mo = 2;
  set_board(3, 4);
  set_board(4, 3);
  mo = 1;
  set_board(3, 3);
  set_board(4, 4);
  getchar();
}

int reverse(char r, char c) {
  int i, j, i2, j2, change = 0;
  int n = r - '1', m = c - 'a';

  SET_CURSOR(5, 0);
  if (board[n][m] || n < 0 || n > 7 || m < 0 || m > 7) {
    printf("     CANNOT PUT THE DISC ON %c%c", c, r);
    return -1;
  } else {
    printf("                                ");
  }

  for (i = m + 1; i < 8 && board[n][i]; i++) {
    if (board[n][i] == mo) {
      if (i != m + 1) {
        change = 1;
        usleep(500000);
        set_board(n, m);

        for (j = m + 1; j < i; j++) {
          usleep(500000);
          set_board(n, j);
        }
      }

      break;
    }
  }

  for (i = m - 1; i > -1 && board[n][i]; i--) {
    if (board[n][i] == mo) {
      if (i != m - 1) {
        if (!change) {
          change = 1;
          usleep(500000);
          set_board(n, m);
        }

        for (j = m - 1; j > i; j--) {
          usleep(500000);
          set_board(n, j);
        }
      }

      break;
    }
  }

  for (i = n + 1; i < 8 && board[i][m]; i++) {
    if (board[i][m] == mo) {
      if (i != n + 1) {
        if (!change) {
          change = 1;
          usleep(500000);
          set_board(n, m);
        }

        for (j = n + 1; j < i; j++) {
          usleep(500000);
          set_board(j, m);
        }
      }

      break;
    }
  }

  for (i = n - 1; i > -1 && board[i][m]; i--) {
    if (board[i][m] == mo) {
      if (i != n - 1) {
        if (!change) {
          change = 1;
          usleep(500000);
          set_board(n, m);
        }

        for (j = n - 1; j > i; j--) {
          usleep(500000);
          set_board(j, m);
        }
      }

      break;
    }
  }

  for (i = n + 1, j = m + 1; i < 8 && j < 8 && board[i][j]; i++, j++) {
    if (board[i][j] == mo) {
      if (i != n + 1) {
        if (!change) {
          change = 1;
          usleep(500000);
          set_board(n, m);
        }

        for (i2 = n + 1, j2 = m + 1; i2 < i; i2++, j2++) {
          usleep(500000);
          set_board(i2, j2);
        }
      }

      break;
    }
  }

  for (i = n - 1, j = m + 1; i > -1 && j < 8 && board[i][j]; i--, j++) {
    if (board[i][j] == mo) {
      if (i != n - 1) {
        if (!change) {
          change = 1;
          usleep(500000);
          set_board(n, m);
        }

        for (i2 = n - 1, j2 = m + 1; i2 > i; i2--, j2++) {
          usleep(500000);
          set_board(i2, j2);
        }
      }

      break;
    }
  }

  for (i = n - 1, j = m - 1; i > -1 && j > -1 && board[i][j]; i--, j--) {
    if (board[i][j] == mo) {
      if (i != n - 1) {
        if (!change) {
          change = 1;
          usleep(500000);
          set_board(n, m);
        }

        for (i2 = n - 1, j2 = m - 1; i2 > i; i2--, j2--) {
          usleep(500000);
          set_board(i2, j2);
        }
      }

      break;
    }
  }

  for (i = n + 1, j = m - 1; i < 8 && j > -1 && board[i][j]; i++, j--) {
    if (board[i][j] == mo) {
      if (i != n + 1) {
        if (!change) {
          change = 1;
          usleep(500000);
          set_board(n, m);
        }

        for (i2 = n + 1, j2 = m - 1; i2 < i; i2++, j2--) {
          usleep(500000);
          set_board(i2, j2);
        }
      }

      break;
    }
  }

  if (!change) {
    SET_CURSOR(5, 4);
    printf("CANNOT PUT THE DISC ON %c%c", c, r);
    return -1;
  }

  return 0;
}

int isPuttable(int s) {
  int i, j, i2, j2, i3, j3, rev_count, candidate_score, max, puttable = 0;
  max = cpu_heuristics == 1 ? 1 : -2048;

  if (cnd_count) {
    for (i = 0; i < cnd_count; i++) {
      free(candidate_cells[i]);
    }
    free(candidate_cells);
    cnd_count = 0;
  }

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      rev_count = 0;

      if (!board[i][j]) {
        candidate_score = -score;

        for (j2 = j + 1; j2 < 8 && board[i][j2]; j2++) {
          if (board[i][j2] == mo) {
            if (cpu_heuristics) {
              for (j3 = j + 1; j3 < j2; j3++) {
                rev_count++;
                candidate_score += val_table[i][j3] << 1;
              }
            } else {
              if (j2 != j + 1) {
                return 1;
              }
            }
            break;
          }
        }

        for (j2 = j - 1; j2 > -1 && board[i][j2]; j2--) {
          if (board[i][j2] == mo) {
            if (cpu_heuristics) {
              for (j3 = j - 1; j3 > j2; j3--) {
                rev_count++;
                candidate_score += val_table[i][j3] << 1;
              }
            } else {
              if (j2 != j - 1) {
                return 1;
              }
            }
            break;
          }
        }

        for (i2 = i + 1; i2 < 8 && board[i2][j]; i2++) {
          if (board[i2][j] == mo) {
            if (cpu_heuristics) {
              for (i3 = i + 1; i3 < i2; i3++) {
                rev_count++;
                candidate_score += val_table[i3][j] << 1;
              }
            } else {
              if (i2 != i + 1) {
                return 1;
              }
            }
            break;
          }
        }

        for (i2 = i - 1; i2 > -1 && board[i2][j]; i2--) {
          if (board[i2][j] == mo) {
            if (cpu_heuristics) {
              for (i3 = i - 1; i3 > i2; i3--) {
                rev_count++;
                candidate_score += val_table[i3][j] << 1;
              }
            } else {
              if (i2 != i - 1) {
                return 1;
              }
            }
            break;
          }
        }

        for (i2 = i + 1, j2 = j + 1; i2 < 8 && j2 < 8 && board[i2][j2];
             i2++, j2++) {
          if (board[i2][j2] == mo) {
            if (cpu_heuristics) {
              for (i3 = i + 1, j3 = j + 1; i3 < i2; i3++, j3++) {
                rev_count++;
                candidate_score += val_table[i3][j3] << 1;
              }
            } else {
              if (i2 != i + 1) {
                return 1;
              }
            }
            break;
          }
        }

        for (i2 = i - 1, j2 = j + 1; i2 > -1 && j2 < 8 && board[i2][j2];
             i2--, j2++) {
          if (board[i2][j2] == mo) {
            if (cpu_heuristics) {
              for (i3 = i - 1, j3 = j + 1; i3 > i2; i3--, j3++) {
                rev_count++;
                candidate_score += val_table[i3][j3] << 1;
              }
            } else {
              if (i2 != i - 1) {
                return 1;
              }
            }
            break;
          }
        }

        for (i2 = i - 1, j2 = j - 1; i2 > -1 && j2 > -1 && board[i2][j2];
             i2--, j2--) {
          if (board[i2][j2] == mo) {
            if (cpu_heuristics) {
              for (i3 = i - 1, j3 = j - 1; i3 > i2; i3--, j3--) {
                rev_count++;
                candidate_score += val_table[i3][j3] << 1;
              }
            } else {
              if (i2 != i - 1) {
                return 1;
              }
            }
            break;
          }
        }

        for (i2 = i + 1, j2 = j - 1; i2 < 8 && j2 > -1 && board[i2][j2];
             i2++, j2--) {
          if (board[i2][j2] == mo) {
            if (cpu_heuristics) {
              for (i3 = i + 1, j3 = j - 1; i3 < i2; i3++, j3--) {
                rev_count++;
                candidate_score += val_table[i3][j3] << 1;
              }
            } else {
              if (i2 != i + 1) {
                return 1;
              }
            }
            break;
          }
        }
      }

      if (rev_count) {
        puttable = 1;

        if (cpu_heuristics == 1) {
          cpu_board[i][j] = 1;
          cnd_count++;
        } else if (cpu_heuristics == 2) {
          rev_count++;
          cpu_board[i][j] = rev_count;
          if (rev_count > max) {
            max = rev_count;
            cnd_count = 1;
          } else if (rev_count == max) {
            cnd_count++;
          }
        } else if (cpu_heuristics == 3) {
          candidate_score += val_table[i][j];
          cpu_board[i][j] = candidate_score;
          if (candidate_score > max) {
            max = candidate_score;
            cnd_count = 1;
          } else if (candidate_score == max) {
            cnd_count++;
          }
        }
      } else {
        cpu_board[i][j] = -2048;
      }
    }
  }

  if (puttable) {
    candidate_cells = (int **)malloc(sizeof(int *) * cnd_count);
    for (i = 0; i < cnd_count; i++) {
      candidate_cells[i] = (int *)malloc(sizeof(int) * 2);
    }
    for (i = 0, i2 = 0; i < 8; i++) {
      for (j = 0; j < 8; j++) {
        if (cpu_board[i][j] == max) {
          candidate_cells[i2][0] = i;
          candidate_cells[i2][1] = j;
          i2++;
        }
      }
    }

    score = max;
    return 1;
  }

  if (s) {
    return 0;
  } else {
    score = -score;
    mo = (mo & 1) + 1;
    return isPuttable(1);
  }
}

void select_cell(char *r, char *c) {
  int rnd;
  srand(time(NULL));
  rnd = rand() % cnd_count;
  *r = '1' + candidate_cells[rnd][0];
  *c = 'a' + candidate_cells[rnd][1];
  printf("%c%c\n", *c, *r);
}

void set_board(int i, int j) {
  printf("\033[%d;%dH%c", 10 + (i << 1), 4 + (j << 2), mo == 1 ? 'o' : 'x');
  disks[mo & 1]++;
  if (board[i][j]) {
    disks[(mo & 1) ^ 1]--;
  }
  SET_CURSOR(2, 23);
  printf("%2d", disks[1]);
  SET_CURSOR(2, 30);
  printf("%2d", disks[0]);
  fflush(stdout);

  board[i][j] = mo;
}

void result() {
  int i, j, countA = 0, countB = 0;
  char playerA[5], playerB[5];

  if (cpu == 3) {
    strcpy(playerA, "CPU1");
    strcpy(playerB, "CPU2");
  } else if (cpu) {
    strcpy(playerA, cpu == 1 ? "CPU" : "YOU");
    strcpy(playerB, cpu == 2 ? "CPU" : "YOU");
  } else {
    strcpy(playerA, "A");
    strcpy(playerB, "B");
  }

  SET_CURSOR(2, 0);
  printf("       ~FINISH~                 \n");
  if (countA == countB) {
    printf("%s: %d ,  %s: %d      \\Draw/", playerA, disks[1], playerB,
           disks[0]);
  } else {
    printf("%s: %d ,  %s: %d      !!Winner %s!!", playerA, disks[1], playerB,
           disks[0], disks[1] > disks[0] ? playerA : playerB);
  }

  SET_CURSOR(27, 0);
}
