#include "QvkStorageUI.h"

#include <QDebug>
#include <QTimer>
#include <QStorageInfo>

QvkStorageUI::QvkStorageUI(Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
    QWidget *widget = new QWidget();
    storageGUI.setupUi( widget );
    storageGUI.labelVideoSize->setText("");
    storageGUI.labelFreeSize->setText("");
    ui->tabWidgetScreencast->setCornerWidget( widget, Qt::TopRightCorner);

    QTimer *timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_systemInfo() ) );
    timer->start( 500 );
}

QvkStorageUI::~QvkStorageUI()
{
}


void QvkStorageUI::slot_newVideoFilename( QString filename )
{
    newVideoFilename = filename;
}


void QvkStorageUI::slot_systemInfo()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();

    // Stop or not start a record if disk space smaller 100MB(Default) 1GB(Max)
    if ( storage.bytesAvailable() <= ui->spinBoxDiskFreeSpace->text().toInt() * 1024 * 1024 )
    {
        ui->pushButtonStop->click();
    }

    if ( storageGUI.comboBoxFreeSize->currentText() == " KB" )
       storageGUI.labelFreeSize->setText( QString::number( storage.bytesAvailable()/1024 ) );

    if ( storageGUI.comboBoxFreeSize->currentText() == " MB" )
       storageGUI.labelFreeSize->setText( QString::number( storage.bytesAvailable()/1024/1024 ) );

    if ( storageGUI.comboBoxFreeSize->currentText() == " GB" )
       storageGUI.labelFreeSize->setText( QString::number( storage.bytesAvailable()/1024/1024/1024 ) );

    QDir dir( ui->lineEditVideoPath->text() );
    QStringList filters;
    filters << newVideoFilename;
    QStringList videoFileList = dir.entryList( filters, QDir::Files, QDir::Time );

    if ( !videoFileList.empty() )
    {
        QString string;
        string.append( ui->lineEditVideoPath->text() );
        string.append( "/" );
        string.append( videoFileList.at( 0 ) );
        QFileInfo file( string );
        file.refresh();

        if ( storageGUI.comboBoxVideoSize->currentText() == " KB" )
           storageGUI.labelVideoSize->setText( QString::number( file.size()/1024 ) );

        if ( storageGUI.comboBoxVideoSize->currentText() == " MB" )
           storageGUI.labelVideoSize->setText( QString::number( file.size()/1024/1024 ) );

        if ( storageGUI.comboBoxVideoSize->currentText() == " GB" )
           storageGUI.labelVideoSize->setText( QString::number( file.size()/1024/1024/1024 ) );
    }
}