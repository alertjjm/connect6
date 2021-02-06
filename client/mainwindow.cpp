#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Set scene in Connect6 game board graphic view
    scene = new BoardScene(this);

    scene->setSceneRect(-12.5, -12.5, 500-25, 500-25);

    ui->gvConnect6Board->setScene(scene);

    // Use brush with board color and pen
    QBrush boardBrush(QColor(200, 150, 50));
    QBrush brush(Qt::black);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(1);

    // Draw whole board
    scene->addRect(-12.5, -12.5, 500-25, 500-25, outlinePen, boardBrush);

    // Draw blocks (19 x 19)
    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        for (int j = 0; j < BOARD_SIZE - 1; j++) {
            scene->addRect(25*i, 25*j, 25, 25, outlinePen, boardBrush);
        }
    }
    /*
    scene->addEllipse(212.5, 212.5, 25, 25, outlinePen, brush);
    */
    scene->setLayableOff();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::on_pushButton_clicked()
{
    /*
    QPair<int, int> pick;
    scene->setLayableOn();
    scene->playernum=1;
    //pick=scene->choose();
    scene->place(pick.first,pick.second);
    pick=scene->choose();
    scene->place(pick.first,pick.second);
    */
}

void MainWindow::on_pushButton_2_clicked()
{
    scene->setLayableOn();
    scene->playernum=0;
}

void MainWindow::on_pushButton_3_clicked()
{
    QString ip=ui->textEdit->toPlainText();
    if(ip==""){
        return;
    }
    MultiPlay *multi=new MultiPlay(scene,ui->textEdit);
    multi->setAddr(ip,8089);
    multi->play();
}

