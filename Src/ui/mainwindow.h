#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChart>
#include <QValueAxis>
#include <QLabel>
#include <QComboBox>

#include "TChartView.h"
#include "threadclasses.h"

QT_BEGIN_NAMESPACE

#define MAX_X   9
#define MAX_Y   2
#define TRUE    1
#define FALSE   0

namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    QTimer* timerDrawLine1;
    QTimer* timerDrawLine2;
    QChart* chart1;     //图表对象
    QChart* chart2;     //图表对象
    QSplineSeries* lineSeries1;
    QSplineSeries* lineSeries2;
    QTimer* timer;
    QTimer* timersend;
    //    QString     SendTextStr;
    //    QByteArray  SendTextByte;

private:
    TDaqThread* threadDAQ;    //数据采集线程
    TProcessThread* threadShow;   //数据处理线程
    QThread* worker;    // 更新图表的线程
    bool m_stop;
    void updateChart();

protected:
    void    closeEvent(QCloseEvent* event);

private slots:
    void Chart_Init();  //电流图表初始化
    void Chart_Init2();  //电压图表初始化

    void on_actZoomReset_triggered(); //电流原始大小
    void on_actZoomIn_triggered();   //电流放大
    void on_actZoomcut_triggered();  //电流缩小
    void on_comboRubberBand_currentIndexChanged(int index);
    void on_comboDragMode_currentIndexChanged(int index);
    void on_actZoomReset2_triggered();  //电压原始大小
    void on_actZoomIn2_triggered();   //电压放大
    void on_actZoomcut2_triggered();   //电压缩小
    void on_beginButton_clicked();   //测试单通道电压值
    void on_beginButton_2_clicked();

    void do_threadA_started();
    void do_threadA_finished();

    void on_actThread_Run_triggered();
    void on_actThread_Quit_triggered();


private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
