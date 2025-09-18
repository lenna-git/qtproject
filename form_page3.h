#ifndef FORM_PAGE3_H
#define FORM_PAGE3_H

#include <QWidget>

namespace Ui {
class Form_page3;
}

class Form_page3 : public QWidget
{
    Q_OBJECT

public:
    explicit Form_page3(QWidget *parent = 0);
    ~Form_page3();

private:
    Ui::Form_page3 *ui;
};

#endif // FORM_PAGE3_H
