#include "multiplay.h"
#include <QMessageBox>
MultiPlay::MultiPlay()
{
    MultiPlay(nullptr, nullptr,nullptr);
}

MultiPlay::MultiPlay(BoardScene *scene, QTextEdit *msg, QLabel* lbl)
{
    this->scene = scene;
    this->msg = msg;
    this->lbl=lbl;
    socket = new QTcpSocket(this);
    state = GAME_NOT_STARTED;
    payload_ptr = payload;
    payload_len = 0;
}

MultiPlay::~MultiPlay()
{
    delete socket;
}

void MultiPlay::setAddr(QString addr, quint16 port)
{
    this->addr = addr;
    this->port = port;
}

void MultiPlay::play()
{
    state = GAME_NOT_STARTED;
    payload_ptr = payload;
    payload_len = 0;
    countInLayedStone = 0;

    socket->connectToHost(addr, port);

    connect(socket, SIGNAL(connected()), this, SLOT(gameStart()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyPacketRead()));

    msg->append(tr("Connecting to ") + addr + ":" + QString::number(port) + " ...");
}

void MultiPlay::gameStart()
{
    GameStartData snd_gsd = {
        0x00, 4, "Jjm"
    };

    msg->append(tr("Connected."));

    if (state == GAME_NOT_STARTED)
    {
        make_game_start_payload(payload, 1024, &payload_len, 0x00, snd_gsd);
        socket->write((const char *)payload_ptr, payload_len);
        payload_len = 0;
    }
}

void MultiPlay::readyPacketRead()
{
    Connect6ProtocolHdr hdr;
    GameStartData rcv_gsd;
    while (payload_len > 0 || socket->bytesAvailable() > 0) {
        if (payload_len == 0) {
            payload_ptr = payload;
            payload_len = socket->bytesAvailable();
            memcpy(payload_ptr, socket->readAll(), payload_len);
        }else{
            payload_ptr = payload;
            payload_len = 9;
            memcpy(payload_ptr, socket->readAll(), 9);
        }
        qDebug() << QByteArray((char*)payload_ptr, payload_len)<<" Bytes:"<<QString::number(payload_len);

        hdr_parsing(payload_ptr, payload_len, &hdr);

        switch (state) {
        case GAME_NOT_STARTED:
            if (hdr.type != GAME_START || hdr.player_num == 0) return;
            if(hdr.player_num>2||hdr.player_num<1) return;
            player_num = hdr.player_num;
            myplayer_num=hdr.player_num;
            qDebug() << "GAME_START packet received. PlayerNum: " << player_num;
            payload_ptr += PROTOCOL_HEADER_SIZE;
            payload_len -= PROTOCOL_HEADER_SIZE;
            qDebug() << game_start_data_parsing(payload_ptr, payload_len, &rcv_gsd);
            qDebug() << rcv_gsd.req_res_flag;
            qDebug() << rcv_gsd.name_length;
            other_player_name = QString::fromUtf8(rcv_gsd.name, rcv_gsd.name_length-1);
            msg->append(tr("Other Player Name: ") + other_player_name);
            lbl->setText(tr("JJM vs ")+other_player_name);
            if(hdr.player_num==1)
                msg->append(tr("Player Num: 1"));
            else
                msg->append(tr("Player Num: 2"));
            payload_ptr += hdr.data_length;
            payload_len -= hdr.data_length;
            state = GAME_STARTED;
            break;
        case GAME_STARTED:
            if(hdr.player_num<1||hdr.player_num>2)return;
            QBrush playerBrush(hdr.player_num == 1 ? Qt::black : Qt::white);
            QPen outlinePen(Qt::black);
            payload_ptr += PROTOCOL_HEADER_SIZE;
            payload_len -= PROTOCOL_HEADER_SIZE;
            PutTurnData rcv_ptd;
            GameOverData gmvr_ptd;
            if (hdr.type == PUT) {
                qDebug() << "PUT packet received.";
                qDebug() << put_turn_data_parsing(payload_ptr, payload_len, &rcv_ptd);
                qDebug() << "rcv_ptd.coord_num =" << rcv_ptd.coord_num;
                qDebug() << "rcv_ptd.x1 =" << rcv_ptd.xy[0];
                qDebug() << "rcv_ptd.y1 =" << rcv_ptd.xy[1];
                for (int i = 0; i < rcv_ptd.coord_num; i++) {
                    scene->addEllipse(25*rcv_ptd.xy[2*i]-12.5, 25*rcv_ptd.xy[2*i+1]-12.5, 25, 25, outlinePen, playerBrush);
                    scene->Board[rcv_ptd.xy[2*i+1]][rcv_ptd.xy[2*i]]=hdr.player_num;
                }
            } else if (hdr.type == TURN) {
                qDebug() << "TURN packet received.";
                qDebug() << put_turn_data_parsing(payload_ptr, payload_len, &rcv_ptd);
                qDebug() << "rcv_ptd.coord_num =" << rcv_ptd.coord_num;
                qDebug() << "rcv_ptd.x1 =" << rcv_ptd.xy[0];
                qDebug() << "rcv_ptd.y1 =" << rcv_ptd.xy[1];
                for (int i = 0; i < rcv_ptd.coord_num; i++) {
                    scene->addEllipse(25*rcv_ptd.xy[2*i]-12.5, 25*rcv_ptd.xy[2*i+1]-12.5, 25, 25, outlinePen, playerBrush);
                    scene->Board[rcv_ptd.xy[2*i+1]][rcv_ptd.xy[2*i]]=hdr.player_num;
                }
                int status=0;
                for(int i=0; i<BOARDSIZE; i++){
                    for(int j=0; j<BOARDSIZE; j++){
                        if(scene->Board[i][j]==0){
                            status=1;
                            break;
                        }
                    }
                    if(status==1)
                        break;
                }
                if(status==0)
                    return;
                playerBrush.setColor(myplayer_num == 1 ? Qt::black : Qt::white);
                scene->setLayableOn();
                QPair<int,int> position=scene->minmaxpick(myplayer_num,1);
                scene->addEllipse(25*position.first-12.5, 25*position.second-12.5, 25, 25, outlinePen, playerBrush);
                scene->Board[position.second][position.first]=myplayer_num;
                layedStoneXY[0]=position.first;
                layedStoneXY[1]=position.second;
                qDebug()<<"x: "<<position.first<<"y: "<<position.second;
                position=scene->minmaxpick(myplayer_num,0);
                scene->addEllipse(25*position.first-12.5, 25*position.second-12.5, 25, 25, outlinePen, playerBrush);
                scene->Board[position.second][position.first]=myplayer_num;
                layedStoneXY[2]=position.first;
                layedStoneXY[3]=position.second;
                qDebug()<<"x: "<<position.first<<"y: "<<position.second;
                countInLayedStone=2;
                requestToSendPUT();
            } else if(hdr.type==GAME_OVER){
                qDebug() << "GAME_OVER packet received.";
                qDebug() << game_over_data_parsing(payload_ptr, payload_len, &gmvr_ptd);
                qDebug() << hdr.player_num<<" Wins";
                QMessageBox m;
                m.information(nullptr,"GAME OVER",QString::number(hdr.player_num)+" Wins");
                scene->setLayableOff();
            } else {
                qDebug() << "Illegal packet received.";
            }
            payload_ptr += hdr.data_length;
            payload_len -= hdr.data_length;
            break;
        }
    }
    payload_ptr=payload;
}

void MultiPlay::requestToSendPUT()
{
    if(!scene->isLayable())
        return;
    PutTurnData snd_ptd;

    snd_ptd.coord_num = countInLayedStone;
    for (size_t i = 0; i < 2*countInLayedStone; i++) {
        snd_ptd.xy[i] = layedStoneXY[i];
    }
    make_put_payload(payload, 1024, &payload_len, (uint8_t)myplayer_num, snd_ptd);
    qDebug() <<"Sending... " <<QByteArray((char*)payload, payload_len);
    socket->write((const char *)payload, payload_len);
    payload_len = 0;
    countInLayedStone = 0;
    socket->flush();
    scene->setLayableOff();
}
