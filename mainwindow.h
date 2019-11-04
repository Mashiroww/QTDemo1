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
    void on_treeView_clicked(const QModelIndex &index);

    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void triggerMenu(QAction * action);

    void openFileMenu(QAction * action);

private:
    Ui::MainWindow *ui;
    QList<QStringList> unitName;
    QMenu *partMenu;
    QMenu *unitMenu;

    void init();
    void readTextFile(QString path);
    void setTree();
    void reset();
};

#endif // MAINWINDOW_H
