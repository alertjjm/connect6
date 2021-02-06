#ifndef MINMAX_H
#define MINMAX_H
#define DEPTH 2
#define BOARDSIZE 19
#define INF 1000000
typedef struct node {
    int boardstatus[BOARDSIZE][BOARDSIZE];
    int x;
    int y;
    int score;
    int depth;
    int count;
    struct node *parent;
    struct node *sibling;
    struct node* firstchild;
}node;
typedef struct pos {
    int x;
    int y;
}pos;
extern pos nextpos;
void MinMax(node* move, int alpha, int beta, int pn);
void MaxMove(node* move, int alpha, int beta, int pn);
void MinMove(node* move, int alpha, int beta, int pn);
void Init(node* p_move, int pn);
void deleting(node* root);
node* getEnd(node* move);
void rendering(int Board[][BOARDSIZE]);
int whoseturn(int turn);
int isnear(int Board[][BOARDSIZE], int x, int y);
int connectcount(int Board[][BOARDSIZE], int x, int y);
int scoring(int Board[][BOARDSIZE], int pn);
#endif // MINMAX_H
