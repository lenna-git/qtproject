#include "mainwindow.h"
#include <QApplication>
#include "log4qt/log4qt.h"
#include "log4qt/basicconfigurator.h"
#include "log4qt/logger.h"

int main(int argc, char *argv[])
{
    // 初始化Log4Qt
    Log4Qt::BasicConfigurator::configure();
    
    // 获取根日志记录器
    Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
    logger->info("Log4Qt框架已初始化成功");
    
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
