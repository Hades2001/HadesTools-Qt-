#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int index = 0;
    FontTools* _fontTools = new FontTools();
    index = ui->stackedWidget->addWidget(_fontTools);
    QListWidgetItem* fontToolsItem = new QListWidgetItem("Font Tools");
    fontToolsItem->setData(Qt::UserRole,index);
    ui->lw_Applist->addItem(fontToolsItem);


    BitmapConversion* _bitmapConversion = new BitmapConversion();
    connect(_bitmapConversion,&BitmapConversion::setWidgetSize,this,[=](QSize size){
        setstackedWidgetSize(size);
    });
    setstackedWidgetSize(_bitmapConversion->getWidgetSize());

    index = ui->stackedWidget->addWidget(_bitmapConversion);
    QListWidgetItem* bitmapConversionItem = new QListWidgetItem("bitmap Conversion");
    bitmapConversionItem->setData(Qt::UserRole,index);
    ui->lw_Applist->addItem(bitmapConversionItem);

    ui->stackedWidget->setCurrentIndex(index);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setstackedWidgetSize(QSize size)
{
    ui->lw_Applist->resize(QSize(200,size.height()));
    ui->stackedWidget->resize(size);
    this->resize(QSize(size.width() + 200, size.height()));
}

void MainWindow::on_lw_Applist_itemClicked(QListWidgetItem *item)
{
    int index = item->data(Qt::UserRole).toInt();
    ui->stackedWidget->setCurrentIndex(index);
}
