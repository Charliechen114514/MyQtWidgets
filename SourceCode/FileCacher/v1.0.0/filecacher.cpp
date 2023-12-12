#include "filecacher.h"
#include "filecacherprivate.h"
#include <QFile>
#include <QFileInfo>

#define Warming_Warning_For_LosingFile QString("缓存文件不存在,请尝试在路径: ") + QFileInfo(this->filePath).absoluteFilePath() + "\n" + \
                                        "或者尝试更换缓存路径"
#define Warming_Warning_For_Cannot_Open_File QString("我们没有办法打开文件: ") + QFileInfo(this->filePath).absoluteFilePath() + "\n" + \
                                        "请确保文件尚未被其他应用占用！或者是其他未知的原因"

FileCacher::FileCacher(const QString path)
{
    this->fileCacherCore = new FileCacherPrivate(",");
    this->filePath = path;
}

bool FileCacher::setDuplicateKeyPolicy(FileCacherPolicy policy)
{
    switch(policy)
    {
    case FileCacherPolicy::DISABLE_DuplicayeKey:
        this->fileCacherCore->setAlertOfNoneAllowanceOfDuplicateKey(false);
    case FileCacherPolicy::ENABLE_DuplicateKey:
        this->fileCacherCore->setAlertOfNoneAllowanceOfDuplicateKey(true);
    break;
    }

    return true;
}

bool FileCacher::isCacheFileAccessible()
{
    return this->filePath.isEmpty() || QFile::exists(this->filePath);
}

void FileCacher::addRecorderToCache(QString key, QString value)
{
    this->fileCacherCore->writeToTargetKeyWithValue(key, value);
}

void FileCacher::addRecorderToCache(QString key, QStringList value)
{
    this->fileCacherCore->writeToTargetKeyWithValues(key, value);
}

void FileCacher::addNewKeyToCache(QString key,bool* isOk, QStringList values)
{
    this->fileCacherCore->writeSingleNewKeyToCache(key, isOk, values);
}

void FileCacher::writeToCacheFile(bool* isOk)
{
    if(!this->isCacheFileAccessible())
    {
        *isOk = false;
        CCSTDC_Only_Assert(this->isCacheFileAccessible());
        CCSTDC_Error_Occured("文件不存在", Warming_Warning_For_LosingFile);
    }

    QFile file(this->filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        *isOk = false;
        CCSTDC_Only_Assert(file.open(QIODevice::WriteOnly));
        CCSTDC_Error_Occured("无法对文件进行写入", Warming_Warning_For_Cannot_Open_File);
    }

    file.write(this->fileCacherCore->generateWriteableCahce().toStdString().c_str());
    file.close();

    *isOk = true;
}

void FileCacher::LoadCache(bool* isOk)
{
    if(!this->isCacheFileAccessible())
    {
        *isOk = false;
        CCSTDC_Only_Assert(this->isCacheFileAccessible());
        CCSTDC_Error_Occured("文件不存在", Warming_Warning_For_LosingFile);
    }

    QFile file(QFileInfo(this->filePath).absoluteFilePath());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        *isOk = false;
        CCSTDC_Only_Assert(file.open(QIODevice::WriteOnly | QIODevice::Text));
        CCSTDC_Error_Occured("无法对文件进行写入", Warming_Warning_For_Cannot_Open_File);
    }

    this->fileCacherCore->writeToCacheMap(file.readAll());

    file.close();
    *isOk = true;
}

QString FileCacher::doGetReadableString()
{
    return this->fileCacherCore->generateWriteableCahce();
}
