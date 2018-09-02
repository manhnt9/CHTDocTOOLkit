/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014, Manh Nguyen Tien - manhnt.9@outlook.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#include "uploaddialog.h"
#include "ui_uploaddialog.h"
#include <QFile>
#include <QTextStream>
#include <cstdlib>
#include <QString>
#include <QDir>

UploadDialog::UploadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadDialog)
{
    ui->setupUi(this);
    setStyleSheet("background-color: white");

    setWindowTitle("FTP Upload");

    ui->lineEdit->setText("thptchuyenhatinh.edu.vn");
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setText("public_html");
}

UploadDialog::~UploadDialog()
{
    delete ui;
}

void UploadDialog::on_pushButton_2_released()
{
    close();
}

void UploadDialog::on_pushButton_released() //upload
{
    genFTP();
    genCMD();

    const char* cmd = "cmd.bat";
    system(cmd);
}


void UploadDialog::genFTP()
{
    QFile file("ftp");
    file.open(QFile::WriteOnly);

    QTextStream textStream(&file);

    QString host = ui->lineEdit->text();
    QString user = ui->lineEdit_2->text();
    QString password = ui->lineEdit_3->text();
    QString rootdir = ui->lineEdit_4->text();

    textStream << "open " << host << "\r\n";
    textStream << user << "\r\n";
    textStream << password << "\r\n";

    textStream << "cd " << rootdir << "\r\n";
    textStream << "binary" << "\r\n";

    textStream << "mkdir chtdoctoolkit" << "\r\n";
    textStream << "cd chtdoctoolkit" << "\r\n";

    textStream << "mkdir data" << "\r\n";
    textStream << "cd data" << "\r\n";

    textStream << "mkdir current" << "\r\n";
    textStream << "cd current" << "\r\n";

    textStream << "mkdir storage" << "\r\n";
    textStream << "mkdir original" << "\r\n";

    const QString currentDir = QDir::currentPath();
    QString dir = currentDir;
    dir.append("/data/upload/chtdoctoolkit/data/current/lastest.text");
    textStream << "put " << dir << "\r\n";

    textStream << "cd storage" << "\r\n";
    dir = currentDir;
    dir.append("/data/upload/chtdoctoolkit/data/current/storage/*");
    textStream << "mput " << dir << "\r\n";

    textStream << "cd .." << "\r\n";
    textStream << "cd original" << "\r\n";
    dir = currentDir;
    dir.append("/data/upload/chtdoctoolkit/data/current/original/*");
    textStream << "mput " << dir << "\r\n";
    textStream << "quit";
}

void UploadDialog::genCMD()
{
    QFile file("cmd.bat");
    file.open(QFile::WriteOnly);

    QTextStream textStream(&file);

    QString dir("\"");
    dir.append(QDir::currentPath());
    dir.append("\"");

    textStream << "copy \"C:\\Windows\\System32\\ftp.exe\" " << dir << "\n";
    textStream << "ftp -i -s:ftp" << "\n";
    textStream << "del ftp" << "\n";
    textStream << "del ftp.exe" << "\n";
    textStream << "del cmd.bat" << "\n";
}


