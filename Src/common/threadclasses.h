#ifndef THREADCLASSES_H
#define THREADCLASSES_H
#pragma execution_character_set("utf-8")

#include <QThread>
#include <QList>
#include <QSemaphore>

#include "easyusb_card_dll.h"

#define BUF_SIZE 1024

extern QList<std::vector<float>> Voltage_Queue;
extern QList<std::vector<float>> Current_Queue;

//TDaqThread 数据采集线程
class TDaqThread : public QThread
{
    Q_OBJECT
protected:
    bool    m_stop;
    void    run();          //线程的任务函数
public:
    explicit TDaqThread(QObject *parent = nullptr);
    void    stopThread();   //结束线程run()函数的运行
};

//TProcessThread 数据处理线程
class TProcessThread : public QThread
{
    Q_OBJECT
protected:
    bool    m_stop;
    void    run();          //线程的任务函数
public:
    explicit TProcessThread(QObject *parent = nullptr);
    void    stopThread();   //结束线程run()函数的运行
};

class TUpdateThread : public QThread
{
    Q_OBJECT
protected:
    bool    m_stop;
    bool    m_update;
    void    run();          //线程的任务函数

signals:
    void updateChart(QSplineSeries*, QSplineSeries*);

public slots:
    void onUpdateFinished();

public:
    explicit TUpdateThread(QObject* parent = nullptr);
    void    stopThread();   //结束线程run()函数的运行
};

#endif // MYTHREAD_H

