#include "picokeyer.h"
#include "ui_picokeyer.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSerialPortInfo>

//#include <QDebug>

PicoKeyer::PicoKeyer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PicoKeyer)
{
    ui->setupUi(this);
    wpmLabel = new QLabel("WPM", this);
    statusBar()->addPermanentWidget(wpmLabel);
    ui->lineEdit->setFocus();
    i_interval = s.value("interval", 50).toInt();
    i_baudRate = s.value("baud", 115200).toInt();
    i_wpm = s.value("wpm", 20).toInt();
    s_sport = s.value("sport", "COM3").toString();
    connectToKeyer();
    tim = new QTimer(this);
    tim->setInterval(i_interval); // allows print of incoming data in one shot
    // this timer is started by on_readyRead()
    connect(tim, &QTimer::timeout, this, &PicoKeyer::on_timeout);
    QTimer::singleShot(550, this, &PicoKeyer::setNewWPM);
}

PicoKeyer::~PicoKeyer()
{
    delete ui;
}

void PicoKeyer::on_actionE_xit_triggered()
{
    close();
}

void PicoKeyer::on_lineEdit_returnPressed()
{
    if(sport)
    {
        //qDebug()<<sport->isOpen();
        const QString out = ui->lineEdit->text();
        for(int i = 0; i < out.length(); i++) {
            sport->putChar(out.at(i).toLatin1());
        }
        ui->plainTextEdit->moveCursor(QTextCursor::End);
        ui->plainTextEdit->appendPlainText("SENDING: ");
        sport->putChar('\r');
        sport->flush();
    }
    ui->lineEdit->selectAll();
}

void PicoKeyer::connectToKeyer()
{
    if(!sport) {
        sport = new QSerialPort(this);
    }
    sport->close(); // if already opened, close it first
    connect(sport, &QSerialPort::errorOccurred, this, [=](int error) {
        if(error > 0) {
            ui->plainTextEdit->appendPlainText("Serial Error: " + QString::number(error));
            ui->plainTextEdit->moveCursor(QTextCursor::End);
        }
    });
    s_sport = s.value("serialPort", "ttyUSB0").toString();
    s.setValue("serialPort", s_sport);
    sport->setPortName(s_sport);
    if(sport->open(QIODevice::ReadWrite)) {
        ui->plainTextEdit->appendPlainText("SERIAL PORT " + s_sport + " OPENED");
        ui->plainTextEdit->appendPlainText("Bit Params: " + QString::number(sport->dataBits()) + QString::number(sport->parity()) + QString::number(sport->stopBits()));
        ui->plainTextEdit->appendPlainText("Flow Control: " + QString::number(sport->flowControl()));
        ui->plainTextEdit->moveCursor(QTextCursor::End);
        sport->setBaudRate(i_baudRate);
        connect(sport, &QSerialPort::readyRead, this, &PicoKeyer::on_readyRead, Qt::UniqueConnection);
//        sport->write(QString::number(i_wpm).toLatin1());
//        sport->write("\r");
    }
    else {
        ui->plainTextEdit->appendPlainText("UNABLE TO OPEN SERIAL PORT " + s_sport);
        ui->plainTextEdit->moveCursor(QTextCursor::End);
    }
}

void PicoKeyer::on_action_Serial_Port_triggered()
{
    const QList<QSerialPortInfo> portinfos = QSerialPortInfo::availablePorts();
    QStringList ports;
    foreach (QSerialPortInfo i, portinfos) {
        ports<<i.portName();
    }
    s_sport = QInputDialog::getItem(this, "Choose Serial Port", "Select the serial port that is connected to the Keyer", ports);
    if(s_sport.isEmpty()) {
        s_sport = s.value("sport", "COM3").toString(); // return to saved value
        return;
    }
    else {
        s.setValue("sport", s_sport); // save for next time
        connectToKeyer();
    }
}

void PicoKeyer::on_readyRead()
{
    tim->stop();
    if(sport)
        inbytes.append(sport->readAll());
    tim->start();
}

void PicoKeyer::on_action_Baud_Rate_triggered()
{
    const QString rate = QInputDialog::getItem(this, "Choose Baud Rate", "Select the baud rate.\n\nDefault for Pico USB is 115200.", QStringList()<<"115200"<<"57600"<<"19200"<<"9600");
    if(rate.isEmpty())
        return;
    if(sport) {
        sport->setBaudRate(rate.toInt());
        s.setValue("baud", rate.toInt());
        ui->plainTextEdit->appendPlainText("SERIAL PORT BAUD RATE SET TO " + rate);
        ui->plainTextEdit->moveCursor(QTextCursor::End);
    }
}


void PicoKeyer::on_action_WPM_triggered()
{
    int curr = s.value("wpm", 15).toInt();
    int rate = QInputDialog::getInt(this, "Choose WPM Rate", "Select the WPM rate.\n\nMin. is 5.", curr, 5, 50);
    if((rate > 4) && (rate < 51))
    {
        i_wpm = rate;
        setNewWPM();
    }
    else {
        ui->plainTextEdit->appendPlainText("INVALID WPM RATE!\n\nMust be between 5 and 50 inclusive.");
        ui->plainTextEdit->moveCursor(QTextCursor::End);
    }
}


void PicoKeyer::on_clearButton_clicked()
{
    ui->plainTextEdit->clear();
}

void PicoKeyer::on_timeout()
{
    tim->stop();
    if(inbytes.length() > 0) {
        //qDebug()<<"inbytes:"<<inbytes;
        QString out = inbytes;
        out.replace('\0', '?').replace(">", "AR").replace("+", "KN");
        inbytes.clear();
        ui->plainTextEdit->insertPlainText(out);
        ui->plainTextEdit->moveCursor(QTextCursor::End);
    }
}

void PicoKeyer::setNewWPM()
{
    // use the configured WPM value and ensure it gets saved
    QString srate = "@" + QString::number(i_wpm) + "\r";
    if(sport) {
        ui->plainTextEdit->appendPlainText("WPM RATE SET TO " + QString::number(i_wpm));
        ui->plainTextEdit->moveCursor(QTextCursor::End);
        sport->write(srate.toLatin1());
        sport->flush();
        s.setValue("wpm", i_wpm);
    }
    wpmLabel->setText(QString::number(i_wpm) + " WPM");
}

void PicoKeyer::on_updateConversationButton_clicked()
{
    ui->plainTextEdit->appendPlainText(ui->plainTextEdit_2->toPlainText());
    ui->plainTextEdit->moveCursor(QTextCursor::End);
}

void PicoKeyer::on_wpmDial_valueChanged(int value)
{
    // send the new value to the keyer as WPM
    i_wpm = ui->wpmDial->value();
    setNewWPM();
    wpmLabel->setText(QString::number(i_wpm) + " W");
}
