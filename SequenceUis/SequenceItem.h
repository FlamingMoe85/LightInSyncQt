#ifndef SEQUENCEITEM_H
#define SEQUENCEITEM_H

#include <QWidget>
#include <QList>

#include <QJsonObject>
#include <QJsonArray>

static QJsonObject staticJsonObj;

namespace Ui {
class SequenceItem;
}

class SequenceItem : public QWidget
{
    Q_OBJECT

public:
    explicit SequenceItem(QWidget *parent = nullptr, QString _collection = " ", qint64 _time = 0, qint64 _fadeIn = 1000);
    explicit SequenceItem(QWidget *parent = nullptr, QJsonObject &loadObj = staticJsonObj);
    ~SequenceItem();

    void DisplayTimeStamp();
    void DisplayFadeIn();
    void SetTimeForCurrentTarget(qint64 _time);
    qint64 GetTimeFromCurrentTarget();
    qint64 GetTimeStamp(){return timeStamp;}
    qint64 GetFadeIn(){return fadeIn;}
    QString GetCollection();
    void Save(QJsonArray &jsonSequenceItem) const;

    int CurrentTarget(){return currentTimeChangeTarget;}

    inline static bool less_than( SequenceItem *i1,  SequenceItem *i2)
    {
        return  i1->GetTimeStamp() < i2->GetTimeStamp(); //p1.x() < p2.x();
    }

    static void SortSequenceItems(QList<SequenceItem*> &_seqItemList)
    {
        std::sort(_seqItemList.begin(), _seqItemList.end(), less_than);
    }

signals:
    void Signal_ConnectForTimeChange(SequenceItem*);

private slots:
    void on_pushButton_Time_clicked();

    void on_pushButton_FadeIn_clicked();

    void on_pushButton_Collection_clicked();

private:
    Ui::SequenceItem *ui;
    qint64 timeStamp, fadeIn;
    int currentTimeChangeTarget;
};

#endif // SEQUENCEITEM_H
