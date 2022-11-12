#ifndef PICOKEYER_H
#define PICOKEYER_H

#include <QLabel>
#include <QSerialPort>
#include <QSettings>
#include <QTimer>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PicoKeyer; }
QT_END_NAMESPACE

class PicoKeyer : public QMainWindow
{
    Q_OBJECT

public:
    PicoKeyer(QWidget *parent = nullptr);
    ~PicoKeyer();

private slots:
    void on_actionE_xit_triggered();
    void on_lineEdit_returnPressed();
    void on_action_Serial_Port_triggered();
    void on_action_Baud_Rate_triggered();
    void on_action_WPM_triggered();
    void on_readyRead();
    void on_clearButton_clicked();
    void on_timeout();
    void connectToKeyer();
    void setNewWPM();
    void on_updateConversationButton_clicked();

    void on_wpmDial_sliderReleased();

private:
    Ui::PicoKeyer *ui;
    QLabel *wpmLabel;
    QSerialPort *sport = nullptr;
    QTimer *tim = nullptr;
    QByteArray inbytes;
    QSettings s = QSettings("PicoKeyer.ini", QSettings::IniFormat, this);
    // load the following from QSettings s;
    int i_interval = 2000;
    int i_baudRate = 115200;
    int i_wpm = 20;
    QString s_sport = "COM3";
};
#endif // PICOKEYER_H
