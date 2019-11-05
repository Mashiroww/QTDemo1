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

//工具栏点击添加Behaviors
void EditMaterial::behaviorsTriggered(QAction * action)
{
    QList<QStandardItem*> findList = behaviorsModel->findItems(action->text());
    //若不存在则创建
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
            ui->stackedWidget->setCurrentIndex(1);
        }
        else
        {
            QWidget * page = new QWidget(ui->stackedWidget);
            page->setObjectName(QString("page_%0").arg(behaviorCount));
            ui->stackedWidget->addWidget(page);
            createBehaviorInfo(page, action->text());
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(page));
        }
    }
    //若存在则选中
    else
    {
        ui->behaviors->setCurrentIndex(behaviorsModel->indexFromItem(findList[0]));
        ui->stackedWidget->setCurrentIndex(behaviorsModel->indexFromItem(findList[0]).row()+1);
    }
}

void EditMaterial::editDescClick()
{
    descDlg->setMaterialDesc(materialDesc);
    descDlg->show();
}

//Description编辑框保存关闭后同步此窗口的Description
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
    for(int i = 0; i < behaviorsModel->rowCount(); i++)
    {
        qDebug()<<"Data: {";
        QWidget * page = ui->stackedWidget->findChild<QWidget*>(QString("page_%0").arg(i+1));
        if(behaviorsModel->item(i)->text() == "Density")
        {
            QComboBox * distribution = page->findChild<QComboBox*>("densityDistribution");
            qDebug()<<"  Distribution: "<<distribution->currentText();

            QCheckBox * useTemp = page->findChild<QCheckBox*>("densityUseTemp");
            qDebug()<<"  Use Temp: "<<useTemp->isChecked();

            QSpinBox * fieldNum = page->findChild<QSpinBox*>("densityFieldNum");
            qDebug()<<"  Field Number: "<<fieldNum->value();

            QTableView * dataTable = page->findChild<QTableView*>("densityDataTable");
            for(int j = 0; j < densityDataModel->columnCount(); j++)
            {
                if(!dataTable->isColumnHidden(j))
                {
                    QString result = (densityDataModel->item(0, j) == nullptr) ? "" : densityDataModel->item(0, j)->data().toString();
                    qDebug()<<" "<<densityDataHeader[j]<<": "<<result;

                }
            }
        }
        else if(behaviorsModel->item(i)->text() == "Elastic")
        {
            for(int i = 0; i < 8; i++)
            {
                QComboBox * typeSelect = page->findChild<QComboBox*>("elasticTypeSelect");
                qDebug()<<"  Type:"<<typeSelect->itemText(i)<<"{";

                QCheckBox * useTemp = page->findChild<QCheckBox*>(QString("elasticUseTemp%0").arg(i+1));
                qDebug()<<"  Use Temp: "<<useTemp->isChecked();

                QSpinBox * fieldNum = page->findChild<QSpinBox*>(QString("elasticFieldNum%0").arg(i+1));
                qDebug()<<"  Field Number: "<<fieldNum->value();

                qDebug()<<"  }";
            }
        }
        qDebug()<<"}";
    }

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
    gb->setGeometry(0, 0, 431, 381);
    gb->setFont(ft);
    gb->setParent(parent);
    if(behavior == "Density")
    {
        QLabel * distribLabel = new QLabel();
        distribLabel->setObjectName("densityDistribLabel");
        distribLabel->setText("Distribution:");
        distribLabel->setParent(gb);
        distribLabel->setGeometry(10, 20, 91, 16);
        QComboBox * distribution = new QComboBox();
        distribution->setObjectName("densityDistribution");
        distribution->addItem("Uniform");
        distribution->setCurrentIndex(0);
        distribution->setGeometry(110, 20, 141, 22);
        distribution->setParent(gb);

        QCheckBox * useTemp = new QCheckBox();
        useTemp->setObjectName("densityUseTemp");
        useTemp->setText("Use temperature-dependent data");
        useTemp->setParent(gb);
        useTemp->setGeometry(10, 50, 241, 16);
        connect(useTemp, &QCheckBox::stateChanged, this, &EditMaterial::densityCheck);

        QLabel * fieldNumLabel = new QLabel();
        fieldNumLabel->setObjectName("densityFieldNumLabel");
        fieldNumLabel->setText("Number of field variables:");
        fieldNumLabel->setGeometry(10, 80, 181, 16);
        fieldNumLabel->setParent(gb);
        QSpinBox * fieldNum = new QSpinBox();
        fieldNum->setObjectName("densityFieldNum");
        fieldNum->setGeometry(200, 80, 61, 22);
        fieldNum->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        fieldNum->setParent(gb);
        connect(fieldNum, QOverload<int>::of(&QSpinBox::valueChanged), this, &EditMaterial::densityField);

        QGroupBox * dataBox = new QGroupBox();
        dataBox->setObjectName("densityDataBox");
        dataBox->setTitle("Data");
        dataBox->setGeometry(10, 110, 411, 261);
        dataBox->setParent(gb);
        QTableView * dataTable = new QTableView();
        dataTable->setObjectName("densityDataTable");
        dataTable->setGeometry(5, 15, 401, 241);
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
        QLabel * typeLabel = new QLabel();
        typeLabel->setObjectName("elasticTypeLabel");
        typeLabel->setText("Type:");
        typeLabel->setGeometry(10, 20, 31, 16);
        typeLabel->setParent(gb);
        QComboBox * typeSelect = new QComboBox();
        typeSelect->setObjectName("elasticTypeSelect");
        typeSelect->addItem("Isotropic");
        typeSelect->addItem("Engineering Constants");
        typeSelect->addItem("Lamina");
        typeSelect->addItem("Orthotropic");
        typeSelect->addItem("Anisotropic");
        typeSelect->addItem("Traction");
        typeSelect->addItem("Coupled Traction");
        typeSelect->addItem("Shear");
        typeSelect->setCurrentIndex(0);
        typeSelect->setGeometry(50, 20, 171, 22);
        typeSelect->setParent(gb);
        connect(typeSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EditMaterial::elasticSelect);

        //放入stackedWidget中
        QStackedWidget * containType = new QStackedWidget();
        containType->setObjectName("elasticContainType");
        containType->setGeometry(0, 45, 431, 341);
        containType->setParent(gb);
        for(int i = 0; i < 8; i++)
        {
            QWidget * container = new QWidget();
            container->setGeometry(0, 0, 431, 291);
            container->setObjectName(QString("elasticType%0").arg(i+1));
            containType->addWidget(container);

            QCheckBox * useTemp = new QCheckBox();
            useTemp->setObjectName(QString("elasticUseTemp%0").arg(i+1));
            useTemp->setText("Use temperature-dependent data");
            useTemp->setParent(container);
            useTemp->setGeometry(10, 5, 231, 16);
            connect(useTemp, &QCheckBox::stateChanged, this, &EditMaterial::elasticCheck);

            QLabel * fieldNumLabel = new QLabel();
            fieldNumLabel->setObjectName(QString("elasticFieldNumLabel%0").arg(i+1));
            fieldNumLabel->setText("Number of field variables:");
            fieldNumLabel->setGeometry(10, 30, 181, 16);
            fieldNumLabel->setParent(container);
            QSpinBox * fieldNum = new QSpinBox();
            fieldNum->setObjectName(QString("elasticFieldNum%0").arg(i+1));
            fieldNum->setGeometry(200, 30, 61, 22);
            fieldNum->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            fieldNum->setParent(container);
            connect(fieldNum, QOverload<int>::of(&QSpinBox::valueChanged), this, &EditMaterial::elasticField);

            if(i < 7)
            {
                QLabel * moduli = new QLabel();
                moduli->setObjectName(QString("elasticModuli%0").arg(i+1));
                moduli->setText("Moduli time scale (for viscoelasticity):");
                moduli->setGeometry(10, 55, 281, 16);
                moduli->setParent(container);
                QComboBox * moduliSelect = new QComboBox();
                moduliSelect->setObjectName(QString("elasticModuliSelect%0").arg(i+1));
                moduliSelect->addItem("Long-term");
                moduliSelect->addItem("Instantaneous");
                moduliSelect->setGeometry(300, 55, 101, 22);
                moduliSelect->setParent(container);

                QCheckBox * noCompre = new QCheckBox();
                noCompre->setObjectName(QString("elasticNoCompre%0").arg(i+1));
                noCompre->setText("No compression");
                noCompre->setGeometry(10, 80, 121, 16);
                noCompre->setParent(container);

                QCheckBox * noTension = new QCheckBox();
                noTension->setObjectName(QString("elasticNoTension%0").arg(i+1));
                noTension->setText("No tension");
                noTension->setGeometry(10, 105, 91, 16);
                noTension->setParent(container);

                QGroupBox * dataBox = new QGroupBox();
                dataBox->setObjectName(QString("elasticDataBox%0").arg(i+1));
                dataBox->setTitle("Data");
                dataBox->setGeometry(10, 130, 411, 201);
                dataBox->setParent(container);
                QTableView * dataTable = new QTableView();
                dataTable->setObjectName(QString("elasticDataTable%0").arg(i+1));
                dataTable->setGeometry(5, 20, 401, 176);
                dataTable->setParent(dataBox);

                QStandardItemModel * tmpModel = new QStandardItemModel();
                elasticDataModel.push_back(tmpModel);
                dataTable->setModel(tmpModel);
                initElastic(i);
                elasticDataModel[i]->setHorizontalHeaderLabels(elasticDataHeader[i]);
                QStringList tmpList;
                tmpList.push_back("1");
                elasticDataModel[i]->setVerticalHeaderLabels(tmpList);
                dataTable->setColumnHidden(elasticDataHeader[i].indexOf("Temp"), true);
            }
            else if(i == 7)
            {
                QGroupBox * dataBox = new QGroupBox();
                dataBox->setObjectName("elasticDataBox8");
                dataBox->setTitle("Data");
                dataBox->setGeometry(10, 55, 411, 291);
                dataBox->setParent(container);
                QTableView * dataTable = new QTableView();
                dataTable->setObjectName("elasticDataTable8");
                dataTable->setGeometry(5, 20, 401, 266);
                dataTable->setParent(dataBox);

                QStandardItemModel * tmpModel = new QStandardItemModel();
                elasticDataModel.push_back(tmpModel);
                dataTable->setModel(tmpModel);
                initElastic(i);
                elasticDataModel[i]->setHorizontalHeaderLabels(elasticDataHeader[i]);
                QStringList tmpList;
                tmpList.push_back("1");
                elasticDataModel[i]->setVerticalHeaderLabels(tmpList);
                dataTable->setColumnHidden(elasticDataHeader[i].indexOf("Temp"), true);
            }
        }
        containType->setCurrentIndex(0);
    }
}

//是否勾选“Use temperature-dependent data”
void EditMaterial::densityCheck(int state)
{
    if(state == 2)
    {
        QTableView * dataTable = ui->stackedWidget->findChild<QTableView*>("densityDataTable");
        dataTable->setColumnHidden(1, false);
    }
    else if(state == 0)
    {
        QTableView * dataTable = ui->stackedWidget->findChild<QTableView*>("densityDataTable");
        dataTable->setColumnHidden(1, true);
    }
}
void EditMaterial::elasticCheck(int state)
{
    QStackedWidget * containType = ui->stackedWidget->findChild<QStackedWidget*>("elasticContainType");
    int curIdx = containType->currentIndex();
    QTableView * curTable = containType->findChild<QTableView*>(QString("elasticDataTable%0").arg(curIdx+1));
    if(state == 2)
    {
        curTable->setColumnHidden(elasticDataHeader[curIdx].indexOf("Temp"), false);
    }
    else if(state == 0)
    {
        curTable->setColumnHidden(elasticDataHeader[curIdx].indexOf("Temp"), true);
    }
}

//根据SpinBox中的数值在表格中生成或删除Field列
void EditMaterial::densityField(int value)
{
    while(densityDataModel->columnCount() - 2 < value)
    {
        QList<QStandardItem*> itemList;
        QStandardItem * fieldItem = new QStandardItem();
        itemList.push_back(fieldItem);
        densityDataModel->appendColumn(itemList);
    }
    while(densityDataModel->columnCount() - 2 > value)
    {
        QList<QStandardItem*> itemList = densityDataModel->takeColumn(densityDataModel->columnCount()-1);
    }

    densityDataHeader.clear();
    densityDataHeader.push_back("Mass Density");
    densityDataHeader.push_back("Temp");
    for(int i = 1; i <= value; i++)
    {
        densityDataHeader.push_back(QString("Field %0").arg(i));
    }
    densityDataModel->setHorizontalHeaderLabels(densityDataHeader);
}
void EditMaterial::elasticField(int value)
{
    QStackedWidget * containType = ui->stackedWidget->findChild<QStackedWidget*>("elasticContainType");
    int curIdx = containType->currentIndex();
    QTableView * curTable = containType->findChild<QTableView*>(QString("elasticDataTable%0").arg(curIdx+1));
    QStringList curHeader = elasticDataHeader[curIdx];
    int existFieldCnt = 0;
    if(curHeader[curHeader.size()-1].startsWith("Field "))
    {
        existFieldCnt = curHeader[curHeader.size()-1].mid(6).toInt();
    }
    if(existFieldCnt < value)
    {
        for(int i = existFieldCnt + 1; i <= value; i++)
        {
            elasticDataHeader[curIdx].push_back(QString("Field %0").arg(i));
        }
    }
    else if(existFieldCnt > value)
    {
        for(int i = existFieldCnt; i > value; i--)
        {
            elasticDataHeader[curIdx].pop_back();
            elasticDataModel[curIdx]->removeColumns(elasticDataModel[curIdx]->columnCount()-1, 1);
        }
    }
    elasticDataModel[curIdx]->setHorizontalHeaderLabels(elasticDataHeader[curIdx]);
}

void EditMaterial::initElastic(int index)
{
    QStringList tmpHeader;
    switch(index)
    {
    case 0:
        tmpHeader << "Young's Modulus"
                  << "Poisson's Ratio"
                  << "Temp";
        break;
    case 1:
        tmpHeader << "E1"
                  << "E2"
                  << "E3"
                  << "Nu12"
                  << "Nu13"
                  << "..."
                  << "Temp";
        break;
    case 2:
        tmpHeader << "E1"
                  << "E2"
                  << "Nu12"
                  << "G12"
                  << "G13"
                  << "..."
                  << "Temp";
        break;
    case 3:
    case 4:
        tmpHeader << "D1111"
                  << "D1122"
                  << "D2222"
                  << "D1133"
                  << "D2233"
                  << "..."
                  << "Temp";
        break;
    case 5:
        tmpHeader << "E/Enn"
                  << "G1/Ess"
                  << "G2/Ett"
                  << "Temp";
        break;
    case 6:
        tmpHeader << "Enn"
                  << "Ess"
                  << "Ett"
                  << "Ens"
                  << "Ent"
                  << "..."
                  << "Temp";
        break;
    case 7:
        tmpHeader << "Shear Modulus"
                  << "Temp";
        break;
    }
    elasticDataHeader.push_back(tmpHeader);
}

void EditMaterial::elasticSelect(int index)
{
    QStackedWidget * stack = ui->stackedWidget->findChild<QStackedWidget*>("elasticContainType");
    stack->setCurrentIndex(index);
}
