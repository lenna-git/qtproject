#include "tablegenerator.h"

TableGenerator::TableGenerator(QObject *parent) : QObject(parent)
{

}

// 基础版generateTable方法实现
void TableGenerator::generateTable(QStandardItemModel *model, 
                                  const QStringList &header, 
                                  const QList<stream_result_all_data *> &dataList)
{
    if (!model) return;
    
    // 设置表头
    model->setHorizontalHeaderLabels(header);
    model->setColumnCount(header.size());
    
    // 清空现有数据
    model->setRowCount(0);
    
    // 根据数据列表填充表格
    for(int i = 0; i < dataList.size(); i++)
    {
        const stream_result_all_data *tmp = dataList.at(i);
        
        // 确保模型有足够的行
        if (i >= model->rowCount())
        {
            model->insertRow(i);
        }
        
        // 填充基础的前三列数据
        // 第一列：检验项目
        if (header.size() > 0)
        {
            QStandardItem *item0 = new QStandardItem;
            item0->setData(tmp->m_testitem, Qt::DisplayRole);
            model->setItem(i, 0, item0);
        }
        
        // 第二列：检验结果
        if (header.size() > 1)
        {
            QStandardItem *item1 = new QStandardItem;
            item1->setData(tmp->m_testresult, Qt::DisplayRole);
            model->setItem(i, 1, item1);
        }
        
        // 第三列：算法检验结果
        if (header.size() > 2)
        {
            QStandardItem *item2 = new QStandardItem;
            item2->setData(tmp->m_wholeresult, Qt::DisplayRole);
            model->setItem(i, 2, item2);
        }
    }
}

// 增强版generateTable方法实现（支持多列）
void TableGenerator::generateTable(QStandardItemModel *model, 
                                  const QStringList &header, 
                                  const QList<stream_result_all_data *> &dataList,
                                  const QList<QMap<int, QString>> &extraColumnsData)
{
    if (!model) return;
    
    // 设置表头
    model->setHorizontalHeaderLabels(header);
    model->setColumnCount(header.size());
    
    // 清空现有数据
    model->setRowCount(0);
    
    // 根据数据列表填充表格
    for(int i = 0; i < dataList.size(); i++)
    {
        const stream_result_all_data *tmp = dataList.at(i);
        
        // 确保模型有足够的行
        if (i >= model->rowCount())
        {
            model->insertRow(i);
        }
        
        // 填充基础的前三列数据
        // 第一列：检验项目
        if (header.size() > 0)
        {
            QStandardItem *item0 = new QStandardItem;
            item0->setData(tmp->m_testitem, Qt::DisplayRole);
            model->setItem(i, 0, item0);
        }
        
        // 第二列：检验结果
        if (header.size() > 1)
        {
            QStandardItem *item1 = new QStandardItem;
            item1->setData(tmp->m_testresult, Qt::DisplayRole);
            model->setItem(i, 1, item1);
        }
        
        // 第三列：算法检验结果
        if (header.size() > 2)
        {
            QStandardItem *item2 = new QStandardItem;
            item2->setData(tmp->m_wholeresult, Qt::DisplayRole);
            model->setItem(i, 2, item2);
        }
        
        // 处理额外的列数据
        if (i < extraColumnsData.size())
        {
            const QMap<int, QString> &rowData = extraColumnsData.at(i);
            QMapIterator<int, QString> it(rowData);
            
            while (it.hasNext())
            {
                it.next();
                int column = it.key();
                const QString &value = it.value();
                
                // 确保列索引有效且在header范围内
                if (column >= 0 && column < header.size())
                {
                    QStandardItem *item = new QStandardItem;
                    item->setData(value, Qt::DisplayRole);
                    model->setItem(i, column, item);
                }
            }
        }
    }
}

// 设置表格视图的基本属性
void TableGenerator::setupTableView(QTableView *view, QStandardItemModel *model)
{
    if (!view || !model)
        return;
    
    // 设置表格模型
    view->setModel(model);
    
    // 设置列宽自动调整

    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // 设置表格属性
    view->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    view->setSelectionBehavior(QAbstractItemView::SelectRows); // 选择整行
}

// 合并单元格
void TableGenerator::mergeCells(QTableView *view, int row, int column, int rowSpan, int columnSpan)
{
    if (!view)
        return;
    
    // 执行单元格合并
    view->setSpan(row, column, rowSpan, columnSpan);
}
