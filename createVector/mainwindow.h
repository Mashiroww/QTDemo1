#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void run();

private:
    Ui::MainWindow *ui;
    QVector<QVector<int>> result;
    QStringList errorInfo;

    void printOutput();
    bool isDigitString(QString str);
    bool checkIntersection(int min, int max);
};

#endif // MAINWINDOW_H
