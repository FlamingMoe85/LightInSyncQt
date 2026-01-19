#ifndef NAMEBUTTON_H
#define NAMEBUTTON_H

#include <QWidget>
#include <QString>


namespace Ui {
class NameButton;
}

class NameButton : public QWidget
{
    Q_OBJECT

public:
    explicit NameButton(QWidget *parent = nullptr, QString _name = " " );
    ~NameButton();

private:
    Ui::NameButton *ui;

signals:
    void Sig_SendName(const QString &name);

private slots:

    void Slot_Clicked();

};

#endif // NAMEBUTTON_H
