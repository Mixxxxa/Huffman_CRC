#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QPlainTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_calculateCRCButton_clicked();
    void on_checkCRCButton_clicked();
    void on_moveRight_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
