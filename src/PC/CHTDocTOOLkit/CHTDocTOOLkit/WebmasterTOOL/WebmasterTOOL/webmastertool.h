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

#ifndef WEBMASTERTOOL_H
#define WEBMASTERTOOL_H

#include <QMainWindow>
#include <docs.h>
#include <QListWidget>
#include "imageviewer.h"
#include <QVector>


namespace Ui {
class webmastertool;
}

class webmastertool : public QMainWindow
{
    Q_OBJECT

public:
    explicit webmastertool(QWidget *parent = 0);
    ~webmastertool();

private:
    Ui::webmastertool *ui;
    void initUI();
    Docs* mLastestDocs;
    QVector <baseDoc>* vec;
    void updateUI();
    int findDoc(QString name);
    QVector <QGraphicsScene*>* initScenes(int pos);
    void viewDoc(int pos);
    imageViewer* viewer;
    void convert();
    void updateList();

private slots:
    void showDoc(QListWidgetItem* itm);
    void on_pushButton_released();
    void on_pushButton_2_released();
    void on_pushButton_3_released();
    void on_pushButton_4_released();
    void on_pushButton_5_released();
    void on_pushButton_6_released();
    void on_pushButton_7_released();
};

#endif // WEBMASTERTOOL_H
