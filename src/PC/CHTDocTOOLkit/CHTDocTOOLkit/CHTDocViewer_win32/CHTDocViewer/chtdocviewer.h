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

#ifndef CHTDOCVIEWER_H
#define CHTDOCVIEWER_H

#include <QDialog>
#include <docs.h>
#include <filedownloader.h>
#include <QListWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <imageviewer.h>

namespace Ui {
class chtdocviewer;
}

class chtdocviewer : public QDialog
{
    Q_OBJECT

public:
    explicit chtdocviewer(QWidget *parent = 0);
    ~chtdocviewer();

signals:

private slots:
    void setUp();
    void prepareNext();
    void getList();

private:
    QString url;
    Ui::chtdocviewer *ui;
    filedownloader downloader;
    imageViewer viewer;
    QVector <baseDoc>* vec;
    void updateUI();
    int findDoc(QString name);
    QVector <QGraphicsScene*>* scenes;
    int pass;
    int currentDoc;
    QVector <QString> weblist;
    void updateBox();


private slots:
    void browse(QString web);
    void showDoc(QListWidgetItem* itm);
    void viewDoc();
    void on_pushButton_2_released();
    void on_pushButton_3_released();
};

#endif // CHTDOCVIEWER_H
