#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editmaterial.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItemModel>
#include <QLabel>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    setTree();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::init()
{
    for(int i = 1; i <= 3; i++)
    {
        QWidget *page = ui->stackedWidget->findChild<QWidget*>(QString("page_%0").arg(i));
        QLabel *label = new QLabel(page);
        label->setText(QString("Page%0").arg(i));
        label->setObjectName(QString("label_%0").arg(i));
        label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        label->setGeometry(x(), y(), 250, 250);
        QFont ft;
        ft.setPointSize(20);
        label->setFont(ft);
        label->setParent(page);
    }
    for(int i = 0; i < 3; i++)
    {
        QStringList tmp;
        unitName.push_back(tmp);
    }
    ui->stackedWidget->setCurrentIndex(0);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    partMenu = new QMenu(this);
    unitMenu = new QMenu(this);
    QAction * ac = nullptr;
    ac = new QAction(QStringLiteral("Create"), this);
    partMenu->addAction(ac);

    ac = new QAction(QStringLiteral("Edit"), this);
    unitMenu->addAction(ac);
    ac = new QAction(QStringLiteral("Delete"), this);
    unitMenu->addAction(ac);

    connect(partMenu, SIGNAL(triggered(QAction *)), this, SLOT(triggerMenu(QAction *)));
    connect(unitMenu, SIGNAL(triggered(QAction *)), this, SLOT(triggerMenu(QAction *)));

    connect(ui->menu, SIGNAL(triggered(QAction *)), this, SLOT(openFileMenu(QAction *)));
}

void MainWindow::reset()
{
    for(int i = 1; i <= 3; i++)
    {
        QWidget *page = ui->stackedWidget->findChild<QWidget*>(QString("page_%0").arg(i));
        QList<QLabel*> pageItems = page->findChildren<QLabel*>();
        foreach(QLabel* item, pageItems)
        {
            delete item;
        }
        QLabel *label = new QLabel(page);
        label->setText(QString("Page%0").arg(i));
        label->setObjectName(QString("label_%0").arg(i));
        label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        label->setGeometry(0, 0, 250, 250);
        label->setHidden(false);
        QFont ft;
        ft.setPointSize(20);
        label->setFont(ft);
        label->setParent(page);
    }
    unitName.clear();
    for(int i = 0; i < 3; i++)
    {
        QStringList tmp;
        unitName.push_back(tmp);
    }
}

void MainWindow::readTextFile(QString path)
{
    reset();
    qDebug()<<path;
    QFile inputFile(path);
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    int partIdx = 0;
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line.startsWith("*Part-"))
        {
            QString tmpPartName = line.mid(6);
            partIdx = tmpPartName.toInt() - 1;
        }
        else if(line.startsWith("*unit, name="))
        {
            QString tmpUnitName = line.mid(12);
            if(!unitName[partIdx].contains(tmpUnitName))
                unitName[partIdx].push_back(tmpUnitName);
        }
    }
    inputFile.close();
    setTree();
}

void MainWindow::setTree()
{
    QStandardItemModel *partsModel = new QStandardItemModel(0, 1, this);
    partsModel->setHeaderData(0, Qt::Horizontal, tr("Model"));
    ui->treeView->setModel(partsModel);

    for(int i = 0; i < unitName.size(); i++)
    {
        QStandardItem *partN = new QStandardItem(QString("Part %0").arg(i+1));
        partsModel->appendRow(partN);

        for(int j = 0; j < unitName[i].size(); j++)
        {
            QStandardItem *unitN = new QStandardItem(unitName[i][j]);
            partN->appendRow(unitN);
        }
    }

}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{

    QString clickStr = index.data().toString();
    if(clickStr.startsWith("Part "))
    {
        int pageIdx = clickStr.mid(5).toInt() - 1;
        ui->stackedWidget->setCurrentIndex(pageIdx);


    }
    else
    {
       QString pageStr = index.parent().data().toString();
       int pageIdx = pageStr.mid(5).toInt() - 1;
       ui->stackedWidget->setCurrentIndex(pageIdx);
       QWidget *nowPage = ui->stackedWidget->findChild<QWidget*>(QString("page_%0").arg(pageIdx+1));
       QLabel *nowLabel = nowPage->findChild<QLabel*>(QString("label_%0").arg(pageIdx+1));
       nowLabel->setText(QString("Page%0 %1").arg(pageIdx+1).arg(clickStr));
    }

}

void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    if(index.data().toString().startsWith("Part "))
    {
        partMenu->exec(QCursor::pos());
    }
    else
    {
        unitMenu->exec(QCursor::pos());
    }
}

void MainWindow::triggerMenu(QAction * action)
{
    if(action->text() == "Create")
    {
        EditMaterial * editMaterial = new EditMaterial();
        editMaterial->show();
    }
}

void MainWindow::openFileMenu(QAction * action)
{
    if(action->objectName() == "openFile")
    {
        QString path = QFileDialog::getOpenFileName(this, tr("Open"), ".", tr("Text files (*.txt)"));
        readTextFile(path);
    }
}
