#include <stdio.h>
#include <unistd.h>
#define SET_CURSOR(i, j) printf("\033[%d;%dH", i+1, j+1)


int board[8][8];


void init();
int reverse(char, char, int);
int isPuttable(int, int);
void set_board(int, int, int);
void result();


int main() {
    int r, c, k=1;
    
    init();

    while(isPuttable(k, 0)) {
        SET_CURSOR(2, 1);
        printf("%c", 'A'+k-1);

        do {
            SET_CURSOR(3, 8);
            printf("                              \n");
            printf("                              \n\n");
            printf("                              ");
            SET_CURSOR(3, 8);
            c = getchar();
            r = getchar();
            while(getchar()!=10);
        } while(reverse(r, c, k));

        k = k % 2 + 1;
    }

    result();

    return 0;
}


void init() {
    int i, j;

    for(i=0; i<8; i++)
        for(j=0; j<8; j++) board[i][j] = 0;
    
    printf("\033[2J\033[0;4H");
    printf("OTHELLO GAME\n\n");
    printf("~A turn~\n");
    printf(" Cell : \n\n\n\n");

    printf("   a ");
    for(i=1; i<8; i++) printf("  %c ", 'a'+i);
    printf("\n +");
    for(i=0; i<8; i++) printf("---+");
    for(i=1; i<9; i++) {
        printf("\n%d|", i);
        for(j=0; j<8; j++) printf("   |");
        printf("\n +");
        for(j=0; j<8; j++) printf("---+");
    }

    set_board(3, 3, 1);
    set_board(3, 4, 2);
    set_board(4, 3, 2);
    set_board(4, 4, 1);
}


int reverse(char r, char c, int k) {
    int i, j, i2, j2, change=0;
    int n=r-'1', m=c-'a';

    SET_CURSOR(5, 0);
    if(board[n][m] || n<0 || n>7 || m<0 || m>7) {
        printf("     CANNOT PUT THE DISC ON %c%c", c, r);
        return -1;
    }
    else printf("                                ");


    for(i=m+1; i<8 && board[n][i]; i++) {
        if(board[n][i] == k) {
            if(i != m+1) {
                change = 1;
                //usleep(500000);
                set_board(n, m, k);

                for(j=m+1; j<i; j++) {
                    usleep(500000);
                    set_board(n, j, k);
                }
            }

            break;
        }
    }

    for(i=m-1; i>-1 && board[n][i]; i--) {
        if(board[n][i] == k) {
            if(i != m-1) {
                if(!change) {
                    change = 1;
                    //usleep(500000);
                    set_board(n, m, k);
                }

                for(j=m-1; j>i; j--) {
                    usleep(500000);
                    set_board(n, j, k);
                }
            }

            break;
        }
    }

    for(i=n+1; i<8 && board[i][m]; i++) {
        if(board[i][m] == k) {
            if(i != n+1) {
                if(!change) {
                    change = 1;
                    //usleep(500000);
                    set_board(n, m, k);
                }

                for(j=n+1; j<i; j++) {
                    usleep(500000);
                    set_board(j, m, k);
                }
            }

            break;
        }
    }

    for(i=n-1; i>-1 && board[i][m]; i--) {
        if(board[i][m] == k) {
            if(i != n-1) {
                if(!change) {
                    change = 1;
                    //usleep(500000);
                    set_board(n, m, k);
                }

                for(j=n-1; j>i; j--) {
                    usleep(500000);
                    set_board(j, m, k);
                }
            }

            break;
        }
    }


    for(i=n+1, j=m+1; i<8 && j<8 && board[i][j]; i++, j++) {
        if(board[i][j] == k) {
            if(i != n+1) {
                if(!change) {
                    change = 1;
                    //usleep(500000);
                    set_board(n, m, k);
                }

                for(i2=n+1, j2=m+1; i2<i; i2++, j2++) {
                    usleep(500000);
                    set_board(i2, j2, k);
                }
            }

            break;
        }
    }

    for(i=n-1, j=m+1; i>-1 && j<8 && board[i][j]; i--, j++) {
        if(board[i][j] == k) {
            if(i != n-1) {
                if(!change) {
                    change = 1;
                    //usleep(500000);
                    set_board(n, m, k);
                }

                for(i2=n-1, j2=m+1; i2>i; i2--, j2++) {
                    usleep(500000);
                    set_board(i2, j2, k);
                }
            }

            break;
        }
    }

    for(i=n-1, j=m-1; i>-1 && j>-1 && board[i][j]; i--, j--) {
        if(board[i][j] == k) {
            if(i != n-1) {
                if(!change) {
                    change = 1;
                    //usleep(500000);
                    set_board(n, m, k);
                }

                for(i2=n-1, j2=m-1; i2>i; i2--, j2--) {
                    usleep(500000);
                    set_board(i2, j2, k);
                }
            }

            break;
        }
    }

    for(i=n+1, j=m-1; i<8 && j>-1 && board[i][j]; i++, j--) {
        if(board[i][j] == k) {
            if(i != n+1) {
                if(!change) {
                    change = 1;
                    //usleep(500000);
                    set_board(n, m, k);
                }

                for(i2=n+1, j2=m-1; i2<i; i2++, j2--) {
                    usleep(500000);
                    set_board(i2, j2, k);
                }
            }

            break;
        }
    }


    if(!change) {
        SET_CURSOR(5, 4);
        printf("CANNOT PUT THE DISC ON %c%c", c, r);
        return -1;
    }

    return 0;
}


int isPuttable(int k, int s) {
    int i, j, i2, j2;

    for(i=0; i<8; i++) {
        for(j=0; j<8; j++) {
            if(!board[i][j]) {
                for(j2=j+1; j2<8 && board[i][j2]; j2++) {
                    if(board[i][j2]==k) {
                        if(j2!=j+1) return 1;
                        break;
                    }
                }

                for(j2=j-1; j2>-1 && board[i][j2]; j2--) {
                    if(board[i][j2]==k) {
                        if(j2!=j-1) return 1;
                        break;
                    }
                }

                for(i2=i+1; i2<8 && board[i2][j]; i2++) {
                    if(board[i2][j]==k) {
                        if(i2!=i+1) return 1;
                        break;
                    }
                }

                for(i2=i-1; i2>-1 && board[i2][j]; i2--) {
                    if(board[i2][j]==k) {
                        if(i2!=i-1) return 1;
                        break;
                    }
                }

                for(i2=i+1, j2=j+1; i2<8 && j2<8 && board[i2][j2]; i2++, j2++) {
                    if(board[i2][j2]==k) {
                        if(i2!=i+1) return 1;
                        break;
                    }
                }

                for(i2=i-1, j2=j+1; i2>-1 && j2<8 && board[i2][j2]; i2--, j2++) {
                    if(board[i2][j2]==k) {
                        if(i2!=i-1) return 1;
                        break;
                    }
                }

                for(i2=i-1, j2=j-1; i2>-1 && j2>-1 && board[i2][j2]; i2--, j2--) {
                    if(board[i2][j2]==k) {
                        if(i2!=i-1) return 1;
                        break;
                    }
                }

                for(i2=i+1, j2=j-1; i2<8 && j2>-1 && board[i2][j2]; i2++, j2--) {
                    if(board[i2][j2]==k) {
                        if(i2!=i+1) return 1;
                        break;
                    }
                }
            }
        }
    }

    if(s) return 0;
    else {
        k = k % 2 + 1;
        return isPuttable(k, 1);
    }
}


void set_board(int i, int j, int k) {
    printf("\033[%d;%dH%c", 10 + (i<<1), 4 + (j<<2), k==1 ? 'o' : 'x');
    fflush(stdout);
    board[i][j] = k;
}


void result() {
    int i, j, countA=0, countB=0;

    for(i=0; i<8; i++)
        for(j=0; j<8; j++)
            if(board[i][j]) board[i][j] == 1 ? countA++ : countB++; 

    SET_CURSOR(2, 0);
    printf("       ~FINISH~               \n");
    if(countA == countB) printf("A: %d ,  B: %d      \\Draw/", countA, countB);
    else printf("A: %d ,  B: %d      !!Winner %c!!", countA, countB, countA>countB ? 'A' : 'B');

    SET_CURSOR(27,0);
}