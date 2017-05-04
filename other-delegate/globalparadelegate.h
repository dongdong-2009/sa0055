#ifndef GLOBALPARADELEGATE_H
#define GLOBALPARADELEGATE_H

#include <QItemDelegate>

class GlobalParaDelegate : public QItemDelegate
{
public:
    GlobalParaDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    void commitSpinBoxData();
    void commitComboBoxData();
};

enum ParaType
{
    String_Type,
    Integer_Type,
    Double_Type,
    Bool_Type
};

enum ColumnIndex
{
    Id,
    Name,
    Value,
    Type,
    IntLowLimit,
    IntHighLimit,
    DoubleLowLimit,
    DoubleHighLimit
};

#endif // GLOBALPARADELEGATE_H
