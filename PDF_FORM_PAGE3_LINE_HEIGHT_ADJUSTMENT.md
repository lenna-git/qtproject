# Form_page3 PDF行距调整说明

## 问题分析

在使用Form_page3生成PDF报告时，发现表格内容挤在一起，文字显示不清晰，阅读体验较差。

通过检查代码，发现问题出在`PDFGenerator::generateFormPage3PDF`方法中的行高设置上。具体来说：

1. 原来的代码将表头高度和数据行高度固定设置为25单位
2. 这个值对于中文内容来说太小，导致文字显示拥挤
3. 表格高度计算方式不够准确

## 已实施的修改

在`pdfgenerator.cpp`文件中修改了以下内容：

```cpp
// 原代码
qreal tableHeight = 25 * (rowCount + 1); // 表头和数据行
qreal headerHeight = 25;
qreal rowHeight = 25;

// 修改后的代码
qreal headerHeight = 35;
qreal rowHeight = 35;
qreal tableHeight = headerHeight + rowHeight * rowCount; // 表头和数据行
```

主要变更：
1. 将表头高度从25增加到35
2. 将数据行高度从25增加到35
3. 优化了表格高度的计算方式，使其更加准确

## 使用说明

修改后，Form_page3生成的PDF报告中的表格内容将会有更充足的垂直空间，文字显示更加清晰，提高阅读体验。

无需修改其他代码，只要重新编译项目，修改就会生效。