#include "globalparaform.h"
#include "dbtools.h"
#include "globalparadelegate.h"
#include "logutil.h"
#include "ui_globalparaform.h"
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlTableModel>

GlobalParaForm::GlobalParaForm(QWidget *parent) : QWidget(parent), ui(new Ui::SystemParaForm)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this);
    model->setTable("sa0055_global_para");
    model->setHeaderData(ColumnIndex::Name, Qt::Orientation::Horizontal, "参数名");
    model->setHeaderData(ColumnIndex::Value, Qt::Orientation::Horizontal, "参数值");
    model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(ColumnIndex::Id, true);
    ui->tableView->setColumnHidden(ColumnIndex::Type, true);
    ui->tableView->setColumnHidden(ColumnIndex::IntLowLimit, true);
    ui->tableView->setColumnHidden(ColumnIndex::IntHighLimit, true);
    ui->tableView->setColumnHidden(ColumnIndex::DoubleLowLimit, true);
    ui->tableView->setColumnHidden(ColumnIndex::DoubleHighLimit, true);

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setItemDelegate(new GlobalParaDelegate());
    ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);
}

GlobalParaForm::~GlobalParaForm()
{
    delete ui;
}

void GlobalParaForm::showEvent(QShowEvent * /*event*/)
{
    model->select();
}

void GlobalParaForm::hideEvent(QHideEvent * /*event*/)
{
    model->submitAll();
}
