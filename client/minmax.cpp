#include "minmax.h"
#include<cstdio>
#include <iostream>
#include <string>
#include <ctime>
#include<vector>
#include<cstdlib>
#include<cstring>
#include<QDebug>
#include<vector>
using namespace std;
int isnear(int Board[][BOARDSIZE], int x, int y) {
    int dy[] = { -1,1,0,-2,2,-3,3};
    int dx[] = { -1,1,0,-2,2,-3,3};
    int posx, posy;
    for (int iy = 0; iy < sizeof(dy)/sizeof(int); iy++) {
        for (int ix = 0; ix < sizeof(dx)/sizeof(int); ix++) {
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
int broadcount(int Board[][BOARDSIZE], int x, int y,int pn){

    return 0;
}
int connectcount(int Board[][BOARDSIZE], int x, int y,int pn,int dols) {
    int comparenum=Board[y][x];
    int totalcount = 0;
    int cnt1, cnt2, counts, tempcount;
    int num = Board[y][x];
    cnt1 = 0; cnt2 = 0;
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
    counts=cnt1+cnt2+1;
    switch (counts) {
    case 1:
        tempcount=1000;
        break;
    case 2:
        tempcount=2000;
        break;
    case 3:
        tempcount=3000;
        break;
    case 4:
        if(pn==comparenum && dols==1)
            tempcount=5000;
        else if(pn==comparenum && dols==0)
            tempcount=4000;
        else if(pn!=comparenum &&dols==1)
            tempcount=50000;
        else
            tempcount=50000;
        break;
    case 5:
        if(pn==comparenum && dols==1)
            tempcount=INF;
        else if(pn==comparenum && dols==0)
            tempcount=6000;
        else
            tempcount=65000;
        break;
    case 6:
        if(pn==comparenum)
            tempcount=INF;
        else
            tempcount=INF-1000;
        break;
    default:
        tempcount=INF;
    }
    if(Board[start.y][start.x]!=0 &&Board[end.y][end.x]!=0)
        totalcount+=0;
    else if(Board[start.y][start.x]==0 &&Board[end.y][end.x]==0)
        totalcount+=tempcount;
    else
        totalcount+=tempcount/2;
    cnt1=0;
    cnt2=0;
    //x,y기주으로 좌상향 검사 cnt1
    while (Board[posy][posx] == num) {
        posx--;
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
    //x,y기준으로 우하향검사cnt2
    while (Board[posy][posx] == num) {
        posx++;
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
    counts=cnt1+cnt2+1;
    switch (counts) {
    case 1:
        tempcount=1000;
        break;
    case 2:
        tempcount=2000;
        break;
    case 3:
        tempcount=3000;
        break;
    case 4:
        if(pn==comparenum && dols==1)
            tempcount=5000;
        else if(pn==comparenum && dols==0)
            tempcount=4000;
        else if(pn!=comparenum &&dols==1)
            tempcount=10000;
        else
            tempcount=10000;
        break;
    case 5:
        if(pn==comparenum && dols==1)
            tempcount=INF;
        else if(pn==comparenum && dols==0)
            tempcount=6000;
        else
            tempcount=15000;
        break;
    case 6:
        if(pn==comparenum)
            tempcount=INF;
        else
            tempcount=INF-1000;
        break;
    default:
        tempcount=INF;
    }
    if(Board[start.y][start.x]!=0 &&Board[end.y][end.x]!=0)
        totalcount+=0;
    else if(Board[start.y][start.x]==0 &&Board[end.y][end.x]==0)
        totalcount+=tempcount;
    else
        totalcount+=tempcount/2;
    cnt1=0;
    cnt2=0;
    //x,y기준으로 우검사cnt1
    while (Board[posy][posx] == num) {
        posx++;
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
    //x,y기준으로 좌검사cnt2
    while (Board[posy][posx] == num) {
        posx--;
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
    counts=cnt1+cnt2+1;
    switch (counts) {
    case 1:
        tempcount=1000;
        break;
    case 2:
        tempcount=2000;
        break;
    case 3:
        tempcount=3000;
        break;
    case 4:
        if(pn==comparenum && dols==1)
            tempcount=5000;
        else if(pn==comparenum && dols==0)
            tempcount=4000;
        else if(pn!=comparenum &&dols==1)
            tempcount=10000;
        else
            tempcount=10000;
        break;
    case 5:
        if(pn==comparenum && dols==1)
            tempcount=INF;
        else if(pn==comparenum && dols==0)
            tempcount=6000;
        else
            tempcount=15000;
        break;
    case 6:
        if(pn==comparenum)
            tempcount=INF;
        else
            tempcount=INF-1000;
        break;
    default:
        tempcount=INF;
    }
    if(Board[start.y][start.x]!=0 &&Board[end.y][end.x]!=0)
        totalcount+=0;
    else if(Board[start.y][start.x]==0 &&Board[end.y][end.x]==0)
        totalcount+=tempcount;
    else
        totalcount+=tempcount/2;
    cnt1=0;
    cnt2=0;
    //x,y기준으로 아래검사cnt1
    while (Board[posy][posx] == num) {
        posy++;
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
    //x,y기준으로 위검사cnt2
    while (Board[posy][posx] == num) {
        posy--;
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
    counts=cnt1+cnt2+1;
    switch (counts) {
    case 1:
        tempcount=1000;
        break;
    case 2:
        tempcount=2000;
        break;
    case 3:
        tempcount=3000;
        break;
    case 4:
        if(pn==comparenum && dols==1)
            tempcount=5000;
        else if(pn==comparenum && dols==0)
            tempcount=4000;
        else if(pn!=comparenum &&dols==1)
            tempcount=10000;
        else
            tempcount=10000;
        break;
    case 5:
        if(pn==comparenum && dols==1)
            tempcount=INF;
        else if(pn==comparenum && dols==0)
            tempcount=6000;
        else
            tempcount=15000;
        break;
    case 6:
        if(pn==comparenum)
            tempcount=INF;
        else
            tempcount=INF-1000;
        break;
    default:
        tempcount=INF;
    }
    if(Board[start.y][start.x]!=0 &&Board[end.y][end.x]!=0)
        totalcount+=0;
    else if(Board[start.y][start.x]==0 &&Board[end.y][end.x]==0)
        totalcount+=tempcount;
    else
        totalcount+=tempcount/2;
    int linecounter=broadcount(Board,x,y,pn);
    if(linecounter>=5)
        totalcount+=INF;
    else if(linecounter==4)
        totalcount+=10000;
    return totalcount;
}

int maxscoring(int Board[][BOARDSIZE], int pn, pos p, int dols) {
    int total_score = 0;
    long temp_score;
    Board[p.y][p.x]=pn;
    for (int x = 0; x < BOARDSIZE; x++) {
        for (int y = 0; y < BOARDSIZE; y++) {
            if (Board[y][x] == 0)
                continue;
            else{
                temp_score=connectcount(Board,x,y,pn, dols);
                if(temp_score==INF && Board[y][x] == pn){
                    Board[p.y][p.x]=0;
                    return INF*1.2;
                }
                else if(temp_score==INF && Board[y][x] != pn){
                    Board[p.y][p.x]=0;
                    return -INF;
                }
                else if (Board[y][x] == pn){
                    total_score += temp_score;
                }
                else{
                    total_score -= temp_score*1.2;
                }
            }
        }
    }
    Board[p.y][p.x]=0;
    return total_score;
}


