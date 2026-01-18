#ifndef SCROLLAREAWITHVERTLAYOUT_H
#define SCROLLAREAWITHVERTLAYOUT_H

#include <QWidget>

namespace Ui {
class ScrollAreaWithVertLayout;
}

class ScrollAreaWithVertLayout : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollAreaWithVertLayout(QWidget *parent = nullptr);
    ~ScrollAreaWithVertLayout();

    void AddToScrollArea(QWidget* _widget);
    QString name;

private:
    Ui::ScrollAreaWithVertLayout *ui;
};

#endif // SCROLLAREAWITHVERTLAYOUT_H
