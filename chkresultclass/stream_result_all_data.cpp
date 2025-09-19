#include "stream_result_all_data.h"

// 内部方法：初始化字段名
void stream_result_all_data::initFieldNames()
{
    // 设置默认的字段名
    m_fieldNames.clear();
    m_fieldNames << "testitem" << "testresult" << "wholeresult";
}

// 默认构造函数
stream_result_all_data::stream_result_all_data()
{
    initFieldNames();
    // 初始化三个默认空值
    m_fieldValues << "" << "" << "";
}

// 构造函数：仅通过字段值初始化
stream_result_all_data::stream_result_all_data(const QStringList &fieldValues)
{
    m_fieldValues = fieldValues;
    
    // 为每个字段值创建默认字段名
    m_fieldNames.clear();
    for (int i = 0; i < m_fieldValues.size(); ++i) {
        if (i == 0) m_fieldNames << "testitem";    // 第一个字段保持为testitem
        else if (i == 1) m_fieldNames << "testresult";  // 第二个字段保持为testresult
        else if (i == 2) m_fieldNames << "wholeresult";  // 第三个字段保持为wholeresult
        else m_fieldNames << QString("field%1").arg(i+1);  // 其他字段使用默认命名
    }
}

// 构造函数：通过字段名和字段值初始化
stream_result_all_data::stream_result_all_data(const QStringList &fieldNames, const QStringList &fieldValues)
{
    m_fieldNames = fieldNames;
    m_fieldValues = fieldValues;
    
    // 确保字段名和字段值数量一致
    if (m_fieldNames.size() > m_fieldValues.size()) {
        // 如果字段名多于字段值，填充空值
        for (int i = m_fieldValues.size(); i < m_fieldNames.size(); ++i) {
            m_fieldValues << "";
        }
    } else if (m_fieldNames.size() < m_fieldValues.size()) {
        // 如果字段值多于字段名，添加默认字段名
        for (int i = m_fieldNames.size(); i < m_fieldValues.size(); ++i) {
            m_fieldNames << QString("field%1").arg(i+1);
        }
    }
}

// 析构函数
stream_result_all_data::~stream_result_all_data()
{
    // 不需要特别的清理操作，QStringList会自动处理内存
}

// 添加带名称的字段
void stream_result_all_data::addField(const QString &name, const QString &value)
{
    m_fieldNames << name;
    m_fieldValues << value;
}

// 添加不带名称的字段（使用默认名称）
void stream_result_all_data::addField(const QString &value)
{
    QString fieldName = QString("field%1").arg(m_fieldNames.size()+1);
    
    // 如果是前三个字段，使用传统名称
    if (m_fieldNames.size() == 0) fieldName = "testitem";
    else if (m_fieldNames.size() == 1) fieldName = "testresult";
    else if (m_fieldNames.size() == 2) fieldName = "wholeresult";
    
    m_fieldNames << fieldName;
    m_fieldValues << value;
}

// 通过索引设置字段值
void stream_result_all_data::setField(int index, const QString &value)
{
    if (index >= 0 && index < m_fieldValues.size()) {
        m_fieldValues[index] = value;
    } else if (index == m_fieldValues.size()) {
        // 如果索引正好是下一个位置，直接添加
        addField(value);
    } else if (index > m_fieldValues.size()) {
        // 如果索引超出范围，先填充空值
        while (m_fieldValues.size() < index) {
            addField("");
        }
        addField(value);
    }
}

// 通过字段名设置字段值
void stream_result_all_data::setField(const QString &name, const QString &value)
{
    int index = m_fieldNames.indexOf(name);
    if (index >= 0) {
        m_fieldValues[index] = value;
    } else {
        // 如果字段名不存在，添加新字段
        addField(name, value);
    }
}

// 通过索引获取字段值
QString stream_result_all_data::getField(int index) const
{
    if (index >= 0 && index < m_fieldValues.size()) {
        return m_fieldValues[index];
    }
    return QString();  // 索引无效时返回空字符串
}

// 通过字段名获取字段值
QString stream_result_all_data::getField(const QString &name) const
{
    int index = m_fieldNames.indexOf(name);
    if (index >= 0) {
        return m_fieldValues[index];
    }
    return QString();  // 字段名不存在时返回空字符串
}

// 获取所有字段值
QStringList stream_result_all_data::getAllFields() const
{
    return m_fieldValues;
}

// 获取所有字段名
QStringList stream_result_all_data::getFieldNames() const
{
    return m_fieldNames;
}

// 获取字段数量
int stream_result_all_data::fieldCount() const
{
    return m_fieldValues.size();
}

// 兼容性方法：获取检验项目
QString stream_result_all_data::testitem() const
{
    return getField(0);  // 第一个字段始终是检验项目
}

// 兼容性方法：获取检验结果
QString stream_result_all_data::testresult() const
{
    return getField(1);  // 第二个字段始终是检验结果
}

// 兼容性方法：获取算法检验结果
QString stream_result_all_data::wholeresult() const
{
    return getField(2);  // 第三个字段始终是算法检验结果
}
