#ifndef CHK_SINGLEITEM_RESULT_H
#define CHK_SINGLEITEM_RESULT_H
#include<QString>
#include<QStringList>


class chk_singleitem_result
{
public:
    // 构造函数 - 只接受QStringList作为输入
    chk_singleitem_result();
    chk_singleitem_result(const QStringList &fieldValues);
    chk_singleitem_result(const QStringList &fieldNames, const QStringList &fieldValues);
    ~chk_singleitem_result();

    // 添加数据
    void addField(const QString &name, const QString &value);
    void addField(const QString &value);
    
    // 设置数据
    void setField(int index, const QString &value);
    void setField(const QString &name, const QString &value);
    
    // 获取数据
    QString getField(int index) const;
    QString getField(const QString &name) const;
    
    // 获取所有字段
    QStringList getAllFields() const;
    QStringList getFieldNames() const;
    
    // 获取字段数量
    int fieldCount() const;
    
    

private:
    QStringList m_fieldValues;  // 存储所有字段的值
    QStringList m_fieldNames;   // 存储所有字段的名称（可选）
    
    // 内部方法：初始化字段名
    void initFieldNames();
};

#endif // CHK_SINGLEITEM_RESULT_H
