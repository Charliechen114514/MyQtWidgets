#ifndef FILECACHERPRIVATE_H
#define FILECACHERPRIVATE_H
#include <QString>
#include <QFile>
#include <QMap>
class FileCacherPrivate
{
public:
    // disable the functioning
    FileCacherPrivate() = delete;
    FileCacherPrivate(const FileCacherPrivate&) = delete;
    const FileCacherPrivate& operator=(const FileCacherPrivate&)=delete;
    // only accessable initializer
    FileCacherPrivate(QString eachParseSeperator = ",", QString keyValueSeperator = ":", QString interLinkSeperator = "\n");

    // setter
    void        resetEachParseSeperator(QString eachParseSeperator){this->eachParseSeperator = eachParseSeperator;}
    void        resetInterLinkseperator(QString interLinkseperator){this->interLinkseperator = interLinkseperator;}
    void        resetkeyValueSeperator(QString keyValueSeperator){this->keyValueSeperator = keyValueSeperator;}
    // setAlert of whether a new key is allowed when keys are already existed when trying to write a new key
    void        setAlertOfNoneAllowanceOfDuplicateKey(bool policy);
    // getter
    QString     getEachParseSeperator(){return this->eachParseSeperator;}
    QString     getInterLinkseperator(){return this->interLinkseperator;}
    QString     getKeyValueSeperator(){return this->keyValueSeperator;}

    // Read each parseLink
    QStringList getTargetValue(QString key);
    // getAllKeys
    QStringList getAllKeys();
    // write each parseLink
    void        writeToTargetKeyWithValues(QString key, QStringList valueList);
    void        writeToTargetKeyWithValue(QString key, QString value);
    // add a new key
    void        writeSingleNewKeyToCache(QString key, bool* isKeyAlreadyExsited, QStringList values = QStringList());
    // parse To Link
    void        writeToCacheMap(QString fromFileLike);
    // generate writable files
    QString     generateWriteableCahce();

    // isKeyAccessable
    bool        isTargetKeyAccessible(QString key){return this->cacheMap.contains(key) || this->waitingKey.contains(key);};


private:
    QString interLinkseperator;
    QString eachParseSeperator;
    QString keyValueSeperator;
    QStringList waitingKey;
    QMultiMap<QString, QString> cacheMap;
    bool isAllowedAppendDuplicateKey;
};

#endif // FILECACHERPRIVATE_H
