#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon1.jpg"));
    letter=' ';
    numberOfLetter=letter[0];
    numberOfEncryptedLetter=-72;
    qExtraLetters=" \n";
    a=3;
    c=23;
    m=65;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_gnrtTable_clicked()
{
    ui->keyTable->clear();
    for (int i = 65;i>0;i--)
        {
            encrypted_letter = numberOfEncryptedLetter;
            letter = numberOfLetter;
            if(i>14)
                qLetters = qLetters + QString::fromLocal8Bit(letter.c_str())
                       + "=" + QString::fromLocal8Bit(encrypted_letter.c_str())+"\n";
            else if(i==14)
                qLetters=qLetters + QString::fromLocal8Bit(letter.c_str())
                        + "=";
            else
                qExtraLetters = qExtraLetters + QString::fromLocal8Bit(letter.c_str())
                                       + "=" + QString::fromLocal8Bit(encrypted_letter.c_str());;
            if(i>1 && i<14)
            {
                qExtraLetters = qExtraLetters + "\n";
            }

            numberOfLetter = -i+1;
            numberOfEncryptedLetter = -(-a*numberOfLetter + c)%m;
        }
    qLetters=qLetters+qExtraLetters;
    ui->keyTable->document()->setPlainText(qLetters);
}

void MainWindow::on_saveTable_clicked()
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
            writeStream << qLetters;
            fileOfTable.close();
        }
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

void MainWindow::on_codeText_clicked()
{
    if(qText=="")
        qText=ui->text->document()->toPlainText();
    if(qLetters=="")
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
        letters=qLetters.toLocal8Bit().constData();
        while (i < text.size())
        {
            letter = text[i];
            unsigned int j=0;
            while (j<letters.size())
            {
                if (letter[0] == letters[j])
                    break;
                j=j+4;
            }
            encrypted_letter = letters[j+2];
            qEncrypted_text =qEncrypted_text + QString::fromLocal8Bit(encrypted_letter.c_str());
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
    if(qLetters=="")
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
        letters=qLetters.toLocal8Bit().constData();
        unsigned int i=0;
        while (i < encrypted_text.size())
        {
            encrypted_letter=encrypted_text[i];
            unsigned int j=2;
            while (j<letters.size())
            {
                if (encrypted_letter[0] == letters[j])
                    break;
                j=j+4;

            }
            letter = letters[j-2];
            qText =qText + QString::fromLocal8Bit(letter.c_str());
            i++;
        }
        ui->text->document()->setPlainText(qText);
        qEncrypted_text="";
        qText="";
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

void MainWindow::on_loadKeys_clicked()
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
            qLetters=readStream.readAll();
            ui->text->document()->setPlainText(qLetters);
            fileOfTable.close();
        }
     }
}
