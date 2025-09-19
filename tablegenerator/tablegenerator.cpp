#include "tablegenerator.h"

TableGenerator::TableGenerator(QObject *parent) : QObject(parent)
{

}

// generateTable方法实现（动态生成行列）
void TableGenerator::generateTable(QStandardItemModel *model, 
                                  QTableView *view, 
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
                item->setTextAlignment(Qt::AlignCenter); // 设置单元格内容居中显示
                model->setItem(i, column, item);
            }
        }
    }
    
    // 如果提供了视图指针，自动合并相同内容的单元格
    if (view)
    {
        autoMergeSameCells(view, model);
    }
}

// 设置表格视图的基本属性
void TableGenerator::setupTableView(QTableView *view, QStandardItemModel *model)
{
    if (!view || !model)
        return;
    
    // 设置表格模型
    view->setModel(model);
    
    // 设置表格属性
    view->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    view->setSelectionBehavior(QAbstractItemView::SelectRows); // 选择整行
    
    // 隐藏行号列（垂直表头）
    view->verticalHeader()->setVisible(false);
    
    // 设置标题行颜色为淡灰色
    view->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #f0f0f0; }");
}

// 合并单元格
void TableGenerator::mergeCells(QTableView *view, int row, int column, int rowSpan, int columnSpan)
{
    if (!view)
        return;
    
    // 执行单元格合并
    view->setSpan(row, column, rowSpan, columnSpan);
}

// 自动合并相同内容的单元格
void TableGenerator::autoMergeSameCells(QTableView *view, QStandardItemModel *model)
{
    if (!view || !model)
        return;
    
    int rowCount = model->rowCount();
    int columnCount = model->columnCount();
    
    // 遍历每一列（从索引1开始，跳过序号列）
    for (int column = 1; column < columnCount; column++)
    {
        if (rowCount <= 1) // 如果行数小于等于1，不需要合并
            continue;
        
        int startRow = 0;
        QVariant startValue = model->data(model->index(startRow, column), Qt::DisplayRole);
        
        // 遍历当前列的每一行
        for (int currentRow = 1; currentRow < rowCount; currentRow++)
        {
            QVariant currentValue = model->data(model->index(currentRow, column), Qt::DisplayRole);
            
            // 如果当前单元格的值与起始单元格的值不同，或者到达最后一行
            if (currentValue != startValue || currentRow == rowCount - 1)
            {
                int span = currentValue != startValue ? currentRow - startRow : currentRow - startRow + 1;
                
                // 如果有两个或更多相同值的单元格，进行合并
                if (span > 1)
                {
                    // 合并单元格，去除行横线
                    mergeCells(view, startRow, column, span, 1);
                }
                
                // 更新起始行和起始值
                startRow = currentRow;
                startValue = currentValue;
            }
        }
    }
}
