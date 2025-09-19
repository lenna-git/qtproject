#ifndef TABLEGENERATOR_H
#define TABLEGENERATOR_H

#include <QObject>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include "chkresultclass/stream_result_all_data.h"
#include <QMap>

class TableGenerator : public QObject
{
    Q_OBJECT
public:
    explicit TableGenerator(QObject *parent = nullptr);
    
    // 生成表格的方法（基础版，支持3列）
    void generateTable(QStandardItemModel *model, 
                      const QStringList &header, 
                      const QList<stream_result_all_data *> &dataList);
                       
    // 生成表格的方法（增强版，支持多列）
    void generateTable(QStandardItemModel *model, 
                      const QStringList &header, 
                      const QList<stream_result_all_data *> &dataList, 
                      const QList<QMap<int, QString>> &extraColumnsData);
    
    // 为表格视图设置模型并进行基本配置
    void setupTableView(QTableView *view, QStandardItemModel *model);
    
    // 合并单元格的方法
    void mergeCells(QTableView *view, int row, int column, int rowSpan, int columnSpan);
    
signals:
    
public slots:
};

#endif // TABLEGENERATOR_H