#include "form_page3.h"
#include "ui_form_page3.h"

Form_page3::Form_page3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_page3)
{
    ui->setupUi(this);
}

Form_page3::~Form_page3()
{
    delete ui;
}
