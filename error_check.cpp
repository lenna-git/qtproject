#include <iostream>
#include <string>

// 简单的错误检查程序
int main() {
    std::string errorMsg = "In static member function 'static bool PDFGenerator::generateReportPDFWithProjects(const QString&,";
    std::string correctFunc = "generateReportPDFWith6Projects";
    std::string wrongFunc = "generateReportPDFWithProjects";
    
    std::cout << "检测到的错误信息包含: " << wrongFunc << std::endl;
    std::cout << "但代码库中实际存在的函数是: " << correctFunc << std::endl;
    std::cout << "这很可能是一个拼写错误，缺少了数字6" << std::endl;
    
    return 0;
}