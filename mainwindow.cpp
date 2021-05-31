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
        const unsigned polynom = CRCHelper::tryParsePolynom(ui->polynomProcess->text());
        ui->polynomProcessBin->setText(QString::number(polynom, 2));

        auto data = CRCHelper::tryParceByteArray(ui->originalTextCRC->toPlainText());
        QString binText;
        for(const auto& i : data)
            binText += QString::number(i, 2).rightJustified(8, '0');
        ui->originalTextCRCBin->setPlainText(binText);

        CRC crc;
        crc.setPolynom(polynom);

        auto check = crc.calculate(data.data(), data.size());
        ui->resultCRCHex->setText(QString::number(check, 16));
        ui->resultCRCBin->setText(QString::number(check, 2).rightJustified(crc.polynomDegree(), '0'));
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
        const QString text = ui->checkText->toPlainText().simplified().remove(' ');

        CRC crc;
        unsigned polynom = CRCHelper::tryParsePolynom(ui->polynomCheck->text());
        crc.setPolynom(polynom);

        std::vector<bool> data;
        for(const auto& ch : text)
            data.push_back(ch == '1');

        auto result = crc.calculate(data);
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
    ui->checkText->setPlainText(ui->originalTextCRCBin->toPlainText() + ui->resultCRCBin->text());
    ui->polynomCheck->setText(ui->polynomProcess->text());
}

void MainWindow::on_encodeButton_clicked()
{
    try
    {
        const auto text = ui->originalTextHuffman->toPlainText();

        Huffman huf;
        const auto& chars = huf.getCharsAndItsCount(text.toStdString());
        const auto root = huf.generateTree(chars);
        auto codes = huf.getCodes(root);

        ui->huffmanTable->setRowCount(static_cast<int>(chars.size()));
        int i = 0;

        for(const auto& [ch, count] : chars)
        {
            for(int j = 0; j < ui->huffmanTable->columnCount(); ++j)
            {
                switch(j)
                {
                case 0:
                    ui->huffmanTable->setItem(i, j, new QTableWidgetItem(QString(ch)));
                    break;
                case 1:
                    ui->huffmanTable->setItem(i, j, new QTableWidgetItem(QString::number(count)));
                    break;
                case 2:
                    ui->huffmanTable->setItem(i, j, new QTableWidgetItem(codes[ch].c_str()));
                    break;
                }
            }
            ++i;
        }
        ui->encodedText->setText(huf.encode(text.toStdString(), codes).c_str());

        const auto avgCodeLength = huf.avgCodeLenght(text.toStdString(), chars, codes);
        ui->avgCodeLength->setText(QString("Средняя длина кода: %1").arg(avgCodeLength));

        const auto entropy = huf.entropy(text.toStdString(), chars);
        ui->entropy->setText(QString("Энтропия: %1").arg(entropy));
    }
    catch (std::runtime_error& ex)
    {
        QMessageBox::warning(this, "Ошибка", ex.what());
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->encodedTextPre->setText(ui->encodedText->toPlainText());
}

void MainWindow::on_decodeButton_clicked()
{
    Huffman::CodesTable table;
    auto uiTable = ui->huffmanTable;

    for(int i = 0; i < uiTable->rowCount(); ++i)
    {
        char ch = uiTable->item(i, 0)->text().at(0).toLatin1();
        std::string code = uiTable->item(i, 2)->text().toStdString();
        table[ch] = code;
    }

    Huffman huf;
    const std::string encodedText = ui->encodedTextPre->toPlainText().toStdString();
    ui->decodedText->setText(huf.decode(encodedText, table).c_str());
}
