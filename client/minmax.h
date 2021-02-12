#ifndef MINMAX_H
#define MINMAX_H
#define DEPTH 2
#define BOARDSIZE 19
#define INF 1000000
typedef struct pos {
    int x;
    int y;
}pos;
int maxscoring(int Board[][BOARDSIZE], int pn, pos p, int dols);
int isnear(int Board[][BOARDSIZE], int x, int y);
int connectcount(int Board[][BOARDSIZE], int x, int y,int pn, int dols);
#endif // MINMAX_H
