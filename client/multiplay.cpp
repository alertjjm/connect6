#include "multiplay.h"
#include <Windows.h>
#include <QMessageBox>
MultiPlay::MultiPlay()
{
    MultiPlay(nullptr, nullptr);
}

MultiPlay::MultiPlay(BoardScene *scene, QTextEdit *msg)
{
    this->scene = scene;
    this->msg = msg;
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
    // Init
    state = GAME_NOT_STARTED;
    payload_ptr = payload;
    payload_len = 0;
    countInLayedStone = 0;

    // Connect
    socket->connectToHost(addr, port);

    connect(socket, SIGNAL(connected()), this, SLOT(gameStart()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyPacketRead()));
    connect(scene, SIGNAL(clickedBoard(uint8_t, uint8_t)),
            this, SLOT(clickedBoard(uint8_t, uint8_t)));

    msg->append(tr("Connecting to ") + addr + ":" + QString::number(port) + " ...");
}

void MultiPlay::gameStart()
{
    // TCP Socket variables for connection
    GameStartData snd_gsd = {
        0x00, 4, "Jjm"
    };

    msg->append(tr("Connected."));

    if (state == GAME_NOT_STARTED)
    {
        // Make GAME_START payload for starting
        make_game_start_payload(payload, 1024, &payload_len, 0x00, snd_gsd);

        // Send GAME_START
        socket->write((const char *)payload_ptr, payload_len);

        payload_len = 0;
    }
}

void MultiPlay::readyPacketRead()
{
    Connect6ProtocolHdr hdr;
    GameStartData rcv_gsd;
    msg->append(tr("Packet received"));
    // Behaviors by state
    while (payload_len > 0 || socket->bytesAvailable() > 0) {

        // If no remained payload
        if (payload_len == 0) {
            // Assign payload pointer and length
            payload_ptr = payload;
            payload_len = socket->bytesAvailable();
            memcpy(payload_ptr, socket->readAll(), payload_len);
        }else{
            // Assign payload pointer and length
            payload_ptr = payload;
            payload_len = 9;
            memcpy(payload_ptr, socket->readAll(), 9);
        }
        qDebug() << QByteArray((char*)payload_ptr, payload_len)<<" Bytes:"<<QString::number(payload_len);

        // Header parsing
        hdr_parsing(payload_ptr, payload_len, &hdr);

        switch (state) {

        case GAME_NOT_STARTED:

            // Not GAME_START -> exit
            if (hdr.type != GAME_START || hdr.player_num == 0) return;
            if(hdr.player_num>2||hdr.player_num<1) return;

            // Get PlayerNum
            player_num = hdr.player_num;
            myplayer_num=hdr.player_num;
            qDebug() << "GAME_START packet received. PlayerNum: " << player_num;

            // Pointer to data field
            payload_ptr += PROTOCOL_HEADER_SIZE;
            payload_len -= PROTOCOL_HEADER_SIZE;

            // GAME_START data parsing
            qDebug() << game_start_data_parsing(payload_ptr, payload_len, &rcv_gsd);

            qDebug() << rcv_gsd.req_res_flag;
            qDebug() << rcv_gsd.name_length;

            // Get other player's name
            other_player_name = QString::fromUtf8(rcv_gsd.name, rcv_gsd.name_length-1);

            msg->append(tr("Other Player Name: ") + other_player_name);
            if(hdr.player_num==1)
                msg->append(tr("Player Num: 1"));
            else
                msg->append(tr("Player Num: 2"));

            // Pointer to end
            payload_ptr += hdr.data_length;
            payload_len -= hdr.data_length;

            // state becomes 'game started'
            state = GAME_STARTED;

            break;

        case GAME_STARTED:
            if(hdr.player_num<1||hdr.player_num>2)return;
            QBrush playerBrush(hdr.player_num == 1 ? Qt::black : Qt::white);
            QPen outlinePen(Qt::black);

            // Pointer to data field
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
                msg->append(tr("TURN packet received"));

                qDebug() << put_turn_data_parsing(payload_ptr, payload_len, &rcv_ptd);
                qDebug() << "rcv_ptd.coord_num =" << rcv_ptd.coord_num;
                qDebug() << "rcv_ptd.x1 =" << rcv_ptd.xy[0];
                qDebug() << "rcv_ptd.y1 =" << rcv_ptd.xy[1];

                for (int i = 0; i < rcv_ptd.coord_num; i++) {
                    scene->addEllipse(25*rcv_ptd.xy[2*i]-12.5, 25*rcv_ptd.xy[2*i+1]-12.5, 25, 25, outlinePen, playerBrush);
                    scene->Board[rcv_ptd.xy[2*i+1]][rcv_ptd.xy[2*i]]=hdr.player_num;
                }

                playerBrush.setColor(myplayer_num == 1 ? Qt::black : Qt::white);
                scene->setLayableOn();
                QPair<int,int> position=scene->choose(myplayer_num,1);
                scene->addEllipse(25*position.first-12.5, 25*position.second-12.5, 25, 25, outlinePen, playerBrush);
                scene->Board[position.second][position.first]=myplayer_num;
                layedStoneXY[0]=position.first;
                layedStoneXY[1]=position.second;
                qDebug()<<"x: "<<position.first<<"y: "<<position.second;
                position=scene->choose(myplayer_num,0);
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

            // Pointer to end
            payload_ptr += hdr.data_length;
            payload_len -= hdr.data_length;

            break;
        }
    }
    payload_ptr=payload;
}

void MultiPlay::clickedBoard(uint8_t x, uint8_t y)
{
    QBrush playerBrush(player_num == 1 ? Qt::black : Qt::white);
    qDebug()<<"cl: "<<x<<" "<<y;
    scene->Board[y][x]=player_num;
    QPen outlinePen(Qt::black);

    scene->addEllipse(25*x-12.5, 25*y-12.5, 25, 25, outlinePen, playerBrush);

    layedStoneXY[countInLayedStone * 2] = x;
    layedStoneXY[countInLayedStone * 2 + 1] = y;
    countInLayedStone++;

    if (countInLayedStone >= 2) {
        requestToSendPUT();
    }
}
void MultiPlay::clickBoard(uint8_t x, uint8_t y)
{
    QBrush playerBrush(player_num == 1 ? Qt::black : Qt::white);

    QPen outlinePen(Qt::black);

    scene->addEllipse(25*x-12.5, 25*y-12.5, 25, 25, outlinePen, playerBrush);

    layedStoneXY[countInLayedStone * 2] = x;
    layedStoneXY[countInLayedStone * 2 + 1] = y;
    countInLayedStone++;

    if (countInLayedStone >= 2) {
        requestToSendPUT();
    }
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

    // Make GAME_START payload for starting
    make_put_payload(payload, 1024, &payload_len, (uint8_t)myplayer_num, snd_ptd);

    qDebug() <<"Sending... " <<QByteArray((char*)payload, payload_len);
    // Send GAME_START
    socket->write((const char *)payload, payload_len);

    payload_len = 0;

    countInLayedStone = 0;
    socket->flush();
    scene->setLayableOff();
}
