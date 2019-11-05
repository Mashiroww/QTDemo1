#include "editmaterialdesc.h"
#include "ui_editMaterialDescription.h"

#include <QDebug>
#include <QMessageBox>

EditMaterialDesc::EditMaterialDesc(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::EditMaterialDesc)
{
    ui->setupUi(this);

    connect(ui->OKBtn, &QPushButton::clicked, this, &EditMaterialDesc::okBtnClick);
    connect(ui->CancelBtn, &QPushButton::clicked, this, &EditMaterialDesc::cancelBtnClick);
}

EditMaterialDesc::~EditMaterialDesc()
{
    delete ui;
}

void EditMaterialDesc::setMaterialDesc(QString desc)
{
    ui->description->setPlainText(desc);
    description = desc;
}

void EditMaterialDesc::okBtnClick()
{
    description = ui->description->toPlainText();
    emit sendDesc(description);
    hide();
}

void EditMaterialDesc::cancelBtnClick()
{
    createSaveMsgBox();
}

void EditMaterialDesc::createSaveMsgBox()
{
    if(description != ui->description->toPlainText())
    {
        QMessageBox::StandardButton btn = QMessageBox::warning(NULL, "Save?", "Content is changed, save?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        if(btn == QMessageBox::Yes)
        {
            emit sendDesc(ui->description->toPlainText());
            hide();
        }
        else if(btn == QMessageBox::No)
        {
            hide();
        }
    }
    else
    {
        hide();
    }
}

void EditMaterialDesc::closeEvent(QCloseEvent *event)
{
    event->ignore();
    createSaveMsgBox();
}
