#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new BoardScene(this);
    scene->setSceneRect(-12.5, -12.5, 500-25, 500-25);
    ui->gvConnect6Board->setScene(scene);
    QBrush boardBrush(QColor(200, 150, 50));
    QBrush brush(Qt::black);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(1);
    scene->addRect(-12.5, -12.5, 500-25, 500-25, outlinePen, boardBrush);
    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        for (int j = 0; j < BOARD_SIZE - 1; j++) {
            scene->addRect(25*i, 25*j, 25, 25, outlinePen, boardBrush);
        }
    }
    scene->setLayableOff();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::on_pushButton_3_clicked()
{
    QString ip=ui->textEdit->toPlainText();
    if(ip==""){
        return;
    }
    MultiPlay *multi=new MultiPlay(scene,ui->textEdit,ui->label_2);
    multi->setAddr(ip,8089);
    multi->play();
}

