#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QWidget>

namespace Ui {
class EditorItem;
}

class EditorItem : public QWidget
{
    Q_OBJECT

public:
    explicit EditorItem(QWidget *parent = nullptr);
    ~EditorItem();

    void SetWidget(QWidget* w);
    void SetTitle(QString _title);

private:
    Ui::EditorItem *ui;
};

#endif // EDITORITEM_H
