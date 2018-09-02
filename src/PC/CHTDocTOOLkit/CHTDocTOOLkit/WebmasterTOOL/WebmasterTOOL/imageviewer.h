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

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QDialog>
#include <QVector>
#include <QGraphicsScene>


namespace Ui {
class imageViewer;
}

class imageViewer : public QDialog
{
    Q_OBJECT

public:
    explicit imageViewer(QWidget *parent = 0);
    ~imageViewer();
    void setUp(QVector <QGraphicsScene*>* scenes);
    int current_pos;
    QString title;



private slots:
    void nextImage();
    void prevImage();


private:
    Ui::imageViewer *ui;
    QVector <QGraphicsScene*>* current_scenes;
    QVector <QGraphicsScene*>* old_scenes;
    void updateTit();


};




#endif // IMAGEVIEWER_H
