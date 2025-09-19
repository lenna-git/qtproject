#include "tablegenerator.h"

TableGenerator::TableGenerator(QObject *parent) : QObject(parent)
{

}

// generateTable方法实现（动态生成行列）
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
        
        // 获取当前行的所有字段值
        QStringList allFields = tmp->getAllFields();
        
        // 动态填充所有列数据
        for (int column = 0; column < header.size(); column++)
        {
            // 确保列索引有效且在数据范围内
            if (column >= 0 && column < allFields.size())
            {
                QStandardItem *item = new QStandardItem;
                item->setData(allFields.at(column), Qt::DisplayRole);
                model->setItem(i, column, item);
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
