#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMainWindow>
#include <QString>
#include <string>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QMessageBox>

using namespace std;

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
    void on_gnrtTable_clicked();

    void on_saveTable_clicked();

    void on_loadText_clicked();

    void on_codeText_clicked();

    void on_decodeText_clicked();

    void on_saveText_clicked();

    void on_loadKeys_clicked();

private:
    Ui::MainWindow *ui;

    string letter, encrypted_letter, letters;
    int numberOfLetter, numberOfEncryptedLetter, a, c, m;

    string text, encrypted_text;
    QString qLetters, qText, qEncrypted_text, qExtraLetters;

    QString fileName;
    QFile fileOfTable, fileOfText, fileOfNewText;

    QMessageBox ermsg;
};

#endif // MAINWINDOW_H
