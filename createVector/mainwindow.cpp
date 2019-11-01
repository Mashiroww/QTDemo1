#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->runBtn, SIGNAL(clicked()), this, SLOT(run()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isDigitString(QString str)
{
    if(str == "")
        return false;
    char *c = str.toUtf8().data();
    if(str.length() == 1)
    {
        if(*c && *c >= '1' && *c <= '9')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        while(*c && *c >= '0' && *c <= '9')
            c++;
        if(*c)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

bool MainWindow::checkIntersection(int min, int max)
{
    bool flag = true;
    foreach(QVector<int> vec, result)
    {
        if(max >= vec[0] && min <= vec[vec.size()-1])
        {
            flag = false;
        }
    }
    return flag;
}

void MainWindow::run()
{
    result.clear();
    errorInfo.clear();
    QString input = ui->inputText->toPlainText();
    QStringList inputList = input.split("\n");
    int line = 1;
    foreach(QString str, inputList)
    {
        QStringList range = str.split(",");
        QVector<int> tmp;
        switch(range.size())
        {
            case 1:
            {
                QString numStr = range[0].trimmed();
                if(isDigitString(numStr))
                {
                    tmp.push_back(numStr.toInt());
                }
                else
                {
                    errorInfo.push_back(QString("Line %0: Invalid input").arg(line));
                }
                break;
            }
            case 2:
            {
                QString minNumStr = range[0].trimmed();
                QString maxNumStr = range[1].trimmed();
                if(isDigitString(minNumStr) && isDigitString(maxNumStr))
                {
                    int minNum = minNumStr.toInt();
                    int maxNum = maxNumStr.toInt();
                    if(minNum <= maxNum && checkIntersection(minNum, maxNum))
                    {
                        for(int i = minNum; i <= maxNum; i++)
                        {
                            tmp.push_back(i);
                        }
                    }
                    else if(minNum > maxNum)
                    {
                        errorInfo.push_back(QString("Line %0: Left number is larger than right number").arg(line));
                    }
                    else
                    {
                        errorInfo.push_back(QString("Line %0: Have intersection").arg(line));
                    }
                }
                else
                {
                    errorInfo.push_back(QString("Line %0: Invalid input").arg(line));
                }
                break;
            }
            default:
            {
                errorInfo.push_back(QString("Line %0: Invalid input").arg(line));
            }
        }
        if(tmp.size() != 0)
            result.push_back(tmp);
        line++;
    }
    printOutput();
}

void MainWindow::printOutput()
{
    QString output = "";
    foreach(QVector<int> vec, result)
    {
        if(vec.size() == 0) continue;
        output += "[";
        for(int i = 0; i < vec.size(); i++)
        {
            if(i != 0)
            {
                output += ", ";
            }
            output += QString::number(vec[i]);
        }
        output += "]\n";
    }

    output += "\n\n";
    foreach(QString error, errorInfo)
    {
        output += error + "\n";
    }
    ui->ouputText->setText(output);
}
