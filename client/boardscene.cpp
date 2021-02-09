#include "boardscene.h"
#include "minmax.h"
#include<QMessageBox>
#include<vector>
#include <ctime>        // std::time
#include <cstdlib>
#include <iostream>     // std::cout
#include <algorithm>
pos nextpos;
BoardScene::BoardScene(QObject *parent) : QGraphicsScene(parent)
{
    srand(time(NULL));
    layable = true;
    player1Brush.setColor(Qt::black);
    player1Brush.setStyle(Qt::SolidPattern);
    player2Brush.setColor(Qt::white);
    player2Brush.setStyle(Qt::SolidPattern);
    outlinePen.setColor(Qt::black);
    for(int i=0; i<BOARDSIZE; i++)
        for(int j=0; j<BOARDSIZE; j++)
            Board[i][j]=0;
}

QPair<uint8_t, uint8_t> BoardScene:: choose(int pn){
    _sleep(2000);
    qDebug()<<"choosing func start";
    std::srand ( unsigned ( std::time(0) ) );
    std::vector<pos> v;
    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            if (Board[y][x] == 0) {
                if (isnear(Board, x, y)) {
                    pos p;
                    p.x = x;
                    p.y = y;
                    v.push_back(p);
                }
            }
        }
    }
    std::random_shuffle(v.begin(),v.end());
    QPair<int,int> maxpos;
    maxpos.first=v[0].x;
    maxpos.second=v[0].y;
    int maxscore=maxscoring(Board,pn,v[0]);
    int tempscore;
    for(int i=0; i<v.size(); i++){
        tempscore=maxscoring(Board,pn,v[i]);
        qDebug()<<"Score:"<<QString::number(tempscore);
        if(maxscore<tempscore){
            maxscore=tempscore;
            maxpos.first=v[i].x;
            maxpos.second=v[i].y;
        }
    }
    qDebug()<<"choosing func end"<<maxpos.first<<" "<<maxpos.second;
    return maxpos;
}
void BoardScene::printing(){
    for(int i=0; i<BOARDSIZE; i++){
        for(int j=0; j<BOARDSIZE; j++){
            printf("%2d", Board[i][j]);
        }
        printf("\n");
    }
}
void BoardScene::place(int x, int y){
    if(!isLayable())
        return;
    int pn;
    QBrush brush;
    if((turn/2)%2==1){
        brush=player2Brush;
        pn=2;
    }
    else{
        brush=player1Brush;
        pn=1;
    }
    //addEllipse(-12.5+25*x, -12.5+25*y, 25, 25, outlinePen, brush);
    Board[y][x]=pn;
    if(wincheck(x,y)){
        setLayableOff();
        QMessageBox m;
        if((turn/2)%2==1){
            m.information(nullptr,"gameover","white win");
        }
        else{
            m.information(nullptr,"gameover","black win");
        }
        return;
    }
    turn++;
}
bool BoardScene::wincheck(int x, int y){
    int cnt1,cnt2,cnt3,cnt4, cnt5,cnt6,cnt7,cnt8;
    int num=Board[y][x];
    cnt1=0; cnt2=0; cnt3=0; cnt4=0; cnt5=0;cnt6=0;cnt7=0;cnt8=0;
    int posx=x;
    int posy=y;
    //x,y기준으로 우상향 검사 cnt1
    while(Board[posy][posx]==num){
        posx++;
        posy--;
        cnt1++;
        if(posx>18||posy<0||posx<0||posy>18)
            break;
    }
    cnt1--;
    posx=x;
    posy=y;
    //x,y기준으로 좌하양 검사 cnt2
    while(Board[posy][posx]==num){
        posx--;
        posy++;
        cnt2++;
        if(posx>18||posy<0||posx<0||posy>18)
            break;
    }
    cnt2--;
    posx=x;
    posy=y;
    //cnt1+cnt2가 5냐? ->자기자신빼고
    if(cnt1+cnt2==5)
        return true;
    //x,y기주으로 좌상향 검사 cnt3
    while(Board[posy][posx]==num){
        posx--;
        posy--;
        cnt3++;
        if(posx>18||posy<0||posx<0||posy>18)
            break;
    }
    cnt3--;
    posx=x;
    posy=y;
    //x,y기준으로 우하향검사cnt4
    while(Board[posy][posx]==num){
        posx++;
        posy++;
        cnt4++;
        if(posx>18||posy<0||posx<0||posy>18)
            break;
    }
    cnt4--;
    posx=x;
    posy=y;
    //cnt3+cnt4가 5냐?
    if(cnt3+cnt4==5)
        return true;
    //x,y기준으로 우검사cnt5
    while(Board[posy][posx]==num){
        posx++;
        cnt5++;
        if(posx>18||posy<0||posx<0||posy>18)
            break;
    }
    cnt5--;
    posx=x;
    posy=y;
    //x,y기준으로 좌검사cnt6
    while(Board[posy][posx]==num){
        posx--;
        cnt6++;
        if(posx>18||posy<0||posx<0||posy>18)
            break;
    }
    cnt6--;
    posx=x;
    posy=y;
    //cnt3+cnt4가 5냐?
    if(cnt5+cnt6==5)
        return true;
    //x,y기준으로 아래검사cnt7
    while(Board[posy][posx]==num){
        posy++;
        cnt7++;
        if(posx>18||posy<0||posx<0||posy>18)
            break;
    }
    cnt7--;
    posx=x;
    posy=y;
    //x,y기준으로 위검사cnt8
    while(Board[posy][posx]==num){
        posy--;
        cnt8++;
        if(posx>18||posy<0||posx<0||posy>18)
            break;
    }
    cnt8--;
    //cnt3+cnt4가 5냐?
    if(cnt7+cnt8==5)
        return true;
    return false;
}
void BoardScene::mousepress(QGraphicsSceneMouseEvent* event){
    if(!isLayable())
        return;
    QPointF p = event->scenePos();
    p.setX(int((p.x()+12.5) / 25) * 25);
    p.setY(int((p.y()+12.5) / 25) * 25);
    uint8_t x=uint8_t(p.x()/25);
    uint8_t y=uint8_t(p.y()/25);
    if (Board[y][x]!=0){
        qDebug()<<"Not layable";
        return;
    }
    qDebug()<<turn;
    place(x,y);
    emit clickedBoard(uint8_t(p.x()/25), uint8_t(p.y()/25));
}

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mousepress(event);
}
