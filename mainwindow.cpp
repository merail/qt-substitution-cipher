#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon1.jpg"));
    setFixedSize(this->size());

    codesOfSymbols.append(32);
    codesOfSymbols.append(-88);
    codesOfSymbols.append(-72);
    for(int i = -1;i > -65;i--)
    {
        codesOfSymbols.append(i);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_codeText_clicked()
{
    if(qText=="")
        qText=ui->text->document()->toPlainText();
    if(substitutionTable=="")
    {
        ermsg.setText("Не создана таблица!");
        ermsg.exec();
    }
    else if(qText=="")
    {
        ermsg.setText("Отсутствует текст!");
        ermsg.exec();
    }
    else
    {
        ui->text->document()->clear();
        qEncrypted_text="";
        unsigned int i=0;
        text=qText.toLocal8Bit().constData();
        letters=substitutionTable.toLocal8Bit().constData();
        while (i < text.size())
        {
            symbol = text[i];
            unsigned int j=0;
            while (j<letters.size())
            {
                if (symbol[0] == letters[j])
                    break;
                j=j+4;
            }
            encryptedSymbol = letters[j+2];
            qEncrypted_text =qEncrypted_text + QString::fromLocal8Bit(encryptedSymbol.c_str());
            i++;
        }
        ui->text->document()->setPlainText(qEncrypted_text);
        qText="";
        qEncrypted_text="";
    }
}

void MainWindow::on_decodeText_clicked()
{
    if(qEncrypted_text=="")
        qEncrypted_text=ui->text->document()->toPlainText();
    if(substitutionTable=="")
    {
        ermsg.setText("Не создана таблица!");
        ermsg.exec();
    }
    else if(qEncrypted_text=="")
    {
        ermsg.setText("Отсутствует текст!");
        ermsg.exec();
    }
    else
    {
        ui->text->document()->clear();
        qText="";
        encrypted_text=qEncrypted_text.toLocal8Bit().constData();
        letters=substitutionTable.toLocal8Bit().constData();
        unsigned int i=0;
        while (i < encrypted_text.size())
        {
            encryptedSymbol=encrypted_text[i];
            unsigned int j=2;
            while (j<letters.size())
            {
                if (encryptedSymbol[0] == letters[j])
                    break;
                j=j+4;

            }
            symbol = letters[j-2];
            qText =qText + QString::fromLocal8Bit(symbol.c_str());
            i++;
        }
        ui->text->document()->setPlainText(qText);
        qEncrypted_text="";
        qText="";
    }
}

void MainWindow::on_loadText_clicked()
{
    fileName =QFileDialog::getOpenFileName( this, tr("Открыть документ"),
                                  QDir::currentPath(), tr("Текстовые документы (*.txt)"),
                                  0, QFileDialog::DontUseNativeDialog );
     if (fileName !="")
     {
        fileOfText.setFileName(fileName);
        if(fileOfText.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream readStream(&fileOfText);
            qText=readStream.readAll();
            ui->text->document()->setPlainText(qText);
            fileOfText.close();
        }
     }
}

void MainWindow::on_saveText_clicked()
{
    fileName =QFileDialog::getSaveFileName( this, tr("Сохранить документ"),
                                  QDir::currentPath(), tr("Текстовые документы (*.txt)"),
                                  0, QFileDialog::DontUseNativeDialog );
     if (fileName !="")
     {
        fileOfNewText.setFileName(fileName+".txt");
        if(fileOfNewText.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream writeStream(&fileOfNewText);
            writeStream << qEncrypted_text;
            fileOfNewText.close();
        }
     }
}

void MainWindow::on_generateSubstitutuionTable_clicked()
{
    ui->keyTable->clear();

    qsrand(qrand());
    generationConstant1 = qrand();
    generationConstant2 = qrand();

    for (int i = 0;i < codesOfSymbols.size();i++)
    {
        symbol = codesOfSymbols.at(i);
        codeOfEncryptedSymbol = codesOfSymbols.at((generationConstant1 * i + generationConstant2) % codesOfSymbols.size());
        encryptedSymbol = codeOfEncryptedSymbol;

        substitutionTable = substitutionTable + QString::fromLocal8Bit(symbol.c_str())
                   + "=" + QString::fromLocal8Bit(encryptedSymbol.c_str())+"\n";
    }

    ui->keyTable->document()->setPlainText(substitutionTable);
}

void MainWindow::on_saveSubstitutionTable_clicked()
{
    fileName =QFileDialog::getSaveFileName( this, tr("Сохранить документ"),
                                  QDir::currentPath(), tr("Текстовые документы (*.txt)"),
                                  0, QFileDialog::DontUseNativeDialog );
     if (fileName !="")
     {
        fileOfTable.setFileName(fileName+".txt");
        if(fileOfTable.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream writeStream(&fileOfTable);
            writeStream << substitutionTable;
            fileOfTable.close();
        }
     }
}

void MainWindow::on_loadSubstitutionTable_clicked()
{
    fileName =QFileDialog::getOpenFileName( this, tr("Открыть документ"),
                                      QDir::currentPath(), tr("Текстовые документы (*.txt)"),
                                      0, QFileDialog::DontUseNativeDialog );
     if (fileName !="")
     {
        fileOfTable.setFileName(fileName);
        if(fileOfTable.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream readStream(&fileOfText);
            substitutionTable=readStream.readAll();
            ui->text->document()->setPlainText(substitutionTable);
            fileOfTable.close();
        }
     }
}
