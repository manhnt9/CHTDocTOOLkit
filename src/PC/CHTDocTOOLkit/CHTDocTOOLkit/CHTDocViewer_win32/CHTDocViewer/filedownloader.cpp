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

#include "filedownloader.h"
#include <QDebug>
#include <QFile>
#include <cstdlib>
#include <QDir>

filedownloader::filedownloader(QObject *parent) :
    QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                   SLOT(fileDownloaded(QNetworkReply*)));
}

void filedownloader::fileDownloaded(QNetworkReply* pReply)
{
    if (isImg == false)
    {
        m_DownloadedData = pReply->readAll();
        QString name = QDir::tempPath();
        name.append("/lastest.text");
        QFile file(name);
        file.open(QFile::WriteOnly);
        file.write(m_DownloadedData);
        file.close();
        pReply->deleteLater();
        emit downloaded();
    }
    else
    {
        m_DownloadedData = pReply->readAll();
        pReply->deleteLater();
        emit imgDownloaded();
    }
}

void filedownloader::getFile(QString url)
{
    isImg = false;
    QNetworkRequest request(url);
    m_WebCtrl.get(request);
}

void filedownloader::getImg(QString url)
{
    isImg = true;
    QNetworkRequest request(url);
    m_WebCtrl.get(request);
}


