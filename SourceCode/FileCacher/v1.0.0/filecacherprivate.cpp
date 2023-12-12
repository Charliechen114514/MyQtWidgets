#include "filecacherprivate.h"
#include "CCSTDC_Assert.h"
#define Default_AllowedAppendDuplicateKey_Policy false


FileCacherPrivate::FileCacherPrivate(QString eachParseSeperator,QString keyValueSeperator,  QString interLinkSeperator)
{
    this->eachParseSeperator = eachParseSeperator;
    this->interLinkseperator = interLinkSeperator;
    this->keyValueSeperator = keyValueSeperator;
    this->waitingKey = QStringList();
    isAllowedAppendDuplicateKey = Default_AllowedAppendDuplicateKey_Policy;
}

void FileCacherPrivate::setAlertOfNoneAllowanceOfDuplicateKey(bool policy)
{
    this->isAllowedAppendDuplicateKey = policy;
}


QStringList FileCacherPrivate::getTargetValue(QString key)
{
    return this->cacheMap.values(key);
}

void FileCacherPrivate::writeToTargetKeyWithValues(QString key, QStringList valueList)
{
    for(QString& each_value : valueList)
        writeToTargetKeyWithValue(key, each_value);
}

void FileCacherPrivate::writeToTargetKeyWithValue(QString key, QString value)
{
    if(!this->cacheMap.contains(key, value))
        this->cacheMap.insert(key, value);
}

void FileCacherPrivate::writeSingleNewKeyToCache(QString key, bool* isKeyAlreadyExsited, QStringList values)
{
    if(this->isTargetKeyAccessible(key))
    {
        if(this->isAllowedAppendDuplicateKey)
        {
            CCSTDC_STDGUI_Only_Assert(!this->isTargetKeyAccessible(key), "发生警告错误", "不允许插入一个已经存在了的值");
            CCSTDC_Error_Occured("What a Pity","索引已经存在了Ouch!");
            *isKeyAlreadyExsited = true;
        }
        else
        {
            // Then, we allowed to append it
            // for case no values
            if(values.empty())
                return;
            // else, there shall be values waiting for appending
            writeToTargetKeyWithValues(key, values);
            *isKeyAlreadyExsited = true;
        }
    }

    // a new keys with no values
    if(values.empty())
    {
        this->waitingKey.push_back(key);
    }
    else
    {
        this->writeToTargetKeyWithValues(key, values);
    }
    *isKeyAlreadyExsited = false;
    return;
}

void FileCacherPrivate::writeToCacheMap(QString fromFileLike)
{
    QStringList eachKeyValueList = fromFileLike.split(this->interLinkseperator);
    for(QString each_one : eachKeyValueList)
    {
        QStringList lists = each_one.split(this->keyValueSeperator);
        if(lists.size() != 2)
            continue; // error line, ignore
        QString currentKey = lists.first();
        lists.pop_front();
        QStringList valueList = lists.first().split(this->eachParseSeperator);
        for(QString& value : valueList)
            this->cacheMap.insert(currentKey, value);
    }
}


QString FileCacherPrivate::generateWriteableCahce()
{
    QStringList keys = this->cacheMap.uniqueKeys();
    QString resultString;
    for(QString& key:keys)
    {
        resultString += (key + this->keyValueSeperator);
        QStringList values = this->cacheMap.values(key);
        for(QString& value : values)
        {
            if(value.isEmpty())
                continue;
            resultString += (value + this->eachParseSeperator);
        }
        if(resultString.endsWith(","))
            resultString = resultString.first(resultString.size() - 1);
        resultString += this->interLinkseperator;
    }

    return resultString;
}


