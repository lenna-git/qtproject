# PDF行距调整说明

## 问题分析

根据您的反馈，生成的PDF中每行内容挤在一起，这是因为行高(`lineHeight`)设置过小导致的。行高是通过将表格高度除以总行数估算值来计算的，当总行数估算过大时，行高就会变得过小，导致内容重叠或拥挤。

## 已实施的修改

我对两个PDF生成函数进行了如下修改：

### 1. generateReportPDF函数
- 将总行数估算从19减少到16，增加了行高
- 添加了调试日志，显示页面尺寸、表格尺寸和实际行高

### 2. generateReportPDFWith6Projects函数
- 将总行数估算从16减少到14，增加了行高
- 添加了调试日志，显示页面尺寸、表格尺寸和实际行高

## 额外优化建议

如果调整后的行高仍然不够理想，您可以尝试以下进一步的优化：

### 1. 增加表格高度
```cpp
// 在generateReportPDF函数中
qreal tableHeight = pageHeight - 100; // 将140改为100，进一步增加表格高度

// 在generateReportPDFWith6Projects函数中
qreal tableHeight = pageHeight - 80; // 将100改为80，进一步增加表格高度
```

### 2. 进一步减少总行数估算
```cpp
// 在generateReportPDF函数中
qreal totalRows = 14; // 将16进一步减少到14

// 在generateReportPDFWith6Projects函数中
qreal totalRows = 12; // 将14进一步减少到12
```

### 3. 固定行高（而不是通过总行数计算）
```cpp
// 替换原来的行高计算方式
qreal lineHeight = 30; // 设置一个固定的行高值，根据需要调整
```

## 调试提示

我已经添加了调试日志，您可以在应用程序的输出窗口中查看实际的尺寸计算值，这将帮助您进一步调整参数以获得最佳效果。

如果您尝试了上述修改后仍有问题，请提供实际生成的PDF文件的截图，以便我能更准确地判断问题所在。