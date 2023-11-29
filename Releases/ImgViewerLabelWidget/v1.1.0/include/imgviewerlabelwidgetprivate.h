#ifndef IMGVIEWERLABELWIDGETPRIVATE_H
#define IMGVIEWERLABELWIDGETPRIVATE_H
#include <QStringList>

class ImgViewerLabelWidgetPrivate
{
public:
    ImgViewerLabelWidgetPrivate();

    void setImageLists(QStringList piclists);
    QStringList& getImageLists();

    QString getTargetPicPath(long index);
    long findTargetPicPath(QString Path);
    bool insertToTargetPlace(QString Path, long indexPlace);

    bool eraseTargetPlace(long index);
    void clearAll();
    long getCurrentPicPathNums();

    bool isFetchPathValid(long index);
    bool isTargetExistedInLists(QString searchingPath);

private:
    QStringList     pathOfPics;
};

#endif // IMGVIEWERLABELWIDGETPRIVATE_H
