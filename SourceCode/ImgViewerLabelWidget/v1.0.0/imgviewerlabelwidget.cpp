#include "imgviewerlabelwidget.h"
#include "ui_imgviewerlabelwidget.h"
#include <imgviewerlabelwidgetprivate.h>
#include <QFileDialog>
#include <QMessageBox>
static const QString PicFormat[] {
    "png格式图片(*.png)",
    "tif格式图片(*.tif)"
    "jpeg格式图片(*.jpg)"
    "bmp格式图片(*.bmp)"
    "SVG格式图片(*.svg)"
};

QString CreateFilterPicFormat(){
    QString res;
    for(const QString& eachFormart : PicFormat)
        res += (eachFormart + ";;");
    return res;
}

ImgViewerLabelWidget::ImgViewerLabelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImgViewerLabelWidget)
{
    ui->setupUi(this);
    this->widgetPrivate = new ImgViewerLabelWidgetPrivate;
}

void ImgViewerLabelWidget::importPics()
{
    QStringList import = QFileDialog::getOpenFileNames(this, "请导入图片","",CreateFilterPicFormat());
    if(import.empty())
        return;

    this->widgetPrivate->setImageLists(std::move(import));
}

void ImgViewerLabelWidget::moveToPrevious()
{
    if(!isExistedPic())
        return;

    curViewIndex--;
    if(curViewIndex < 0)
    {
        bool select = QMessageBox::information(this,"到头啦！","到开始的这一张了！要帮你跳转到最后一张嘛?");
        if(select)
            curViewIndex = this->widgetPrivate->getCurrentPicPathNums() - 1;
    }

    showCurrentPic();
}

void ImgViewerLabelWidget::moveToAfterWard()
{
    if(!isExistedPic())
        return;
    curViewIndex++;
    if(curViewIndex >= this->widgetPrivate->getCurrentPicPathNums())
    {
        bool select = QMessageBox::information(this,"到头啦！","到最后的这一张了！要帮你跳转到开始的那一张嘛?");
        if(select)
            curViewIndex = 0;
    }

    showCurrentPic();
}

void ImgViewerLabelWidget::showCurrentPic()
{
    if(!this->widgetPrivate->isFetchPathValid(curViewIndex))
    {
        QMessageBox::critical(this,"访问不存在的文件！", "ouch！貌似访问到了不存在的图片！我还是不动好了！");
        return;
    }

    this->ui->showPicLabel->setPixmap(QPixmap::fromImage \
                                      (QImage(this->widgetPrivate->getTargetPicPath(curViewIndex))) \
                                      .scaled(this->ui->showPicLabel->size(),Qt::KeepAspectRatio));

}

void ImgViewerLabelWidget::showTargetPic(long index)
{
    if(!this->widgetPrivate->isFetchPathValid(index))
    {
        QMessageBox::critical(this,"访问不存在的文件！", "ouch！貌似访问到了不存在的图片！我还是不动好了！");
        return;
    }

    this->ui->showPicLabel->setPixmap(QPixmap::fromImage \
                                      (QImage(this->widgetPrivate->getTargetPicPath(index))) \
                                          .scaled(this->ui->showPicLabel->size(),Qt::KeepAspectRatio));

    curViewIndex = index;
}

void ImgViewerLabelWidget::addPicsToCurrentPlace()
{
    QStringList import = QFileDialog::getOpenFileNames(this, "请导入图片","",CreateFilterPicFormat());
    if(import.empty())
        return;

    int index = this->curViewIndex;

    for(QString& each : import)
        this->widgetPrivate->insertToTargetPlace(each, index++);

    return;
}

void ImgViewerLabelWidget::deleteCurrentPic()
{
    this->widgetPrivate->eraseTargetPlace(curViewIndex);
    this->moveToAfterWard();
}

void ImgViewerLabelWidget::deleteAllPics()
{
    this->widgetPrivate->clearAll();
    curViewIndex = 0;
}

bool ImgViewerLabelWidget::isExistedPic()
{
    return this->widgetPrivate->getCurrentPicPathNums() != 0;
}

ImgViewerLabelWidget::~ImgViewerLabelWidget()
{
    delete ui;
}

void ImgViewerLabelWidget::on_btn_backward_clicked()
{
    moveToPrevious();
}

void ImgViewerLabelWidget::on_btn_forward_clicked()
{
    moveToAfterWard();
}


void ImgViewerLabelWidget::on_btn_insertHere_clicked()
{
    if(!this->isExistedPic())
        importPics();
    else
        addPicsToCurrentPlace();
}

void ImgViewerLabelWidget::on_btn_deleteThis_clicked()
{
    deleteCurrentPic();
}

