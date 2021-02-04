#include "minmax.h"
#include<cstdio>
#include <iostream>
#include <string>
#include <ctime>
#include<vector>
#include<cstdlib>
#include<cstring>
#include<vector>
using namespace std;
typedef struct pos {
    int x;
    int y;
};
pos nextpos;
void deleting(node* root) {
    node* pos = root->firstchild;
    delete root;
    while (pos != NULL) {
        node* parallel = pos;
        pos = pos->firstchild;
        delete[] parallel;
    }
}
int whoseturn(int turn) {
    if ((turn / 2) % 2 == 1) {
        return 2;
    }
    else {
        return 1;
    }
}
void MinMax(node* move, int alpha, int beta, int pn) {
    node* cur = move;
    if (cur == NULL)
        return;
    if (pn!=whoseturn(move->count))
        MaxMove(cur, alpha, beta, pn);
    else
        MinMove(cur, alpha, beta, pn);
}
void MaxMove(node* move, int alpha, int beta, int pn) {
    int max_score = -INF;
    int a, b;
    a = alpha;
    b = beta;
    node* child = move->firstchild;
    if (child == NULL) {
        node* end = getEnd(move->sibling);
        MinMax(end->parent, a, INF, pn);
    }
    while (child->sibling != NULL) {
        if (child->score > max_score)
            max_score = child->score;
        if (child->score > b)	//beta cut off
            break;
        child = child->sibling;
    }
    move->score = max_score;
    if (move->score < b)
        b = move->score;
    if (move->parent == NULL) {
        printf("%d\n", move->score);
        node* pos = move->firstchild;
        while (pos->sibling != NULL) {
            if (pos->score == move->score) {
                nextpos.x = pos->x;
                nextpos.y = pos->y;
                break;
            }
            pos = pos->sibling;
        }
        return;
    }
    else if (move->score < a) {
        move->parent->score = move->score;
        if (move->parent->sibling->sibling != NULL) {
            node* end = getEnd(move->parent->sibling);
            MinMax(end->parent, a, INF, pn);
        }
        else if (move->parent->sibling->sibling == NULL) {
            if (move->parent->parent != NULL) {
                MinMax(move->parent->parent, a, b, pn);
            }
            else
                return;
        }
    }
    else if (move->sibling->sibling != NULL && move->sibling->firstchild != NULL) {
        node* end = getEnd(move->sibling);
        MinMax(end->parent, -INF, b, pn);
    }
    else if (move->sibling->sibling == NULL && move->parent != NULL)
        MinMax(move->parent, a, b, pn);
    return;
}
int isnear(int Board[][BOARDSIZE], int x, int y) {
    int dy[] = { -1,1,0 };
    int dx[] = { -1,1,0 };
    int posx, posy;
    for (int iy = 0; iy < 3; iy++) {
        for (int ix = 0; ix < 3; ix++) {
            posx = (x + dx[ix]);
            posy= (y + dy[iy]);
            if (posx < 0 || posy < 0 || posx>18 || posy>18)
                continue;
            if ((dy[iy]*dy[iy] + dx[ix]*dx[ix] != 0) && Board[posy][posx] != 0)
                return 1;
        }
    }
    return 0;
}
int connectcount(int Board[][BOARDSIZE], int x, int y) {
    int totalcount = 0;
    int cnt1, cnt2, cnt3, cnt4, cnt5, cnt6, cnt7, cnt8;
    int num = Board[y][x];
    cnt1 = 0; cnt2 = 0; cnt3 = 0; cnt4 = 0; cnt5 = 0; cnt6 = 0; cnt7 = 0; cnt8 = 0;
    int posx = x;
    int posy = y;
    pos start;
    pos end;
    //x,y기준으로 우상향 검사 cnt1
    while (Board[posy][posx] == num) {
        posx++;
        posy--;
        cnt1++;
        if (posx > 18 || posy < 0 || posx < 0 || posy>18) {
            if (posx < 0)
                posx = 0;
            if (posy < 0)
                posy = 0;
            if (posx > 18)
                posx = 18;
            if (posy > 18)
                posy = 18;
            break;
        }
    }
    cnt1--;
    end.x = posx;
    end.y = posy;
    posx = x;
    posy = y;
    //x,y기준으로 좌하양 검사 cnt2
    while (Board[posy][posx] == num) {
        posx--;
        posy++;
        cnt2++;
        if (posx > 18 || posy < 0 || posx < 0 || posy>18) {
            if (posx < 0)
                posx = 0;
            if (posy < 0)
                posy = 0;
            if (posx > 18)
                posx = 18;
            if (posy > 18)
                posy = 18;
            break;
        }
    }
    cnt2--;
    start.x = posx;
    start.y = posy;
    posx = x;
    posy = y;
    if(Board[start.y][start.x]!=0 && Board[end.y][end.x]!=0){
        //do nothing
    }
    else if (Board[start.y][start.x] != 0 || Board[end.y][end.x] != 0) {
        totalcount = totalcount + cnt1 + cnt2 - 1;
        if (cnt1 + cnt2 >= 4)
            return INF;
    }
    else {
        totalcount = totalcount + cnt1 + cnt2 + 1;
        if (cnt1 + cnt2 >= 3)
            return INF;
    }
    //x,y기주으로 좌상향 검사 cnt3
    while (Board[posy][posx] == num) {
        posx--;
        posy--;
        cnt3++;
        if (posx > 18 || posy < 0 || posx < 0 || posy>18) {
            if (posx < 0)
                posx = 0;
            if (posy < 0)
                posy = 0;
            if (posx > 18)
                posx = 18;
            if (posy > 18)
                posy = 18;
            break;
        }
    }
    cnt3--;
    end.x = posx;
    end.y = posy;
    posx = x;
    posy = y;
    //x,y기준으로 우하향검사cnt4
    while (Board[posy][posx] == num) {
        posx++;
        posy++;
        cnt4++;
        if (posx > 18 || posy < 0 || posx < 0 || posy>18) {
            if (posx < 0)
                posx = 0;
            if (posy < 0)
                posy = 0;
            if (posx > 18)
                posx = 18;
            if (posy > 18)
                posy = 18;
            break;
        }
    }
    cnt4--;
    start.x = posx;
    start.y = posy;
    posx = x;
    posy = y;
    if (Board[start.y][start.x] != 0 && Board[end.y][end.x] != 0) {
        //do nothing
    }
    else if (Board[start.y][start.x] != 0 || Board[end.y][end.x] != 0) {
        totalcount = totalcount + cnt3 + cnt4 - 1;
        if (cnt3 + cnt4 >= 4)
            return INF;
    }
    else {
        totalcount = totalcount + cnt3 + cnt4 + 1;
        if (cnt3 + cnt4 >= 3)
            return INF;
    }
    //x,y기준으로 우검사cnt5
    while (Board[posy][posx] == num) {
        posx++;
        cnt5++;
        if (posx > 18 || posy < 0 || posx < 0 || posy>18) {
            if (posx < 0)
                posx = 0;
            if (posy < 0)
                posy = 0;
            if (posx > 18)
                posx = 18;
            if (posy > 18)
                posy = 18;
            break;
        }
    }
    cnt5--;
    end.x = posx;
    end.y = posy;
    posx = x;
    posy = y;
    //x,y기준으로 좌검사cnt6
    while (Board[posy][posx] == num) {
        posx--;
        cnt6++;
        if (posx > 18 || posy < 0 || posx < 0 || posy>18) {
            if (posx < 0)
                posx = 0;
            if (posy < 0)
                posy = 0;
            if (posx > 18)
                posx = 18;
            if (posy > 18)
                posy = 18;
            break;
        }
    }
    cnt6--;
    start.x = posx;
    start.y = posy;
    posx = x;
    posy = y;
    if (Board[start.y][start.x] != 0 && Board[end.y][end.x] != 0) {
        //do nothing
    }
    else if (Board[start.y][start.x] != 0 || Board[end.y][end.x] != 0) {
        totalcount = totalcount + cnt5 + cnt6 - 1;
        if (cnt5 + cnt6 >= 4)
            return INF;
    }
    else {
        totalcount = totalcount + cnt5 + cnt6 + 1;
        if (cnt5 + cnt6 >= 3)
            return INF;
    }
    //x,y기준으로 아래검사cnt7
    while (Board[posy][posx] == num) {
        posy++;
        cnt7++;
        if (posx > 18 || posy < 0 || posx < 0 || posy>18) {
            if (posx < 0)
                posx = 0;
            if (posy < 0)
                posy = 0;
            if (posx > 18)
                posx = 18;
            if (posy > 18)
                posy = 18;
            break;
        }
    }
    cnt7--;
    end.x = posx;
    end.y = posy;
    posx = x;
    posy = y;
    //x,y기준으로 위검사cnt8
    while (Board[posy][posx] == num) {
        posy--;
        cnt8++;
        if (posx > 18 || posy < 0 || posx < 0 || posy>18) {
            if (posx < 0)
                posx = 0;
            if (posy < 0)
                posy = 0;
            if (posx > 18)
                posx = 18;
            if (posy > 18)
                posy = 18;
            break;
        }
    }
    cnt8--;
    start.x = posx;
    start.y = posy;
    posx = x;
    posy = y;
    if (Board[start.y][start.x] != 0 && Board[end.y][end.x] != 0) {
        //do nothing
    }
    else if (Board[start.y][start.x] != 0 || Board[end.y][end.x] != 0) {
        totalcount = totalcount + cnt7 + cnt8 - 1;
        if (cnt7 + cnt8 >= 4)
            return INF;
    }
    else {
        totalcount = totalcount + cnt7 + cnt8 + 1;
        if (cnt7 + cnt8 >= 3)
            return INF;
    }
    return totalcount;
}
void MinMove(node* move, int alpha, int beta, int pn) {
    int min_score = INF;
    int a, b;
    a = alpha;
    b = beta;
    node* child = move->firstchild;
    if (child == NULL) {
        node* end = getEnd(move->sibling);
        MinMax(end->parent, a, INF, pn);
    }
    while (child->sibling != NULL) {
        if (child->score < min_score)
            min_score = child->score;
        if (child->score < a)	//alpha cut off
            break;
        child = child->sibling;
    }
    move->score = min_score;
    if (move->score >a)
        a = move->score;
    if (move->score >b) {
        move->parent->score = move->score;//에러
        if (move->parent->sibling->sibling != NULL) {
            node* end = getEnd(move->parent->sibling);
            MinMax(end->parent, -INF, b, pn);
        }
        else if (move->parent->sibling->sibling == NULL) {
            if (move->parent->parent != NULL) {
                MinMax(move->parent->parent, a, b, pn);
            }
            else
                return;
        }
    }
    else if (move->sibling->sibling != NULL && move->sibling->firstchild != NULL) {
        node* end = getEnd(move->sibling);
        MinMax(end->parent, a, INF, pn);
    }
    else if (move->sibling->sibling == NULL && move->parent != NULL)
        MinMax(move->parent, a, b, pn);
    return;
}
int scoring(int Board[][BOARDSIZE], int pn) {
    int total_score = 0;
    for (int x = 0; x < BOARDSIZE; x++) {
        for (int y = 0; y < BOARDSIZE; y++) {
            if (Board[y][x] == 0);
            else if (Board[y][x] == pn)
                total_score += connectcount(Board, x, y);
            else
                total_score -= connectcount(Board, x, y);
        }
    }
    return total_score;
}
void Init(node* parent, int pn) {
    vector<pos> v;
    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            if (parent->boardstatus[y][x] == 0) {
                if (isnear(parent->boardstatus, x, y)) {
                    pos p;
                    p.x = x;
                    p.y = y;
                    v.push_back(p);
                }
            }
        }
    }
    int index = v.size();	//한 노드당 가지수
    node* move = new node[index];
    node* tail=NULL;
    for (int i = 0; i < index; i++) {
        move[i].depth = parent->depth + 1;
        //바둑 두기
        memcpy(move[i].boardstatus,parent->boardstatus, sizeof(int) * BOARDSIZE * BOARDSIZE);
        pos temp = v[i];
        move[i].boardstatus[temp.y][temp.x]=whoseturn(move[i].count);
        move[i].x = temp.x;
        move[i].y = temp.y;
        move[i].score = scoring(move[i].boardstatus, pn);
        move[i].parent = parent;
        move[i].count = move[i].parent->count + 1;
        move[i].firstchild = NULL;
        if (i == 0) {
            parent->firstchild = &move[i];
            tail = new node;
            tail->firstchild = NULL;
            move[i].sibling = tail;
            move[i].firstchild = NULL;
        }
        if (i == index - 1) {
            tail = new node;
            tail->sibling = NULL;
            tail->parent = NULL;
            tail->firstchild = NULL;
            move[i].sibling = tail;
        }
        else
            move[i].sibling = &move[i + 1];
    }
    if (parent->depth < DEPTH) {
        for (int i = 0; i < index; i++)
            Init(&move[i],pn);
    }
}
node* getEnd(node* move) {
    node* end =move;
    while (end->firstchild != NULL)
        end = end->firstchild;
    return end;
}

void rendering(int Board[][BOARDSIZE]) {
    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            printf("%2d", Board[y][x]);
        }
        printf("%2d", y);
        printf("\n");
    }
    for (int i = 0; i < BOARDSIZE; i++)
        printf("%2d", i);
    printf("\n");
}

