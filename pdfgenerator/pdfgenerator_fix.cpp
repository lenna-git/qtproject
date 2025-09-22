// 这个文件包含了修复编译错误的补丁

// 修复编译错误：添加generateReportPDFWithProjects函数作为generateReportPDFWith6Projects的别名
// 这样可以解决编译错误，同时保持向后兼容性

#include "pdfgenerator.h"
#include <QFontDatabase>
#include <QDebug>
#include"log4qt/logger.h"
#include"log4qt/log4qt.h"

using Log4Qt::Logger;
extern Logger* myLog;

// 添加缺失的函数，作为generateReportPDFWith6Projects的别名
bool PDFGenerator::generateReportPDFWithProjects(const QString &fileName, const ReportContent &content, QWidget *parent)
{
    myLog->warn("警告：调用了已弃用的函数generateReportPDFWithProjects，正在重定向到generateReportPDFWith6Projects");
    // 直接调用正确的函数
    return generateReportPDFWith6Projects(fileName, content, parent);
}
