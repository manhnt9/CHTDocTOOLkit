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

#include "imageviewer.h"
#include "ui_imageviewer.h"

imageViewer::imageViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::imageViewer)
{
    ui->setupUi(this);

    setWindowTitle("Xem văn bản");

    QAction* next = new QAction(0, this);
    next->setShortcut(QKeySequence("Right"));
    ui->graphicsView->addAction(next);
    connect(next, SIGNAL(triggered()), this, SLOT(need()));

    QAction* prev = new QAction(0, this);
    prev->setShortcut(QKeySequence("Left"));
    ui->graphicsView->addAction(prev);
    connect(prev, SIGNAL(triggered()), this, SLOT(prevImage()));

    current_scenes = 0;
    maxScenes = 0;

}

imageViewer::~imageViewer()
{
    delete ui;
}

void imageViewer::need()
{   
    if(current_scenes->size() == maxScenes) {
        nextImage();
        return;
    } else if(prevDone == true)
        emit needNext();
}

void imageViewer::setUp(QVector<QGraphicsScene*> *scenes)
{
    if(current_scenes != 0) {
        for(int i = 0; i != current_scenes->size(); i++) {
            delete current_scenes->at(i);
        }
        current_scenes->clear();
    }

    current_scenes = scenes;
    current_pos = 0;
    ui->graphicsView->setScene(current_scenes->at(current_pos));
    updateTit();
}

void imageViewer::nextImage()
{
    if(current_pos+1 == current_scenes->size()) {
        current_pos = 0;
        ui->graphicsView->setScene(current_scenes->at(current_pos));
    }
    else
        ui->graphicsView->setScene(current_scenes->at(++current_pos));
    updateTit();
}

void imageViewer::prevImage()
{
    if(current_pos == 0) {
        current_pos = current_scenes->size() -1;
        ui->graphicsView->setScene(current_scenes->at(current_pos));
    }
    else
        ui->graphicsView->setScene(current_scenes->at(--current_pos));
    updateTit();
}

void imageViewer::updateTit()
{
    QString tit = title;
    tit.append(" Trang ");
    tit.append(QString::number(current_pos+1));
    tit.append("/");
    tit.append(QString::number(maxScenes));
    setWindowTitle(tit);

}

