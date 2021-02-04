#include "boardscene.h"
#include<QMessageBox>
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
    Board[BOARDSIZE/2][BOARDSIZE/2]=1;
    turn=2;
}
QPair<int, int> BoardScene:: choose(){
    QPair<int,int> pos;
    pos.first=rand()%19;
    pos.second=rand()%19;
    while(Board[pos.first][pos.second]!=0){
        pos.first=rand()%19;
        pos.second=rand()%19;
    }
    return pos;
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
    addEllipse(-12.5+25*x, -12.5+25*y, 25, 25, outlinePen, brush);
    Board[y][x]=pn;
    if(wincheck(x,y)){
        setLayableOff();
        QMessageBox m;
        if((turn/2)%2==1)
            m.information(nullptr,"게임 끝","흰 승리.");
        else
            m.information(nullptr,"게임 끝","흑 승리.");
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
    posx=x;
    posy=y;
    //cnt3+cnt4가 5냐?
    if(cnt7+cnt8==5)
        return true;
    return false;
}
void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPair<int, int> pick;
    if(!isLayable())
        return;
    QPointF p = event->scenePos();
    p.setX(int((p.x()+12.5) / 25) * 25);
    p.setY(int((p.y()+12.5) / 25) * 25);
    uint8_t x=uint8_t(p.x()/25);
    uint8_t y=uint8_t(p.y()/25);
    if (Board[y][x]!=0) return;
    //qDebug() << p.x()/25 << p.y()/25;
    qDebug()<<turn;
    place(x,y);
    if((turn/2)%2==playernum){
        pick=choose();
        place(pick.first,pick.second);
        pick=choose();
        place(pick.first,pick.second);
    }
    emit clickedBoard(uint8_t(p.x()/25), uint8_t(p.y()/25));
}
