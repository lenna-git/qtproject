#include "mainwindow.h"
#include <QApplication>
#include "log4qt/propertyconfigurator.h"
#include "log4qt/basicconfigurator.h"
#include "log4qt/logger.h"

using namespace Log4Qt;
Logger* myLog;

int main(int argc, char *argv[])
{
    // 初始化Log4Qt
    Log4Qt::BasicConfigurator::configure();
    QString logConfigFilePath = "D:/code/test2/AlgCheckApplog.conf";
    Log4Qt::PropertyConfigurator::configure(logConfigFilePath);

    const char *loggername = "AlgCheckApp";
    myLog=Logger::logger(loggername);
    myLog->info("Log4Qt框架已初始化成功");
    
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
