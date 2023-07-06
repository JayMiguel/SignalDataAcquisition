#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
#include "easyusb_card_dll.h"
#include <QVector>
#include <iomanip>  // 包含头文件以使用 std::setprecision

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (threadDAQ->isRunning())
    {
        threadDAQ->stopThread();   //结束线程
    }

    if (threadShow->isRunning())
    {
        threadShow->stopThread();   //结束线程
    }

    QMainWindow::closeEvent(event);
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMinimumSize(1000, 1000);

    //关联到button按钮中
    ui->zoomButton->setDefaultAction(ui->actZoomIn);
    ui->shrinkButton->setDefaultAction(ui->actZoomcut);
    ui->originaiButton->setDefaultAction(ui->actZoomReset);
    ui->zoomButton2->setDefaultAction(ui->actZoomIn2);
    ui->shrinkButton2->setDefaultAction(ui->actZoomcut2);
    ui->originaiButton2->setDefaultAction(ui->actZoomReset2);
    ui->Thread_Run->setDefaultAction(ui->actThread_Run);
    ui->Thread_Quit->setDefaultAction(ui->actThread_Quit);

    Chart_Init();
    Chart_Init2();

    threadDAQ = new TDaqThread(this);        //数据采集线程
    threadShow = new TProcessThread(this);   //数据处理线程
    threadUpdate = new TUpdateThread(this);   //图表更新线程

    connect(threadDAQ, &TDaqThread::started, this, &MainWindow::do_threadA_started);
    connect(threadDAQ, &TDaqThread::finished, this, &MainWindow::do_threadA_finished);
    connect(threadUpdate, &TUpdateThread::updateChart, this, &MainWindow::onUpdateChart);
    connect(this, &MainWindow::updateFinished, threadUpdate, &TUpdateThread::onUpdateFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Chart_Init()
{
    //初始化图表的实例
    chart1 = new QChart();

    //初始化曲线的实例
    lineSeries1 = new QSplineSeries();

    // 设置曲线的名称
    lineSeries1->setName("随机数测试曲线");

    // 把曲线添加到图表中
    chart1->addSeries(lineSeries1);

    // 声明并初始化X轴、Y轴
    axisX1 = new QValueAxis();
    axisY1 = new QValueAxis();

    // 设置坐标轴显示的范围
    axisX1->setMin(0);
    axisX1->setMax(MAX_X);
    axisY1->setMin(-MAX_Y);
    axisY1->setMax(MAX_Y);

    // 设置坐标轴上的格点
    axisX1->setTickCount(10);
    axisY1->setTickCount(10);

    // 设置坐标轴显示的名称
    QFont font("Microsoft YaHei", 8, QFont::Normal);//微软雅黑。字体大小8
    axisX1->setTitleFont(font);
    axisY1->setTitleFont(font);
    axisX1->setTitleText("X-Test");
    axisY1->setTitleText("Y-Test");

    // 设置网格不显示
    axisY1->setGridLineVisible(true);

    // 图表添加坐标轴
    chart1->addAxis(axisX1, Qt::AlignBottom);
    chart1->addAxis(axisY1, Qt::AlignLeft);

    // 把曲线关联到坐标轴
    lineSeries1->attachAxis(axisX1);
    lineSeries1->attachAxis(axisY1);

    // 把图表添加到图表视图
    ui->chartView->setChart(chart1);
    ui->chartView->setRenderHint(QPainter::Antialiasing);      // 设置渲染：抗锯齿，如果不设置那么曲线就显得不平滑

}
void MainWindow::Chart_Init2()
{
    // 初始化图表的实例
    chart2 = new QChart();

    // 初始化曲线的实例
    lineSeries2 = new QSplineSeries();
    lineSeries2->setName("随机数测试曲线");

    // 把曲线添加到图标中
    chart2->addSeries(lineSeries2);

    // 初始化X轴和Y轴
    axisX2 = new QValueAxis();
    axisY2 = new QValueAxis();

    // 设置坐标轴显示的范围
    axisX2->setMin(0);
    axisX2->setMax(MAX_X);
    axisY2->setMin(-MAX_Y);
    axisY2->setMax(MAX_Y);

    // 设置坐标轴上的格点
    axisX2->setTickCount(10);
    axisY2->setTickCount(10);

    // 设置坐标轴显示的名称
    QFont font("Microsoft YaHei", 8, QFont::Normal);//微软雅黑。字体大小8
    axisX2->setTitleFont(font);
    axisY2->setTitleFont(font);
    axisX2->setTitleText("时间");
    axisY2->setTitleText("电压");

    // 设置网格不显示
    axisY2->setGridLineVisible(true);

    // 设置坐标轴的位置
    chart2->addAxis(axisX2, Qt::AlignBottom);
    chart2->addAxis(axisY2, Qt::AlignLeft);

    // 把曲线关联到坐标轴
    lineSeries2->attachAxis(axisX2);
    lineSeries2->attachAxis(axisY2);

    // 把图表添加到图表视图
    ui->chartView2->setChart(chart2);
}

void MainWindow::on_actZoomReset_triggered()
{
    //恢复原始大小
    ui->chartView->chart()->zoomReset();
}

// 点击放大按钮
void MainWindow::on_actZoomIn_triggered()
{
    // 放大20%
    ui->chartView->chart()->zoom(1.2);
}

// 点击缩小按钮
void MainWindow::on_actZoomcut_triggered()
{
    // 缩小20%
    ui->chartView->chart()->zoom(0.8);
}


void MainWindow::on_comboDragMode_currentIndexChanged(int index)
{
    // 设置拖动模式dragMode
    ui->chartView->setDragMode(QGraphicsView::DragMode(index));
}

void MainWindow::on_comboRubberBand_currentIndexChanged(int index)
{
    // 设置框选方式, rubberBand
    ui->chartView->setCustomZoomRect(index == 4); // 是否自定义模式

    QFlags<QChartView::RubberBand> flags = QChartView::NoRubberBand; // 默认为禁用橡皮筋行为

    switch (index)
    {
    case 0:
        ui->chartView->setRubberBand(QChartView::NoRubberBand);
        break;
    case 1:
        flags |= QChartView::VerticalRubberBand;    // 垂直方向选择
        break;
    case 2:
        flags |= QChartView::HorizontalRubberBand;  // 水平方向选择
        break;
    case 3:
    case 4:
        flags |= QChartView::RectangleRubberBand;   // 矩形框选
        break;
    }

    ui->chartView->setRubberBand(flags);
}
void MainWindow::on_actZoomReset2_triggered()
{
    //电压恢复原始大小
    ui->chartView2->chart()->zoomReset();
}
void MainWindow::on_actZoomIn2_triggered()
{
    //电压放大
    ui->chartView2->chart()->zoom(1.2);
}
void MainWindow::on_actZoomcut2_triggered()
{
    ui->chartView2->chart()->zoom(0.8);
}

// 点击开始按钮
void MainWindow::on_beginButton_clicked()
{
    float adv;
    int result;

    // 开启设备
    result = OpenUsbV20();
    if (result == 0)
    {
        qDebug() << tr("开启设备成功");

        // 单通道采集一个数据---
        DASingleOutV20(0, 0.6 / 4095 * 3.3);
        ADSingleV20(0, &adv);

        ui->ad->setText(QString("%1").arg(adv));
    }
    else
    {
        qDebug() << tr("开启设备失败");
    }
}

void MainWindow::on_beginButton_2_clicked()
{
    // 关闭设备
    CloseUsbV20();
}

//与线程的started()信号关联
void MainWindow::do_threadA_started()
{
    ui->actThread_Run->setEnabled(false);
    ui->actThread_Quit->setEnabled(true);
}

//与线程的finished()信号关联
void MainWindow::do_threadA_finished()
{
    ui->actThread_Run->setEnabled(true);
    ui->actThread_Quit->setEnabled(false);
}

void MainWindow::onUpdateChart(QSplineSeries* lineSeries1, QSplineSeries* lineSeries2)
{
    chart1->removeAllSeries();
    chart1->addSeries(lineSeries1);
    lineSeries1->setName("随机数测试曲线");

    // 把曲线关联到坐标轴
    lineSeries1->attachAxis(axisX1);
    lineSeries1->attachAxis(axisY1);

    ////////////////////////////////////////////////////////////

    chart2->removeAllSeries();
    chart2->addSeries(lineSeries2);
    lineSeries2->setName("随机数测试曲线");

    // 把曲线关联到坐标轴
    lineSeries2->attachAxis(axisX2);
    lineSeries2->attachAxis(axisY2);

    emit updateFinished();
}

void MainWindow::on_actThread_Run_triggered()
{
    //"启动线程"按钮
    threadShow->start();
    threadDAQ->start();
    threadUpdate->start();
}

void MainWindow::on_actThread_Quit_triggered()
{
    //"结束线程"按钮
    threadShow->stopThread();
    threadDAQ->stopThread();
    threadUpdate->stopThread();
}
