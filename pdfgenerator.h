#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

#include <QObject>
#include <QString>
#include <QWidget>

// PDF生成器类，用于处理不同页面的PDF生成需求
class PDFGenerator : public QObject
{
    Q_OBJECT
public:
    explicit PDFGenerator(QObject *parent = nullptr);
    
    // 为Form_page1生成PDF报告
    static bool generateReportPDF(const QString &fileName, QWidget *parent = nullptr);
    
    // 为Form_page2生成表格PDF
    static bool generateTablePDF(const QString &fileName, QWidget *parent = nullptr);
    
    // 生成临时PDF文件，预览并询问是否保存（用于Form_page1）
    static void generateAndManageReportPDF(QWidget *parent = nullptr);
    
    // 生成临时PDF文件，预览并询问是否保存（用于Form_page2）
    static void generateAndManageTablePDF(QWidget *parent = nullptr);

signals:
    
public slots:
};

#endif // PDFGENERATOR_H