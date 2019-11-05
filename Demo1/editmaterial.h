#pragma once

#include "editmaterialdesc.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QStandardItemModel>
#include <QMap>

namespace Ui {
class EditMaterial;
}

class EditMaterial : public QDialog
{
    Q_OBJECT

public:
    explicit EditMaterial(QWidget *parent = nullptr);
    virtual ~EditMaterial() override;

signals:

public slots:

private slots:
    void behaviorsTriggered(QAction * action);
    void editDescClick();
    void updateDesc(QString desc);
    void okBtnClick();
    void cancelBtnClick();
    void behaviorClick(const QModelIndex &index);

    void densityCheck(int state);
    void densityField(int value);


    void elasticCheck(int state);
    void elasticField(int value);
    void elasticSelect(int index);
private:
    Ui::EditMaterial *ui;

    QString materialName;
    QString materialDesc;
    QMenuBar * menuBar;
    QMenu * general;
    QMenu * mechanical;
    QMenu * elasticity;
    QAction * density;
    QAction * elastic;

    QStandardItemModel * behaviorsModel;
    QStandardItemModel * densityDataModel;
    QStringList densityDataHeader;
    QList<QStandardItemModel*> elasticDataModel;
    QList<QStringList> elasticDataHeader;
    QMap<QString, QList<int>> elasticInfo;
    QMap<QString, QList<double>> elasticData;

    EditMaterialDesc * descDlg;

    void initElastic(int index);
    void createBehaviorInfo(QWidget * parent, QString behavior);

};


