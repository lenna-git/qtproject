#ifndef STREAM_RESULT_ALL_DATA_H
#define STREAM_RESULT_ALL_DATA_H
#include<QString>
#include<QStringList>


class stream_result_all_data
{
public:
    // 构造函数 - 只接受QStringList作为输入
    stream_result_all_data();
    stream_result_all_data(const QStringList &fieldValues);
    stream_result_all_data(const QStringList &fieldNames, const QStringList &fieldValues);
    ~stream_result_all_data();

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
    
    // 兼容性方法，保持与原有代码的兼容
    QString testitem() const;
    QString testresult() const;
    QString wholeresult() const;

private:
    QStringList m_fieldValues;  // 存储所有字段的值
    QStringList m_fieldNames;   // 存储所有字段的名称（可选）
    
    // 内部方法：初始化字段名
    void initFieldNames();
};

#endif // STREAM_RESULT_ALL_DATA_H
