#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

namespace Ui
{
class MainWindow;
}

class TreeModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum WidgetIndex
    {
        ServerPara = 0,
        DevicePara,
        PlanCurve,
        IntegralFormula,
        CommonFormula,
        CalcVar,
        AlarmRule,
        AIDump,
        AITrace,
        GlobalPara,
        MaxWidgetNum
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TreeModel *model;
    QMenu popupMenu;

    QWidget *stackedWidget[WidgetIndex::MaxWidgetNum];

    void initActions();
    void initStackedWidgets();
    void initTreeView();
    void processLeftButtonClicked(const QModelIndex &index);
    void processRightButtonClicked(const QModelIndex &index);
    void returnToFirstNode();
};

#endif // MAINWINDOW_H
