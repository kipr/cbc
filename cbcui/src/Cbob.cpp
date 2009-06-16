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

#include "Cbob.h"
#include "unistd.h"
#include "fcntl.h"
#include "string.h"

#include <unistd.h>
#include <errno.h>

#include <QMutex>

Cbob::Cbob(QObject *parent) : QThread(parent), m_quit(0), m_cbobFd(-1), m_uart0Rx(-1), m_uart1Tx(-1), m_uart1Rx(-1), m_uart0Tx(-1), m_sharedData("/tmp/cbc_data"), m_beep("/mnt/kiss/beep.wav")
{
    m_cbobFd = open(CBOB_DEVICE, O_RDWR);

    m_cbobOut = 0;
    m_cbobIn = 1;

    memset((void *)m_cbobData, 0, sizeof(Cbob_Data)*2);

    mkfifo("/tmp/uart0tx", 0666);
    mkfifo("/tmp/uart0rx", 0666);
    mkfifo("/tmp/uart1tx", 0666);
    mkfifo("/tmp/uart1rx", 0666);

    m_uart0Tx = open("/tmp/uart0tx", O_RDONLY | O_NONBLOCK);
    m_uart1Tx = open("/tmp/uart1tx", O_RDONLY | O_NONBLOCK);

}

Cbob::~Cbob()
{
    stop();

    unlink("/tmp/uart0tx");
    unlink("/tmp/uart0rx");
    unlink("/tmp/uart1tx");
    unlink("/tmp/uart1rx");

    if (m_cbobFd > 0) {
        close(m_cbobFd);
        m_cbobFd = -1;
    }
}

void Cbob::run()
{
    if (m_cbobFd < 0)
        return;

    exchange();
    initSharedData();

    while (!m_quit) {
        exchange();

        //qWarning(".");
        updateFifos();
        exchangeSharedData();
        emit refresh();

        usleep(SPI_POLL_DELAY); // FIXME: Slow update for testing
    }
}

void Cbob::sync()
{
    uint32 inKey = 0;
    uint8 *inKeyPtr;


    inKeyPtr = (uint8 *)&inKey;

    while (inKey != 0xDEADBEEF) {
        uint8 byte = 0;

        int result = read(m_cbobFd, &byte, 1);

        inKeyPtr[0] = inKeyPtr[1];
        inKeyPtr[1] = inKeyPtr[2];
        inKeyPtr[2] = inKeyPtr[3];
        inKeyPtr[3] = byte;
    }
}

void Cbob::syncWrite()
{
    uint32 inKey = 0;
    uint8 *inKeyPtr;

    uint32 outKey = 0xDEADBEEF;
    uint8 *outKeyPtr;

    inKeyPtr = (uint8 *)&inKey;
    outKeyPtr = (uint8 *)&outKey;

    while (inKey != 0xDEADBEEF) {
        uint8 byte = *outKeyPtr;

        int result = read(m_cbobFd, &byte, 1);

        inKeyPtr[0] = inKeyPtr[1];
        inKeyPtr[1] = inKeyPtr[2];
        inKeyPtr[2] = inKeyPtr[3];
        inKeyPtr[3] = byte;


        if (byte == *outKeyPtr)
            outKeyPtr++;
        else
            outKeyPtr = (uint8 *)&outKey;
    }
}

void Cbob::exchange()
{

    if (m_cbobData[m_cbobOut].key == 0xDEADBEEF) {
        syncWrite();
        m_cbobIn ^= 1;
        m_cbobOut ^= 1;

    }
    else sync();

    int result = read(m_cbobFd, (void *)((uint32)&(m_cbobData[m_cbobIn])+4), sizeof(Cbob_Data) - 4);

    m_cbobData[m_cbobIn].key = 0;
    return;
}

void Cbob::stop()
{
    m_quit = 1;

    while (isRunning());
}

void Cbob::updateFifos()
{
    int  count, total_out = 0;
    if (m_uart0Rx < 0) {
        m_uart0Rx = open("/tmp/uart0rx", O_WRONLY | O_NONBLOCK);
    }
    if (m_uart1Rx < 0) {
        m_uart1Rx = open("/tmp/uart1rx", O_WRONLY | O_NONBLOCK);
    }

    if (m_uart0Rx > 0) {
        count = write(m_uart0Rx, (void *)m_cbobData[m_cbobIn].UART0_Buffer, m_cbobData[m_cbobIn].UART0_Buffer_Count);
        if (count < 0 && !(count == -EAGAIN)) {
            close(m_uart0Rx);
            m_uart0Rx = -1;
        }
    }

    if (m_uart1Rx > 0) {
        count = write(m_uart1Rx, (void *)m_cbobData[m_cbobIn].UART1_Buffer, m_cbobData[m_cbobIn].UART1_Buffer_Count);
        if (count < 0 && !(count == -EAGAIN)) {
            close(m_uart1Rx);
            m_uart1Rx = -1;
        }
    }

    m_cbobData[m_cbobIn].UART0_Buffer_Count = 0;
    m_cbobData[m_cbobIn].UART1_Buffer_Count = 0;

    count = read(m_uart0Tx, (void *)m_cbobData[m_cbobOut].UART0_Buffer, 48);
    if (count > 0) {
        m_cbobData[m_cbobOut].UART0_Buffer_Count = count;
        total_out += count;
    }
    else
        m_cbobData[m_cbobOut].UART0_Buffer_Count = 0;

    count = read(m_uart1Tx, (void *)m_cbobData[m_cbobOut].UART1_Buffer, 48);
    if (count > 0) {
        m_cbobData[m_cbobOut].UART1_Buffer_Count = count;
        total_out += count;
    }
    else
        m_cbobData[m_cbobOut].UART1_Buffer_Count = 0;

    if (total_out) m_cbobData[m_cbobOut].key = 0xDEADBEEF;
}

void Cbob::initSharedData()
{
    int i, j;

    m_sharedData.shared().enable_digital_outputs = m_cbobData[m_cbobOut].Enable_Digital_Outputs;
    m_sharedData.shared().digital_output_values = m_cbobData[m_cbobOut].Digital_Output_Values;

    for (i = 0;i < 3;i++)
        m_sharedData.shared().motor_thresholds[i] = m_cbobData[m_cbobOut].Motor_Thresholds[i];

    for (i = 0;i < 4;i++) {
        for (j = 0;j < 6;j++) {
            m_sharedData.shared().pid_gains[i][j] = m_cbobData[m_cbobOut].Motor_PID_Gains[i][j];
        }
    }

    for (i = 0;i < 4;i++)
        m_sharedData.shared().motor_pwm[i] = m_cbobData[m_cbobOut].Motor_PWM[i];

    for (i = 0;i < 4;i++)
        m_sharedData.shared().motor_tps[i] = m_cbobData[m_cbobOut].Motor_TPS_Targets[i];

    for (i = 0;i < 4;i++)
        m_sharedData.shared().motor_counter_targets[i] = m_cbobData[m_cbobOut].Motor_Counter_Targets[i];

    m_sharedData.shared().clear_motor_counters = m_cbobData[m_cbobOut].Motor_Clear_Counter;

    for (i = 0;i < 4;i++)
        m_sharedData.shared().servo_targets[i] = m_cbobData[m_cbobOut].Servo_Position[i];

    m_sharedData.shared().enable_servos = m_cbobData[m_cbobOut].Enable_Servos;


    /* This stuff is incoming */
    for (i = 0;i < 8;i++)
        m_sharedData.shared().analogs[i] = m_cbobData[m_cbobIn].Analog[i];

    m_sharedData.shared().digitals = m_cbobData[m_cbobIn].Digitals;

    m_sharedData.shared().acc_x = m_cbobData[m_cbobIn].Acc_X;
    m_sharedData.shared().acc_y = m_cbobData[m_cbobIn].Acc_Y;
    m_sharedData.shared().acc_z = m_cbobData[m_cbobIn].Acc_Z;

    m_sharedData.shared().volts = m_cbobData[m_cbobIn].Batt_Voltage;

    m_sharedData.shared().button = m_cbobData[m_cbobIn].ButtonState;

    m_sharedData.shared().motor_in_motion = m_cbobData[m_cbobIn].Motor_In_Motion;

    for (i = 0;i < 4;i++)
        m_sharedData.shared().motor_speed[i] = m_cbobData[m_cbobIn].Motor_TPS[i];

    for (i = 0;i < 4;i++)
        m_sharedData.shared().motor_counter[i] = m_cbobData[m_cbobIn].Motor_Counter[i];
}

void Cbob::exchangeSharedData()
{
    int i, j;

    /* This stuff is outgoing */
    m_cbobData[m_cbobOut].key = 0xDEADBEEF;
    m_cbobData[m_cbobOut].Enable_Digital_Outputs = m_sharedData.shared().enable_digital_outputs;
    m_cbobData[m_cbobOut].Digital_Output_Values = m_sharedData.shared().digital_output_values;

    for (i = 0;i < 3;i++)
        m_cbobData[m_cbobOut].Motor_Thresholds[i] = m_sharedData.shared().motor_thresholds[i];

    for (i = 0;i < 4;i++) {
        for (j = 0;j < 6;j++) {
            m_cbobData[m_cbobOut].Motor_PID_Gains[i][j] = m_sharedData.shared().pid_gains[i][j];
        }
    }

    for (i = 0;i < 4;i++)
        m_cbobData[m_cbobOut].Motor_PWM[i] = m_sharedData.shared().motor_pwm[i];

    for (i = 0;i < 4;i++)
        m_cbobData[m_cbobOut].Motor_TPS_Targets[i] = m_sharedData.shared().motor_tps[i];

    for (i = 0;i < 4;i++)
        m_cbobData[m_cbobOut].Motor_Counter_Targets[i] = m_sharedData.shared().motor_counter_targets[i];

    m_cbobData[m_cbobOut].Motor_Clear_Counter = m_sharedData.shared().clear_motor_counters;
    m_sharedData.shared().clear_motor_counters = 0;

    for (i = 0;i < 4;i++)
        m_cbobData[m_cbobOut].Servo_Position[i] = m_sharedData.shared().servo_targets[i];

    m_cbobData[m_cbobOut].Enable_Servos = m_sharedData.shared().enable_servos;


    /* This stuff is incoming */
    for (i = 0;i < 8;i++)
        m_sharedData.shared().analogs[i] = m_cbobData[m_cbobIn].Analog[i];

    m_sharedData.shared().digitals = m_cbobData[m_cbobIn].Digitals ^ 0xff;

    m_sharedData.shared().acc_x = m_cbobData[m_cbobIn].Acc_X;
    m_sharedData.shared().acc_y = m_cbobData[m_cbobIn].Acc_Y;
    m_sharedData.shared().acc_z = m_cbobData[m_cbobIn].Acc_Z;

    m_sharedData.shared().volts = m_cbobData[m_cbobIn].Batt_Voltage;

    m_sharedData.shared().button = m_cbobData[m_cbobIn].ButtonState;

    m_sharedData.shared().motor_in_motion = m_cbobData[m_cbobIn].Motor_In_Motion;

    for (i = 0;i < 4;i++)
        m_sharedData.shared().motor_speed[i] = m_cbobData[m_cbobIn].Motor_TPS[i];

    for (i = 0;i < 4;i++)
        m_sharedData.shared().motor_counter[i] = m_cbobData[m_cbobIn].Motor_Counter[i];

    if (m_sharedData.shared().tone_freq) {
        m_beep.play();
        m_sharedData.shared().tone_freq = 0;
    }
}


