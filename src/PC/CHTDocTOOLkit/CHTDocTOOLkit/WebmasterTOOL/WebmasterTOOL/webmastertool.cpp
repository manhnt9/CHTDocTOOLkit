/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014, ZerOX - Zer0.X@outlook.com
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

#include "webmastertool.h"
#include "ui_webmastertool.h"
#include <QFile>
#include <QTextStream>
#include <cstdlib>
#include <QVector>
#include <QMessageBox>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QDate>
#include <QIcon>
#include <uploaddialog.h>

webmastertool::webmastertool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::webmastertool)
{
    ui->setupUi(this);

    setStyleSheet("background-color: white");

    setWindowTitle("WebmasterTOOL");
    initUI();

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
            SLOT(showDoc(QListWidgetItem*)));

    mLastestDocs = new Docs;
    vec = &mLastestDocs->container;

    updateUI();
}

webmastertool::~webmastertool()
{
    delete ui;
}

void webmastertool::initUI()
{
    QIcon icon(":/icons/Up.png");

    ui->pushButton_4->setIcon(icon);
    ui->pushButton_4->setIconSize(QSize(90, 90));

    icon = QIcon(":/icons/Down.png");
    ui->pushButton_5->setIcon(icon);
    ui->pushButton_5->setIconSize(QSize(90, 90));

    icon = QIcon(":/icons/Open.png");
    ui->pushButton->setIcon(icon);
    ui->pushButton->setIconSize(QSize(31, 31));

    icon = QIcon(":/icons/Create.png");
    ui->pushButton_2->setIcon(icon);
    ui->pushButton_2->setIconSize(QSize(150, 150));

    icon = QIcon(":/icons/Remove.png");
    ui->pushButton_3->setIcon(icon);
    ui->pushButton_3->setIconSize(QSize(35, 35));

    icon = QIcon(":/icons/Reset.png");
    ui->pushButton_6->setIcon(icon);
    ui->pushButton_6->setIconSize(QSize(50, 50));

    icon = QIcon(":/icons/Upload.png");
    ui->pushButton_7->setIcon(icon);
    ui->pushButton_7->setIconSize(QSize(150, 150));

    QString border("border: 1px solid steelblue;");

    ui->lineEdit->setStyleSheet(border);
    ui->lineEdit_2->setStyleSheet(border);
    ui->lineEdit_3->setStyleSheet(border);
    ui->lineEdit_4->setStyleSheet(border);
    ui->lineEdit_5->setStyleSheet(border);
    ui->lineEdit_6->setStyleSheet(border);
    ui->lineEdit_7->setStyleSheet(border);

    ui->listWidget->setStyleSheet("border: 2px solid steelblue;");
}

void webmastertool::updateUI()
{
    ui->lineEdit_5->setText(QDate::currentDate().toString("dd/MM/yyyy"));

    ui->listWidget->clear();
    mLastestDocs->import();

    for (int i = 0; i != vec->size(); i++)
    {
        const baseDoc& doc = vec->at(i);
        ui->listWidget->addItem(doc.name);
    }


}

void webmastertool::showDoc(QListWidgetItem* itm)
{

    QString name = itm->text();
    int pos = findDoc(name);
    QString text;

    text.append(vec->at(pos).name);
    text.append("\n");
    text.append("Ngày tạo: ");
    text.append(vec->at(pos).date);
    text.append("\n");
    text.append("Thuộc bộ: ");
    text.append(vec->at(pos).group);

    QMessageBox view (QMessageBox::Information,
                      "Xem thông báo", text,
                      QMessageBox::Yes | QMessageBox::No);

    view.setButtonText(QMessageBox::Yes, "Xem");
    view.setButtonText(QMessageBox::No, "OK");
    if (view.exec() == QMessageBox::Yes)
    {
        viewDoc(pos);
    }
}

inline int webmastertool::findDoc(QString name)
{
    for (int i = 0; i != vec->size(); i++)
    {
        const baseDoc& doc = vec->at(i);
        if (doc.name == name)
            return i;
    }
    return -1;
}




void webmastertool::on_pushButton_released()
{
    QString path;
    path = QFileDialog::getOpenFileName(this, "Chọn văn bản", "", "Văn bản PDF (*.PDF)");
    ui->lineEdit_2->setText(path);
}



void webmastertool::viewDoc(int pos)
{
    viewer = new imageViewer;
    viewer->setModal(false);
    QVector <QGraphicsScene*>* scenes = initScenes(pos);
    viewer->title = vec->at(pos).name;
    viewer->setUp(scenes);
    viewer->show();
}

QVector <QGraphicsScene*>* webmastertool::initScenes(int pos)
{
    QVector <QGraphicsScene*>* scenes = new QVector <QGraphicsScene*>;
    const QVector <QString>* v = &vec->at(pos).paths;
    for(int i = 0; i != v->size(); i++) {
        QGraphicsScene* scene = new QGraphicsScene;
        QString name = v->at(i);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(name));
        scene->addItem(item);
        scenes->append(scene);
    }
    return scenes;
}

void webmastertool::on_pushButton_2_released() // creation
{
    convert();
    const char* cmd = "cmd.bat";
    system(cmd);
    updateUI();
}

void webmastertool::convert()
{
    QFile file("cmd.bat");
    file.open(QFile::WriteOnly);

    QTextStream textStream(&file);

    textStream << "\n";
    textStream << "del data\\upload\\chtdoctoolkit\\data\\current\\lastest.text";
    textStream << "\n";

    textStream << "\n";
    textStream << "del data\\upload\\chtdoctoolkit\\data\\current\\storage\\*.png";
    textStream << "\n";

    textStream << "\n";
    textStream << "del data\\upload\\chtdoctoolkit\\data\\current\\original\\*.pdf";
    textStream << "\n";

    QString cmd1("\"");
    cmd1.append(ui->lineEdit_2->text());
    cmd1.append("\"");

    cmd1.replace("/", "\\");

    QString cmd2("\"");
    cmd2.append(QDir::currentPath());
    cmd2.append("\"");

    cmd2.replace("/", "\\");

    textStream << "copy " << cmd1 << " " << cmd2;

    cmd1 = ("\"");
    cmd1.append(ui->lineEdit_2->text());
    cmd1.append("\"");

    cmd2 = ("\"");
    cmd2.append(QDir::currentPath());
    cmd2.append("\\data\\upload\\chtdoctoolkit\\data\\current\\original\\");
    cmd2.append("\"");

    textStream << "\n";
    textStream << "copy " << cmd1 << " " << cmd2;
    int i,n;
    i = n = 0;

    i = ui->lineEdit_3->text().toInt();
    n = ui->lineEdit_4->text().toInt();

    textStream << "\n";

    QFileInfo fileInf(ui->lineEdit_2->text());
    baseDoc tempDoc;

    tempDoc.name = ui->lineEdit->text();
    tempDoc.date = ui->lineEdit_5->text();
    tempDoc.original = "data/current/original/";
    tempDoc.original.append(fileInf.fileName());
    if(!ui->lineEdit_6->text().isEmpty()) {
        tempDoc.group = ui->lineEdit_6->text();
    }
    else tempDoc.group = "None";

    if(!ui->lineEdit_7->text().isEmpty()) {
        tempDoc.link = ui->lineEdit_6->text();
    }
    else tempDoc.link = "None";


    for(--i; i < n ; ++i) {
        cmd1 = " convert -density 300 -depth 3 -resize 750 *.pdf[";
        cmd1.append(QString::number(i));
        cmd1.append("] temp\\converted\\");
        QString fileName = fileInf.baseName();
        fileName.append(QString::number(i));
        fileName.append(".png");
        cmd1.append(fileName);
        QString path = "data/current/storage/";
        path.append(fileName);
        tempDoc.paths.append(path);
        textStream << cmd1;
        textStream << "\n";
    }

    textStream << "copy \"temp\\converted\\*.png\" \"data\\upload\\chtdoctoolkit\\data\\current\\storage\" ";

    textStream << "\n";
    textStream << "del " << "temp\\converted\\*.png";
    textStream << "\n";

    textStream << "del " << "*.pdf";
    textStream << "\n";

    cmd1 = ("\"");
    cmd1.append(QDir::currentPath());
    cmd1.append("/data/current/lastest.text");
    cmd1.append("\"");

    cmd2 = ("\"");
    cmd2.append(QDir::currentPath());
    cmd2.append("\\data\\upload\\chtdoctoolkit\\data\\current");
    cmd2.append("\"");

    textStream << "copy \"data\\current\\lastest.text\" \"data\\upload\\chtdoctoolkit\\data\\current\"";
    textStream << "\n";

    textStream << "C:\\Windows\\system32\\xcopy \"data\\upload\\chtdoctoolkit\\data\\current\" \"data\\current\" /e /y  ";

    textStream << "\n" << "del " << "cmd.bat";

    vec->append(tempDoc);
    mLastestDocs->update(vec);

}

void webmastertool::on_pushButton_3_released()
{
    if(vec->isEmpty())
        return;

    vec->remove(findDoc(ui->listWidget->currentItem()->text()));
    mLastestDocs->update(vec);
    updateUI();
    updateList();
}


void webmastertool::on_pushButton_4_released()
{
    if(ui->listWidget->currentItem() == 0)
        return;

    QString text = ui->listWidget->currentItem()->text();

    int pos = findDoc(text);

    if(pos == 0) {
        const baseDoc temp1 = vec->at(0);
        vec->removeFirst();
        vec->push_back(temp1);
        mLastestDocs->update(vec);
        updateUI();
        return;
    }

    const baseDoc temp1 = vec->at(pos);
    const baseDoc temp2 = vec->at(pos-1);
    vec->replace(pos-1, temp1);
    vec->replace(pos, temp2);
    mLastestDocs->update(vec);
    updateUI();
}

void webmastertool::on_pushButton_5_released()
{
    if(ui->listWidget->currentItem() == 0)
        return;

    QString text = ui->listWidget->currentItem()->text();

    int pos = findDoc(text);

    if(pos == (vec->size() -1) ) {
        const baseDoc temp1 = vec->at(pos);
        vec->removeLast();
        vec->push_front(temp1);
        mLastestDocs->update(vec);
        updateUI();
        return;
    }

    const baseDoc temp1 = vec->at(pos);
    const baseDoc temp2 = vec->at(pos+1);
    vec->replace(pos+1, temp1);
    vec->replace(pos, temp2);
    mLastestDocs->update(vec);
    updateUI();
}


void webmastertool::on_pushButton_6_released()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
}

void webmastertool::on_pushButton_7_released()
{
    updateList();
    UploadDialog* dialog = new UploadDialog;
    dialog->show();
}

void webmastertool::updateList()
{
    QFile file("cmd.bat");
    file.open(QFile::WriteOnly);

    QTextStream textStream(&file);

    textStream << "copy \"data\\current\\lastest.text\" \"data\\upload\\chtdoctoolkit\\data\\current\"";
    textStream << "\n";
    textStream << "del cmd.bat";
    file.close();

    const char* cmd = "cmd.bat";
    system(cmd);
}
