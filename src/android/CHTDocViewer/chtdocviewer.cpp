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

#include "chtdocviewer.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

chtdocviewer::chtdocviewer(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    current_scenes = NULL;
    maxScenes = 0;

    pass =0;

    vec = new QVector <baseDoc>;

    connect(&downloader, SIGNAL(downloaded()), SLOT(getDocs()));
    connect(&downloader, SIGNAL(imgDownloaded()), SLOT(viewDoc()));
    connect(this, SIGNAL(needNext()), SLOT(prepareNext()));

    label = new QLabel("DANH SÁCH CÁC TÀI LIỆU");
    nextButton = new QPushButton("Trang sau");
    prevButton = new QPushButton("Trang trước");
    backButton = new QPushButton("Trở về");

    connect(nextButton, SIGNAL(clicked()), SLOT(need()));
    connect(prevButton, SIGNAL(clicked()), SLOT(prevImage()));
    connect(backButton, SIGNAL(clicked()), SLOT(setMainLayout()));

    graphicsView = new QGraphicsView;
    graphicsView->scale(1.5, 1.5);

    url = "http://thptchuyenhatinh.edu.vn";

    QString temp = url;
    temp.append("/chtdoctoolkit/data/current/lastest.text");

    downloader.getFile(temp);

    listWidget = new QListWidget;
    listWidget->addItem("Đang tải...");

    setMainLayout();
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(showDoc(QListWidgetItem*)));
}

chtdocviewer::~chtdocviewer()
{

}

void chtdocviewer::getDocs()
{
    QFile file("lastest.text");
    file.open(QFile::ReadOnly);

    QTextStream textStream(&file);
    textStream.setCodec("UTF-16");

    baseDoc doc;
    QString temp;

    if (textStream.readLine() != "cht") {
        listWidget->addItem("Không có dữ liệu");
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

    listWidget->clear();

    for(int i = 0; i != vec->size(); i++)
    {
        const baseDoc& doc = vec->at(i);
        listWidget->addItem(doc.name);
    }


}

void chtdocviewer::setMainLayout()
{
    mainLayout->addWidget(label);

    mainLayout->addWidget(listWidget);
}

void chtdocviewer::setViewLayout()
{

    mainLayout->removeWidget(label);
    mainLayout->removeWidget(listWidget);

    mainLayout->addWidget(prevButton);
    mainLayout->addWidget(backButton);
    mainLayout->addWidget(nextButton);

    mainLayout->addWidget(graphicsView);
}

int chtdocviewer::findDoc(QString name)
{
    for(int i = 0; i != vec->size(); ++i)
    {
        const baseDoc& doc = vec->at(i);
        if(doc.name == name)
            return i;
    }
    return -1;
}

void chtdocviewer::showDoc(QListWidgetItem *itm)
{
    if(itm->text() == "Không có dữ liệu")
        return;
    pass = 0;
    QString name = itm->text();
    int pos = findDoc(name);

    currentDoc = pos;
    QString temp = url;
    temp.append("/chtdoctoolkit/");
    QString path = vec->at(currentDoc).paths.at(0);
    temp.append(path);
    downloader.getImg(temp);
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
        prevDone = true;
        nextImage();
        return;
    }

    scenes = new QVector <QGraphicsScene*>;
    QGraphicsScene* scene = new QGraphicsScene;
    QPixmap pix;
    pix.loadFromData(downloader.m_DownloadedData);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pix);
    scene->addItem(item);
    scenes->append(scene);
    pass = -1;
    maxScenes = vec->at(currentDoc).paths.size();
    title = vec->at(currentDoc).name;
    prevDone = true;
    setUp(scenes);    

    setViewLayout();
}

void chtdocviewer::setUp(QVector<QGraphicsScene*> *scenes)
{
    if(current_scenes != NULL) {
        for(int i = 0; i != current_scenes->size(); i++) {
            delete current_scenes->at(i);
        }
        current_scenes->clear();
    }

    current_scenes = scenes;
    current_pos = 0;

    graphicsView->setScene(current_scenes->at(current_pos));

}

void chtdocviewer::prepareNext()
{
    prevDone = false;
    int pos = current_pos;

    QString temp = url;
    temp.append("/chtdoctoolkit/");


    if(pos+1 == vec->at(currentDoc).paths.size())
        return;

    QString path = vec->at(currentDoc).paths.at(pos+1);
    temp.append(path);
    downloader.getImg(temp);
}

void chtdocviewer::need()
{
    if(current_scenes->size() == maxScenes) {
        nextImage();
        return;
    } else if(prevDone == true)
        emit needNext();
}

void chtdocviewer::nextImage()
{
    if(current_pos+1 == current_scenes->size()) {
        current_pos = 0;
        graphicsView->setScene(current_scenes->at(current_pos));
    }
    else
        graphicsView->setScene(current_scenes->at(++current_pos));
}

void chtdocviewer::prevImage()
{
    if(current_pos == 0) {
        current_pos = current_scenes->size() -1;
        graphicsView->setScene(current_scenes->at(current_pos));
    }
    else
        graphicsView->setScene(current_scenes->at(--current_pos));
}
















