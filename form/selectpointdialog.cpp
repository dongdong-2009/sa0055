#include "selectpointdialog.h"
#include "calcvartablemanager.h"
#include "constvalue.h"
#include "deviceparatablemanager.h"
#include "enumtablemanager.h"
#include "foreignkeycolumndelegate.h"
#include "iec104infotablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "treemodel.h"
#include "ui_selectpointdialog.h"
#include "util.h"
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QTime>
#include <QtSql>
#include "selectpointtreenode.h"

SelectPointDialog::SelectPointDialog(QWidget *parent, SelectPoint::Types types, SelectPoint::Mode mode, SelectPoint::Source pointSource,
                                     bool specifiedDeviceFlag, int specifiedDeviceId)
    : QDialog(parent), ui(new Ui::SelectPointDialog), types(types), mode(mode)
{
    ui->setupUi(this);

    model = new TreeModel(this);
    model->init2SelectPoint(types, pointSource, specifiedDeviceFlag, specifiedDeviceId);

    proxyModel = new SelectPointFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->treeView->setModel(proxyModel);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    ui->treeView->setSelectionMode(mode == SelectPoint::Mode::Single ? QAbstractItemView::SelectionMode::SingleSelection
                                                                     : QAbstractItemView::SelectionMode::MultiSelection);

    connect(ui->filterLineEdit, &QLineEdit::textChanged, [=](const QString &text) { proxyModel->setFilterFixedString(text); });
    connect(ui->cancelButton, &QPushButton::clicked, [=]() { reject(); });
    connect(ui->okButton, &QPushButton::clicked, this, &SelectPointDialog::_checkSelectResult);
}

void SelectPointDialog::_checkSelectResult()
{
    QModelIndexList indexList = ui->treeView->selectionModel()->selectedIndexes();
    QList<SelectPoint> resultList;

    foreach(QModelIndex index, indexList)
    {
        QModelIndex idx = proxyModel->mapToSource(index);
        SelectPointTreeNode *item = static_cast<SelectPointTreeNode *>(idx.internalPointer());
        resultList << SelectPoint(item->dataType(), item->deviceId(), item->pointDescId());
    }

    if(resultList.size() == 0)
    {
        QMessageBox::warning(this, "提示", QString("请选择数据点!\n"));
        return;
    }
    else if(mode == SelectPoint::Mode::Single && resultList.size() > 1)
    {
        QMessageBox::warning(this, "提示", QString("只能选择一个数据点!\n"));
        return;
    }

    emit selectedOk(resultList);
    accept();
}

SelectPointDialog::~SelectPointDialog()
{
    delete ui;
}
