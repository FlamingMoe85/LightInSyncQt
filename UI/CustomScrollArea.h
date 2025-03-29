#ifndef CUSTOMSCROLLAREA_H
#define CUSTOMSCROLLAREA_H

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class CustomScrollArea;
}

class CustomScrollArea : public QWidget
{
    Q_OBJECT

public:
    explicit CustomScrollArea(QWidget *parent = nullptr);
    ~CustomScrollArea();

    void AddWidget(QWidget* w);
    void RemoveWidget(QWidget* w);
    void SetHeightOfItems(int _h);
    void SetWidthOfItems(int _w);

private:
    Ui::CustomScrollArea *ui;

    QWidget* scrollWidget;
    QVBoxLayout* vLayout;

    unsigned int height, h;
};

#endif // CUSTOMSCROLLAREA_H
