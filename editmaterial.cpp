#include "editmaterial.h"

#include "ui_editMaterialDialog.h"
#include "ui_editMaterialDescription.h"

#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QTableView>


EditMaterial::EditMaterial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditMaterial)
{
    ui->setupUi(this);

    materialName = "";
    materialDesc = "";

    descDlg = new EditMaterialDesc();
    descDlg->hide();

    behaviorsModel = new QStandardItemModel(this);
    ui->behaviors->setModel(behaviorsModel);

    menuBar = new QMenuBar(ui->materialBehaviors);
    menuBar->setGeometry(QRect(0, 0, 381, 21));

    general = menuBar->addMenu("General");
    density = new QAction();
    density->setText("Density");
    general->addAction(density);

    mechanical = menuBar->addMenu("Mechanical");
    elasticity = mechanical->addMenu("Elasticity");
    elastic = new QAction();
    elastic->setText("Elastic");
    elasticity->addAction(elastic);

    ui->behaviorLayout->setMenuBar(menuBar);

    connect(general, SIGNAL(triggered(QAction *)), this, SLOT(behaviorsTriggered(QAction *)));
    connect(mechanical, SIGNAL(triggered(QAction *)), this, SLOT(behaviorsTriggered(QAction *)));

    connect(ui->behaviors, &QListView::clicked, this, &EditMaterial::behaviorClick);

    connect(ui->editDescBtn, &QPushButton::clicked, this, &EditMaterial::editDescClick);
    connect(descDlg, SIGNAL(sendDesc(QString)), this, SLOT(updateDesc(QString)));

    connect(ui->OKBtn, &QPushButton::clicked, this, &EditMaterial::okBtnClick);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &EditMaterial::cancelBtnClick);
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
        ui->behaviors->setCurrentIndex(behaviorsModel->indexFromItem(behavior));
        int behaviorCount = behaviorsModel->rowCount();
        if(behaviorCount == 1)
        {
            QWidget * page = ui->stackedWidget->findChild<QWidget*>("page_1");
            createBehaviorInfo(page, action->text());
        }
        else
        {
            QWidget * page = new QWidget(ui->stackedWidget);
            page->setObjectName(QString("page_%0").arg(behaviorCount));
            ui->stackedWidget->addWidget(page);
            createBehaviorInfo(page, action->text());
        }
    }
    else
    {
        ui->behaviors->setCurrentIndex(behaviorsModel->indexFromItem(findList[0]));
    }
}

void EditMaterial::editDescClick()
{
    descDlg->setMaterialDesc(materialDesc);
    descDlg->show();
}

void EditMaterial::updateDesc(QString desc)
{
    materialDesc = desc;
    ui->description->setText(materialDesc);
}

void EditMaterial::okBtnClick()
{
    materialName = ui->name->text();
    qDebug()<<"Name: "<<materialName;
    qDebug()<<"Desc: "<<materialDesc;
    hide();
}

void EditMaterial::cancelBtnClick()
{
    hide();
}

void EditMaterial::behaviorClick(const QModelIndex &index)
{
    ui->stackedWidget->setCurrentIndex(index.row()+1);

}

void EditMaterial::createBehaviorInfo(QWidget * parent, QString behavior)
{
    QFont ft;
    ft.setPointSize(10);
    QGroupBox * gb = new QGroupBox();
    gb->setObjectName(behavior);
    gb->setTitle(behavior);
    gb->setGeometry(0, 0, 431, 281);
    gb->setFont(ft);
    gb->setParent(parent);
    if(behavior == "Density")
    {
        QLabel * destribLabel = new QLabel();
        destribLabel->setObjectName("destribLabel");
        destribLabel->setText("Distribution:");
        destribLabel->setParent(gb);
        destribLabel->setGeometry(10, 20, 91, 16);
        QComboBox * destribution = new QComboBox();
        destribution->setObjectName("destribution");
        destribution->addItem("Uniform");
        destribution->setCurrentIndex(0);
        destribution->setGeometry(110, 20, 141, 22);
        destribution->setParent(gb);

        QCheckBox * useTemp = new QCheckBox();
        useTemp->setObjectName("useTemp");
        useTemp->setText("Use temperature-dependent data");
        useTemp->setParent(gb);
        useTemp->setGeometry(10, 50, 241, 16);
        connect(useTemp, &QCheckBox::stateChanged, this, &EditMaterial::densityCheck);

        QLabel * fieldNumLabel = new QLabel();
        fieldNumLabel->setObjectName("fieldNumLabel");
        fieldNumLabel->setText("Number of field variables:");
        fieldNumLabel->setGeometry(10, 80, 181, 16);
        fieldNumLabel->setParent(gb);
        QSpinBox * fieldNum = new QSpinBox();
        fieldNum->setObjectName("fieldNum");
        fieldNum->setGeometry(200, 80, 61, 22);
        fieldNum->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        fieldNum->setParent(gb);

        QGroupBox * dataBox = new QGroupBox();
        dataBox->setObjectName("dataBox");
        dataBox->setTitle("Data");
        dataBox->setGeometry(10, 110, 411, 161);
        dataBox->setParent(gb);
        QTableView * dataTable = new QTableView();
        dataTable->setObjectName("dataTable");
        dataTable->setGeometry(5, 15, 401, 141);
        dataTable->setParent(dataBox);
        densityDataModel = new QStandardItemModel();
        dataTable->setModel(densityDataModel);
        densityDataHeader.push_back("Mass Density");
        densityDataHeader.push_back("Temp");
        densityDataModel->setHorizontalHeaderLabels(densityDataHeader);
        QStringList verHeader;
        verHeader.push_back("1");
        densityDataModel->setVerticalHeaderLabels(verHeader);
        dataTable->setColumnHidden(1, true);
    }
    else if(behavior == "Elastic")
    {

    }

}

void EditMaterial::densityCheck(int state)
{
    if(state == 2)
    {
        QTableView * dataTable = ui->stackedWidget->findChild<QTableView*>("dataTable");
        dataTable->setColumnHidden(1, false);
    }
    else if(state == 0)
    {
        QTableView * dataTable = ui->stackedWidget->findChild<QTableView*>("dataTable");
        dataTable->setColumnHidden(1, true);
    }
}

void EditMaterial::densityField()
{

}
