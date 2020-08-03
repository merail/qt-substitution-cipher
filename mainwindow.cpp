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


void MainWindow::on_generateSubstitutuionTable_clicked()
{
    ui->substitutionTable->clear();
    qSubstitutionTable.clear();

    qsrand(qrand());
    generationConstant1 = qrand();
    generationConstant2 = qrand();

    for (int i = 0;i < codesOfSymbols.size();i++)
    {
        symbol = codesOfSymbols.at(i);
        codeOfEncryptedSymbol = codesOfSymbols.at((generationConstant1 * i + generationConstant2) % codesOfSymbols.size());
        encryptedSymbol = codeOfEncryptedSymbol;

        qSubstitutionTable = qSubstitutionTable + QString::fromLocal8Bit(symbol.c_str())
                   + "=" + QString::fromLocal8Bit(encryptedSymbol.c_str()) + "\n";
    }

    ui->substitutionTable->document()->setPlainText(qSubstitutionTable);
}

void MainWindow::on_saveSubstitutionTable_clicked()
{
    fileName = QFileDialog::getSaveFileName( this, tr("Сохранить документ"),
                                  QDir::currentPath(), tr("Текстовые документы (*.txt)"),
                                  0, QFileDialog::DontUseNativeDialog );

    file.setFileName(fileName + ".txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream writeStream(&file);
        writeStream << qSubstitutionTable;
        file.close();
    }
}

void MainWindow::on_loadSubstitutionTable_clicked()
{
    fileName = QFileDialog::getOpenFileName( this, tr("Открыть документ"),
                                      QDir::currentPath(), tr("Текстовые документы (*.txt)"),
                                      0, QFileDialog::DontUseNativeDialog );

    file.setFileName(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream readStream(&file);
        qSubstitutionTable = readStream.readAll();
        ui->text->document()->setPlainText(qSubstitutionTable);
        file.close();
    }
}

void MainWindow::on_loadText_clicked()
{
    fileName = QFileDialog::getOpenFileName( this, tr("Открыть документ"),
                                  QDir::currentPath(), tr("Текстовые документы (*.txt)"),
                                  0, QFileDialog::DontUseNativeDialog );

    file.setFileName(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream readStream(&file);
        qText = readStream.readAll();
        ui->text->document()->setPlainText(qText);
        file.close();
    }
}

void MainWindow::on_codeText_clicked()
{
    qText = ui->text->document()->toPlainText();

    if(qSubstitutionTable.isEmpty())
    {
        errorMessageBox.setText("Не создана таблица!");
        errorMessageBox.exec();
    }
    else if(qText.isEmpty())
    {
        errorMessageBox.setText("Отсутствует текст!");
        errorMessageBox.exec();
    }
    else
    {
        ui->text->document()->clear();
        qEncryptedText.clear();

        unsigned int i = 0;
        text = qText.toLocal8Bit().constData();
        qText.clear();
        substitutionTable=qSubstitutionTable.toLocal8Bit().constData();
        while (i < text.size())
        {
            symbol = text[i];
            unsigned int j = 0;
            while (j < substitutionTable.size())
            {
                if (symbol[0] == substitutionTable[j])
                    break;
                j = j + 4;
            }
            encryptedSymbol = substitutionTable[j+2];
            qEncryptedText = qEncryptedText + QString::fromLocal8Bit(encryptedSymbol.c_str());
            i++;
        }

        ui->text->document()->setPlainText(qEncryptedText);
    }
}

void MainWindow::on_decodeText_clicked()
{
    qEncryptedText = ui->text->document()->toPlainText();

    if(qSubstitutionTable.isEmpty())
    {
        errorMessageBox.setText("Не создана таблица!");
        errorMessageBox.exec();
    }
    else if(qEncryptedText.isEmpty())
    {
        errorMessageBox.setText("Отсутствует текст!");
        errorMessageBox.exec();
    }
    else
    {
        ui->text->document()->clear();
        qText.clear();

        encryptedText = qEncryptedText.toLocal8Bit().constData();
        qEncryptedText.clear();
        substitutionTable = qSubstitutionTable.toLocal8Bit().constData();
        unsigned int i = 0;
        while (i < encryptedText.size())
        {
            encryptedSymbol = encryptedText[i];
            unsigned int j = 2;
            while (j < substitutionTable.size())
            {
                if (encryptedSymbol[0] == substitutionTable[j])
                    break;
                j = j + 4;

            }
            symbol = substitutionTable[j-2];
            qText = qText + QString::fromLocal8Bit(symbol.c_str());
            i++;
        }
        ui->text->document()->setPlainText(qText);
    }
}

void MainWindow::on_saveText_clicked()
{
    fileName = QFileDialog::getSaveFileName( this, tr("Сохранить документ"),
                                  QDir::currentPath(), tr("Текстовые документы (*.txt)"),
                                  0, QFileDialog::DontUseNativeDialog );

    file.setFileName(fileName+".txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream writeStream(&file);
        writeStream << qEncryptedText;
        file.close();
    }
}
