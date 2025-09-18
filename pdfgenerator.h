#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QStringList>

// 定义ReportContent结构体，用于存储可变动的报告内容
typedef struct {
    QString reportTitle;         // 报告标题
    QString algorithmName;       // 算法名称
    QString title1_1;              // 第一大行中第一小行的标题
    QString content1_1;              // 第一大行中第一小行的内容
    QString title1_2;           //第一大行中第二小行的标题
    QString content1_2;              // 第一大行中第二小行的内容
    QString title1_3;
    QString content1_3;
    QString designUnit;          // 设计单位
    QString algorithmCategory;   // 算法类别
    QString testPersonnel;       // 检验人员
    QString testDate;            // 检验时间
    QString testLevel;           // 检验水平
    QStringList projectNames;    // 项目名称列表
    QStringList projectResults;  // 项目结果列表
    QString testConclusion;      // 检验结论
    QString remarks;             // 备注
} ReportContent;

// PDF生成器类，用于处理不同页面的PDF生成需求
class PDFGenerator : public QObject
{
    Q_OBJECT
public:
    explicit PDFGenerator(QObject *parent = nullptr);


// 为Form_page1生成PDF报告（接受自定义内容）
static bool generateReportPDF(const QString &fileName, const ReportContent &content, QWidget *parent = nullptr);

// 为Form_page2生成表格PDF
static bool generateTablePDF(const QString &fileName, QWidget *parent = nullptr);



// 生成临时PDF文件，预览并询问是否保存（用于Form_page1，接受自定义内容）
static void generateAndManageReportPDF(const ReportContent &content, QWidget *parent = nullptr);
    
    // 生成临时PDF文件，预览并询问是否保存（用于Form_page2）
static void generateAndManageTablePDF(QWidget *parent = nullptr);

signals:
    
public slots:
};

#endif // PDFGENERATOR_H
