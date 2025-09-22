#include "pdfgenerator.h"
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QtGlobal>
#include <cmath>
#include <QDateTime>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QTimer>
#include <QApplication>
#include <QPixmap>
#include <QTableView>
#include <QTextEdit>
#include <QLabel>
#include <QFontDatabase>
#include <QTextCodec>
#include <QTextLayout>
#include <QTextLine>
#include <QTextDocument>
#include<QStandardItemModel>
#include "../chkresultclass/chk_singleitem_result.h"
#include "../chkresultclass/chk_items_result.h"
#include"log4qt/logger.h"
#include"log4qt/log4qt.h"

using Log4Qt::Logger;
extern Logger* myLog;

PDFGenerator::PDFGenerator(QObject *parent) : QObject(parent)
{
}




// 绘制只有6个检验项目的PDF报告
bool PDFGenerator::generateReportPDFWith6Projects(const QString &fileName, const ReportContent &content, QWidget *parent)
{

    // 创建QPrinter对象并设置为PDF输出模式
    QPrinter printer(QPrinter::ScreenResolution); // 降低分辨率，提高兼容性
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // 设置页面属性
    printer.setPageSize(QPrinter::A4);  // 设置A4纸张
    printer.setOrientation(QPrinter::Portrait);  // 纵向
    printer.setFullPage(false);  // 不全页打印

    // 设置边距
    printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);

    // 创建QPainter并关联到QPrinter
    QPainter painter;
    if (!painter.begin(&printer))
    {
        myLog->warn("无法创建PDF内容");
        if (parent) {
            QMessageBox::critical(parent, "错误", "无法创建PDF内容");
            myLog->info("显示错误消息框：无法创建PDF内容");
        }
        return false;
    }

    // 获取页面的可打印区域（考虑边距）
    QRectF printableRect = printer.pageRect(QPrinter::DevicePixel);
    qreal pageWidth = printableRect.width();
    qreal pageHeight = printableRect.height();

    // 设置渲染质量
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    // 设置字体
    QFont font;
    // 尝试多种中文字体以确保兼容性
    QStringList chineseFonts = {"SimHei", "WenQuanYi Micro Hei", "Heiti TC", "Arial Unicode MS"};
    QString selectedFont;
    
    // 检查系统中可用的中文字体
    QFontDatabase fontDatabase;
    foreach (const QString &fontFamily, chineseFonts) {
        if (fontDatabase.hasFamily(fontFamily)) {
            selectedFont = fontFamily;
            break;
        }
    }
    
    // 如果没有找到指定的中文字体，使用默认字体
    if (selectedFont.isEmpty()) {
        selectedFont = "SimHei";
        myLog->info("没有找到指定的中文字体，使用默认字体");
    }
    
    font.setFamily(selectedFont);
    myLog->info(QString("选择的字体: %1").arg(selectedFont));
    painter.setFont(font);

    // 计算内容区域
    qreal margin = 10; // 进一步减小边距，最大化表格可用宽度
    qreal tableLeft = margin;
    qreal tableTop = 50; // 降低表格顶部位置，为表格提供更多垂直空间
    qreal tableWidth = pageWidth - 2 * margin;
    qreal tableHeight = pageHeight - 100; // 增加表格高度，使其占满整个页面
    qreal totalRows = 17; // 减少总行数的估算，增加行高
    qreal lineHeight = tableHeight / totalRows; // 重新计算行高，确保表格占满页面
    myLog->info(QString("页面尺寸(6项目): %1x%2").arg(pageWidth).arg(pageHeight));
    myLog->info(QString("表格尺寸(6项目): %1x%2").arg(tableWidth).arg(tableHeight));
    myLog->info(QString("行高(6项目): %1").arg(lineHeight));
    qreal col1Width = 150; // 增加第一列宽度，使其能够显示4个中文字符
    qreal col2Width = 170; // 调整第二列宽度，保持表格整体平衡
    qreal col3Width = 130; // 增加第三列宽度，使其能够显示"分组轮数"4个中文字符
    qreal col4Width = 130; // 增加第四列宽度，确保"分组轮数二"单元格中的"分"字能够完整显示

    // 先填充表格背景色
    painter.fillRect(tableLeft, tableTop, tableWidth, tableHeight, Qt::white);

    // 绘制标题
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRectF(tableLeft, tableTop - 50, tableWidth, 50), Qt::AlignCenter, content.reportTitle); // 增加标题高度从30到50，解决标题文字显示不全问题
    font.setPointSize(12);
    font.setBold(false);
    painter.setFont(font);

    // 绘制表格顶部边框
    painter.drawLine(tableLeft, tableTop, tableLeft + tableWidth, tableTop);

    // 标准表格布局 - 将算法名称单元格与其上下两个单元格合并为一个单元格
    qreal currentY = tableTop;
    
    // 绘制第一部分 - 合并算法名称和算法一的三行单元格
    // 绘制表格外边框
    painter.drawLine(tableLeft, tableTop, tableLeft, currentY + 3 * lineHeight); // 左侧边框
    painter.drawLine(tableLeft + col1Width, tableTop, tableLeft + col1Width, currentY + 3 * lineHeight); // 第一列右边框
    painter.drawLine(tableLeft + col1Width + col2Width, tableTop, tableLeft + col1Width + col2Width, currentY + 3 * lineHeight); // 第二列右边框
    painter.drawLine(tableLeft, currentY + 3 * lineHeight, tableLeft + tableWidth, currentY + 3 * lineHeight); // 底部边框
    
    // 绘制其他列的垂直分隔线，但只在各自的行内显示
    // 第一行的其他列分隔线
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第一行底部横线
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第二行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第二行底部横线
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第三行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 重置currentY到表格顶部，用于填充内容
    currentY = tableTop;
    
    // 填写算法名称 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft, currentY, col1Width, 3 * lineHeight), Qt::AlignCenter, "算法名称");
    
    // 填写算法一 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, 3 * lineHeight), Qt::AlignCenter, content.algorithmName);
    
    // 填写第一行其他列内容
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_1);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_1);
    
    // 填写第二行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_2);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_2);
    
    // 填写第三行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_3);
    // 增加内边距并调整对齐方式，确保'分组轮数二'单元格中的文字完整显示
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width + 5, currentY, col4Width - 10, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_3);
    
    currentY += lineHeight; // 增加到下一行

    // 绘制第二行 - 设计信息
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width, currentY, tableLeft + col1Width + col2Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写第二行内容
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter, "设计单位");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, lineHeight), Qt::AlignCenter, content.designUnit);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter, "算法类别");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter, content.algorithmCategory);

    // 绘制第三行 - 检验信息
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width, currentY, tableLeft + col1Width + col2Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写第三行内容
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter, "检验人员");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, lineHeight), Qt::AlignCenter, content.testPersonnel);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter, "检验时间");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter, content.testDate);

    // 绘制第四行 - 检验水平
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写第四行内容
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter, "检验水平");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, tableWidth - col1Width, lineHeight), Qt::AlignCenter, content.testLevel);

    // 绘制检验项目部分
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight * 6); // 第一列垂直分隔线（已增加到150宽度）
    painter.drawLine(tableLeft + col1Width + 80, currentY, tableLeft + col1Width + 80, currentY + lineHeight * 6); // 序号列垂直分隔线（固定宽度80）
    painter.drawLine(tableLeft + col1Width + 80 + 150, currentY, tableLeft + col1Width + 80 + 150, currentY + lineHeight * 6); // 项目名称列垂直分隔线（调整宽度到150）

    // 填写检验项目内容
    int projectCount = qMin(content.projectNames.size(), content.projectResults.size());
    projectCount = qMin(projectCount, 6); // 限制最多显示6个项目
    
    // 先绘制第一列的合并单元格文本 - 现在可以显示4个中文字符
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight * 6), Qt::AlignCenter | Qt::TextWordWrap, "检验项目");
    
    // 循环绘制6行内容
    for (int i = 0; i < 6; i++) {
        currentY += lineHeight;
        // 绘制其他三列的行分隔线（不包括检验项目列）
        painter.drawLine(tableLeft + col1Width, currentY, tableLeft + tableWidth, currentY);
        
        // 为其他列绘制垂直分隔线
        painter.drawLine(tableLeft + col1Width, currentY - lineHeight, tableLeft + col1Width, currentY);
        painter.drawLine(tableLeft + col1Width + 80, currentY - lineHeight, tableLeft + col1Width + 80, currentY);
        painter.drawLine(tableLeft + col1Width + 80 + 150, currentY - lineHeight, tableLeft + col1Width + 80 + 150, currentY);
        
        // 第二列：序号列 - 分6行，序号1-6（固定宽度80）
        painter.drawText(QRectF(tableLeft + col1Width, currentY - lineHeight, 80, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, QString::number(i + 1));
        
        // 第三列：项目名称列（调整宽度到150，保持表格整体平衡）
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width + 80, currentY - lineHeight, 150, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectNames[i]);
        }
        
        // 第四列：是否通过列（剩余宽度）
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width + 80 + 150, currentY - lineHeight, tableWidth - col1Width - 80 - 150, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectResults[i]);
        }
    }

    // 绘制检验结论行
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写检验结论
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "检验结论");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, tableWidth - col1Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.testConclusion);

    // 绘制备注行 - 高度调整为3行
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight * 3);
    painter.drawLine(tableLeft, currentY + lineHeight * 3, tableLeft + tableWidth, currentY + lineHeight * 3);

    // 填写备注 - 备注二应该回到左边单元格
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight * 3), Qt::AlignCenter | Qt::TextWordWrap, "备 注");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, tableWidth - col1Width, lineHeight * 3), Qt::AlignLeft | Qt::TextWordWrap, content.remarks);

    // 计算从表格顶部到备注底部的准确高度
    qreal tableFinalHeight = currentY + lineHeight * 3 - tableTop;
    
    // 绘制表格外边框，并确保底部边框完整显示
    painter.drawRect(tableLeft, tableTop, tableWidth, tableFinalHeight);
    // 单独绘制底部边框以确保完整显示
    painter.drawLine(tableLeft, currentY + lineHeight * 3, tableLeft + tableWidth, currentY + lineHeight * 3);

    // 显示附加说明文字
    qreal additionalNoteY = currentY + lineHeight * 3 +10; // 直接基于备注部分的结束位置,增加10，让附注行跟表格不贴太进
    painter.drawText(QRectF(tableLeft, additionalNoteY, tableWidth, lineHeight), Qt::AlignLeft | Qt::TextSingleLine, "附：各检验分项详细结果。");

    // 结束绘制
    painter.end();
    myLog->info("PDF文件已成功生成（6个检验项目版本）");

    // 强制刷新和释放资源
    printer.newPage(); // 确保所有数据都写入文件
    
    return true;
}

// 为Form_page2生成表格PDF
bool PDFGenerator::generateTablePDF(const QString &fileName, QWidget *parent)
{
    // 创建打印机对象
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // 创建QPainter用于PDF绘制
    QPainter painter;
    if (!painter.begin(&printer)) {
        myLog->warn("无法创建PDF文件");
        if (parent) {
            QMessageBox::critical(parent, "错误", "无法创建PDF文件");
            myLog->info("显示错误消息框：无法创建PDF文件");
        }
        return false;
    }

    // 设置绘图属性
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置画笔
    QPen pen(Qt::black, 1);
    painter.setPen(pen);

    // 获取PDF页面的尺寸
    QRectF pageRect = printer.pageRect(QPrinter::DevicePixel);

    // 计算表格尺寸和位置（保留边距）
    qreal margin = 50; // 边距
    qreal tableWidth = pageRect.width() - 2 * margin;
    qreal tableHeight = pageRect.height() - 2 * margin;
    qreal tableLeft = margin;
    qreal tableTop = margin;

    // 确保表格有足够的空间
    if (tableWidth <= 0 || tableHeight <= 0) {
        painter.end();
        return false;
    }

    // 计算每个单元格的宽度和高度
    int rows = 4;
    int cols = 7;
    qreal cellWidth = tableWidth / cols;
    qreal cellHeight = tableHeight / rows;

    // 绘制4行7列的表格
    // 绘制横线
    for (int i = 0; i <= rows; i++) {
        qreal y = tableTop + i * cellHeight;
        // 跳过2.1和3.1单元格之间的横线
        if (i == 2) {
            // 只绘制第一列之外的横线
            painter.drawLine(tableLeft + cellWidth, y, tableLeft + tableWidth, y);
        } else {
            painter.drawLine(tableLeft, y, tableLeft + tableWidth, y);
        }
    }

    // 绘制竖线（完整显示所有竖线）
    for (int j = 0; j <= cols; j++) {
        qreal x = tableLeft + j * cellWidth;
        painter.drawLine(x, tableTop, x, tableTop + tableHeight);
    }

    // 绘制表格内容
    painter.setFont(QFont("Arial", 10));
    // 第一行为标题行，显示标题一到标题七
    painter.setFont(QFont("Arial", 10, QFont::Bold)); // 标题行使用粗体
    for (int j = 0; j < cols; j++) {
        QRectF cellRect(tableLeft + j * cellWidth, tableTop, cellWidth, cellHeight);
        QString titleText = QString("标题%1").arg(j+1);
        painter.drawText(cellRect, Qt::AlignCenter, titleText);
    }

    // 其他行显示数据
    painter.setFont(QFont("Arial", 10)); // 恢复普通字体
    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // 处理合并单元格
            if (i == 1 && j == 0) {
                // 合并2.1和3.1单元格
                QRectF mergedCellRect(tableLeft, tableTop + cellHeight, cellWidth, 2 * cellHeight);
                QString mergedText = "合并单元格\n(2.1+3.1)";
                painter.drawText(mergedCellRect, Qt::AlignCenter | Qt::TextWordWrap, mergedText);
                // 跳过3.1单元格的绘制
                continue;
            }
            // 跳过已合并的3.1单元格
            if (i == 2 && j == 0) {
                continue;
            }
            QRectF cellRect(tableLeft + j * cellWidth, tableTop + i * cellHeight, cellWidth, cellHeight);
            QString cellText = QString("%1,%2").arg(i+1).arg(j+1);
            painter.drawText(cellRect, Qt::AlignCenter, cellText);
        }
    }

    // 完成绘制
    painter.end();
    return true;
}

// 为Form_page1绘制PDF报告（接受自定义内容,支持9个检验项目）
bool PDFGenerator::generateReportPDFWith9Projects(const QString &fileName, const ReportContent &content, QWidget *parent)
{

    // 创建QPrinter对象并设置为PDF输出模式
    QPrinter printer(QPrinter::ScreenResolution); // 降低分辨率，提高兼容性
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // 设置页面属性
    printer.setPageSize(QPrinter::A4);  // 设置A4纸张
    printer.setOrientation(QPrinter::Portrait);  // 纵向
    printer.setFullPage(false);  // 不全页打印

    // 设置边距
    printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);

    // 创建QPainter并关联到QPrinter
    QPainter painter;
    if (!painter.begin(&printer))
    {
        myLog->warn("无法创建PDF内容");
        if (parent) {
            QMessageBox::critical(parent, "错误", "无法创建PDF内容");
            myLog->info("显示错误消息框：无法创建PDF内容");
        }
        return false;
    }

    // 获取页面的可打印区域（考虑边距）
    QRectF printableRect = printer.pageRect(QPrinter::DevicePixel);
    qreal pageWidth = printableRect.width();
    qreal pageHeight = printableRect.height();

    // 设置渲染质量
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    // 设置字体
    QFont font;
    // 尝试多种中文字体以确保兼容性
    QStringList chineseFonts = {"SimHei", "WenQuanYi Micro Hei", "Heiti TC", "Arial Unicode MS"};
    QString selectedFont;
    
    // 检查系统中可用的中文字体
    QFontDatabase fontDatabase;
    foreach (const QString &fontFamily, chineseFonts) {
        if (fontDatabase.hasFamily(fontFamily)) {
            selectedFont = fontFamily;
            break;
        }
    }
    
    // 如果没有找到指定的中文字体，使用默认字体
    if (selectedFont.isEmpty()) {
        selectedFont = "SimHei";
        myLog->info("没有找到指定的中文字体，使用默认字体");
    }
    
    font.setFamily(selectedFont);
    myLog->info(QString("选择的字体: %1").arg(selectedFont));
    painter.setFont(font);

    // 计算内容区域
    qreal margin = 15; // 减小边距，增加标题和表格的可用宽度
    qreal tableLeft = margin;
    qreal tableTop = 80; // 增加表格顶部位置，为标题提供更多空间
    qreal tableWidth = pageWidth - 2 * margin;
    qreal tableHeight = pageHeight - 140; // 相应调整表格高度
    qreal totalRows = 19; // 减少总行数的估算，增加行高
    qreal lineHeight = tableHeight / totalRows; // 使用新的表格高度计算行高，确保与修改的尺寸一致
    myLog->info(QString("页面尺寸: %1x%2").arg(pageWidth).arg(pageHeight));
    myLog->info(QString("表格尺寸: %1x%2").arg(tableWidth).arg(tableHeight));
    myLog->info(QString("行高: %1").arg(lineHeight));
    qreal col1Width = 150; // 增加第一列宽度，使其能够显示4个中文字符
    qreal col2Width = 170; // 调整第二列宽度，保持表格整体平衡
    qreal col3Width = 130; // 增加第三列宽度，使其能够显示"分组轮数"4个中文字符
    qreal col4Width = 130; // 增加第四列宽度，确保"分组轮数二"单元格中的"分"字能够完整显示

    // 先填充表格背景色
    painter.fillRect(tableLeft, tableTop, tableWidth, tableHeight, Qt::white);

    // 绘制标题
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRectF(tableLeft, tableTop - 50, tableWidth, 50), Qt::AlignCenter, content.reportTitle); // 增加标题高度从30到50，解决标题文字显示不全问题
    font.setPointSize(12);
    font.setBold(false);
    painter.setFont(font);

    // 绘制表格顶部边框
    painter.drawLine(tableLeft, tableTop, tableLeft + tableWidth, tableTop);

    // 标准表格布局 - 将算法名称单元格与其上下两个单元格合并为一个单元格
    qreal currentY = tableTop;
    
    // 绘制第一部分 - 合并算法名称和算法一的三行单元格
    // 绘制表格外边框
    painter.drawLine(tableLeft, tableTop, tableLeft, currentY + 3 * lineHeight); // 左侧边框
    painter.drawLine(tableLeft + col1Width, tableTop, tableLeft + col1Width, currentY + 3 * lineHeight); // 第一列右边框
    painter.drawLine(tableLeft + col1Width + col2Width, tableTop, tableLeft + col1Width + col2Width, currentY + 3 * lineHeight); // 第二列右边框
    painter.drawLine(tableLeft, currentY + 3 * lineHeight, tableLeft + tableWidth, currentY + 3 * lineHeight); // 底部边框
    
    // 绘制其他列的垂直分隔线，但只在各自的行内显示
    // 第一行的其他列分隔线
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第一行底部横线
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第二行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 第二行底部横线
    painter.drawLine(tableLeft + col1Width + col2Width, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);
    
    // 第三行的其他列分隔线
    currentY += lineHeight;
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    
    // 重置currentY到表格顶部，用于填充内容
    currentY = tableTop;
    
    // 填写算法名称 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft, currentY, col1Width, 3 * lineHeight), Qt::AlignCenter, "算法名称");
    
    // 填写算法一 - 在合并后的大单元格中居中显示
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, 3 * lineHeight), Qt::AlignCenter, content.algorithmName);
    
    // 填写第一行其他列内容
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_1);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_1);
    
    // 填写第二行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_2);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_2);
    
    // 填写第三行其他列内容
    currentY += lineHeight;
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.title1_3);
    // 增加内边距并调整对齐方式，确保'分组轮数二'单元格中的文字完整显示
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width + 5, currentY, col4Width - 10, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.content1_3);
    
    currentY += lineHeight; // 增加到下一行

    // 绘制第二行 - 设计信息
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width, currentY, tableLeft + col1Width + col2Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写第二行内容
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter, "设计单位");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, lineHeight), Qt::AlignCenter, content.designUnit);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter, "算法类别");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter, content.algorithmCategory);

    // 绘制第三行 - 检验信息
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width, currentY, tableLeft + col1Width + col2Width, currentY + lineHeight);
    painter.drawLine(tableLeft + col1Width + col2Width + col3Width, currentY, tableLeft + col1Width + col2Width + col3Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写第三行内容
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter, "检验人员");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, col2Width, lineHeight), Qt::AlignCenter, content.testPersonnel);
    painter.drawText(QRectF(tableLeft + col1Width + col2Width, currentY, col3Width, lineHeight), Qt::AlignCenter, "检验时间");
    painter.drawText(QRectF(tableLeft + col1Width + col2Width + col3Width, currentY, col4Width, lineHeight), Qt::AlignCenter, content.testDate);

    // 绘制第四行 - 检验水平
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写第四行内容
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter, "检验水平");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, tableWidth - col1Width, lineHeight), Qt::AlignCenter, content.testLevel);

    // 绘制检验项目部分
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight * 10); // 第一列垂直分隔线，支持10个项目
    painter.drawLine(tableLeft + col1Width + 80, currentY, tableLeft + col1Width + 80, currentY + lineHeight * 10); // 序号列垂直分隔线
    painter.drawLine(tableLeft + col1Width + 80 + 150, currentY, tableLeft + col1Width + 80 + 150, currentY + lineHeight * 10); // 项目名称列垂直分隔线

    // 填写检验项目内容
    int projectCount = qMin(content.projectNames.size(), content.projectResults.size());
    
    // 先绘制第一列的合并单元格文本 - 现在可以显示4个中文字符
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight * 10), Qt::AlignCenter | Qt::TextWordWrap, "检验项目");
    
    // 循环绘制10行内容
    for (int i = 0; i < 9; i++) {
        currentY += lineHeight;
        // 绘制其他三列的行分隔线（不包括检验项目列）
        painter.drawLine(tableLeft + col1Width, currentY, tableLeft + tableWidth, currentY);
        
        // 为其他列绘制垂直分隔线
        painter.drawLine(tableLeft + col1Width, currentY - lineHeight, tableLeft + col1Width, currentY);
        painter.drawLine(tableLeft + col1Width + 80, currentY - lineHeight, tableLeft + col1Width + 80, currentY);
        painter.drawLine(tableLeft + col1Width + 80 + 150, currentY - lineHeight, tableLeft + col1Width + 80 + 150, currentY);
        
        // 第二列：序号列 - 分9行，序号1-9（固定宽度80）
        painter.drawText(QRectF(tableLeft + col1Width, currentY - lineHeight, 80, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, QString::number(i + 1));
        
        // 第三列：项目名称列（调整宽度到150，保持表格整体平衡）
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width + 80, currentY - lineHeight, 150, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectNames[i]);
        }
        
        // 第四列：是否通过列（剩余宽度）
        if (i < projectCount) {
            painter.drawText(QRectF(tableLeft + col1Width + 80 + 150, currentY - lineHeight, tableWidth - col1Width - 80 - 150, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.projectResults[i]);
        }
    }

    // 绘制检验结论行
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight);
    painter.drawLine(tableLeft, currentY + lineHeight, tableLeft + tableWidth, currentY + lineHeight);

    // 填写检验结论
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, "检验结论");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, tableWidth - col1Width, lineHeight), Qt::AlignCenter | Qt::TextSingleLine, content.testConclusion);

    // 绘制备注行 - 高度调整为3行
    currentY += lineHeight;
    painter.drawLine(tableLeft, currentY, tableLeft + tableWidth, currentY);
    painter.drawLine(tableLeft + col1Width, currentY, tableLeft + col1Width, currentY + lineHeight * 3);
    painter.drawLine(tableLeft, currentY + lineHeight * 3, tableLeft + tableWidth, currentY + lineHeight * 3);

    // 填写备注 - 支持多行显示
    painter.drawText(QRectF(tableLeft, currentY, col1Width, lineHeight * 3), Qt::AlignCenter | Qt::TextWordWrap, "备 注");
    painter.drawText(QRectF(tableLeft + col1Width, currentY, tableWidth - col1Width, lineHeight * 3), Qt::AlignLeft | Qt::TextWordWrap, content.remarks);

    // 计算从表格顶部到备注底部的准确高度
    qreal tableFinalHeight = currentY + lineHeight * 3 - tableTop;
    
    // 绘制表格外边框
    painter.drawRect(tableLeft, tableTop, tableWidth, tableFinalHeight);

    // 显示附加说明文字
    qreal additionalNoteY = currentY + lineHeight * 3 +10; // 直接基于备注部分的结束位置
    painter.drawText(QRectF(tableLeft, additionalNoteY, tableWidth, lineHeight), Qt::AlignLeft | Qt::TextSingleLine, "附：各检验分项详细结果。");

    // 结束绘制
    painter.end();
    myLog->info("PDF文件已成功生成");

    // 强制刷新和释放资源
    printer.newPage(); // 确保所有数据都写入文件
    
    return true;
}

// 生成包含表格模型的单个PDF报告
bool PDFGenerator::generateSinglePDFwithTableModel(const QString &title, QAbstractItemModel *model, const QString &remarks, const QString &fileName)
{
    // 确保必要的参数不为空
    if (model == nullptr) {
        qWarning("模型为空，无法生成表格内容");
        return false;
    }
    // 如果没有提供fileName参数，则显示保存对话框让用户选择
    QString outputFileName;
    if (fileName.isEmpty()) {
        outputFileName = QFileDialog::getSaveFileName(nullptr, "保存PDF文件", QDir::homePath(), "PDF文件 (*.pdf)");
        if (outputFileName.isEmpty()) {
            return false; // 用户取消保存
        }
    } else {
        outputFileName = fileName; // 使用传入的文件名
    }

    // 创建QPrinter对象并配置为PDF输出
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(outputFileName);

    // 设置页面属性
    printer.setPageSize(QPrinter::A4);  // 设置A4纸张
    printer.setOrientation(QPrinter::Portrait);  // 纵向
    printer.setFullPage(false);  // 不全页打印

    // 设置边距
    printer.setPageMargins(20, 20, 20, 20, QPrinter::Millimeter);

    // 创建QPainter对象用于绘制PDF内容
    QPainter painter;
    if (!painter.begin(&printer)) {
        qWarning("无法创建PDF文件");
        return false;
    }

    // 设置渲染质量
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 获取页面的可打印区域
    QRectF printableRect = printer.pageRect(QPrinter::DevicePixel);
    qreal pageWidth = printableRect.width();
    qreal pageHeight = printableRect.height();

    // 设置字体 - 增强中文字体渲染能力
    QFont font;
    // 扩展中文字体列表，增加更多可能性
    QStringList chineseFonts = {"SimHei", "WenQuanYi Micro Hei", "Heiti TC", "Arial Unicode MS", "Microsoft YaHei", "NSimSun", "FangSong", "KaiTi", "STSong"};
    QString selectedFont;
    
    // 检查系统中可用的中文字体
    QFontDatabase fontDatabase;
    foreach (const QString &fontFamily, chineseFonts) {
        if (fontDatabase.hasFamily(fontFamily)) {
            selectedFont = fontFamily;
            break;
        }
    }
    
    // 如果没有找到指定的中文字体，使用默认字体
    if (selectedFont.isEmpty()) {
        selectedFont = "SimHei";
        myLog->info("没有找到指定的中文字体，使用默认字体");
    }
    
    font.setFamily(selectedFont);
    font.setPointSize(6); // 使用较小的字体大小以提高密集文本的可读性
    font.setStyleStrategy(QFont::PreferAntialias); // 启用抗锯齿
    painter.setFont(font);
    myLog->info(QString("设置字体: %1, 大小: %2").arg(font.family()).arg(font.pointSize()));
    myLog->info(QString("字体抗锯齿已%1").arg((font.styleStrategy() & QFont::PreferAntialias ? "启用" : "禁用")));

    // 调试信息
    myLog->info("正在生成Form_page3的PDF报告");
    myLog->info(QString("标题内容:%1").arg(title));
    myLog->info(QString("表格行数:%1，表格列数:%2").arg(model->rowCount()).arg(model->columnCount()));
    myLog->info(QString("备注内容:%1").arg(remarks));

    // 1. 绘制标题
    QString titleText = title.isEmpty() ? "检验结果报告" : title;
    // 保存当前字体设置
    QFont originalFont = font;
    // 临时设置标题字体
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);
    // 绘制标题
    painter.drawText(QRectF(0, 20, pageWidth, 300), Qt::AlignCenter, titleText);
    // 恢复原始字体设置
    font = originalFont;
    painter.setFont(font);

    // 2. 绘制表格内容
    qreal tableTop = 400; // 表格起始位置
    qreal tableLeft = 30; // 增加左边距
    qreal tableWidth = pageWidth - 60; // 减小表格宽度，增加页边距

    int rowCount = model->rowCount();
    int columnCount = model->columnCount();
    
    // 计算行高和列宽 - 调整为合适的高度，确保内容不会挤在一起
    qreal headerHeight = 200; // 表头高度
    qreal rowHeight = 200; // 数据行高度
    myLog->info(QString("表格行高设置 - 表头: %1, 数据行: %2").arg(headerHeight).arg(rowHeight));
    
    // 直接使用均匀分布的列宽
    QVector<qreal> columnWidths;
    qreal uniformWidth = tableWidth / columnCount;
    for (int col = 0; col < columnCount; ++col) {
        columnWidths.append(uniformWidth);
    }
    
    // 添加调试信息
    myLog->info(QString("表格列宽设置 - 列数: %1, 每列宽度: %2").arg(columnCount).arg(uniformWidth));
    
    // 检测需要合并的单元格（从索引1开始，跳过序号列）
    QVector<QVector<int>> mergedCells(rowCount, QVector<int>(columnCount, 1)); // 记录每个单元格需要合并的行数
    
    for (int col = 1; col < columnCount; ++col) {
        if (rowCount <= 1) continue;
        
        int startRow = 0;
        QVariant startValue = model->data(model->index(startRow, col), Qt::DisplayRole);
        
        for (int currentRow = 1; currentRow < rowCount; ++currentRow) {
            QVariant currentValue = model->data(model->index(currentRow, col), Qt::DisplayRole);
            
            // 如果当前单元格的值与起始单元格的值不同，或者到达最后一行
            if (currentValue != startValue || currentRow == rowCount - 1) {
                int span = currentValue != startValue ? currentRow - startRow : currentRow - startRow + 1;
                
                // 如果有两个或更多相同值的单元格，标记为合并
                if (span > 1) {
                    mergedCells[startRow][col] = span;
                    // 标记被合并的单元格为0，表示不需要绘制
                    for (int i = startRow + 1; i < startRow + span; ++i) {
                        mergedCells[i][col] = 0;
                    }
                }
                
                // 更新起始行和起始值
                startRow = currentRow;
                if (currentValue != startValue) {
                    startValue = currentValue;
                }
            }
        }
    }
    
    // 计算每页能显示的行数 - 优化可用高度计算
    qreal availableHeight = pageHeight - tableTop - 50; // 减少底部预留空间
    
    // 重要: 直接根据页面高度和行高计算，不减去表头高度，因为表头在每页都是独立绘制的
    int rowsPerPage = static_cast<int>(availableHeight / rowHeight);
    
    // 确保至少显示2行数据
    if (rowsPerPage <= 0) {
        rowsPerPage = 1;
    }
    
    myLog->info(QString("每页可显示行数: %1").arg(rowsPerPage));

    // 强制设置每页至少显示2行数据，避免显示太少
    if (rowsPerPage < 2) {
        rowsPerPage = 2;
    }

    // 增加额外的调试信息，帮助分析表格显示问题
    myLog->info(QString("表格总览 - 总行数: %1, 页面高度: %2").arg(rowCount).arg(pageHeight));
    myLog->info(QString("表格起始位置: %1, 可用高度: %2").arg(tableTop).arg(availableHeight));
    myLog->info(QString("预计需要页面数: %1").arg(static_cast<int>(ceil(static_cast<qreal>(rowCount) / rowsPerPage))));
    
    // 绘制数据行，处理分页和单元格合并
    int currentPage = 0;
    int rowsProcessed = 0;
    qreal currentX = 0;
    
    // 保存原始的tableTop值，用于计算
    qreal originalTableTop = tableTop;
    
    while (rowsProcessed < rowCount) {
        // 计算当前页要显示的起始行和结束行
        int startRow = rowsProcessed;
        int endRow = qMin(startRow + rowsPerPage, rowCount);
        
        // 如果不是第一页，创建新页面并重置表格位置
        if (currentPage > 0) {
            printer.newPage();
            tableTop = 50; // 新页面上表格起始位置
        } else {
            // 第一页保持原始tableTop
            tableTop = originalTableTop;
        }
        
        // 在当前页绘制表头
        currentX = tableLeft;
        qreal headerY = tableTop;
        font.setBold(true);
        font.setPointSize(7); // 确保表头使用正确的字体大小
        painter.setFont(font);
        
        for (int col = 0; col < columnCount; ++col) {
            QRectF headerRect(currentX, headerY, columnWidths[col], headerHeight);
            painter.drawRect(headerRect);
            QString headerText = model->headerData(col, Qt::Horizontal).toString();
            painter.drawText(headerRect, Qt::AlignCenter | Qt::TextWordWrap, headerText);
            currentX += columnWidths[col];
        }
        
        // 在当前页绘制数据行
        font.setBold(false);
        painter.setFont(font);
        
        for (int row = startRow; row < endRow; ++row) {
            // 不要因为合并单元格而跳过行，确保每一行都被处理
            // 即使这一行的所有单元格都被合并，我们仍然需要处理它来正确计算行位置
            bool skipRowCompletely = false;
            
            // 检查这一行是否包含任何可见的单元格数据
            // 对于序号列（第0列），我们总是尝试绘制
            bool hasAnyData = false;
            for (int c = 0; c < columnCount; ++c) {
                if (mergedCells[row][c] > 0 || c == 0) {
                    hasAnyData = true;
                    break;
                }
            }
            
            // 只有当这一行完全没有数据时才跳过
            if (!hasAnyData) {
                skipRowCompletely = true;
            }
            
            if (skipRowCompletely) {
                continue;
            }
            
            currentX = tableLeft;
            qreal currentY = tableTop + headerHeight + (row - startRow) * rowHeight;
            
            for (int col = 0; col < columnCount; ++col) {
                int rowspan = mergedCells[row][col];
                
                // 如果当前单元格是被合并的单元格，则跳过
                if (rowspan == 0) {
                    currentX += columnWidths[col];
                    continue;
                }
                
                // 计算单元格的实际高度
                qreal cellHeight = rowspan * rowHeight;
                
                // 确保单元格不会超出当前页的边界
                if (row + rowspan > endRow) {
                    // 如果合并单元格跨页，调整为只显示当前页的部分
                    cellHeight = (endRow - row) * rowHeight;
                    rowspan = endRow - row;
                    myLog->info(QString("单元格跨页处理 - 行: %1, 列: %2, 调整后 rowspan: %3").arg(row).arg(col).arg(rowspan));
                }
                
                // 先获取单元格文本
                QModelIndex index = model->index(row, col);
                QString cellText = model->data(index).toString();
                
                myLog->info(QString("绘制单元格 - 行: %1, 列: %2, 内容: %3").arg(row).arg(col).arg(cellText));
                myLog->info(QString("单元格位置 - X: %1, Y: %2, 宽度: %3, 高度: %4").arg(currentX).arg(currentY).arg(columnWidths[col]).arg(cellHeight));
                
                QRectF cellRect(currentX, currentY, columnWidths[col], cellHeight);
                
                // 绘制单元格边框
                painter.drawRect(cellRect);
                
                // 确保文本编码正确，处理可能的乱码
                QTextCodec *codec = QTextCodec::codecForName("UTF-8");
                if (codec) {
                    // 确保正确处理UTF-8编码文本
                    QString processedText = codec->toUnicode(cellText.toUtf8());
                    cellText = processedText;
                }
                
                // 增加内边距以提高可读性并减少文本重叠
                QRectF adjustedRect = cellRect.adjusted(4, 4, -4, -4);
                
                // 使用简化的文本绘制方法，确保兼容性
                QTextOption textOption(Qt::AlignCenter);
                textOption.setTextDirection(Qt::LeftToRight);
                textOption.setWrapMode(QTextOption::WordWrap);
                
                // 即使文本可能超出单元格，也尝试绘制所有文本，优先显示完整内容
                // 增加内边距调整，确保文本不紧贴边框
                QRectF textDisplayRect = adjustedRect.adjusted(2, 2, -2, -2);
                
                // 强制绘制所有文本，启用自动换行
                painter.drawText(textDisplayRect, Qt::AlignCenter | Qt::TextWordWrap, cellText);
                
                currentX += columnWidths[col];
            }
        }
        
        // 确保正确更新已处理的行数，即使有合并单元格
        rowsProcessed = endRow;
        
        // 显式绘制完整的表格外框，确保底部边框完整
        qreal tableBottom = tableTop + headerHeight + (endRow - startRow) * rowHeight;
        QRectF tableRect(tableLeft, tableTop, tableWidth, tableBottom - tableTop);
        painter.drawRect(tableRect);
        
        myLog->info(QString("当前页处理完成 - 已处理行数: %1/%2").arg(rowsProcessed).arg(rowCount));
        currentPage++;
    }
    
    // 添加调试信息
    myLog->info("Form_page3 PDF生成完成 - 使用的设置：");
    myLog->info(QString("页面尺寸: %1x%2").arg(pageWidth).arg(pageHeight));
    myLog->info(QString("字体: %1, 大小: %2").arg(font.family()).arg(font.pointSize()));
    myLog->info(QString("表格设置 - 表头高度: %1, 数据行高度: %2").arg(headerHeight).arg(rowHeight));
    
    // 3. 绘制备注内容 - 在表格下方显示
    if (!remarks.isEmpty()) {
        // 计算表格最后一页实际显示的行数
        qreal lastPageRowsDisplayed = rowCount % rowsPerPage;
        if (lastPageRowsDisplayed == 0) {
            lastPageRowsDisplayed = rowsPerPage; // 如果整除，最后一页显示完整的rowsPerPage行
        }
        qreal remarksTop = tableTop + headerHeight + lastPageRowsDisplayed * rowHeight + 20; // 在表格下方留出20像素间距（减小间距）
        qreal maxWidth = tableWidth;
               
        // 计算文本所需的高度（暂时不需要实际使用）
        QFontMetrics fontMetrics(font);
        QStringList lines = remarks.split('\n');
        qreal totalHeight = 0;
        
        // 调整备注行高为更合适的值
        qreal lineHeight = 150;
        
        // 计算每行可以显示的汉字数量，确保与当前Qt版本兼容
        int charsPerLine = 20; // 默认值
        
        // 使用fontMetrics计算一个汉字的宽度
        qreal avgCharWidth = fontMetrics.width("中");
        
        if (avgCharWidth > 0 && maxWidth > 0) {
            // 计算可显示的汉字数量，留10%的边距
            charsPerLine = static_cast<int>((maxWidth * 0.9) / avgCharWidth);
            // 确保结果在合理范围内
            charsPerLine = qMax(10, qMin(50, charsPerLine));
        }
        
        myLog->info(QString("备注每行可以显示的汉字数量:%1").arg(charsPerLine));
        int linesNeededAll=0;
        for (const QString &line : lines) {
            // 方法1：使用QFontMetrics计算文本在指定宽度下需要的行数
            QRect textRect = fontMetrics.boundingRect(0, 0, static_cast<int>(maxWidth), 1000, Qt::TextWordWrap, line);
            int linesFromMetrics = (textRect.height() + lineHeight - 1) / lineHeight; // 向上取整

            // 方法2：根据汉字数量计算需要的行数（中文通常占2个字符宽度）
            int totalChars = line.length();
            myLog->info(QString("totalChars:%1").arg(totalChars));
            int linesFromCharCount = (totalChars + charsPerLine - 1) / charsPerLine; // 向上取整
            myLog->info(QString("linesFromCharCount:%1").arg(linesFromCharCount));
            // 取两种方法计算结果的较大值，确保有足够的空间
            int linesNeeded = qMax(linesFromMetrics, linesFromCharCount);
            myLog->info(QString("linesNeeded:%1").arg(linesNeeded));
            linesNeededAll+=linesNeeded;
        }

        myLog->info(QString("linesNeededAll:%1").arg(linesNeededAll));
        totalHeight = linesNeededAll*lineHeight;
        myLog->info(QString("totalHeight:%1").arg(totalHeight));
        
        // 绘制备注内容
        font.setBold(false);
        painter.setFont(font);
        
        // 绘制备注内容 - 使用动态计算的高度值，确保多行文字完全显示
        painter.drawText(QRectF(tableLeft, remarksTop, maxWidth, totalHeight), Qt::AlignTop | Qt::TextWordWrap, remarks);
    }

    // 完成绘制
    painter.end();
    return true;
}



// 用于Form_page1生成只有6个检验项目的pdf
void PDFGenerator::generateAndManageReportPDFWith6Projects(const ReportContent &content, QWidget *parent)
{
    myLog->info("开始生成Form_page1的PDF报告（6个检验项目版本）"); 
    
    // 调用通用管理函数，传入默认文件名和生成函数
    managePDFReport("document.pdf", 
                  [content, parent](const QString &fileName) { return generateReportPDFWith6Projects(fileName, content, parent); }, 
                  parent);
}

// Form_page1生成9个检验项目的pdf
void PDFGenerator::generateAndManageReportPDF(const ReportContent &content, QWidget *parent)
{
    myLog->info("开始生成Form_page1的PDF报告");
    
    // 调用通用管理函数，传入默认文件名和生成函数
    managePDFReport("document.pdf", 
                  [content, parent](const QString &fileName) { return generateReportPDFWith9Projects(fileName, content, parent); }, 
                  parent);
}

// 生成临时PDF文件，预览并询问是否保存（用于Form_page2）
void PDFGenerator::generateAndManageTablePDF(QWidget *parent)
{
    myLog->info("开始生成Form_page2的PDF表格");
    
    // 调用通用管理函数，传入默认文件名和生成函数
    managePDFReport("table.pdf", 
                  [parent](const QString &fileName) { return generateTablePDF(fileName, parent); }, 
                  parent);
    
    // 显示成功消息
    // 已移除PDF生成成功弹窗
    // if (parent) {
    //     QMessageBox::information(parent, "成功", "PDF文件已成功生成！");
    // }
}

// 通用的PDF生成和管理函数
//生成临时pdf文件用于预览，并在关闭时由用户选择是否保存
void PDFGenerator::managePDFReport(const QString &defaultFileName, 
                                  std::function<bool(const QString &)> generatorFunc, 
                                  QWidget *parent)
{
    myLog->info("开始生成PDF报告");
    
    // 创建临时文件，用于存储PDF内容
    QString tempFileName = QDir::temp().absoluteFilePath(QString("qt_temp_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")));

    // 测试是否可以创建文件
    QFile testFile(tempFileName);
    if (!testFile.open(QIODevice::WriteOnly)) {
        myLog->warn("无法创建临时PDF文件");
        if (parent) {
            QMessageBox::critical(parent, "错误", "无法创建临时PDF文件");
            myLog->info("显示错误消息框：无法创建临时PDF文件");
        }
        return;
    }
    testFile.close();

    myLog->info(QString("PDF临时文件路径：%1").arg(tempFileName));
    myLog->info(QString("临时文件是否存在：%1").arg(QFile::exists(tempFileName) ? "是" : "否"));
    myLog->info(QString("临时文件所在目录是否存在：%1").arg(QFileInfo(tempFileName).absoluteDir().exists() ? "是" : "否"));

    // 使用传入的生成函数生成PDF
    if (!generatorFunc(tempFileName)) {
        return; // 如果生成失败，直接返回
    }

    qApp->processEvents(); // 处理所有待处理的事件

    myLog->info("PDF内容已生成，准备打开预览");

    // 打开文件进行预览前，添加一个小延迟，确保文件完全写入和释放
    QThread::msleep(300); // 延迟300毫秒
    qApp->processEvents(); // 再次处理事件

    // 检查文件是否存在且可读，而不是直接打开
    myLog->info(QString("准备打开预览前 - 临时文件路径：%1").arg(tempFileName));
    myLog->info(QString("准备打开预览前 - 临时文件是否存在：%1").arg(QFile::exists(tempFileName) ? "是" : "否"));

    QFileInfo fileInfo(tempFileName);
    if (fileInfo.exists() && fileInfo.isReadable()) {
        
        // 自动打开PDF文件进行预览
        QUrl url = QUrl::fromLocalFile(tempFileName);
        if (!QDesktopServices::openUrl(url)) {
            myLog->warn("无法打开PDF文件进行预览");
            if (parent) {
                QMessageBox::warning(parent, "警告", "无法打开PDF预览，请手动查看临时文件：\n" + tempFileName);
                myLog->info(QString("显示警告消息框：无法打开PDF预览，请手动查看临时文件：%1").arg(tempFileName));
            }
        }
    } else {
        myLog->warn("文件仍然被锁定，无法打开预览");
        if (parent) {
            QMessageBox::warning(parent, "警告", "PDF文件被锁定，无法打开预览。请稍后重试。");
            myLog->info("显示警告消息框：PDF文件被锁定，无法打开预览。请稍后重试。");
        }
    }

    // 使用QTimer延迟询问用户是否保存，给预览程序足够的时间打开文件
    QTimer::singleShot(1000, parent, [=]() {
        // 显示对话框，询问用户是否保存PDF
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "保存PDF", "是否保存此PDF文件？",
                                     QMessageBox::Save | QMessageBox::Discard);
        myLog->info("显示问题消息框：是否保存此PDF文件？");

        if (reply == QMessageBox::Save) {
            // 用户选择保存，打开文件保存对话框
            QString saveFileName = QFileDialog::getSaveFileName(
                  nullptr, "保存PDF文件", defaultFileName, "PDF文件 (*.pdf)");

            if (!saveFileName.isEmpty()) {
                // 尝试多次复制文件，增加重试次数和延迟以处理文件锁定
                bool copied = false;
                int retryCount = 0;
                const int maxRetries = 5; // 增加重试次数

                while (!copied && retryCount < maxRetries) {
                    int delayMs = 500 + (retryCount * 300); // 递增的延迟时间
                    QThread::msleep(delayMs); // 每次重试前的延迟递增
                    qApp->processEvents(); // 处理事件，确保文件系统操作正常

                    myLog->info(QString("尝试复制文件（%1/%2），延迟：%3ms").arg(retryCount + 1).arg(maxRetries).arg(delayMs));
                    myLog->info(QString("源文件：%1").arg(tempFileName));
                    myLog->info(QString("目标文件：%1").arg(saveFileName));
                    myLog->info(QString("源文件是否存在：%1").arg(QFile::exists(tempFileName) ? "是" : "否"));
                    myLog->info(QString("目标文件是否存在：%1").arg(QFile::exists(saveFileName) ? "是" : "否"));

                    // 如果目标文件存在，先尝试删除它
                    if (QFile::exists(saveFileName)) {
                        myLog->info("目标文件已存在，尝试删除");
                        if (!QFile::remove(saveFileName)) {
                            myLog->warn("无法删除现有文件，可能被其他程序占用");
                            retryCount++;
                            continue;
                        }
                    }

                    if (QFile::copy(tempFileName, saveFileName)) {
                        copied = true;
                        myLog->info(QString("PDF文件已保存到：%1").arg(saveFileName));
//                        QMessageBox::information(nullptr, "成功", "PDF文件已保存\n" + saveFileName);
                    } else {
                        retryCount++;
                        myLog->warn(QString("无法保存PDF文件到指定位置，正在重试（%1/%2）").arg(retryCount).arg(maxRetries));
                    }
                }

                if (!copied) {
                    qWarning("多次尝试后仍然无法保存PDF文件");
                    // 提供一个替代方案
                    QString message = QString("无法保存PDF文件。可能文件正被其他程序占用。\n\n")
                                    + "临时文件位置：" + tempFileName + "\n\n"
                                    + "请手动打开此文件夹并复制该文件。\n\n"
                                    + "是否现在打开临时文件夹？";

                    if (QMessageBox::Yes == QMessageBox::question(nullptr, "保存失败", message, QMessageBox::Yes | QMessageBox::No)) {
                        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(tempFileName).absolutePath()));
                    }
                }
            }
        }

        // 尝试删除临时文件，但不强制
        QTimer::singleShot(2000, [=]() {
            if (QFile::exists(tempFileName)) {
                if (QFile::remove(tempFileName)) {
                    myLog->info("已删除临时PDF文件");
                } else {
                    myLog->warn("无法删除临时PDF文件，可能被其他程序占用");
                    // 不显示错误消息，因为这是正常现象
                }
            }
        });
    });
    
}

//// 生成临时PDF文件，预览并询问是否保存（用于Form_page3）

void PDFGenerator::generateAndManagePDFwithTableModel(const QString &title, QAbstractItemModel *model, const QString &remarks)
{
    myLog->info("开始生成Form_page3的PDF报告");
        
    // 检查是否获取到了必要的模型数据
    if (!model) {
        qWarning("无法获取表格模型数据，无法生成PDF");
        return;
    }
    
    // 使用managePDFReport函数处理PDF生成、预览和保存逻辑
    managePDFReport("检验结果报告.pdf", [=](const QString &fileName) {
        return generateSinglePDFwithTableModel(title, model, remarks, fileName);
    }, nullptr);
}

// 生成多页PDF文件，预览并询问是否保存
void PDFGenerator::generateAndManageMultiPagePDFWithItemsResultList(const QList<chk_items_result *> &itemsResultList)
{
    // 检查itemsResultList是否为空
    if (itemsResultList.isEmpty()) {
        qWarning("itemsResultList为空，无法生成PDF报告");
        return;
    }
    
    // 调用managePDFReport函数来管理PDF报告的生成、预览和保存
    managePDFReport("多页检验结果报告.pdf", [=](const QString &fileName) {
        return generateMultiPagePDFWithItemsResultList(itemsResultList, fileName);
    }, nullptr);
}

// 新函数：根据标题、datalist、表头和备注生成PDF，自动转换datalist为model，然后生成pdf
bool PDFGenerator::generateSinglePDFWithDataList(const QString &title, const QList<chk_singleitem_result *> &dataList, const QStringList &mheader, const QString &remarks, const QString &fileName)
{
    // 创建QStandardItemModel来存储表格数据
    QStandardItemModel *model = new QStandardItemModel();
    
    // 检查datalist是否为空
    if (dataList.isEmpty()) {
        qWarning("数据列表为空，无法生成表格内容");
        delete model;
        return false;
    }
    
    // 设置表头
    QStringList headers;
    if (!mheader.isEmpty()) {
        // 优先使用传入的mheader作为表头
        headers = mheader;
    } else {
        // 如果没有传入表头，尝试从dataList中获取字段名
        const QStringList fieldNames = dataList.first()->getFieldNames();
        if (!fieldNames.isEmpty()) {
            headers = fieldNames;
        } else {
            // 如果都没有，使用默认的列名
            int fieldCount = dataList.first()->fieldCount();
            for (int i = 0; i < fieldCount; ++i) {
                headers << QString("列%1").arg(i + 1);
            }
        }
    }
    model->setHorizontalHeaderLabels(headers);
    
    // 填充表格数据
    for (int row = 0; row < dataList.size(); ++row) {
        const chk_singleitem_result *data = dataList.at(row);
        for (int col = 0; col < data->fieldCount(); ++col) {
            QString value = data->getField(col);
            QStandardItem *item = new QStandardItem(value);
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(row, col, item);
        }
    }
    
    // 调用现有的generateSinglePDFwithTableModel函数生成PDF
    // 如果提供了fileName参数，直接使用该参数调用generateSinglePDFwithTableModel
    if (!fileName.isEmpty()) {
        generateSinglePDFwithTableModel(title, model, remarks, fileName);
    } else {
        managePDFReport("检验结果报告.pdf", [=](const QString &fileName) {
            return generateSinglePDFwithTableModel(title, model, remarks, fileName);
        }, nullptr);
    }
    
    // 清理临时创建的model
    delete model;
    
    return true;
}

// 新函数：根据chk_items_result列表生成多页PDF报告
bool PDFGenerator::generateMultiPagePDFWithItemsResultList(const QList<chk_items_result *> &itemsResultList, const QString &fileName)
{
    // 确保itemsResultList不为空
    if (itemsResultList.isEmpty()) {
        qWarning("itemsResultList为空，无法生成PDF报告");
        return false;
    }
    
    // 如果没有提供fileName参数，则显示保存对话框让用户选择
    QString outputFileName;
    if (fileName.isEmpty()) {
        outputFileName = QFileDialog::getSaveFileName(nullptr, "保存PDF文件", QDir::homePath(), "PDF文件 (*.pdf)");
        if (outputFileName.isEmpty()) {
            return false; // 用户取消保存
        }
    } else {
        outputFileName = fileName; // 使用传入的文件名
    }
    
    // 创建QPrinter对象并配置为PDF输出
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(outputFileName);
    
    // 设置页面属性
    printer.setPageSize(QPrinter::A4);  // 设置A4纸张
    printer.setOrientation(QPrinter::Portrait);  // 纵向
    printer.setFullPage(false);  // 不全页打印
    
    // 设置边距
    printer.setPageMargins(20, 20, 20, 20, QPrinter::Millimeter);
    
    // 创建QPainter对象用于绘制PDF内容
    QPainter painter;
    if (!painter.begin(&printer)) {
        qWarning("无法创建PDF文件");
        return false;
    }
    
    // 处理每个chk_items_result对象
    for (int i = 0; i < itemsResultList.size(); ++i) {
        const chk_items_result *itemResult = itemsResultList.at(i);
        
        // 如果不是第一个结果集，创建新页面
        if (i > 0) {
            printer.newPage();
        }
        
        // 创建QStandardItemModel来存储当前结果集的数据
        QStandardItemModel *model = new QStandardItemModel();
        
        // 获取当前结果集的标题、数据列表、表头和备注
        QString title = itemResult->getTitle();
        QList<chk_singleitem_result *> dataList = itemResult->getDataList();
        QStringList headers = itemResult->getTableHeader();
        QString remarks = itemResult->getRemark();
        
        // 检查数据列表是否为空
        if (dataList.isEmpty()) {
            myLog->warn(QString("数据列表为空，跳过结果集: %1").arg(title));
            delete model;
            continue;
        }
        
        // 设置表头
        model->setHorizontalHeaderLabels(headers);
        
        // 填充表格数据
        for (int row = 0; row < dataList.size(); ++row) {
            const chk_singleitem_result *data = dataList.at(row);
            for (int col = 0; col < qMin(data->fieldCount(), headers.size()); ++col) {
                QString value = data->getField(col);
                QStandardItem *item = new QStandardItem(value);
                item->setTextAlignment(Qt::AlignCenter);
                model->setItem(row, col, item);
            }
        }
        
        // 获取页面的可打印区域
        QRectF printableRect = printer.pageRect(QPrinter::DevicePixel);
        qreal pageWidth = printableRect.width();
        qreal pageHeight = printableRect.height();
        
        // 调用generateSinglePDFwithTableModel函数的核心逻辑来绘制当前结果集
        // 这里我们需要直接调用核心绘图逻辑，而不是整个函数，因为我们已经有了painter对象
        
        // 设置字体
        QFont font;
        QStringList chineseFonts = {"SimHei", "WenQuanYi Micro Hei", "Heiti TC", "Arial Unicode MS", "Microsoft YaHei", "NSimSun", "FangSong", "KaiTi", "STSong"};
        QString selectedFont;
        
        // 检查系统中可用的中文字体
        QFontDatabase fontDatabase;
        foreach (const QString &fontFamily, chineseFonts) {
            if (fontDatabase.hasFamily(fontFamily)) {
                selectedFont = fontFamily;
                break;
            }
        }
        
        // 如果没有找到指定的中文字体，使用默认字体
        if (selectedFont.isEmpty()) {
            selectedFont = "SimHei";
        }
        
        font.setFamily(selectedFont);
        font.setStyleStrategy(QFont::PreferAntialias); // 启用抗锯齿
        painter.setFont(font);
        
        // 1. 绘制标题
        QString titleText = title.isEmpty() ? "检验结果报告" : title;
        // 保存当前字体设置
        QFont originalFont = font;
        // 临时设置标题字体
        font.setPointSize(16);
        font.setBold(true);
        painter.setFont(font);
        // 绘制标题
        painter.drawText(QRectF(0, 20, pageWidth, 300), Qt::AlignCenter, titleText);
        // 恢复原始字体设置
        font = originalFont;
        painter.setFont(font);
        
        // 2. 绘制表格内容 - 直接调用generateSinglePDFwithTableModel的表格绘制逻辑
        qreal tableTop = 400; // 表格起始位置
        qreal tableLeft = 30; // 增加左边距
        qreal tableWidth = pageWidth - 60; // 减小表格宽度，增加页边距
        
        int rowCount = model->rowCount();
        int columnCount = model->columnCount();
        
        // 计算行高和列宽
        qreal headerHeight = 200; // 表头高度
        qreal rowHeight = 200; // 数据行高度
        
        // 使用均匀分布的列宽
        QVector<qreal> columnWidths;
        qreal uniformWidth = tableWidth / columnCount;
        for (int col = 0; col < columnCount; ++col) {
            columnWidths.append(uniformWidth);
        }
        
        // 计算每页能显示的行数
        qreal availableHeight = pageHeight - tableTop - 50;
        int rowsPerPage = static_cast<int>(availableHeight / rowHeight);
        if (rowsPerPage <= 0) {
            rowsPerPage = 1;
        }
        
        // 绘制数据行，处理分页和单元格合并
        int currentPage = 0;
        int rowsProcessed = 0;
        qreal currentX = 0;
        qreal currentTableTop = tableTop;
        
        // 用于分页处理
        while (rowsProcessed < rowCount) {
            // 计算当前页要显示的起始行和结束行
            int startRow = rowsProcessed;
            int endRow = qMin(startRow + rowsPerPage, rowCount);
            
            // 如果不是当前结果集的第一页，创建新页面
            if (currentPage > 0) {
                printer.newPage();
                currentTableTop = 50; // 新页面上表格起始位置
            }
            
            // 在当前页绘制表头
            currentX = tableLeft;
            qreal headerY = currentTableTop;
            font.setBold(true);
            font.setPointSize(7);
            painter.setFont(font);
            
            for (int col = 0; col < columnCount; ++col) {
                QRectF headerRect(currentX, headerY, columnWidths[col], headerHeight);
                painter.drawRect(headerRect);
                QString headerText = model->headerData(col, Qt::Horizontal).toString();
                painter.drawText(headerRect, Qt::AlignCenter | Qt::TextWordWrap, headerText);
                currentX += columnWidths[col];
            }
            
            // 在当前页绘制数据行
            font.setBold(false);
            painter.setFont(font);
            
            for (int row = startRow; row < endRow; ++row) {
                currentX = tableLeft;
                qreal currentY = currentTableTop + headerHeight + (row - startRow) * rowHeight;
                
                for (int col = 0; col < columnCount; ++col) {
                    QRectF cellRect(currentX, currentY, columnWidths[col], rowHeight);
                    painter.drawRect(cellRect);
                    
                    QModelIndex index = model->index(row, col);
                    QString cellText = model->data(index).toString();
                    
                    QRectF adjustedRect = cellRect.adjusted(4, 4, -4, -4);
                    painter.drawText(adjustedRect, Qt::AlignCenter | Qt::TextWordWrap, cellText);
                    
                    currentX += columnWidths[col];
                }
            }
            
            // 显式绘制完整的表格外框
            qreal tableBottom = currentTableTop + headerHeight + (endRow - startRow) * rowHeight;
            QRectF tableRect(tableLeft, currentTableTop, tableWidth, tableBottom - currentTableTop);
            painter.drawRect(tableRect);
            
            rowsProcessed = endRow;
            currentPage++;
        }
        
        // 3. 绘制备注内容
        if (!remarks.isEmpty()) {
            // 计算表格最后一页实际显示的行数，考虑分页情况
            qreal lastPageRowsDisplayed = rowCount % rowsPerPage;
            if (lastPageRowsDisplayed == 0) {
                lastPageRowsDisplayed = rowsPerPage; // 如果整除，最后一页显示完整的rowsPerPage行
            }
            
            // 计算备注的起始位置，考虑表格实际占用的空间
            qreal remarksTop;
            if (currentPage > 1) {
                // 如果表格分成了多页，备注放在最后一页的顶部下方
                remarksTop = 50 + headerHeight + lastPageRowsDisplayed * rowHeight + 20;
            } else {
                // 如果表格只有一页，备注放在表格下方
                remarksTop = currentTableTop + headerHeight + lastPageRowsDisplayed * rowHeight + 20;
            }
            
            qreal maxWidth = tableWidth;
            
            font.setBold(false);
            painter.setFont(font);
            
            // 绘制备注内容
            painter.drawText(QRectF(tableLeft, remarksTop, maxWidth, 500), Qt::AlignTop | Qt::TextWordWrap, remarks);
        }
        
        // 清理临时创建的model
        delete model;
    }
    
    // 完成绘制
    painter.end();
    return true;
}
