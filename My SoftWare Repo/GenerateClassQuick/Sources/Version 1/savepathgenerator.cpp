#include "savepathgenerator.h"
#include <QStandardPaths>
#include "utils.h"
const QString headerSuffix = ".h";
const QString sourceSuffix = ".cpp";

void SavePathGenerator::generatorPathForLists_ALL_IN_ONE(QList<NewClass*>& classes, QString common_path)
{
    for(NewClass*& each : classes)
    {
        each->setHeaderSavePlace(common_path + each->getName() + headerSuffix);
        each->setSourceSavePlace(common_path + each->getName() + sourceSuffix);
    }
}

void SavePathGenerator::generatorPathForLists_PAIR_SRIN(QList<NewClass*>& classes, QString header, QString sources)
{
    for(NewClass*& each : classes)
    {
        each->setHeaderSavePlace(header + each->getName() + headerSuffix);
        each->setSourceSavePlace(sources + each->getName() + sourceSuffix);
    }
}

bool SavePathGenerator::getSavingDir(QWidget* window, QString& path)
{
    QString pathDir = QFileDialog::getExistingDirectory(window, "选择保存路径",DESKTOP_PATH);
    if(pathDir.isEmpty()){
        return false;
    }
    path = pathDir + "/";
    return true;
}
