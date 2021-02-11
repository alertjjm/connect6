#ifndef BOARDSCENE_H
#define BOARDSCENE_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPair>
#include<random>
#include<time.h>
#define BOARDSIZE 19
class BoardScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BoardScene(QObject *parent = nullptr);
    void setLayableOn() { layable = true; }
    void setLayableOff() { layable = false; }
    bool isLayable(){return layable;}
    void setBrush1(const QColor& c) { player1Brush.setColor(c); }
    void setBrush1(Qt::GlobalColor c) { player1Brush.setColor(c); }
    void place(int x, int y);
    QPair<uint8_t, uint8_t> minmaxpick(int pn, int dols);
    int Board[BOARDSIZE][BOARDSIZE];
    QPair<uint8_t, uint8_t> choose(int pn,int dols);
    int playernum;
    void mousepress(QGraphicsSceneMouseEvent *event);
    void printing();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    bool wincheck(int x, int y);

signals:
    void clickedBoard(uint8_t x, uint8_t y);
private:
    bool layable;
    QBrush player1Brush;
    QBrush player2Brush;
    QPen outlinePen;
    int turn;
};

#endif // BOARDSCENE_H
