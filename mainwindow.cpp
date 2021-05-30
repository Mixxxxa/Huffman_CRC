#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "crchelper.h"
#include "crc.h"
#include "huffman.h"
#include <vector>
#include <stdexcept>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculateCRCButton_clicked()
{
    try
    {
        auto data = CRCHelper::tryParceByteArray(ui->originalTextCRC->toPlainText());
        unsigned polynom = CRCHelper::tryParsePolynom(ui->polynomProcess->text());

        QString binText;
        for(const auto& i : data)
            binText += QString::number(i, 2).rightJustified(8, '0');
        ui->originalTextCRCBin->setPlainText(binText);

        CRC crc;
        crc.setPolynom(polynom);
        ui->polynomProcessBin->setText(QString::number(crc.polynom(), 2));
        auto result = crc.calculate(data.data(), data.size());
        ui->resultCRCHex->setText(QString::number(result, 16));
        ui->resultCRCBin->setText(QString::number(result, 2));
    }
    catch (std::runtime_error& ex)
    {
        QMessageBox::warning(this, "Ошибка разбора", ex.what());
    }
}

void MainWindow::on_checkCRCButton_clicked()
{
    try
    {
        QString text = ui->checkText->toPlainText().simplified().remove(' ');
        QString calculatedCRC = ui->calculatedCRC->text();
        if(!calculatedCRC.isEmpty())
            text += calculatedCRC;

        CRC crc;
        unsigned polynom = CRCHelper::tryParsePolynom(ui->polynomCheck->text());
        crc.setPolynom(polynom);

        std::vector<uint8_t> data;

        bool ok = false;
        if(text.size() > 8)
        {
            for(int i = 0; i <= text.size() / 8; ++i)
            {
                data.push_back(text.midRef(i*8, 8).toUInt(&ok, 2));
            }
        }
        auto result = crc.calculate(data.data(), data.size());
        ui->polynomCheckBin->setText(QString::number(crc.polynom(), 2));
        ui->crcCheckResult->setText(QString::number(result, 16));
    }
    catch (std::runtime_error& ex)
    {
        QMessageBox::warning(this, "Ошибка проверки", ex.what());
    }
}

void MainWindow::on_moveRight_clicked()
{
    ui->checkText->setPlainText(ui->originalTextCRCBin->toPlainText());
}



void MainWindow::on_encodeButton_clicked()
{
    try
    {
        const auto text = ui->originalTextHuffman->toPlainText();

        Huffman huf;
        const auto& check = huf.getCharsAndItsCount(text.toStdString());
        auto root = huf.generateTree(check);
        auto codes = huf.getCodes(root);
        for(const auto& i : codes)
        {
        }
    }
    catch (std::runtime_error& ex)
    {
        QMessageBox::warning(this, "Ошибка", ex.what());
    }
}
