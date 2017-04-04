#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<iostream>
#include<vector>
#include<map>
#include<QTableWidget>
#include<QtSql>
#include<QDebug>
#include<QPushButton>
#include<QLayout>
#include<QGridLayout>
#include<QLineEdit>
#include<QLabel>
#include <QtConcurrent/QtConcurrent>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void setData();
    void getTaghaza();
    void getCost();
    void FindMin();
    void Result();
     void ShowData();
    ~MainWindow();

private slots:
     void InsertRow();
     void DropRow();

     void InsertCol();
     void DropCol();

     void getDataFromTableCell();

     void Reset();


    
private:
    Ui::MainWindow *ui;

    void ClearDB();


    QGridLayout *gridlayout;
    QWidget * central;
    QTableWidget *m_table;

    QPushButton  *InsertRowBTN;
    QPushButton  *DropRowBTN;

    QPushButton  *InsertColBTN;
    QPushButton  *DropColBTN;

    QPushButton  *ResultBTN;

    QPushButton  *ResetButton;

    QSqlDatabase db;
    bool isDbOpen;
    QSqlQuery *query;

    bool isQueryExecute;




    int ResultValue;
};

#endif // MAINWINDOW_H
