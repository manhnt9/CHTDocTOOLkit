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

#include "docs.h"
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

baseDoc::baseDoc()
{
}

baseDoc::~baseDoc()
{
    paths.clear();
}

Docs::Docs() : storage("data/current/lastest.text"), errorText("")
{
}

Docs::~Docs()
{
}

bool Docs::import()
{
    container.clear();
    QFile file(storage);

    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::information(0, "Lỗi", "Lỗi file lưu trữ");
        return false;
    }

    QTextStream textStream(&file);
    textStream.setCodec("UTF-16");
    baseDoc doc;
    QString temp;

    if (textStream.readLine() != "cht") {
        QMessageBox::information(0, "Lỗi", "Lỗi file lưu trữ");
        return false;
    }

    while(!textStream.atEnd())
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
        container.append(doc);
    }
    file.close();
    return true;
}

bool Docs::update(QVector<baseDoc>* vec)
{
    QFile file(storage);

    if (!file.open(QFile::WriteOnly) )
    {
        QMessageBox::information(0, "Lỗi", "Lỗi file lưu trữ");
        return false;
    }

    QTextStream textStream(&file);

    textStream.setCodec("UTF-16");
    textStream << "cht" << "\r\n";


    for(int i = 0; i != vec->size(); i++) {

        textStream << vec->at(i).name << "\r\n";
        textStream << vec->at(i).date << "\r\n";
        textStream << vec->at(i).original << "\r\n";
        textStream << vec->at(i).group << "\r\n";
        textStream << vec->at(i).link << "\r\n";


        const QVector <QString>* v = &vec->at(i).paths;
        for(int j = 0; j != v->size(); j++) {
            textStream << v->at(j) << "\r\n";
        }

        textStream << "#" << "\r\n";;
    }
    file.close();
    return true;
}

QString Docs::getError()
{
    return errorText;
}




