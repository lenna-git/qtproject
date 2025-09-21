#ifndef CHK_ITEMS_RESULT_H
#define CHK_ITEMS_RESULT_H

#include <QString>
#include <QStringList>
#include <QList>
#include "chk_singleitem_result.h"

//一个分表的结构
//含一个标题title，一个表格内容dataList，一个表头tableHeader，一个备注
class chk_items_result
{
public:
    // 构造函数和析构函数
    chk_items_result();
    chk_items_result(const QString &title, 
                     const QList<chk_singleitem_result *> &dataList, 
                     const QStringList &tableHeader, 
                     const QString &remark = "");
    ~chk_items_result();

    // 获取和设置标题
    QString getTitle() const;
    void setTitle(const QString &title);

    // 获取和设置数据列表
    QList<chk_singleitem_result *> getDataList() const;
    void setDataList(const QList<chk_singleitem_result *> &dataList);
    void addDataItem(chk_singleitem_result *item);
    void clearDataList();

    // 获取和设置表格表头
    QStringList getTableHeader() const;
    void setTableHeader(const QStringList &header);

    // 获取和设置备注
    QString getRemark() const;
    void setRemark(const QString &remark);

private:
    QString m_title;                  // 标题
    QList<chk_singleitem_result *> m_dataList;  // chk_singleitem_result组成的数据列表
    QStringList m_tableHeader;        // 表格的表头
    QString m_remark;                 // 备注
};

#endif // CHK_ITEMS_RESULT_H
