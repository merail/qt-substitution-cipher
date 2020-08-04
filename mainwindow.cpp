#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSizePolicy>
#include <QTextCodec>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon1.jpg"));
    setFixedSize(this->size());

    model = new QStandardItemModel(67, 2);
    ui->substitutionTable->setModel(model);

    for(int i = -64;i < 0;i++)
    {
        codesOfSymbols.append(i);
        if(i == -59)
            codesOfSymbols.append(-88);
        if(i == -27)
            codesOfSymbols.append(-72);
    }
    codesOfSymbols.append(32);

    ui->constant1LineEdit->setValidator(new QIntValidator(this));
    ui->constant2LineEdit->setValidator(new QIntValidator(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_generateSubstitutuionTable_clicked()
{
    qSubstitutionTable.clear();

    qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);
    if(!ui->constant1LineEdit->text().isEmpty())
        generationConstant1 = ui->constant1LineEdit->text().toInt();
    else
        generationConstant1 = qrand();
    if(!ui->constant2LineEdit->text().isEmpty())
        generationConstant2 = ui->constant2LineEdit->text().toInt();
    else
        generationConstant2 = qrand();

    for (int i = 0;i < codesOfSymbols.size();i++)
    {
        symbol = codesOfSymbols.at(i);
        qSymbol = QString::fromLocal8Bit(symbol.c_str());
        codeOfEncryptedSymbol = codesOfSymbols.at((generationConstant1 * i + generationConstant2) % codesOfSymbols.size());
        encryptedSymbol = codeOfEncryptedSymbol;
        qEncryptedSymbol = QString::fromLocal8Bit(encryptedSymbol.c_str());

        index = model->index(i, 0, QModelIndex());
        model->setData(index, qSymbol);
        index = model->index(i, 1, QModelIndex());
        model->setData(index, qEncryptedSymbol);
    }

    substitutuinTableIsCreated = true;
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

        for (int i=0; i<67; ++i)
        {
            for (int j=0; j<2; j++)
            {
                 writeStream << ui->substitutionTable->model()->index(i, j).data().toString();
            }
            writeStream << "\n";
        }

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

        model->clear();
        model->setRowCount(67);
        model->setColumnCount(2);
        unsigned int i = 0;
        while(!file.atEnd())
        {
            string str = file.readLine().toStdString();
            qSymbol = QString::fromLocal8Bit(str.c_str()).at(0);
            qEncryptedSymbol = QString::fromLocal8Bit(str.c_str()).at(1);
            index = model->index(i, 0, QModelIndex());
            model->setData(index, qSymbol);
            index = model->index(i, 1, QModelIndex());
            model->setData(index, qEncryptedSymbol);

            i++;
        }

        file.close();
    }

    substitutuinTableIsCreated = true;
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

    if(!substitutuinTableIsCreated)
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
        substitutionTable = qSubstitutionTable.toLocal8Bit().constData();
        while (i < text.size())
        {
            symbol = text[i];
            for(int j = 0;j < model->rowCount();j++)
            {
                if(symbol == ui->substitutionTable->model()->index(j, 0).data().toString().toLocal8Bit().constData())
                {
                    encryptedSymbol = ui->substitutionTable->model()->index(j, 1).data().toString().toLocal8Bit().constData();
                    break;
                }
            }

            qEncryptedText = qEncryptedText + QString::fromLocal8Bit(encryptedSymbol.c_str());
            i++;
        }

        ui->text->document()->setPlainText(qEncryptedText);
    }
}

void MainWindow::on_decodeText_clicked()
{
    qEncryptedText = ui->text->document()->toPlainText();

    if(!substitutuinTableIsCreated)
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
            for(int j = 0;j < model->rowCount();j++)
            {
                if(encryptedSymbol == ui->substitutionTable->model()->index(j, 1).data().toString().toLocal8Bit().constData())
                {
                    symbol = ui->substitutionTable->model()->index(j, 0).data().toString().toLocal8Bit().constData();
                    break;
                }
            }

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
        writeStream << ui->text->toPlainText();
        file.close();
    }
}
