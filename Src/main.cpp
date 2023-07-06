#include "mainwindow.h"
#include "CCreateDump.h"
#include "threadclasses.h"

#include <QApplication>
#include <QMutex>

static QMutex g_mutex;

void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QDateTime _datetime = QDateTime::currentDateTime();
    QString szDate = _datetime.toString("yyyy-MM-dd hh:mm:ss.zzz");//"yyyy-MM-dd hh:mm:ss ddd"
    QString txt(szDate);

    switch (type)
    {
    case QtDebugMsg://调试信息提示
    {
        txt += QString(" [Debug] ");
        break;
    }
    case QtInfoMsg://信息输出
    {
        txt += QString(" [Info] ");
        break;
    }
    case QtWarningMsg://一般的warning提示
    {
        txt += QString(" [Warning] ");
        break;
    }
    case QtCriticalMsg://严重错误提示
    {
        txt += QString(" [Critical] ");
        break;
    }
    case QtFatalMsg://致命错误提示
    {
        txt += QString(" [Fatal] ");
        //abort();
        break;
    }
    default:
    {
        txt += QString(" [Trace] ");
        break;
    }
    }

    txt.append(QString(" %1").arg(context.file));
    txt.append(QString("<%1>: ").arg(context.line));
    txt.append(msg);

    g_mutex.lock();
    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << txt << "\r\n";
    file.close();
    g_mutex.unlock();
}

int main(int argc, char *argv[])
{
    // 创建dmp文件
    std::string strDumPath = "dmp";
    CCreateDump::Instance()->DeclarDumpFile(strDumPath);

    QApplication a(argc, argv);

    //注册MessageHandler
    qInstallMessageHandler(customMessageHandler);

    MainWindow w;
    w.show();

    return a.exec();
}
