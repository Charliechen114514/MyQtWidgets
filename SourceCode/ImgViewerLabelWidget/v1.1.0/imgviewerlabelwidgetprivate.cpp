#include "imgviewerlabelwidgetprivate.h"

ImgViewerLabelWidgetPrivate::ImgViewerLabelWidgetPrivate()
{}

void ImgViewerLabelWidgetPrivate::setImageLists(QStringList listsOfPics)
{
    this->pathOfPics = listsOfPics;
}

QStringList& ImgViewerLabelWidgetPrivate::getImageLists()
{
    return this->pathOfPics;
}

QString ImgViewerLabelWidgetPrivate::getTargetPicPath(long index)
{
    if(!isFetchPathValid(index))
        return "";

    return this->pathOfPics.at(index);
}

bool ImgViewerLabelWidgetPrivate::isFetchPathValid(long index)
{
    if(index < 0 || index >= this->pathOfPics.size())
        return false;
    return true;
}

bool ImgViewerLabelWidgetPrivate::isTargetExistedInLists(QString Path)
{
    return this->pathOfPics.indexOf(Path) != -1;
}

long ImgViewerLabelWidgetPrivate::getCurrentPicPathNums()
{
    return this->pathOfPics.size();
}

long ImgViewerLabelWidgetPrivate::findTargetPicPath(QString Path)
{
    return this->pathOfPics.indexOf(Path);
}

bool ImgViewerLabelWidgetPrivate::insertToTargetPlace(QString Path, long indexPlace)
{
    if(indexPlace < 0 || indexPlace > this->pathOfPics.size())
        return false;

    this->pathOfPics.insert(indexPlace, Path);
    return true;
}

bool ImgViewerLabelWidgetPrivate::eraseTargetPlace(long index)
{
    if(!isFetchPathValid(index))
        return false;
    this->pathOfPics.remove(index);
    return true;
}

void ImgViewerLabelWidgetPrivate::clearAll()
{
    this->pathOfPics.clear();
}
