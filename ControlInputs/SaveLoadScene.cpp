#include "SaveLoadScene.h"
#include "ui_SaveLoadScene.h"

SaveLoadScene::SaveLoadScene(QWidget *parent, QString _name) :
    QWidget(parent),
    ui(new Ui::SaveLoadScene)
{
    ui->setupUi(this);
    name = _name;
    UpdateAvailableCurves();

    connect(ui->pushButton_Save, SIGNAL(clicked()), this, SLOT(Slot_Save()));
    connect(ui->pushButton_Load, SIGNAL(clicked()), this, SLOT(Slot_Load()));
    connect(ui->pushButton_SaveAll, SIGNAL(clicked()), this, SLOT(Slot_SaveAll()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SceneSelected(int)));

    connect(&sliderPingTimer, SIGNAL(timeout()), this, SLOT(Slot_SliderPing()));


    sliderPingTimer.setInterval(10);
    sliderPingTimer.start();


}

SaveLoadScene::~SaveLoadScene()
{
    delete ui;
}

void SaveLoadScene::Slot_Save()
{
    Save(ui->plainTextEdit->toPlainText());
    disconnect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SceneSelected(int)));
    UpdateAvailableCurves();
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SceneSelected(int)));
}

void SaveLoadScene::Slot_SaveAll()
{
    for(Position* posUi : posUis)
    {
        posUi->DoSave(true);
    }
    Slot_Save();
}

void SaveLoadScene::Slot_SceneSelected(int i)
{
    Load(ui->comboBox->itemText(i));
}

void SaveLoadScene::UpdateAvailableCurves()
{
    QDir dir;
    qDebug() << "QDir::current(): " << QDir::current();
    QStringList filter("*" + name + ".json");
    availableScenes = QDir::current().entryList(filter);
    qDebug() << "availableCurves: " << availableScenes;
    listModel.setStringList(availableScenes);
    ui->comboBox->setModel(&listModel);
}

void SaveLoadScene::Slot_Load()
{
    Load(ui->comboBox->itemText(ui->comboBox->currentIndex()));
}

void SaveLoadScene::Slot_SliderPing()
{
    for(Position* posUi : posUis)
    {
        posUi->PingForSliderMove();
    }
}

void SaveLoadScene::Save(QString fileName) const
{
    QString f = name + "_" + fileName + name + ".json";
    QFile saveFile(f);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
    }


    QJsonObject jUiObj;
    QJsonArray jUiArr;
    for(Position* posUi : posUis)
    {
        QJsonObject jObj;
        QJsonArray jArr;
        for(int i=0; i<posUi->AmtOfSlider(); i++)
        {
            QJsonValue jx(posUi->GetSliderValue(i));
            jArr.append(jx);
        }
        jObj["sliders"] = jArr;
        jObj["override"] = posUi->GetOverride();
        jObj["save"] = posUi->SupposedToSave();
        //json.append()
        jUiArr.append(jObj);
        posUi->DoSave(false);
    }
    jUiObj["pos_uis"] = jUiArr;
    saveFile.write(QJsonDocument(jUiObj).toJson());
}

void SaveLoadScene::Load(QString fileName) const
{
    QFile loadFile(fileName);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject jUiObj = loadDoc.object();

    QJsonArray jUiArr = jUiObj["pos_uis"].toArray();

    int i=0;
    for (const QJsonValue &v : jUiArr)
    {
        QJsonObject jObj = v.toObject();
        if(v["save"].toBool())
        {
            posUis[i]->SetOverride(v["override"].toBool());
            QJsonArray jAr = v["sliders"].toArray();
            int k=0;
            for(const QJsonValue &v : jAr)
            {
                posUis[i]->SetSliderValue(k, v.toInt());
                k++;
            }
        }
        i++;
    }

    /*
    for(ShadeWidget *sW : shadeWidgets)
    {
        sW->hoverPoints()->ClearPoints();
    }

    for (const QJsonValue &v : loadArr)
    {
        shadeWidgets[i++]->hoverPoints()->FromJson(v.toObject());
    }
    */
}
