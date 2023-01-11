#ifndef SETNOEDIT_H
#define SETNOEDIT_H

#include <QStyledItemDelegate>

class setNoEdit : public QStyledItemDelegate
{
    Q_OBJECT;
public:
    setNoEdit(QObject *parent = 0)  : QStyledItemDelegate(parent){}

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index){
        return false;
    }

    QWidget *createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const{
        return nullptr;
    }
};

#endif // SETNOEDIT_H
