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

#ifndef CHTDOCVIEWER_H
#define CHTDOCVIEWER_H

#include <QWidget>
#include <docs.h>
#include <filedownloader.h>
#include <QListWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVector>


class chtdocviewer : public QWidget
{
    Q_OBJECT

public:
    chtdocviewer(QWidget *parent = 0);
    ~chtdocviewer();

private:
    QVBoxLayout* mainLayout;
    //QVBoxLayout* viewLayout;
    void setViewLayout();

private slots:
    void getDocs();
    void prepareNext();
    void setMainLayout();

private:
    filedownloader downloader;
    QVector <baseDoc>* vec;
    int findDoc(QString name);
    QString url;
    QVector <QGraphicsScene*>* scenes;
    int pass;
    int currentDoc;

private slots:
    void showDoc(QListWidgetItem* itm);
    void viewDoc();

private:
    QListWidget* listWidget;
    QLabel* label;
    QLabel* label2;
    QPushButton* nextButton;
    QPushButton* prevButton;
    QPushButton* backButton;

    QGraphicsView* graphicsView;
    QVector <QGraphicsScene*>* current_scenes;

private:
    void setUp(QVector <QGraphicsScene*>* scenes);
    int current_pos;
    void nextImage();
    int maxScenes;
    bool prevDone;
    QString title;


signals:
    void needNext();
private slots:
    void prevImage();
    void need();

};

#endif // CHTDOCVIEWER_H
