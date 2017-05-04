#include "newformuladialog.h"
#include "enumtablemanager.h"
#include "formuladeftablemanager.h"
#include "maptablemanager.h"
#include "ui_newformuladialog.h"
#include <QMessageBox>
#include <QSqlTableModel>

NewFormulaDialog::NewFormulaDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NewFormulaDialog)
{
    ui->setupUi(this);

    ui->formualTypeComboBox->setModel(EnumTableManager::formulaTypeEnumManager()->model());
    ui->formualTypeComboBox->setModelColumn(EnumTableManager::formulaTypeEnumManager()->nameColumnIndex());

    connect(ui->okPushButton, &QPushButton::clicked, this, &NewFormulaDialog::okButtonClicked);
    connect(ui->cancelPushButton, &QPushButton::clicked, [=]() { reject(); });

    int num = FormulaDefTableManager::formulaDefinitionTableManager()->model()->rowCount() + 1;
    QString defaultName = QString("公式#%1").arg(num);
    ui->formualLineEdit->setText(defaultName);
}

NewFormulaDialog::~NewFormulaDialog()
{
    delete ui;
}

void NewFormulaDialog::okButtonClicked()
{

    if (ui->formualLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "提示", "输入的公式名称为空!");
        return;
    }

    int id = MapTableManager::mapTableManager(MapTableManager::TableIndex::FORMULA)->addDesc(ui->formualLineEdit->text());
    if (id == MapTableManager::Error::AlreadyExist)
    {
        QMessageBox::warning(this, "提示", "输入的公式名称已经被使用!");
        return;
    }

    emit createFormulaOk(id, ui->formualTypeComboBox->currentIndex());
    accept();
}
