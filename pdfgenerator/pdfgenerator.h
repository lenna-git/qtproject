#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QStringList>
#include <QList>
#include <functional>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QTextLayout>
#include <QTextLine>
#include <QTextDocument>
#include "../chkresultclass/chk_singleitem_result.h"

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

// 生成只有6个检验项目的PDF报告
static bool generateReportPDFWith6Projects(const QString &fileName, const ReportContent &content, QWidget *parent = nullptr);

// 为了修复编译错误添加的别名函数
static bool generateReportPDFWithProjects(const QString &fileName, const ReportContent &content, QWidget *parent = nullptr);

// 为Form_page2生成表格PDF
static bool generateTablePDF(const QString &fileName, QWidget *parent = nullptr);

// 生成包含表格模型的单个PDF报告
static bool generateSinglePDFwithTableModel(const QString &title, QAbstractItemModel *model, const QString &remarks, const QString &fileName = "");

// 新函数：根据标题、datalist、表头和备注生成PDF，自动转换datalist为model
    static bool generateSinglePDFWithDataList(const QString &title, const QList<chk_singleitem_result *> &dataList, const QStringList &mheader, const QString &remarks, const QString &fileName = "");

// 生成临时PDF文件，预览并询问是否保存（用于Form_page1，接受自定义内容）
static void generateAndManageReportPDF(const ReportContent &content, QWidget *parent = nullptr);

    // 生成临时PDF文件，预览并询问是否保存（用于Form_page1，只有6个检验项目）
static void generateAndManageReportPDFWith6Projects(const ReportContent &content, QWidget *parent = nullptr);
    
    // 生成临时PDF文件，预览并询问是否保存（用于Form_page2）
static void generateAndManageTablePDF(QWidget *parent = nullptr);

    // 生成临时PDF文件，预览并询问是否保存（用于Form_page3）
//    static void generateAndManageFormPage3PDF(const QString &title, QAbstractItemModel *model, const QString &remarks);

    // 生成临时PDF文件，预览并询问是否保存（基于表格模型）
    static void generateAndManagePDFwithTableModel(const QString &title, QAbstractItemModel *model, const QString &remarks);

signals:
    
public slots:

private:
    PDFGenerator(); // 私有构造函数，防止实例化
    
    // 通用的PDF生成和管理函数
    static void managePDFReport(const QString &defaultFileName, 
                               std::function<bool(const QString &)> generatorFunc, 
                               QWidget *parent);
};

#endif // PDFGENERATOR_H
