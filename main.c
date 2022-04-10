/*
Antoni Misztal 417741
zad.1 Macierze rzadkie
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX_SAVED 26
#define DL_NAZPLIKU 30

typedef struct matrixlist {
    int line, column, info1;
    struct matrixlist *pion, *poziom;
} matrixlist, *mlist;

typedef struct sentinelslist {
    char info2;
    int height, width;
    mlist adres;
    struct sentinelslist *next;
} sentinelslist, *slist;

slist szukaj (char x, slist sl) {
    slist u;
    u = sl;
    while (u != NULL) {
        if (u->info2 == x)
            return u;
        u = u->next;
    }
    return NULL;
}

void nastepny_znak (char *c, slist sl) {
    char x;
    slist u;
    x = (*c);
    if ((*c) == 'Z') {
        (*c) = 'A';
    } else if ((*c) == 'z') {
        (*c) = 'a';
    } else {
        (*c)++;
    }
    while ((*c) != x) {
        u = szukaj((*c), sl);
        if (u == NULL) {
            return;
        } else if ((*c) == 'Z') {
            (*c) = 'A';
        } else if ((*c) == 'z') {
            (*c) = 'a';
        } else {
            (*c)++;
        }
    }
    printf("\npamięć pełna !!!\n");
}

void usun (slist x, slist *sl) {
    int i = 0;
    mlist u,w,z;
    slist u2, w2;
    u = x->adres;
    for (i; i <= x->height; i++) {
        w = u->poziom;
        while (w != u) {
        z = w;
        w = w->poziom;
        free(z);
        }
        u = u->pion;
        free(w);
    }
    if (*sl == x) {
        u2 = (*sl);
        (*sl) = (*sl)->next;
        free (u2);
    } else if ((*sl)->next != NULL) {
        u2 = (*sl)->next;
        w2 = (*sl);
        while (u2 != x) {
            u2 = u2->next;
            w2 = w2->next;
        }
        w2->next = u2->next;
        free(u2);
    }
}

void ini_sentinelslist (slist *sl) {
    *sl = NULL;
}

slist new_sentinelslist (mlist X, char *x, int lines, int columns) {
    slist u;
    u = malloc(sizeof(struct sentinelslist));
    u->adres = X;
    u->info2 = (*x);
    u->width = columns;
    u->height = lines;
    return u;
}

void add_sentinelslist (slist *sl, slist u) {
    u->next = *sl;
    *sl = u;
}

mlist new_matrixlist (int l, int c, int i) {
    mlist u;
    u = malloc(sizeof(struct matrixlist));
    u->column = c;
    u->line = l;
    u->info1 = i;
    return u;
}

mlist create_matrix1 (slist *sl, char *x, int lines, int columns) {
    int i;
    mlist w,u;
    w = malloc(sizeof(struct matrixlist));
    w->column = 0;
    w->line = 0;
    w->pion = w;
    w->poziom = w;
    add_sentinelslist(sl, new_sentinelslist(w, x, lines, columns));
    u = w;
    i = 0;
    while (i != columns) {
        i++;
        u->poziom = new_matrixlist(0, i, 0);
        u = u->poziom;
        u->poziom = w;
        u->pion = u;
    }
    u = w;
    i = 0;
    while (i != lines) {
        i++;
        u->pion = new_matrixlist(i, 0, 0);
        u = u->pion;
        u->pion = w;
        u->poziom = u;
    }
    return w;
}

int fill_matrix (mlist w, int l, int c, int i) {
    mlist u,v,z;
    z = new_matrixlist(l, c, i);
    u = w;
    while (u->column != c) {
        u = u->poziom;
    }
    if (u->pion == u) {
        u->pion = z;
        z->pion = u;
    } else {
        v = u;
        while ((v->pion != u) && (v->pion->line < l)) {
            v = v->pion;
        }
        if (v->pion == u) {
            v->pion = z;
            z->pion = u;
        } else if (v->pion->line == l) {
            return 0;
        } else {
            z->pion = v->pion;
            v->pion = z;
        }
    }
    u = w;
    while (u->line != l) {
        u = u->pion;
    }
    if (u->poziom == u) {
        u->poziom = z;
        z->poziom = u;
    } else {
        v = u;
        while ((v->poziom != u) && (v->poziom->column < c)) {
            v = v->poziom;
        }
        if (v->poziom == u) {
            v->poziom = z;
            z->poziom = u;
        } else if (v->poziom->column == c) {
            return 0;
        } else {
            z->poziom = v->poziom;
            v->poziom = z;
        }
    }
    return 1;
}

void printmatrix (mlist w, int height, int width) {
    int i;
    mlist u;
    u = w->pion;
    while (u != w) {
        u = u->poziom;
        int i = 1;
        while ((u->column != 0) || (i <= width)) {
            if (u->column == i){
                printf("%d ", u->info1);
                u = u->poziom;
                i++;
            } else {
                printf("0 ");
                i++;
                }
        }
        printf("\n");
        u = u->pion;
    }
}

int czytaj (slist *sl, char *x){
    int lin,col,l,c,i;
    int k = 1;
    char nazwapliku[DL_NAZPLIKU+1];
    FILE * we ;
    mlist w;
    do
    { printf ("podaj nazwe pliku do odczytu: ") ;
      gets (nazwapliku) ;
      if ( ( we = fopen (nazwapliku, "rt")) == NULL)
      printf ("***** nie ma takiego pliku\n") ;
    }
    while (we == NULL);
    przystanek:
    if (fscanf(we, "%d", &lin) != 1) {
        return 0;
    }
    if (fscanf(we, "%d", &col) != 1) {
        return 0;
    }
    w = create_matrix1(sl, x, lin, col);
    while (fscanf(we, "%d %d %d", &l, &c, &i) == 3 && k == 1) {
        if (l < 1 || l > lin || c < 1 || c > col) {
            k = 0;
        }   else if ((fill_matrix(w, l, c, i)) == 0) {
            k = 0;
        }
    }
    printf("\nMacierz będzie znana jako %c \n", *x);
    if (getc(we) == 'x') {
        nastepny_znak(x, *sl);
        goto przystanek;
    }
    fclose(we);
    nastepny_znak(x,*sl);
    return 1;
}

mlist dodaj (slist *sl, slist A, slist B, char *x) {
    int i = 1;
    mlist u,w,z;
    w = A->adres->pion->poziom;
    z = B->adres->pion->poziom;
    u = create_matrix1(sl, x, A->height, A->width);
    for (i; i <= A->height; i++) {
        while ((w->column != 0) && (z->column != 0)) {
            if (w->column < z->column) {
                fill_matrix(u, w->line, w->column, w->info1);
                w = w->poziom;
            } else if (w->column > z->column) {
                fill_matrix(u, z->line, z->column, z->info1);
                z = z->poziom;
            } else {
                fill_matrix(u, w->line, w->column, (w->info1)+(z->info1));
                w = w->poziom;
                z = z->poziom;
            }
        }
        while (w->column !=0) {
            fill_matrix(u, w->line, w->column, w->info1);
            w = w->poziom;
        }
        while (z->column !=0) {
            fill_matrix(u, z->line, z->column, z->info1);
            z = z->poziom;
        }
        w = w->pion->poziom;
        z = z->pion->poziom;
    }
    return u;
}

void transpozycja (slist w) {
    int i,j;
    mlist u,z;
    u = w->adres;
    for (i = 0; i <= w->width; i++) {
            j = u->column;
            u->column = u->line;
            u->line = j;
            z = u->poziom;
            u->poziom = u->pion;
            u->pion = z;
            u = z;
    }
    u = u->poziom;
    for (i = 1; i <= w->height; i++) {
        while (u->line != 0) {
            j = u->column;
            u->column = u->line;
            u->line = j;
            z = u->poziom;
            u->poziom = u->pion;
            u->pion = z;
            u = z;
        }
        u = u->poziom;
    }
    i = w->height;
    w->height = w->width;
    w->width = i;
}

mlist mnozenie1 (slist *sl,slist A, slist a,char *c) {
    int i = 1;
    int k = 0;
    mlist u,x,y;
    x = A->adres->pion->poziom;
    y = a->adres->poziom->pion;
    u = create_matrix1(sl, c, A->height, 1);
    for (i; i <= A->height; i++) {
        while ((y->line != 0) && (x->column != 0)) {
            if (y->line > x->column) {
                x = x->poziom;
            } else if (y->line < x->column) {
                y = y->pion;
            } else {
                k = k + (y->info1)*(x->info1);
                x = x->poziom;
                y = y->pion;
            }
        }
        while (y->line != 0) {
            y = y->pion;
        }
        while (x->column != 0) {
            x = x->poziom;
        }
        if (k != 0) {
            fill_matrix(u, i, 1, k);
        }
        k = 0;
        x = x->pion->poziom;
        y = y->pion;
    }
    return u;
}

void recznie (slist *sl, char *x){
    int lin,col,l,c,i;
    int k = 1;
    mlist w;
    przystanek2:
    scanf("%d", &lin);
    scanf("%d", &col);
    w = create_matrix1(sl, x, lin, col);
    while (scanf("%d %d %d", &l, &c, &i) == 3 && k == 1) {
        while ((getchar()) != '\n');
        if (l < 1 || l > lin || c < 1 || c > col) {
            k = 0;
        }   else if ((fill_matrix(w, l, c, i)) == 0) {
            k = 0;
        }
    }
    printmatrix(w, lin, col);
    printf("\nMacierz będzie znana jako %c \n", *x);
    nastepny_znak(x, *sl);
}

void printmatrix2 (mlist w, int height, int width) {
    int i,k;
    mlist u;
    u = w->pion;
    while (u != w) {
        u = u->poziom;
        int i = 1;
        k = 0;
        while ((u->column != 0) || (i <= width)) {
            if (u->column == i){
                if (k != 1) {
                    printf("wiersz %d:   ",u->line);
                    k = 1;
                }
                printf(" kolumna %d: %d ",u->column, u->info1);
                u = u->poziom;
                i++;
            }
            i++;
        }
        if (k == 1) {
            printf("\n");
        }
        u = u->pion;
    }
}

int main()
{
    int i;
    char c1 = 'A';
    char c2 = 'a';
    char command[3];
    char x,y;
    slist sl1, sl2;
    slist u,w;
    ini_sentinelslist(&sl1);
    ini_sentinelslist(&sl2);
    printf("1.Program zawiera dwie listy, jedna jest dedykowana do przechowywania macierzy,\ndruga do przechowywania wierszy.\n");
    printf("(maksymalnie 26 macierzy i 26 wektorów)\n");
    printf("(można zapisywać macierze tam gdzie powinny być wiersze,\na wiersze tam gdzie powinny być macierze, ale ograniczy to możliwe operacje)\n");
    printf("(przykład poprawnego formatowania plikow z macierzami oraz\ntego jak wprowadzac recznie macierze jest w dolaczonym pliku tekstowym)\n");
    printf("2.Polecenia dostepne w sekcji liczenia to:\n");
    printf("x+y, gdzie x i y to dowolne litery alfabetu angielskiego (zarowno male i duze),\nta komenda skutkuje proba dodania macierzy\n");
    printf("X*x, gdzie obydwie litery to dowolne litery alfabetu angielskiego\n(pierwsza duza, druga mala), ta komenda sprobuje pomnozyc X przez wektor x\n");
    printf("X^T, gdzie X to dowolna litera alfabetu angielskiego (duza lub mala),\nta komenda transponuje macierz\n");
    printf("3.wpisanie znaku ! cofa do pierwszego pytania\n");
    stop1:
    while ((getchar()) != '\n');
    printf("\nCo chcesz robic?\ni = wczytywanie p = drukowanie c = liczyc d = usuwanie e = wyjscie\n");
    scanf("%c",&x);
    switch (x) {
    case 'i':
    while ((getchar()) != '\n');
    printf("\nWprowadzasz ręcznie, czy poprzez plik? f = plik h = ręcznie\n");
    scanf("%c",&x);
    if (x == 'f') {
        printf("\nMacierze czy wektory? m = macierze v = wektory\n");
        stop2:
        while ((getchar()) != '\n');
        scanf("%c",&x);
        if (x == 'm') {
            while ((getchar()) != '\n');
            czytaj(&sl1, &c1);
        } else if (x == 'v') {
            while ((getchar()) != '\n');
            czytaj(&sl2, &c2);
        } else if (x == '!') {
            goto stop1;
        } else {
            printf("\nbledna komenda, prosze sprobowac jeszcze raz\n");
            goto stop2;
        }
    } else if (x == 'h') {
        printf("\nMacierze czy wektory? m = macierze v = wektory\n");
        stop3:
        while ((getchar()) != '\n');
        scanf("%c",&x);
        if (x == 'm') {
            recznie(&sl1, &c1);
        } else if (x == 'v') {
            recznie(&sl2, &c2);
        } else if (x == '!') {
            goto stop1;
        } else {
            printf("\nbledna komenda, prosze sprobowac jeszcze raz\n");
            goto stop3;
        }
    } else if (x == '!') {
            goto stop1;
    } else {
        printf("\nbledna komenda, prosze sprobowac jeszcze raz\n");
        goto stop1;
    }
    break;
    case 'p':
    while ((getchar()) != '\n');
    printf("\nPrzedstawić macierze g = graficznie czy t = tekstowo?\n");
    scanf("%c",&y);
    if (y != 'g' && y != 't') {
        printf("\nbledna komenda, prosze sprobowac jeszcze raz\n");
        goto stop1;
    }
    while ((getchar()) != '\n');
    printf("\nJedną macierz/wektor, czy wszystkie? o = jedną a = wszystkie\n");
    scanf("%c",&x);
    if (x == 'o') {
        printf("Którą?\n");
        stop4:
        while ((getchar()) != '\n');
        scanf("%c",&x);
        if (x >= 'A' && x <= 'Z' && (u = szukaj(x, sl1)) != NULL) {
            if (y == 't') {
                printmatrix2(u->adres, u->height, u->width);
            } else {
                printmatrix(u->adres, u->height, u->width);
            }
        } else if (x >= 'a' && x <= 'z' && (u = szukaj(x, sl2)) != NULL) {
            if (y == 't') {
                printmatrix2(u->adres, u->height, u->width);
            } else {
                printmatrix(u->adres, u->height, u->width);
            }
        } else if (x == '!') {
            goto stop1;
        } else {
            printf("\nnie ma takiej macierzy\n");
            goto stop4;
        }
    } else if (x == 'a') {
        while ((getchar()) != '\n');
        printf("\nMacierze, czy wektory? m = macierze v = wektory\n");
        scanf("%c",&x);
        if (x == 'm') {
            c1 = 'A';
            for (i = 1; i<=26; i++) {
                u = szukaj(c1, sl1);
                if (u != NULL) {
                    if (y == 't') {
                        printf("\nMacierz %c:\n", u->info2);
                        printmatrix2(u->adres, u->height, u->width);
                        printf("\n");
                    } else {
                        printf("\nMacierz %c:\n", u->info2);
                        printmatrix(u->adres, u->height, u->width);
                        printf("\n");
                    }
                }
                (c1)++;
            }
        } else if (x == 'v') {
            c2 = 'a';
            for (i = 1; i<=26; i++) {
                u = szukaj(c2, sl2);
                if (u != NULL) {
                    if (y == 't') {
                        printf("\nMacierz %c:\n", u->info2);
                        printmatrix2(u->adres, u->height, u->width);
                        printf("\n");
                    } else {
                        printf("\nMacierz %c:\n", u->info2);
                        printmatrix(u->adres, u->height, u->width);
                        printf("\n");
                    }
                }
                (c2)++;
            }
        } else if (x == '!') {
            goto stop1;
        }
    } else if (x == '!') {
            goto stop1;
        }
    else {
        printf("\nbledna komenda, prosze sprobowac jeszcze raz\n");
        goto stop1;
    }
    break;
    case 'c':
    while ((getchar()) != '\n');
    printf("\nCo robimy?\n");
    scanf("%s",command);
    if ((command[1] == '+') && (u = szukaj(command[0], sl1)) != NULL && (w = szukaj(command[2], sl1)) != NULL) {
        if (u->height != w->height || u->width != w->width) {
            printf("\nmacierze nie sa takich samych rozmiarow\n");
            goto stop1;
        }
        printmatrix2(dodaj(&sl1, u, w ,&c1), u->height, u->width);
        printf("Wynik bedzie znany jako macierz %c", c1);
        nastepny_znak(&c1, sl1);
    } else if (command[1] == '*') {
        if (command[0] >='A' && command[0] <= 'Z') {
            u = szukaj(command[0], sl1);
        } else {
            printf("\nniepoprawna komenda\n");
            goto stop1;
        }
        if (command[2] >= 'a' && command[2] <= 'z') {
            w = szukaj(command[2], sl2);
        } else {
            printf("\nniepoprawna komenda\n");
            goto stop1;
        }
        if (u == NULL) {
            printf("\nNie ma macierzy %c\n", command[0]);
            goto stop1;
        }
        if (w == NULL) {
            printf("\nNie ma macierzy %c\n", command[2]);
            goto stop1;
        }
        if (u->width != w->height || w->width != 1) {
            printf("\nco najmniej jedna z wybranych macierzy jest złych rozmiarów\n");
            goto stop1;
        }
        mnozenie1(&sl2, u, w, &c2);
        printf("Wynik bedzie znany jako macierz %c", c2);
        nastepny_znak(&c2, sl2);
    } else if (command[1] == '^' && command[2] == 'T') {
        if (command[0] >= 'A' && command[0] <= 'Z') {
            if ((u = szukaj(command[0], sl1)) == NULL) {
                printf("\nnie ma takiej macierzy\n");
                goto stop1;
            }
            transpozycja(u);
            printf("\ntranspozycja sie udala\n");
        } else if (command[0] >= 'a' && command[0] <= 'z') {
            if ((u = szukaj(command[0], sl2)) == NULL) {
                printf("\nnie ma takiego wektora\n");
                goto stop1;
            }
            transpozycja(u);
            printf("\ntranspozycja sie udala\n");
        }
    } else if (x == '!') {
            goto stop1;
    } else {
        printf("\nbledna komenda, prosze sprobowac jeszcze raz\n");
        goto stop1;
    }
    break;
    case 'd':
    while ((getchar()) != '\n');
    printf("\nJedną macierz/wektor, czy wszystkie? o = jedną a = wszystkie\n");
    scanf("%c",&x);
    if (x == 'o') {
        while ((getchar()) != '\n');
        printf("\nKtórą?\n");
        scanf("%c",&x);
        if (x >= 'A' && x <= 'Z') {
            u = szukaj(x, sl1);
            if (u != NULL) {
                usun(u, &sl1);
            }
        } else if (x >= 'a' && x <= 'z') {
            u = szukaj(x, sl2);
            if (u != NULL) {
                usun(u, &sl2);
            }
        } else if (x == '!') {
            goto stop1;
        }
    } else if (x == 'a') {
        while ((getchar()) != '\n');
        printf("\nMacierze, czy wektory? m = macierze v = wektory\n");
        scanf("%c",&x);
        if (x == 'm') {
            c1 = 'A';
            for (i = 1; i<=26; i++) {
                u = szukaj(c1, sl1);
                if (u != NULL) {
                    usun(u, &sl1);
                }
                (c1)++;
            }
        } else if (x == 'v') {
            c2 = 'a';
            for (i = 1; i<=26; i++) {
                u = szukaj(c2, sl2);
                if (u != NULL) {
                    usun(u, &sl2);
                }
                (c2)++;
            }
        } else if (x == '!') {
            goto stop1;
        }
    } else if (x == '!') {
            goto stop1;
    }  else {
        printf("\nbledna komenda, prosze sprobowac jeszcze raz\n");
        goto stop1;
    }
    break;
    case 'e':
    u = sl1;
    while (u != NULL) {
        w = u;
        u = u->next;
        free(w);
    }
    u = sl2;
    while (u != NULL) {
        w = u;
        u = u->next;
        free(w);
    }
    exit(0);
    break;
    case '!':
    goto stop1;
    default:
    printf("\nbledna komenda, prosze sprobowac jeszcze raz\n");
    goto stop1;
    }
    goto stop1;
    return 0;
}
