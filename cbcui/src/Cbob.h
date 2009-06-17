/**************************************************************************
 *  Copyright 2008,2009 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of CBC Firmware.                                    *
 *                                                                        *
 *  CBC Firmware is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  CBC Firmware is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this copy of CBC Firmware.  Check the LICENSE file         *
 *  in the project root.  If not, see <http://www.gnu.org/licenses/>.     *
 **************************************************************************/

#ifndef __CBOB_H__
#define __CBOB_H__

#include "cbob_struct.h"
#include <QThread>
#include <QBuffer>
#include <QMutexLocker>
#include <QSound>

#include "SharedMem.h"
#include "cbc_data.h"

#define CBOB_DEVICE "/dev/cbob"
#define SPI_POLL_DELAY 10000

class Cbob : public QThread
{
    Q_OBJECT

public:
    Cbob(QObject *parent = 0);
    ~Cbob();

    void stop();

    //int readSerial(int uart, char *data, int count);
    //int writeSerial(int uart, char *data, int count);
    /*
    void setGain(int motor, int gain, int value);
    void setVelocity(int motor, int velocity);
    void setPosition(int motor, int position);

    int getVelocity(int motor);
    int getPWM(int motor);
    int getPosition(int motor);
    int getDigitals();
    int getAnalog(int port);*/
signals:
    int refresh();

protected:
    void run();

    void sync();
    void syncWrite();
    void exchange();

    void updateFifos();

    void exchangeSharedData();
    void initSharedData();

private:
    volatile Cbob_Data m_cbobData[2];
    volatile int m_cbobOut, m_cbobIn, m_quit;
    int m_uart0Tx, m_uart0Rx;
    int m_uart1Tx, m_uart1Rx;
    int m_cbobFd;

    QSound m_beep;

    QBuffer m_uart0Buffer;
    QBuffer m_uart1Buffer;

//  QMutex m_uart0Mutex;
//  QMutex m_uart1Mutex;
//  QMutex m_cbobMutex;

    SharedMem<cbc_data> m_sharedData;
};

#endif
