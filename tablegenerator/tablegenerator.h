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
    
    // 生成表格的方法（动态生成行列）
    void generateTable(QStandardItemModel *model, 
                      QTableView *view, 
                      const QStringList &header, 
                      const QList<stream_result_all_data *> &dataList);
    
    // 为表格视图设置模型并进行基本配置
    void setupTableView(QTableView *view, QStandardItemModel *model);
    
    // 合并单元格的方法
    void mergeCells(QTableView *view, int row, int column, int rowSpan, int columnSpan);
    
    // 自动合并相同内容的单元格（相邻单元格内容相同则合并）
    void autoMergeSameCells(QTableView *view, QStandardItemModel *model);
    
signals:
    
public slots:
};

#endif // TABLEGENERATOR_H