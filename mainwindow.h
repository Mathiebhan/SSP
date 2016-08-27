#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QByteArray>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readSerial();
    void updateTemperature(QString);
    void updateHumidity(QString);
    void updateTilt(QString);
    void on_Generate_clicked();
    void on_Quit_clicked();
    void on_actionContact_Dolle_triggered();
    void on_actionQuit_triggered();

    //void on_actionPrint_triggered();
    //void on_actionReset_triggered();



private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9325;
    static const quint16 arduino_uno_product_id = 47;
    QByteArray serialData;
    QString serialBuffer;
    QString TiltIO;
    QString parsed_data;
    double Temperaturevalue;
    double Humidityvalue;
};

#endif //  end MAINWINDOW_H



