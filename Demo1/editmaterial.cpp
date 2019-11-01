#include "editmaterial.h"
#include "ui_editMaterialDialog.h"

#include <QDebug>


EditMaterial::EditMaterial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditMaterial)
{
    ui->setupUi(this);

    behaviorsModel = new QStandardItemModel(this);
    ui->behaviors->setModel(behaviorsModel);

    menuBar = new QMenuBar(ui->materialBehaviors);
    menuBar->setGeometry(QRect(0, 0, 381, 21));

    general = menuBar->addMenu("&General");
    density = new QAction();
    density->setText("Density");
    general->addAction(density);

    mechanical = menuBar->addMenu("Mechanical");
    elasticity = mechanical->addMenu("&Elasticity");
    elastic = new QAction();
    elastic->setText("Elastic");
    elasticity->addAction(elastic);

    ui->behaviorLayout->setMenuBar(menuBar);

    connect(general, SIGNAL(triggered(QAction *)), this, SLOT(behaviorsTriggered(QAction *)));
    connect(mechanical, SIGNAL(triggered(QAction *)), this, SLOT(behaviorsTriggered(QAction *)));
}

EditMaterial::~EditMaterial()
{
    delete ui;
    delete menuBar;
    delete general;
    delete mechanical;
    delete elasticity;
    delete density;
    delete elastic;
}

void EditMaterial::behaviorsTriggered(QAction * action)
{
    QList<QStandardItem*> findList = behaviorsModel->findItems(action->text());
    if(findList.size() == 0)
    {
        QStandardItem * behavior = new QStandardItem(action->text());
        behaviorsModel->appendRow(behavior);
    }
    else
    {
        ui->behaviors->setCurrentIndex(behaviorsModel->indexFromItem(findList[0]));
    }
}


