// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "crchelper.h"
#include "crc.h"
#include "huffman.h"
#include <vector>
#include <stdexcept>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->huffmanTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculateCRCButton_clicked()
{
    try
    {
        const auto data = CRCHelper::tryParceByteArray(ui->originalTextCRC->toPlainText());
        QString binText;
        for(const auto& i : data)
            binText += QString::number(i, 2).rightJustified(CHAR_BIT, '0');
        ui->originalTextCRCBin->setPlainText(binText);

        const unsigned polynom = CRCHelper::tryParsePolynom(ui->polynomProcess->text());
        ui->polynomProcessBin->setText(QString::number(polynom, 2));

        CRC crc(polynom);
        const auto check = crc.calculate(data.data(), data.size());
        ui->resultCRCHex->setText(QString::number(check, 16));
        ui->resultCRCBin->setText(QString::number(check, 2).rightJustified(crc.polynomDegree(), '0'));
    }
    catch (std::runtime_error& ex)
    {
        QMessageBox::warning(this, "Ошибка вычисления CRC", ex.what());
    }
}

void MainWindow::on_checkCRCButton_clicked()
{
    try
    {
        const QString text = ui->checkText->toPlainText().simplified().remove(' ');
        const unsigned polynom = CRCHelper::tryParsePolynom(ui->polynomCheck->text());

        std::vector<bool> data(text.size());
        for(size_t i = 0; i < data.size(); ++i)
        {
            const QChar& ch = text.at(static_cast<int>(i));
            if(ch == '0')
                continue;
            else if(ch == '1')
                data[i] = true;
            else
                throw std::runtime_error(
                                        QString("Найден недопустимый символ в исходных данных: \"%1\"")
                                        .arg(ch).
                                        toStdString());
        }

        CRC crc(polynom);
        const auto result = crc.calculate(data);
        ui->crcCheckResult->setText(QString::number(result, 16));
        ui->polynomCheckBin->setText(QString::number(crc.polynom(), 2));
    }
    catch (std::runtime_error& ex)
    {
        QMessageBox::warning(this, "Ошибка проверки CRC", ex.what());
    }
}

void MainWindow::on_moveRight_clicked()
{
    ui->checkText->setPlainText(ui->originalTextCRCBin->toPlainText() + ui->resultCRCBin->text());
    ui->polynomCheck->setText(ui->polynomProcess->text());
}

void MainWindow::on_encodeButton_clicked()
{
    try
    {
        const auto text = ui->originalTextHuffman->toPlainText();
        if(text.isEmpty())
            throw std::runtime_error("Строка для кодирования пуста");

        const auto& chars = Huffman::getCharsAndItsCount(text.toStdString());
        const auto root = Huffman::generateTree(chars);
        auto codes = Huffman::getCodes(root);

        ui->huffmanTable->setRowCount(static_cast<int>(chars.size()));

        enum Columns { Char, Count, StringCode };
        int i = 0;
        for(const auto& [ch, count] : chars)
        {
            for(int j = 0; j < ui->huffmanTable->columnCount(); ++j)
            {
                switch(j)
                {
                case Columns::Char:
                    ui->huffmanTable->setItem(i, j, new QTableWidgetItem(QString(ch)));
                    break;
                case Columns::Count:
                    ui->huffmanTable->setItem(i, j, new QTableWidgetItem(QString::number(count)));
                    break;
                case Columns::StringCode:
                    ui->huffmanTable->setItem(i, j, new QTableWidgetItem(codes[ch].c_str()));
                    break;
                default:
                    assert(false);
                }
            }
            ++i;
            qDebug().noquote() << QString("%1;%2;%3").arg(ch).arg(QString::number(count)).arg(codes[ch].c_str());
        }
        ui->encodedText->setText(Huffman::encode(text.toStdString(), codes).c_str());

        const auto avgCodeLength = Huffman::avgCodeLenght(text.toStdString(), chars, codes);
        ui->avgCodeLength->setText(QString("Средняя длина кода: %1").arg(avgCodeLength));

        const auto entropy = Huffman::entropy(text.toStdString(), chars);
        ui->entropy->setText(QString("Энтропия: %1").arg(entropy));
    }
    catch (std::runtime_error& ex)
    {
        QMessageBox::warning(this, "Ошибка кодирования", ex.what());
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->encodedTextPre->setText(ui->encodedText->toPlainText());
}

void MainWindow::on_decodeButton_clicked()
{
    try
    {
        const auto uiTable = ui->huffmanTable;
        Huffman::CodesTable table;

        for(int i = 0; i < uiTable->rowCount(); ++i)
        {
            char ch = uiTable->item(i, 0)->text().at(0).toLatin1();
            std::string code = uiTable->item(i, 2)->text().toStdString();
            table[ch] = code;
        }

        const std::string encodedText = ui->encodedTextPre->toPlainText().toStdString();
        ui->decodedText->setText(Huffman::decode(encodedText, table).c_str());
    }
    catch (std::runtime_error& ex)
    {
        QMessageBox::warning(this, "Ошибка декодирования", ex.what());
    }
}
