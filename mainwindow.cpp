#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSizePolicy>
#include <QTextCodec>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->size());

    ui->constant1LineEdit->setValidator(new QIntValidator(this));
    ui->constant2LineEdit->setValidator(new QIntValidator(this));

    setEnglishAlphabetTable();
}

MainWindow::~MainWindow()
{
    delete ui;
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

        codesOfSymbols.clear();
        model->clear();
        model->setRowCount(2);
        unsigned int i = 0;
        while(!file.atEnd())
        {
            string str = file.readLine().toStdString();
            codesOfSymbols.append(int(str.at(0)));
            qCharacter = QString::fromLocal8Bit(str.c_str()).at(0);
            qEncryptedCharacter = QString::fromLocal8Bit(str.c_str()).at(1);

            QStandardItem *pair = new QStandardItem();
            model->setItem(0, i, pair);
            index = model->index(0, i, QModelIndex());
            model->setData(index, qCharacter);
            index = model->index(1, i, QModelIndex());
            model->setData(index, qEncryptedCharacter);

            i++;
        }

        file.close();
    }

    substitutuinTableIsCreated = true;
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
        codeOfEncryptedCharacter = codesOfSymbols.at((generationConstant1 * i + generationConstant2) % codesOfSymbols.size());
        encryptedCharacter = codeOfEncryptedCharacter;
        qEncryptedCharacter = QString::fromLocal8Bit(encryptedCharacter.c_str());

        index = model->index(1, i, QModelIndex());
        model->setData(index, qEncryptedCharacter);
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

        for (int i = 0;i < 2;i++)
        {
            for (int j = 0;j < 2;j++)
            {
                 writeStream << ui->substitutionTable->model()->index(i, j).data().toString();
            }
            writeStream << "\n";
        }

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
            character = text[i];
            for(int j = 0;j < model->columnCount();j++)
            {
                if(character == ui->substitutionTable->model()->index(0, j).data().toString().toLocal8Bit().constData())
                {
                    encryptedCharacter = ui->substitutionTable->model()->index(1, j).data().toString().toLocal8Bit().constData();
                    break;
                }
            }

            qEncryptedText = qEncryptedText + QString::fromLocal8Bit(encryptedCharacter.c_str());
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
            encryptedCharacter = encryptedText[i];
            for(int j = 0;j < model->columnCount();j++)
            {
                if(encryptedCharacter == ui->substitutionTable->model()->index(1, j).data().toString().toLocal8Bit().constData())
                {
                    character = ui->substitutionTable->model()->index(0, j).data().toString().toLocal8Bit().constData();
                    break;
                }
            }

            qText = qText + QString::fromLocal8Bit(character.c_str());
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

void MainWindow::on_englishAlphabetRadioButton_clicked()
{
    setEnglishAlphabetTable();
}

void MainWindow::on_russianAlphabetRadioButton_clicked()
{
    codesOfSymbols.clear();

    charactersCount = 67;

    model = new QStandardItemModel(2, charactersCount);
    ui->substitutionTable->setModel(model);

    for(int i = -64;i < 0;i++)
    {
        codesOfSymbols.append(i);
        if(i == -59)
            codesOfSymbols.append(-88);
        if(i == -27)
            codesOfSymbols.append(-72);
    }

    for (int i = 0;i < codesOfSymbols.size();i++)
    {
        character = codesOfSymbols.at(i);
        qCharacter = QString::fromLocal8Bit(character.c_str());

        index = model->index(0, i, QModelIndex());
        model->setData(index, qCharacter);
    }
}

void MainWindow::setEnglishAlphabetTable()
{
    codesOfSymbols.clear();

    charactersCount = 53;

    model = new QStandardItemModel(2, charactersCount);
    ui->substitutionTable->setModel(model);

    for(int i = 65;i < 123;i++)
    {
        codesOfSymbols.append(i);
        if(i == 90)
            i = i + 6;
    }
    codesOfSymbols.append(32);

    for (int i = 0;i < codesOfSymbols.size();i++)
    {
        character = codesOfSymbols.at(i);
        qCharacter = QString::fromLocal8Bit(character.c_str());

        index = model->index(0, i, QModelIndex());
        model->setData(index, qCharacter);
    }
}
