#ifndef EXLISTWIDGET_H
#define EXLISTWIDGET_H

#include <QListWidget>

class ExListWidget : public QListWidget
{
Q_OBJECT

public:
    ExListWidget(QWidget *parent = 0);

    void initializeListItem(QString listItemString);
    void setValue(int value);

signals:
    void valueChanged(int value);
private:
    QList<int> valueList;
    QStringList nameList;
    int getValue();
};

#endif // EXLISTWIDGET_H
