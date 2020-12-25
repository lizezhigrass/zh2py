#include "zh2py.h"
#include <QFile>
#include<QTextStream>
#include<QTextCodec>
#include<QDebug>
#include<QRegularExpression>

Zh2py::Zh2py(QString Traditional2SimpleFile, QString WordsDictFile, QString PhrasesDict, QString PhrasesDictMap)
{
    dictsInited = true;
    initTraditionDict(Traditional2SimpleFile);
    initWordsDict(WordsDictFile);
    initPhrasesDict(PhrasesDict);
    initPhrasesMapDict(PhrasesDictMap);
}

bool Zh2py::dictsWaInited()
{
    return dictsInited;
}

bool Zh2py::initTraditionDict(QString Traditional2SimpleFile)
{
    QFile file;

    QString str;
    QStringList strList;

    file.setFileName(Traditional2SimpleFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        dictsInited = false;
        return false;
    }
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("utf-8"));
    while(!in.atEnd())
    {
        str = in.readLine();
        strList = str.split("\t");

        if(strList.count() == 2)
        {
            m_traditionDict.insert(strList.at(0).at(0),strList.at(1).at(0));
        }
     }
    file.close();
    if(m_traditionDict.count() > 0)
    {
        dictsInited = dictsInited && true;
    } else{
        dictsInited = false;
    }
    return dictsInited;
}

bool Zh2py::initPhrasesMapDict(QString PhrasesDictMap)
{
    QFile file;

    QString str;
    QStringList strList;
    file.setFileName(PhrasesDictMap);
    if(!file.open(QIODevice::ReadOnly))
    {
        dictsInited = false;
        return false;
    }
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("utf-8"));
    while(!in.atEnd())
    {
        str = in.readLine();
        strList = str.split("\t");

        if(strList.count() == 2)
        {
            m_phrasesDictMap.insert(strList.at(0),strList.at(1));
        }
    }
    file.close();
    if(m_phrasesDictMap.count() > 0)
    {
        dictsInited = dictsInited && true;
    } else{
        dictsInited = false;
    }
    return dictsInited;
}

bool Zh2py::initPhrasesDict(QString PhrasesDict)
{
    QFile file;

    QString str;
    QStringList strList;
    file.setFileName(PhrasesDict);
    if(!file.open(QIODevice::ReadOnly))
    {
        dictsInited = false;
        return false;
    }
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("utf-8"));
    while(!in.atEnd())
    {
        str = in.readLine();
        strList = str.split("\t");

        if(strList.count() == 2)
        {
            m_phrasesDict.insert(strList.at(0),strList.at(1));
        }
    }
    file.close();
    if(m_phrasesDict.count() > 0)
    {
        dictsInited = dictsInited && true;
    } else{
        dictsInited = false;
    }
    return dictsInited;
}

bool Zh2py::initWordsDict(QString WordsDictFile)
{
    QFile file;

    QString str;
    QStringList strList;
    file.setFileName(WordsDictFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        dictsInited = false;
        return false;
    }
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("utf-8"));
    while(!in.atEnd())
    {
        str = in.readLine();
        strList = str.split("\t");

        if(strList.count() == 2)
        {
            m_wordsDict.insert(strList.at(0),strList.at(1));
        }
    }
    file.close();
    if(m_wordsDict.count() > 0)
    {
        dictsInited = dictsInited && true;
    } else{
        dictsInited = false;
    }
    return dictsInited;
}

QString Zh2py::getpy(QString &checkStr, pytype typeSet)
{
    int n = checkStr.count();
    QString CheckingStr;
    QString ResultStr;
    QChar chari;
    ushort uNum;

    for(int i = 0; i < n;)
    {
        chari = checkStr.at(i);
        uNum = chari.unicode();
        CheckingStr.clear();
        while(uNum >= 0x4E00 && uNum <= 0x9FA5)
        {
            CheckingStr.append(tradition2Simple(chari));
            i++;
            chari = checkStr.at(i);
            uNum = chari.unicode();
        }
        if(CheckingStr.isEmpty())
        {
            ResultStr.append(chari);
            i++;
        }
        else
        {

            CheckingStr = checkPureCh(CheckingStr);
            switch (typeSet) {
                case STYLE_FIRST_LETTER:
                    ResultStr.append(getFirstLetter(CheckingStr));
                    break;
                case STYLE_TONE:
                    ResultStr.append(" ");
                    ResultStr.append(removeSdNum(CheckingStr));
                    if(i < n-1) ResultStr.append(" ");
                    break;
                case STYLE_TONE_NUM:
                    ResultStr.append(" ");
                    ResultStr.append(CheckingStr);
                    if(i < n-1) ResultStr.append(" ");
                    break;
                default:
                    ResultStr.append(" ");
                    ResultStr.append(replaceSdLetters(CheckingStr));
                    if(i < n-1) ResultStr.append(" ");
            }
        }
    }
    if(ResultStr.startsWith(" ")) ResultStr.remove(0,1);
    return ResultStr;
}

QChar Zh2py::tradition2Simple(QChar& str)
{
    QChar resultChar(m_traditionDict.value(str));
    if(resultChar.isNull()) return str;
    return resultChar;
}

QString Zh2py::checkPureCh(const QString& str)
{
    int n = str.count();
    QString checkingStr;
    QString checkingMap;
    QString resultStr;
    int m;
    int checkNum;

    for(int i = 0; i < n;)
    {
        checkingMap = m_phrasesDictMap.value(str.at(i));
        if(checkingMap.isEmpty())  //非多音字处理
        {
            resultStr.append(chinkSingleChar(str.at(i)));
            resultStr.append(" ");
            i++;
        } else {  //查到多音字记录
            checkingStr.clear();
            m = checkingMap.count();  //多音词数映射表
            for(int j = m; j > 0; j--)
            {
               checkNum = QString(checkingMap.at(j-1)).toInt();
               checkingStr = m_phrasesDict.value(str.mid(i,checkNum));
               if(!checkingStr.isEmpty())
               {
                   resultStr.append(checkingStr);
                   resultStr.append(" ");
                   i = i + checkNum;
                   break;
               }
               if(j == 1)
               {
                   resultStr.append(chinkSingleChar(str.at(i)));
                   resultStr.append(" ");
                   i++;
               }
            }
        }
    }

    if(resultStr.endsWith(" ")) resultStr.remove(resultStr.count()-1,1);
    return resultStr;
}

QString Zh2py::chinkSingleChar(const QChar& theChar)
{
    QString resultStr = m_wordsDict.value(theChar);
    if(resultStr.isEmpty())
        return theChar;
    if(resultStr.contains(" "))
        resultStr = resultStr.mid(0,resultStr.indexOf(" "));
    return resultStr;
}

QString Zh2py::getFirstLetter(const QString& str)
{
    int n = str.count();
    QString resultStr;
    for(int i=0;i<n;i++)
    {
        if((i == 0) && (str.at(i) != " "))
            resultStr.append(str.at(i));
        if((i < n-2) && (str.at(i) == " "))
            resultStr.append(str.at(i+1));
    }
    return resultStr;
}

QString Zh2py::removeSdNum(const QString& str)
{
    int n = str.count();
    QString resultStr;
    for(int i=0;i<n;i++)
    {
        if(! str.at(i).isNumber()) resultStr.append(str.at(i));
    }
    return resultStr;
}

QString Zh2py::replaceSdLetters(const QString& str)
{
    QStringList strList = str.split(" ");
//    strList.replaceInStrings(QRegularExpression("([jqxy])u"),"\\1ü");
    QString resultStr;
    QString strn;
    int sd;
    int n = strList.count();
    for(int i = 0; i < n; i++)
    {
        strn = strList.at(i);
        sd = QString(strn.at(strn.count()-1)).toInt();
        strn.remove(strn.count()-1,1);
        switch(sd)
        {
            case 1:
                if(strn.contains("a")) {strn.replace("a","ā"); break;}
                if(strn.contains("o")) {strn.replace("o","ō"); break;}
                if(strn.contains("e")) {strn.replace("e","ē"); break;}
                if(strn.contains("iu")) {strn.replace("iu","īu"); break;}
                if(strn.contains("ui")) {strn.replace("ui","ūi"); break;}
                if(strn.contains("i")) {strn.replace("i","ī"); break;}
                if(strn.contains("u")) {strn.replace("u","ū"); break;}
                if(strn.contains("v")) {strn.replace("v","ǖ"); break;}
            case 2:
                if(strn.contains("a")) {strn.replace("a","á"); break;}
                if(strn.contains("o")) {strn.replace("o","ó"); break;}
                if(strn.contains("e")) {strn.replace("e","é"); break;}
                if(strn.contains("iu")) {strn.replace("iu","íu"); break;}
                if(strn.contains("ui")) {strn.replace("ui","úi"); break;}
                if(strn.contains("i")) {strn.replace("i","í"); break;}
                if(strn.contains("u")) {strn.replace("u","ú"); break;}
                if(strn.contains("v")) {strn.replace("v","ǘ"); break;}
            case 3:
                if(strn.contains("a")) {strn.replace("a","ǎ"); break;}
                if(strn.contains("o")) {strn.replace("o","ǒ"); break;}
                if(strn.contains("e")) {strn.replace("e","ě"); break;}
                if(strn.contains("iu")) {strn.replace("iu","ǐu"); break;}
                if(strn.contains("ui")) {strn.replace("ui","ǔi"); break;}
                if(strn.contains("i")) {strn.replace("i","ǐ"); break;}
                if(strn.contains("u")) {strn.replace("u","ǔ"); break;}
                if(strn.contains("v")) {strn.replace("v","ǚ"); break;}
            case 4:
                if(strn.contains("a")) {strn.replace("a","à"); break;}
                if(strn.contains("o")) {strn.replace("o","ò"); break;}
                if(strn.contains("e")) {strn.replace("e","è"); break;}
                if(strn.contains("iu")) {strn.replace("iu","ìu"); break;}
                if(strn.contains("ui")) {strn.replace("ui","ùi"); break;}
                if(strn.contains("i")) {strn.replace("i","ì"); break;}
                if(strn.contains("u")) {strn.replace("u","ù"); break;}
                if(strn.contains("v")) {strn.replace("v","ǜ"); break;}
        }
        resultStr.append(strn);
        if(i < n-1) resultStr.append(" ");
    }
    return resultStr;
}
