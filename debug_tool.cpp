#include <QCoreApplication>
#include <QDebug>
#include <QFontDatabase>
#include "pdfgenerator/pdfgenerator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    qDebug() << "=== 调试工具启动 ===";
    
    // 检查QFontDatabase的hasFamily方法是否可以正确调用
    QFontDatabase fontDb;
    qDebug() << "QFontDatabase::hasFamily(\"SimHei\") 返回: " << fontDb.hasFamily("SimHei");
    
    // 列出所有可用的中文字体
    QStringList chineseFonts = {"SimHei", "WenQuanYi Micro Hei", "Heiti TC", "Arial Unicode MS"};
    for (const QString &font : chineseFonts) {
        qDebug() << "字体\"" << font << "\" 是否可用: " << fontDb.hasFamily(font);
    }
    
    qDebug() << "=== 调试工具结束 ===";
    
    return a.exec();
}