#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QByteArray>
#include <QString>

#define labelX 20
#define labelY 20
#define boxX 150

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("串口调试助手");
    resize(600,400);

    //设置串口初始化界面
    QLabel *portLabel=new QLabel("串口",this);
    portLabel->move(labelX,40);
    QComboBox *portBox=new QComboBox(this);
    portBox->move(boxX,40);

    QLabel *baudRateLabel=new QLabel("波特率",this);
    baudRateLabel->move(labelX,80);
    QComboBox *baudRateBox=new QComboBox(this);
    baudRateBox->move(boxX,80);
    baudRateBox->addItem("9600");
    baudRateBox->addItem("115200");

    QLabel *dataBitLabel=new QLabel("数据位",this);
    dataBitLabel->move(labelX,120);
    QComboBox *dataBitBox=new QComboBox(this);
    dataBitBox->move(boxX,120);
    dataBitBox->addItem("7");
    dataBitBox->addItem("8");

    QLabel *stopBitLabel=new QLabel("停止位",this);
    stopBitLabel->move(labelX,160);
    QComboBox *stopBitBox=new QComboBox(this);
    stopBitBox->move(boxX,160);
    stopBitBox->addItem("1");
    stopBitBox->addItem("2");

    QLabel *checkLabel=new QLabel("校验位",this);
    checkLabel->move(labelX,200);
    QComboBox *checkBItBox=new QComboBox(this);
    checkBItBox->move(boxX,200);
    checkBItBox->addItem("无");
    checkBItBox->addItem("奇校验");
    checkBItBox->addItem("偶校验");
    //打开串口
    QPushButton *openBtn=new QPushButton("打开串口",this);
    openBtn->move(20,300);
    //发送数据
    QPushButton *sendBtn=new QPushButton("发送数据",this);
    sendBtn->move(120,300);
    //文本输入
    QTextEdit *textEdit=new QTextEdit(this);
    textEdit->move(300,300);
    textEdit->resize(250,50);
    //文本显示
    QTextBrowser *textBrowser=new QTextBrowser(this);
    textBrowser->move(300,20);
    textBrowser->resize(250,200);

    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        qDebug()<<info.portName();
        QSerialPort serialport;
        serialport.setPort(info);
        if(serialport.open(QSerialPort::ReadWrite))
        {
            portBox->addItem(serialport.portName());
        }
    }

    connect(openBtn,&QPushButton::clicked,[=](){
        serial.setPortName(portBox->currentText());
        serial.open(QSerialPort::ReadWrite);
        switch(baudRateBox->currentIndex())
        {
            case 0:
            serial.setBaudRate(QSerialPort::Baud9600);
            break;
        case 1:
            serial.setBaudRate(QSerialPort::Baud115200);
            break;
        default:
            break;
        }
        switch (dataBitBox->currentIndex()) {
        case 0:
            serial.setDataBits(QSerialPort::Data7);
            break;
        case 1:
            serial.setDataBits(QSerialPort::Data8);
            break;
        default:
            break;
        }
        switch(stopBitBox->currentIndex()){
        case 0:
            serial.setStopBits(QSerialPort::OneStop);
            break;
        case 1:
            serial.setStopBits(QSerialPort::TwoStop);
            break;
        default:
            break;
        }
        switch(checkBItBox->currentIndex()){
        case 0:
            serial.setParity(QSerialPort::NoParity);
            break;
        case 1:
            serial.setParity(QSerialPort::EvenParity);
            break;
        case 2:
            serial.setParity(QSerialPort::OddParity);
            break;
        default:
            break;
        }
        serial.setFlowControl(QSerialPort::NoFlowControl);
    });

    connect(sendBtn,&QPushButton::clicked,[=](){
        serial.write(textEdit->toPlainText().toLatin1());
    });

    QByteArray buf;
    buf=serial.readAll();
    if(!buf.isEmpty())
    {
        QString str=buf;
        textBrowser->setText(str);
    }
    buf.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}



