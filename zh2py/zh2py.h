#ifndef ZH2PY_H
#define ZH2PY_H
#include<QString>
#include<QHash>

#include "zh2py_global.h"

class ZH2PY_EXPORT Zh2py
{
public:
    enum pytype{STYLE_NORMAL,STYLE_TONE,STYLE_TONE_NUM,STYLE_FIRST_LETTER};

    Zh2py(QString Traditional2SimpleFile = "./tradition.dict",
          QString WordsDictFile = "./words.dict",
          QString PhrasesDict = "./phrases.dict",
          QString PhrasesDictMap = "./phrases.dict.mp");
    bool initTraditionDict(QString Traditional2SimpleFile = "./tradition.dict");
    bool initWordsDict(QString WordsDictFile = "./words.dict");
    bool initPhrasesDict(QString PhrasesDict = "./phrases.dict");
    bool initPhrasesMapDict(QString PhrasesDictMap = "./phrases.dict.mp");
    QString getpy(QString &checkStr, pytype typeSet = pytype::STYLE_NORMAL);
    bool dictsWaInited();

private:
    QChar tradition2Simple(QChar& str);
    QString tradition2Simple(const QString& str);
    QString checkPureCh(const QString& str);
    QString chinkSingleChar(const QChar& theChar);
    QString chechPhrases(const QString& str);

    QString getFirstLetter(const QString& str);
    QString removeSdNum(const QString& str);
    QString replaceSdLetters(const QString& str);

    bool dictsInited;

    QHash<QString,QString> m_wordsDict;
    QHash<QChar,QChar> m_traditionDict;
    QHash<QString,QString> m_phrasesDict;
    QHash<QString,QString> m_phrasesDictMap;
};

#endif // ZH2PY_H
