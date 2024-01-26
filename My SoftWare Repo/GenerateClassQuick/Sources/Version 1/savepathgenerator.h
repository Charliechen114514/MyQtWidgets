#ifndef SAVEPATHGENERATOR_H
#define SAVEPATHGENERATOR_H
#include <QString>
#include "newclass.h"
#include <QFileDialog>
#include <QMap>
class SavePathGenerator
{
public:
    SavePathGenerator() = default;
    SavePathGenerator(const SavePathGenerator&) = delete;
    const SavePathGenerator& operator=(const SavePathGenerator&) = delete;

    static void generatorPathForLists_ALL_IN_ONE(QList<NewClass*>& classes, QString common_path);
    static void generatorPathForLists_PAIR_SRIN(QList<NewClass*>& classes, QString header, QString sources);
    static bool getSavingDir(QWidget* window, QString& path);

    static QMap<QString, QStringList>* generatePairlyPreview(QList<NewClass*>& classes)
    {
        QMap<QString, QStringList>* preview = new QMap<QString, QStringList>;
        for(NewClass* each: classes)
        {
            QStringList hdsr;
            hdsr << each->getHeaderSavePlace() << each->getSourceSavePlace();
            preview->insert(each->getName(), hdsr);
        }
        return preview;
    }
};

#endif // SAVEPATHGENERATOR_H
