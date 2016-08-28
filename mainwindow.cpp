#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <string>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include <QFile>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) //Starts
{
    ui->setupUi(this);
    //ui->temp_lcdNumber->display("------");
    //ui->hum_lcdNumber->display("------");
    //Qlabel->Dolle
    arduino = new QSerialPort(this);
    serialBuffer = ""; parsed_data = ""; Temperaturevalue = 0,0; Humidityvalue = 0,0; TiltIO = "Unknown";

        /*
        //Testing code, prints the description, vendor id, and product id of all ports.
          Used it to determine the values for the arduino uno.
        {
        qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            qDebug() << "Description: " << serialPortInfo.description() << "\n";
            qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
            qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
        }
        //Identify the port the arduino uno is on.
        */

    bool arduino_is_available = false;
    QString arduino_uno_port_name;

    //  For each available serial port
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
            if((serialPortInfo.productIdentifier() == arduino_uno_product_id)
                    && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
                arduino_is_available = true;
                arduino_uno_port_name = serialPortInfo.portName();
            }
        }
    }

    if(arduino_is_available){
        qDebug() << "Found the arduino port...\n";
        arduino->setPortName(arduino_uno_port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        qDebug() << "Couldn't find the correct port for the arduino.\n";
        QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to arduino.");
    }
}

MainWindow::~MainWindow()
{
    if(arduino->isOpen()){
        arduino->close(); //    Close the serial port if it's open.
    }
    delete ui;
}

void MainWindow::readSerial()
{

    QStringList buffer_split = serialBuffer.split(","); //  split the serialBuffer string, parsing with ',' as the separator
    if(buffer_split.length() < 3){
        // no parsed value yet so continue accumulating bytes from serial in the buffer.
        serialData = arduino->readAll();
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }else{

        serialBuffer = "";
        qDebug() << buffer_split << "\n";
        parsed_data = buffer_split[1];

        MainWindow::updateTilt(TiltIO);

        Temperaturevalue = (9/5.0) * (parsed_data.toDouble()) + 32;
        qDebug() << "Temperature: " << Temperaturevalue << "\n";
        parsed_data = QString::number(Temperaturevalue, 'g', 4); // format precision of temperature_value to 4 digits or fewer
        MainWindow::updateTemperature(parsed_data);

        Humidityvalue = (9/5.0) * (parsed_data.toDouble()) + 32;
        qDebug() << "Humidity: " << Humidityvalue << "\n";
        parsed_data = QString::number(Humidityvalue, 'g', 4);
        MainWindow::updateHumidity(parsed_data);


    }

}

void MainWindow::updateTemperature(QString sensor_reading)
{
    ui->temp_lcd->display(sensor_reading);
}
void MainWindow::updateHumidity(QString sensor_reading)
{
    ui->hum_lcd->display(sensor_reading);
}
void MainWindow::updateTilt(QString sensor_reading)
{
    ui->Tilt->setText(sensor_reading);
}

void MainWindow::on_Quit_clicked()
{
    qApp->quit();
}

void MainWindow::on_Generate_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
               tr("Spreadsheet files (*.csv);;C++ Files (*.cpp *.h)"));

}


void MainWindow::on_actionContact_Dolle_triggered()
{

    QMessageBox::information(this, "Contact Dolle", "Contact Dolle please use:\n"
                                                    "Call: --------- \n"
                                                    "Email: support@dolle.dk");
}


void MainWindow::on_actionQuit_triggered()
{
     qApp->quit();
}

/*void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFileName("print.ps");
        QPainter painter;
        painter.begin(&printer);

        for (int page = 0; page < numberOfPages; ++page) {

            // Use the painter to draw on the page.

            if (page != lastPage)
                printer.newPage();
        }

        painter.end();
    }

void MainWindow::on_actionReset_triggered()
{

}
*/

