#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "crc.h"
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
        auto data = tryParceByteArray(ui->originalTextCRC->toPlainText());
        unsigned polynom = tryParsePolynom(ui->polynomProcess->text());

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
        //auto data = tryParceByteArray(ui->originalTextCRC->toPlainText());
        //unsigned polynom = tryParsePolynom(ui->polynomProcess->text());
        //
        //QString binText;
        //for(const auto& i : data)
        //    binText += QString::number(i, 2).rightJustified(8, '0');
        //ui->originalTextCRCBin->setPlainText(binText);
        //
        //CRC crc;
        //crc.setPolynom(polynom);
        //ui->polynomProcessBin->setText(QString::number(crc.polynom(), 2));
        //auto result = crc.calculate(data.data(), data.size());
        //ui->resultCRCHex->setText(QString::number(result, 16));
        //ui->resultCRCBin->setText(QString::number(result, 2));
        //
        //////////////////////////////////
        QString text = ui->checkText->toPlainText().simplified().remove(' ');
        QString calculatedCRC = ui->calculatedCRC->text();
        if(!calculatedCRC.isEmpty())
            text += calculatedCRC;

        CRC crc;
        unsigned polynom = tryParsePolynom(ui->polynomCheck->text());
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

std::vector<uint8_t> MainWindow::tryParceByteArray(const QString& text)
{
    if(text.isEmpty())
        throw std::runtime_error("Поле исходного текста пусто");

    std::vector<uint8_t> result;

    QRegularExpression isHex("^(0x)?[0-9a-f]{2}$", QRegularExpression::CaseInsensitiveOption);
    bool parseOk = false;
    uint8_t temp = 0;

    auto tokens = text.split(' ');
    for(const auto& i : tokens)
    {
        if(i.contains(isHex))
        {
            temp = i.toUInt(&parseOk, 16);
            if(parseOk)
            {
                result.push_back(temp);
                continue;
            }
        }
        throw std::runtime_error(std::string("Ошибка разбора \"" + i.toStdString() + "\""));
    }
    return result;
}

unsigned int MainWindow::tryParsePolynom(const QString& text)
{
    QString prepared = text.simplified().remove(' ');
    if(prepared.isEmpty())
        throw std::runtime_error("Пустой полином");

    unsigned int polynom = 0;

    int matchedCount = 0;
    bool converted = false;
    bool zeroDegreeFound = false;

    QRegularExpression polynomDegreePart("x\\^(\\d{1,2})", QRegularExpression::CaseInsensitiveOption);
    auto matched = polynomDegreePart.globalMatch(prepared);
    for(; matched.hasNext(); ++matchedCount)
    {
        const auto i = matched.next().captured(1);
        unsigned degree = i.toUInt(&converted, 10);
        if(!converted)
            throw std::runtime_error(std::string("Ошибка полинома в \"" + i.toStdString() + "\""));
        polynom |= (1 << degree);
        if(degree == 0)
            zeroDegreeFound = true;
    }
    if(matchedCount != prepared.count('x', Qt::CaseInsensitive))
        throw std::runtime_error("Не все части полинома удалось распознать");

    const bool hasPlusOneAtTheEnd = prepared.endsWith("+1");
    if(hasPlusOneAtTheEnd)
    {
        if(zeroDegreeFound)
            throw std::runtime_error("Ошибка полинома");
        else
            polynom |= 1;
    }

    return polynom;
}