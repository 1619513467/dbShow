#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->mOpenButton,&QPushButton::clicked,this,&MainWindow::onOpenButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenButton()
{
    QString strFileName = QFileDialog::getOpenFileName(this,"��DB�ļ�",QCoreApplication::applicationFilePath());
    ui->lineEdit->setText(strFileName);
    // ���ӵ����ݿ�
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(strFileName);
    if (!db.open())
    {
        QMessageBox::warning(this,u8"��ʾ",u8"�ļ���ʧ�ܣ�");
        return;
    }
    // ��ȡ���ݿ��еı���б�
    QStringList tableNames = db.tables();
    // ���� QTabWidget
    QTabWidget* tabWidget = new QTabWidget(this);
    // Ϊÿ����񴴽�һ�� QTableWidget ���������
    for (auto it : tableNames)
    {
        QSqlTableModel *pModel = new QSqlTableModel(tabWidget, db);
        pModel->setTable(it);
        pModel->select();
        QTableView *tableView = new QTableView();
        tableView->setModel(pModel);
        tabWidget->addTab(tableView,it);
    }
    ui->verticalLayout->addWidget(tabWidget);
    db.close();
}

