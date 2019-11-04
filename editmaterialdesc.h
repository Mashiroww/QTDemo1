#pragma once

#include <QDialog>

namespace Ui {
class EditMaterialDesc;
}

class EditMaterialDesc : public QDialog
{
    Q_OBJECT

public:
    explicit EditMaterialDesc(QWidget *parent = nullptr);
    virtual ~EditMaterialDesc() override;

    void setMaterialDesc(QString desc);
    void createSaveMsgBox();
    void closeEvent(QCloseEvent *event) override;

signals:
    void sendDesc(QString);


private slots:
    void okBtnClick();
    void cancelBtnClick();

private:
    Ui::EditMaterialDesc *ui;

    QString description;
};
