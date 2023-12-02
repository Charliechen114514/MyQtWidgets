#ifndef FILECACHER_H
#define FILECACHER_H
#include <QFile>
#include <CCSTDC_Assert.h>
#include <FileCacherlib_global.h>
// pre-announcements of FileCacherPrivate
class FileCacherPrivate;
enum class FileCacherError{
    Cache_Not_Exsited = 1,
    Cache_Not_AllowedDuplicate_Key
};

enum class FileCacherPolicy{
    // pair of allowed Duplicate keys
    DISABLE_DuplicayeKey    = 0,
    ENABLE_DuplicateKey     = 1
};

class FILECACHERLIB_EXPORT FileCacher
{
public:
    // Disable the All Default Constructing Interface
    FileCacher() = delete;
    const FileCacher& operator=(const FileCacher&) = delete;
    FileCacher(const FileCacher&) = delete;

    // realConstructor
    FileCacher(const QString path);
    // set Policy
    bool setDuplicateKeyPolicy(FileCacherPolicy policy);

    // view if writeable or readable
    bool isCacheFileAccessible();
    // write cache Laterly
    void addRecorderToCache(QString key, QString value);
    void addRecorderToCache(QString key, QStringList values);
    void addNewKeyToCache(QString key,bool* isOk, QStringList values = QStringList());

    // writeToFile
    void writeToCacheFile(bool* isOk);

    // LoadCache
    void LoadCache(bool* isOk);

    QString doGetReadableString();
private:
    // core of the fileCacher
    FileCacherPrivate* fileCacherCore;
    QString filePath;
};

#endif // FILECACHER_H
