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

#include "chtdocviewer.h"
#include "ui_chtdocviewer.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <cstdlib>
#include <QDir>

chtdocviewer::chtdocviewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chtdocviewer)
{
    ui->setupUi(this);

    pass =0;

    vec = new QVector <baseDoc>;

    viewer.setModal(false);

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
            SLOT(showDoc(QListWidgetItem*)));

    connect(&downloader, SIGNAL(downloaded()), SLOT(setUp()));
    connect(&downloader, SIGNAL(imgDownloaded()), SLOT(viewDoc()));
    connect(&viewer, SIGNAL(needNext()), SLOT(prepareNext()));

    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), SLOT(browse(QString)));

    getList();

    browse(ui->comboBox->currentText());
}

void chtdocviewer::browse(QString web)
{
    url = "http://";

    url.append(web);

    QString temp = url;

    temp.append("/chtdoctoolkit/data/current/lastest.text");

    downloader.getFile(temp);
}

void::chtdocviewer::getList()
{
    QString name = QDir::tempPath();
    name.append("/url");
    QFile file(name);
    file.open(QFile::ReadOnly);

    QTextStream textStream(&file);

    while (!textStream.atEnd())
    {
        QString web = textStream.readLine();

        weblist.append(web);
    }
    updateBox();
}

void chtdocviewer::updateBox()
{
    ui->comboBox->clear();
    for(int i = 0; i != weblist.size(); i++) {
        ui->comboBox->addItem(weblist.at(i));
    }
    QString name = QDir::tempPath();
    name.append("/url");
    QFile file(name);
    file.open(QFile::WriteOnly);

    QTextStream textStream(&file);

    for(int i = 0; i != weblist.size(); i++){
        textStream << weblist.at(i) << "\r\n";
    }
}

chtdocviewer::~chtdocviewer()
{
    delete ui;
}

void chtdocviewer::setUp()
{
    QString name = QDir::tempPath();
    name.append("/lastest.text");
    QFile file(name);
    file.open(QFile::ReadOnly);

    QTextStream textStream(&file);
    textStream.setCodec("UTF-16");

    baseDoc doc;
    QString temp;

    vec->clear();

    if (textStream.readLine() != "cht") {
        ui->listWidget->clear();
        ui->listWidget->addItem("Không có dữ liệu");
        return;
    }

    while (!textStream.atEnd())
    {
        doc.name = textStream.readLine();
        doc.date = textStream.readLine();
        doc.original = textStream.readLine();
        doc.group = textStream.readLine();
        doc.link = textStream.readLine();

        doc.paths.clear();

        while((temp = textStream.readLine()) != "#")
            {
                doc.paths.append(temp);
            }
        vec->append(doc);
    }

    updateUI();

}

void chtdocviewer::updateUI()
{
    ui->listWidget->clear();
    for(int i = 0; i != vec->size(); i++)
    {
        const baseDoc& doc = vec->at(i);
        ui->listWidget->addItem(doc.name);
    }
}

void chtdocviewer::showDoc(QListWidgetItem *itm)
{
    if( itm->text() == "Không có dữ liệu")
        return;

    pass = 0;
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
                      "Xem tài liệu", text,
                      QMessageBox::Yes | QMessageBox::No);

    view.setButtonText(QMessageBox::Yes, "Xem");
    view.setButtonText(QMessageBox::No, "OK");

    if (view.exec() == QMessageBox::Yes)
    {
        currentDoc = pos;
        QString temp = url;
        temp.append("/chtdoctoolkit/");
        QString path = vec->at(currentDoc).paths.at(0);
        temp.append(path);
        downloader.getImg(temp);
    }
}

int chtdocviewer::findDoc(QString name)
{
    for(int i = 0; i != vec->size(); i++)
    {
        const baseDoc& doc = vec->at(i);
        if(doc.name == name)
            return i;
    }
    return -1;
}

void chtdocviewer::viewDoc()
{
    if(pass == -1) {
        QGraphicsScene* scene = new QGraphicsScene;
        QPixmap pix;
        pix.loadFromData(downloader.m_DownloadedData);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pix);
        scene->addItem(item);
        scenes->append(scene);
        viewer.prevDone = true;
        viewer.nextImage();
        return;
    }

    scenes = new QVector <QGraphicsScene*>;
    QGraphicsScene* scene = new QGraphicsScene;
    QPixmap pix;
    pix.loadFromData(downloader.m_DownloadedData);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pix);
    scene->addItem(item);
    scenes->append(scene);
    viewer.show();
    pass = -1;
    viewer.maxScenes = vec->at(currentDoc).paths.size();
    viewer.title = vec->at(currentDoc).name;
    viewer.prevDone = true;
    viewer.setUp(scenes);
    viewer.show();



}

void chtdocviewer::prepareNext()
{
    viewer.prevDone = false;
    int pos = viewer.current_pos;

    QString temp = url;
    temp.append("/chtdoctoolkit/");


    if(pos+1 == vec->at(currentDoc).paths.size())
        return;

    QString path = vec->at(currentDoc).paths.at(pos+1);
    temp.append(path);
    downloader.getImg(temp);

}


void chtdocviewer::on_pushButton_2_released()
{
    weblist.append(ui->lineEdit->text());
    updateBox();
    ui->comboBox->setCurrentText(ui->lineEdit->text());
}



void chtdocviewer::on_pushButton_3_released()
{
    for(int i = 0; i != weblist.size(); i++) {
        if(weblist.at(i) == ui->comboBox->currentText()){
            weblist.remove(i);
            updateBox();
            return;
        }
    }
}
