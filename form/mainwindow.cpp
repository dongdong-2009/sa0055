#include "mainwindow.h"
#include "calcvarform.h"
#include "datadumpform.h"
#include "datatraceform.h"
#include "deviceparaform.h"
#include "eventalarmform.h"
#include "formulaform.h"
#include "globalparaform.h"
#include "integralformulaform.h"
#include "logutil.h"
#include "plancurveform.h"
#include "serverparaform.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("AVC/AGC后台配置工具 [*]");

    initActions();
    initStackedWidgets();
    initTreeView();
}

MainWindow::~MainWindow()
{
    delete ui;
}
