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
#include <QList>

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
    void on_loadText_clicked();

    void on_codeText_clicked();

    void on_decodeText_clicked();

    void on_saveText_clicked();

    void on_generateSubstitutuionTable_clicked();

    void on_saveSubstitutionTable_clicked();

    void on_loadSubstitutionTable_clicked();

private:
    Ui::MainWindow *ui;

    QList<int> codesOfSymbols;

    int generationConstant1;
    int generationConstant2;
    string symbol;
    string encryptedSymbol;
    int codeOfSymbol;
    int codeOfEncryptedSymbol;

    string letters;

    string text, encrypted_text;
    QString substitutionTable, qText, qEncrypted_text, qExtraLetters;

    QString fileName;
    QFile tableFile;
    QFile textFile;
    QFile codedTextFile;

    QMessageBox ermsg;
};

#endif // MAINWINDOW_H
