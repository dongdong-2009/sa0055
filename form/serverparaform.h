#ifndef SERVERPARAFORM_H
#define SERVERPARAFORM_H

#include <QWidget>

namespace Ui
{
class ServerParaForm;
}

class QDataWidgetMapper;
class QSqlTableModel;

class ServerParaForm : public QWidget
{
    Q_OBJECT

public:
    explicit ServerParaForm(QWidget *parent = 0);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    ~ServerParaForm();

    QStringList configValidCheck();
    void saveData();
private:
    Ui::ServerParaForm *ui;
    QDataWidgetMapper *mapper;
    void _createDataWidgetMapper();
    void _setUnusedWidgetDisable();
};

#endif // SERVERPARAFORM_H
