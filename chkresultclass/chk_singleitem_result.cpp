#include "chk_singleitem_result.h"

// 内部方法：初始化字段名
void chk_singleitem_result::initFieldNames()
{
    // 设置默认的字段名，统一使用默认命名方式
    m_fieldNames.clear();
    m_fieldNames << "field1" << "field2" << "field3";
}

// 默认构造函数
chk_singleitem_result::chk_singleitem_result()
{
    initFieldNames();
    // 初始化三个默认空值
    m_fieldValues << "" << "" << "";
}

// 构造函数：仅通过字段值初始化
chk_singleitem_result::chk_singleitem_result(const QStringList &fieldValues)
{
    m_fieldValues = fieldValues;
    
    // 为每个字段值创建默认字段名，所有字段统一使用默认命名方式
    m_fieldNames.clear();
    for (int i = 0; i < m_fieldValues.size(); ++i) {
        m_fieldNames << QString("field%1").arg(i+1);  // 所有字段使用默认命名
    }
}

// 构造函数：通过字段名和字段值初始化
chk_singleitem_result::chk_singleitem_result(const QStringList &fieldNames, const QStringList &fieldValues)
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
chk_singleitem_result::~chk_singleitem_result()
{
    // 不需要特别的清理操作，QStringList会自动处理内存
}

// 添加带名称的字段
void chk_singleitem_result::addField(const QString &name, const QString &value)
{
    m_fieldNames << name;
    m_fieldValues << value;
}

// 添加不带名称的字段（使用默认名称）
void chk_singleitem_result::addField(const QString &value)
{
    // 所有字段统一使用默认命名方式
    QString fieldName = QString("field%1").arg(m_fieldNames.size()+1);
    
    m_fieldNames << fieldName;
    m_fieldValues << value;
}

// 通过索引设置字段值
void chk_singleitem_result::setField(int index, const QString &value)
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
void chk_singleitem_result::setField(const QString &name, const QString &value)
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
QString chk_singleitem_result::getField(int index) const
{
    if (index >= 0 && index < m_fieldValues.size()) {
        return m_fieldValues[index];
    }
    return QString();  // 索引无效时返回空字符串
}

// 通过字段名获取字段值
QString chk_singleitem_result::getField(const QString &name) const
{
    int index = m_fieldNames.indexOf(name);
    if (index >= 0) {
        return m_fieldValues[index];
    }
    return QString();  // 字段名不存在时返回空字符串
}

// 获取所有字段值
QStringList chk_singleitem_result::getAllFields() const
{
    return m_fieldValues;
}

// 获取所有字段名
QStringList chk_singleitem_result::getFieldNames() const
{
    return m_fieldNames;
}

// 获取字段数量
int chk_singleitem_result::fieldCount() const
{
    return m_fieldValues.size();
}
