#include "calcvalueform.h"
#include "ui_calcvalueform.h"

CalcValueForm::CalcValueForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalcValueForm)
{
    ui->setupUi(this);

    ui->tableWidget->resizeColumnsToContents();
}

CalcValueForm::~CalcValueForm()
{
    delete ui;
}
