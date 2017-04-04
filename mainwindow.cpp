#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("Tahghigh");
    db.setUserName("Khashayar");
    db.setPassword("khodam");
    isDbOpen=db.open();

    if (!isDbOpen) {
        qDebug()<<"Error occurred opening the database. "<<db.lastError().text();
    }

    query=new QSqlQuery(db);
    if(!query->exec("CREATE TABLE Taghaza(id INTEGER PRIMARY KEY AUTOINCREMENT,value INTEGER,row INTEGER,col INTEGER,flag INTEGER);"))
    {
        qDebug()<<query->lastError().text();
    }
    if(!query->exec("CREATE TABLE ChangeTaghaza(id INTEGER PRIMARY KEY AUTOINCREMENT,value INTEGER,row INTEGER,col INTEGER,flag INTEGER);"))
    {
        qDebug()<<query->lastError().text();
    }

    if(!query->exec("CREATE TABLE Arze(id INTEGER PRIMARY KEY AUTOINCREMENT,value INTEGER,row INTEGER,col INTEGER,flag INTEGER);"))
    {
        qDebug()<<query->lastError().text();
    }
    if(!query->exec("CREATE TABLE ChangeArze(id INTEGER PRIMARY KEY AUTOINCREMENT,value INTEGER,row INTEGER,col INTEGER,flag INTEGER);"))
    {
        qDebug()<<query->lastError().text();
    }

    if(!query->exec("CREATE TABLE Cost(id INTEGER PRIMARY KEY AUTOINCREMENT,row INTEGER,col INTEGER,value INTEGER,flag INTEGER);"))
    {
        qDebug()<<query->lastError().text();
    }
    if(!query->exec("CREATE TABLE ChangeCost(id INTEGER PRIMARY KEY AUTOINCREMENT,row INTEGER,col INTEGER,value INTEGER,flag INTEGER);"))
    {
        qDebug()<<query->lastError().text();
    }

    central=new QWidget(this);

    setCentralWidget(central);
    gridlayout=new QGridLayout(central);
    gridlayout->setAlignment(central, Qt::AlignCenter);


    m_table=new QTableWidget();


    QTableWidgetItem *Hitem = new QTableWidgetItem;

    const int lastCol = m_table->columnCount();
    m_table->setColumnCount(lastCol + 1);



    Hitem->setText(QString::fromUtf8("عرضه"));
    m_table->setHorizontalHeaderItem(lastCol, Hitem);


    const int lastRow = m_table->rowCount();
    m_table->setRowCount(lastRow + 1);


    QTableWidgetItem *Vitem = new QTableWidgetItem;
    Vitem->setText(QString::fromUtf8("تقاضا"));
    m_table->setVerticalHeaderItem(lastRow, Vitem);



    QLabel * label=new QLabel();
    label->setText(QString::fromUtf8("نتیجه"));
    label->setStyleSheet("QLabel{background: green;color : white;}");
    label->setAlignment(Qt::AlignCenter);
    m_table->setCellWidget(0,0,label);




    m_table->horizontalHeader()->setStretchLastSection(true);

    gridlayout->addWidget(m_table);


    InsertColBTN=new QPushButton();
    DropColBTN=new QPushButton();

    InsertRowBTN=new QPushButton();
    DropRowBTN=new QPushButton();

    InsertRowBTN->setText(QString::fromUtf8("اضافه کردن سطر"));
    DropRowBTN->setText(QString::fromUtf8("حذف کردن سطر"));

    InsertColBTN->setText(QString::fromUtf8("اضافه کردن ستون"));
    DropColBTN->setText(QString::fromUtf8("حذف کردن ستون"));

    ResultBTN=new QPushButton();
    ResultBTN->setText(QString::fromUtf8("محاسبه"));

    ResetButton=new QPushButton();
    ResetButton->setText(QString::fromUtf8("ریست"));

    gridlayout->addWidget(InsertColBTN);
    gridlayout->addWidget(InsertRowBTN);

    gridlayout->addWidget(DropColBTN);
    gridlayout->addWidget(DropRowBTN);

    gridlayout->addWidget(ResultBTN);
    gridlayout->addWidget(ResetButton);


    connect(InsertRowBTN, SIGNAL(clicked()), SLOT(InsertRow()));
    connect(InsertColBTN, SIGNAL(clicked()), SLOT(InsertCol()));
    connect(DropColBTN, SIGNAL(clicked()), SLOT(DropCol()));
    connect(DropRowBTN, SIGNAL(clicked()), SLOT(DropRow()));
    connect(ResultBTN, SIGNAL(clicked()), SLOT(getDataFromTableCell()));
    connect(ResetButton, SIGNAL(clicked()), SLOT(Reset()));

    //    QColor color(0,255,0);
    //    m_table->item(m_table->rowCount(),m_table->columnCount())->setBackgroundColor(color);

    ResultValue=0;

}

void MainWindow::Reset()
{
    m_table->clear();
    ResultValue=0;

    while (m_table->rowCount() > 0)
    {
        m_table->removeRow(0);
    }

    while (m_table->columnCount() > 0)
    {
        m_table->removeColumn(0);
    }

    QTableWidgetItem *Hitem = new QTableWidgetItem;

    const int lastCol = m_table->columnCount();
    m_table->setColumnCount(lastCol + 1);

    Hitem->setText(QString::fromUtf8("عرضه"));
    m_table->setHorizontalHeaderItem(lastCol, Hitem);


    const int lastRow = m_table->rowCount();
    m_table->setRowCount(lastRow + 1);


    QTableWidgetItem *Vitem = new QTableWidgetItem;
    Vitem->setText(QString::fromUtf8("تقاضا"));
    m_table->setVerticalHeaderItem(lastRow, Vitem);

    QLabel * label=new QLabel();
    label->setText(QString::fromUtf8("نتیجه"));
    label->setStyleSheet("QLabel{background: green;color : white;}");
    label->setAlignment(Qt::AlignCenter);
    m_table->setCellWidget(0,0,label);

    ClearDB();
}

void MainWindow::InsertRow()
{
    const int lastRow = m_table->rowCount();
    m_table->insertRow(lastRow-1);


    for (int c = 0; c < m_table->horizontalHeader()->count(); ++c)
    {
        m_table->horizontalHeader()->setSectionResizeMode(
                    c, QHeaderView::Stretch);
    }

    for(int i=0;i<m_table->columnCount();i++)
    {

        m_table->setCellWidget(lastRow-1,i,new QLineEdit);


    }


}

void MainWindow::InsertCol()
{
    const int lastCol = m_table->columnCount();
    m_table->insertColumn(lastCol-1);


    for (int c = 0; c < m_table->horizontalHeader()->count(); ++c)
    {
        m_table->horizontalHeader()->setSectionResizeMode(
                    c, QHeaderView::Stretch);
    }

    for(int i=0;i<m_table->rowCount();i++)
        m_table->setCellWidget(i,lastCol-1, new QLineEdit);

}

void MainWindow::DropCol()
{
    const int lastCol = m_table->columnCount();
    m_table->removeColumn(lastCol-2);
}

void MainWindow::DropRow()
{
    const int lastRow = m_table->rowCount();
    m_table->removeRow(lastRow-2);
}

void MainWindow::getDataFromTableCell()
{

    query->clear();


    QFuture<void> future = QtConcurrent::run(this, &MainWindow::setData);
    future.waitForFinished();

    QFuture<void> future1 = QtConcurrent::run(this, &MainWindow::FindMin);
    future1.waitForFinished();

    QFuture<void> future2 = QtConcurrent::run(this, &MainWindow::Result);
    future2.waitForFinished();

    qDebug()<<"RESULT = "<<ResultValue;


    ((QLabel*)m_table->cellWidget(m_table->rowCount()-1,m_table->columnCount()-1))->setText(QString::number(ResultValue));

    QFuture<void> future3 = QtConcurrent::run(this, &MainWindow::ClearDB);






}

void MainWindow::setData()
{
    for(int i=0;i<m_table->rowCount();i++)
    {
        for(int j=0;j<m_table->columnCount();j++)
        {

            if((QLineEdit*)m_table->cellWidget(i,j)!=NULL)
            {
                if(j==m_table->columnCount()-1 && i!=m_table->rowCount()-1)
                {
                    query->prepare("INSERT INTO Arze(value,row,col,flag) VALUES(:value,:row,:col,:flag)");

                    int data=((QLineEdit*)m_table->cellWidget(i,j))->text().toInt();

                    query->bindValue(":value",data);
                    query->bindValue(":flag",0);
                    query->bindValue(":row",i);
                    query->bindValue(":col",j);

                    if(query->exec())
                    {
                        qDebug()<<i<<" , "<<j<<" Arze : "<<((QLineEdit*)m_table->cellWidget(i,j))->text();
                    }
                    else
                    {
                        qDebug()<<query->lastError().text();

                    }
                }
                else
                {
                    if(i==m_table->rowCount()-1 && j!=m_table->columnCount()-1)
                    {
                        query->prepare("INSERT INTO Taghaza(value,row,col,flag) VALUES(:value,:row,:col,:flag)");
                        int data=((QLineEdit*)m_table->cellWidget(i,j))->text().toInt();

                        query->bindValue(":value",data);
                        query->bindValue(":flag",0);
                        query->bindValue(":row",i);
                        query->bindValue(":col",j);
                        if(query->exec())
                        {

                            qDebug()<<i<<" , "<<j<<" Taghaza : "<<((QLineEdit*)m_table->cellWidget(i,j))->text();
                        }
                        else
                        {
                            qDebug()<<query->lastError().text();
                        }
                    }
                    else
                    {
                        if(i!=m_table->rowCount()-1 && j!=m_table->columnCount()-1)
                        {
                            query->prepare("INSERT INTO Cost(row,col,value,flag) VALUES(:row,:col,:value,:flag)");
                            query->bindValue(":row",i);
                            query->bindValue(":col",j);
                            int data=((QLineEdit*)m_table->cellWidget(i,j))->text().toInt();
                            query->bindValue(":value",data);
                            query->bindValue(":flag",0);
                            if(query->exec())
                            {
                                qDebug()<<i<<" , "<<j<<" Cost : "<<((QLineEdit*)m_table->cellWidget(i,j))->text();
                            }
                            else
                            {
                                qDebug()<<query->lastError().text();
                            }


                        }



                    }
                }


            }
        }
    }

}

void MainWindow::FindMin()
{


    QSqlQueryModel Arze_query;
    Arze_query.setQuery("SELECT value FROM Arze WHERE flag='0'");

    QSqlQueryModel Taghaza_query;
    Taghaza_query.setQuery("SELECT value FROM Taghaza WHERE flag='0'");



    int     _Arze[Arze_query.rowCount()],
            _Taghaza[Taghaza_query.rowCount()];

    vector<int> _CostRow,
            _CostCol;


    int c=Arze_query.rowCount();
    for(int i=0;i<c;i++)
    {
        _Arze[i]=Arze_query.record(i).value("value").toInt();
    }

    for(int i=0;i<Taghaza_query.rowCount();i++)
    {
        _Taghaza[i]=Taghaza_query.record(i).value("value").toInt();
    }

    int     LoopFlag=1,
            Row=0,
            Col=0;

    while(LoopFlag!=0)
    {
        if(_Arze[Row]<_Taghaza[Col])
        {

            qDebug()<<_Arze[Row] <<" < "<<_Taghaza[Col];

            query->prepare("INSERT INTO ChangeTaghaza (value,row,col,flag) VALUES(:value,:row,:col,:flag)");
            query->bindValue(":value",_Taghaza[Col]-_Arze[Row]);
            query->bindValue(":flag",0);
            query->bindValue(":row",Row);
            query->bindValue(":col",Col);




            if(!query->exec())
            {
                qDebug()<<query->lastError().text();
            }

            _Taghaza[Col]-=_Arze[Row];
            //            ((QLineEdit*)m_table->cellWidget(m_table->rowCount()-1,Col))->setText(QString::number(_Taghaza[Col]));
            //            ((QLineEdit*)m_table->cellWidget(Row,Col))->setStyleSheet("QLineEdit{background: blue;color : white;}");

            query->prepare("INSERT INTO ChangeCost(row,col,value,flag) VALUES(:row,:col,:value,:flag)");
            query->bindValue(":row",Row);
            query->bindValue(":col",Col);
            query->bindValue(":value",_Arze[Row]);
            query->bindValue(":flag",0);

            ((QLineEdit*)m_table->cellWidget(Row,Col))->setText(((QLineEdit*)m_table->cellWidget(Row,Col))->text().append(" | ").append(QString::number(_Arze[Row])));
            qDebug()<<"Change Cost : "<<"Row : "<<Row<<" Col : "<<Col<<" Value : "<<_Arze[Row];

            if(!query->exec())
            {
                qDebug()<<query->lastError().text();
            }



            _CostRow.push_back(Row);
            _CostCol.push_back(Col);


            //            ((QLineEdit*)m_table->cellWidget(Row,Col))->setStyleSheet("QLineEdit{background: blue;color : white;}");

            query->prepare("UPDATE Taghaza SET flag='1' WHERE row=:row and col=:col");
            query->bindValue(":row",Row);
            query->bindValue(":col",Col);

            if(!query->exec())
            {
                qDebug()<<query->lastError().text();
            }

            ((QLineEdit*)m_table->cellWidget(Row,m_table->columnCount()-1))->setText(QString::number(0));
            Row++;

        }
        else
        {

            if(_Arze[Row]>_Taghaza[Col])
            {
                qDebug()<<_Arze[Row] <<" > "<<_Taghaza[Col];

                query->prepare("INSERT INTO ChangeArze (value,row,col,flag) VALUES(:value,:row,:col,:flag)");
                query->bindValue(":value",_Arze[Row]-_Taghaza[Col]);
                query->bindValue(":flag",0);
                query->bindValue(":row",Row);
                query->bindValue(":col",Col);

                if(!query->exec())
                {
                    qDebug()<<query->lastError().text();
                }

                _Arze[Row]-=_Taghaza[Col];
                ((QLineEdit*)m_table->cellWidget(m_table->rowCount()-1,Col))->setText(QString::number(_Arze[Row]));
                //                ((QLineEdit*)m_table->cellWidget(Row,Col))->setStyleSheet("QLineEdit{background: blue;color : white;}");


                query->prepare("INSERT INTO ChangeCost(row,col,value,flag) VALUES(:row,:col,:value,:flag)");
                query->bindValue(":row",Row);
                query->bindValue(":col",Col);
                query->bindValue(":value",_Taghaza[Col]);
                query->bindValue(":flag",0);

                ((QLineEdit*)m_table->cellWidget(Row,Col))->setText(((QLineEdit*)m_table->cellWidget(Row,Col))->text().append(" | ").append(QString::number(_Taghaza[Col])));
                qDebug()<<"Change Cost : "<<"Row : "<<Row<<" Col : "<<Col<<" Value : "<<_Taghaza[Col];


                if(!query->exec())
                {
                    qDebug()<<query->lastError().text();
                }

                _CostRow.push_back(Row);
                _CostCol.push_back(Col);
                //                ((QLineEdit*)m_table->cellWidget(Row,Col))->setStyleSheet("QLineEdit{background: blue;color : white;}");

                query->prepare("UPDATE Arze SET flag='1' WHERE row=:row and col=:col");
                query->bindValue(":col",Col);
                query->bindValue(":row",Col);

                if(!query->exec())
                {
                    qDebug()<<query->lastError().text();
                }

                ((QLineEdit*)m_table->cellWidget(m_table->rowCount()-1,Col))->setText(QString::number(0));
                Col++;
            }
            else
            {
                if(_Arze[Row]==_Taghaza[Col])
                {
                    qDebug()<<_Arze[Row] <<" = "<<_Taghaza[Col];


                    query->prepare("INSERT INTO ChangeCost(row,col,value,flag) VALUES(:row,:col,:value,:flag)");
                    query->bindValue(":row",Row);
                    query->bindValue(":col",Col);
                    query->bindValue(":value",_Taghaza[Col]);
                    query->bindValue(":flag",0);

                    ((QLineEdit*)m_table->cellWidget(Row,Col))->setText(((QLineEdit*)m_table->cellWidget(Row,Col))->text().append(" | ").append(QString::number(_Taghaza[Col])));
                    qDebug()<<"Change Cost : "<<"Row : "<<Row<<" Col : "<<Col<<" Value : "<<_Taghaza[Col];


                    if(!query->exec())
                    {
                        qDebug()<<query->lastError().text();
                    }

                    _CostRow.push_back(Row);
                    _CostCol.push_back(Col);
                    //                    ((QLineEdit*)m_table->cellWidget(Row,Col))->setStyleSheet("QLineEdit{background: blue;color : white;}");
                    ((QLineEdit*)m_table->cellWidget(Row,m_table->columnCount()-1))->setText(QString::number(0));
                    ((QLineEdit*)m_table->cellWidget(m_table->rowCount()-1,Col))->setText(QString::number(0));

                    LoopFlag=0;
                }
            }
        }

    }

    for(int i=0;i<_CostRow.size();i++)
    {
        for(int j=0;j<_CostCol.size();j++)
        {
            query->prepare("UPDATE Cost SET flag='1' WHERE row=:row and col=:col and flag='0'");
            query->bindValue(":row",_CostRow[i]);
            query->bindValue(":col",_CostCol[j]);

            if(!query->exec())
            {
                qDebug()<<query->lastError().text();
            }

            ((QLineEdit*)m_table->cellWidget(_CostRow[i],_CostCol[j]))->setStyleSheet("QLineEdit{background: orange;color : white;}");
        }
    }

}

void MainWindow::Result()
{
    vector<int>
            _Cost,
            _CostRow,
            _CostCol;



    QSqlQueryModel Change_Arze_query;
    Change_Arze_query.setQuery("SELECT value FROM ChangeArze WHERE flag='0'");


    if(Change_Arze_query.lastError().isValid())
    {
        qDebug()<<Change_Arze_query.lastError().text();
    }

    int     _Change_Arze[Change_Arze_query.rowCount()];

    int count=Change_Arze_query.rowCount();

    for(int i=0;i<count;i++)
    {
        _Change_Arze[i]=Change_Arze_query.record(i).value("value").toInt();
    }

    QSqlQueryModel Change_Taghaza_query;
    Change_Taghaza_query.setQuery("SELECT value FROM ChangeTaghaza WHERE flag='0'");

    if(Change_Taghaza_query.lastError().isValid())
    {
        qDebug()<<Change_Taghaza_query.lastError().text();
    }

    int _Change_Taghaza[Change_Taghaza_query.rowCount()];

    for(int i=0;i<Change_Taghaza_query.rowCount();i++)
    {
        _Change_Taghaza[i]=Change_Taghaza_query.record(i).value("value").toInt();
    }

    QSqlQueryModel Change_Cost_query;
    Change_Cost_query.setQuery("SELECT * FROM ChangeCost WHERE flag='0'");

    if(Change_Cost_query.lastError().isValid())
    {
        qDebug()<<Change_Cost_query.lastError().text();
    }



    int     _Change_Cost[Change_Cost_query.rowCount()],
            _ChangeCostRow[Change_Cost_query.rowCount()],
            _ChangeCostCol[Change_Cost_query.rowCount()];



    QSqlQueryModel Cost_query;
    Cost_query.setQuery("SELECT * FROM Cost WHERE flag='1'");

    if(Cost_query.lastError().isValid())
    {
        qDebug()<<Cost_query.lastError().text();
    }

    count=Cost_query.rowCount();
    int c1=Change_Cost_query.rowCount();

    for(int j=0;j<c1;j++)
    {
        qDebug()<<"Value Change : "<<Change_Cost_query.record(j).value("value").toInt();
        for(int i=0;i<count;i++)
        {
            qDebug()<<"Value  : "<<Cost_query.record(i).value("value").toInt();

            //            _Change_Cost[i]=Change_Cost_query.record(i).value("value").toInt();
            //            _ChangeCostRow[i]=Change_Cost_query.record(i).value("row").toInt();
            //            _ChangeCostCol[i]=Change_Cost_query.record(i).value("col").toInt();



            if(Cost_query.record(i).value("row").toInt()==Change_Cost_query.record(j).value("row").toInt()
                    && Cost_query.record(i).value("col").toInt()==Change_Cost_query.record(j).value("col").toInt())
            {
                qDebug()<<"Row : "<<Cost_query.record(i).value("row").toInt()<<" = "<<Change_Cost_query.record(j).value("row").toInt()
                       <<"  Col : "<<Cost_query.record(i).value("col").toInt()<<" = "<<Change_Cost_query.record(j).value("col").toInt();

                _Cost.push_back(Cost_query.record(i).value("value").toInt());
                _CostRow.push_back(Cost_query.record(i).value("row").toInt());
                _CostCol.push_back(Cost_query.record(i).value("col").toInt());

                ResultValue+=(Cost_query.record(i).value("value").toInt()*Change_Cost_query.record(j).value("value").toInt());
            }

        }
    }

}

void MainWindow::ClearDB()
{
    if(query->exec("DELETE FROM COST"))
    {
        qDebug()<<"DELETE FROM COST";
    }
    if(query->exec("DELETE FROM ChangeCost"))
    {
        qDebug()<<"DELETE FROM ChangeCost";
    }
    if(query->exec("DELETE FROM Arze"))
    {
        qDebug()<<"DELETE FROM Arze";
    }
    if(query->exec("DELETE FROM ChangeArze"))
    {
        qDebug()<<"DELETE FROM ChangeArze";
    }
    if(query->exec("DELETE FROM Taghaza"))
    {
        qDebug()<<"DELETE FROM Taghaza";
    }
    if(query->exec("DELETE FROM ChangeTaghaza"))
    {
        qDebug()<<"DELETE FROM ChangeTaghaza";
    }
}


MainWindow::~MainWindow()
{
    db.close();

    delete gridlayout;
    delete central;
    delete m_table;
    delete InsertRowBTN;
    delete DropRowBTN;
    delete InsertColBTN;
    delete DropColBTN;
    delete ResultBTN;
    delete ResetButton;
    delete query;
    delete ui;
}


