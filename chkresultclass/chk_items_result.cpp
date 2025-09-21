#include "chk_items_result.h"

// 默认构造函数
chk_items_result::chk_items_result()
{
}

// 参数化构造函数
chk_items_result::chk_items_result(const QString &title, 
                                   const QList<chk_singleitem_result *> &dataList, 
                                   const QStringList &tableHeader, 
                                   const QString &remark)
    : m_title(title), m_tableHeader(tableHeader), m_remark(remark)
{
    // 设置数据列表
    m_dataList = dataList;
}

// 析构函数
chk_items_result::~chk_items_result()
{
    // 清理数据列表中的指针
    clearDataList();
}

// 获取标题
QString chk_items_result::getTitle() const
{
    return m_title;
}

// 设置标题
void chk_items_result::setTitle(const QString &title)
{
    m_title = title;
}

// 获取数据列表
QList<chk_singleitem_result *> chk_items_result::getDataList() const
{
    return m_dataList;
}

// 设置数据列表
void chk_items_result::setDataList(const QList<chk_singleitem_result *> &dataList)
{
    // 先清理现有的数据列表
    clearDataList();
    
    // 复制新的数据列表
    m_dataList = dataList;
}

// 添加数据项
void chk_items_result::addDataItem(chk_singleitem_result *item)
{
    if (item) {
        m_dataList.append(item);
    }
}

// 清理数据列表
void chk_items_result::clearDataList()
{
    // 释放所有chk_singleitem_result对象的内存
    for (chk_singleitem_result *item : m_dataList) {
        if (item) {
            delete item;
        }
    }
    
    // 清空列表
    m_dataList.clear();
}

// 获取表格表头
QStringList chk_items_result::getTableHeader() const
{
    return m_tableHeader;
}

// 设置表格表头
void chk_items_result::setTableHeader(const QStringList &header)
{
    m_tableHeader = header;
}

// 获取备注
QString chk_items_result::getRemark() const
{
    return m_remark;
}

// 设置备注
void chk_items_result::setRemark(const QString &remark)
{
    m_remark = remark;
}