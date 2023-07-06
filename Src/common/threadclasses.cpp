#include "threadclasses.h"
#include <QDebug>


QList<float*> g_bufList; // 缓冲区列表
QList<std::vector<float>> Voltage_Queue;
QList<std::vector<float>> Current_Queue;

TDaqThread::TDaqThread(QObject *parent): QThread(parent), m_stop(false)
{

}

void TDaqThread::stopThread()
{
    m_stop = true;
}

void TDaqThread::run()
{
    m_stop = false;
    while (!m_stop)
    {
        // 每次开始读取都新建一个缓冲区
        float* buffer = new float[BUF_SIZE];

        static int sampleRate = 10000;

        int result = OpenUsbV20();
        if (result != 0) 
        {
            qDebug() << tr("数据采集卡打开失败");
            return;
        }
        else
        {
            qDebug() << tr("数据采集卡打开成功");
        }

        // 读取数据到缓冲区
        int madResult = MADContinuV20(0, 1, BUF_SIZE, sampleRate, buffer);
        if (madResult == -1)
        {
            qDebug() << tr("多通道连续采集失败");
        }

        g_bufList.append(buffer);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

TProcessThread::TProcessThread(QObject *parent) : QThread(parent), m_stop(false)
{

}

void TProcessThread::run()
{
    float bufData[BUF_SIZE];
    std::vector<float> Voltage_Data;  // 存储偶数索引位置的元素的数组
    std::vector<float> Current_Data;   // 存储奇数索引位置的元素的数组

    m_stop = false;

    qDebug() << tr("开始处理数据");

    while (!m_stop)
    {
        if (g_bufList.size() == 0)
        {
            QThread::msleep(100);
            continue;
        }
        //获取已满缓冲区的指针
        float* bufferFull = g_bufList.takeFirst();    

        for (int i = 0; i < BUF_SIZE; i++, bufferFull++)         //数据处理
        {
            bufData[i] = *bufferFull;
        }

        for (int i = 0; i < BUF_SIZE; i++) {
            if (i % 2 == 0) {
                // 偶数索引，存储到 Voltage_Data
                Voltage_Data.push_back(bufData[i]);
            }
            else {
                // 奇数索引，存储到 Current_Data
                Current_Data.push_back(bufData[i]);
            }
        }

        Voltage_Queue.push_back(Voltage_Data);
        Current_Queue.push_back(Current_Data);
    }
}

void TProcessThread::stopThread()
{
    m_stop = true;
}