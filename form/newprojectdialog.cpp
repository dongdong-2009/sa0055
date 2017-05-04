#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent), ui(new Ui::newProjectDialog)
{
    ui->setupUi(this);

    ui->groupBox_2->setEnabled(false);
    connect(ui->radioButton_3, &QRadioButton::toggled, [=](bool checked) { ui->groupBox_2->setEnabled(!checked); });
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}
