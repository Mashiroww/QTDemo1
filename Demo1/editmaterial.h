#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QStandardItemModel>

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

private:
    Ui::EditMaterial *ui;
    QString materialName;
    QMenuBar * menuBar;
    QMenu * general;
    QMenu * mechanical;
    QMenu * elasticity;
    QAction * density;
    QAction * elastic;

    QStandardItemModel * behaviorsModel;

};


