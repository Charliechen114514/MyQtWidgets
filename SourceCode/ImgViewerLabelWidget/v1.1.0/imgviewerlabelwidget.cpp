#include "imgviewerlabelwidget.h"
#include "ui_imgviewerlabelwidget.h"
#include <imgviewerlabelwidgetprivate.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QDragEnterEvent>
const QString NOIMGS_STR= "当前没有图片！请拖拽或者是通过文件对话框导入导入！";
#define SET_INFO_STR(curIndex, tolNum) QString("当前共有:") + QString::number(tolNum) + "图片, " + \
                                        QString("你在:") + QString::number(curIndex + 1) + "张"

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
    setAcceptDrops (true);
    ui->showPicLabel->setAcceptDrops(true);
    this->widgetPrivate = new ImgViewerLabelWidgetPrivate;
    initTheInfoLabel();
    curViewIndex = 0;
}

void ImgViewerLabelWidget::importPics()
{
    QStringList import = QFileDialog::getOpenFileNames(this, "请导入图片","",CreateFilterPicFormat());
    if(import.empty())
        return;

    this->widgetPrivate->setImageLists(std::move(import));
}

void ImgViewerLabelWidget::initTheInfoLabel()
{
    if(this->widgetPrivate->getCurrentPicPathNums() == 0)
        ui->infoLabel->setText(NOIMGS_STR);
    else
        ui->infoLabel->setText(SET_INFO_STR(curViewIndex, this->widgetPrivate->getCurrentPicPathNums()));
}

void ImgViewerLabelWidget::updateInfoLabel(){
    ui->infoLabel->setText(SET_INFO_STR(curViewIndex, this->widgetPrivate->getCurrentPicPathNums()));
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
        qDebug() << curViewIndex;
        QMessageBox::critical(this,"访问不存在的文件！", "ouch！貌似访问到了不存在的图片！我还是不动好了！");
        return;
    }

    this->ui->showPicLabel->setPixmap(QPixmap::fromImage \
                                      (QImage(this->widgetPrivate->getTargetPicPath(curViewIndex))) \
                                      .scaled(this->ui->showPicLabel->size(),Qt::KeepAspectRatio));

    updateInfoLabel();
}

void ImgViewerLabelWidget::showTargetPic(long index)
{

    if(!this->widgetPrivate->isFetchPathValid(index))
    {
        qDebug() << index;
        QMessageBox::critical(this,"访问不存在的文件！", "ouch！貌似访问到了不存在的图片！我还是不动好了！");
        return;
    }

    this->ui->showPicLabel->setPixmap(QPixmap::fromImage \
                                      (QImage(this->widgetPrivate->getTargetPicPath(index))) \
                                          .scaled(this->ui->showPicLabel->size(),Qt::KeepAspectRatio));

    curViewIndex = index;
    updateInfoLabel();
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

void ImgViewerLabelWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        // temporily we just catch the first one
        QList<QUrl> info = event->mimeData()->urls();
        QStringList res;
        for(QUrl& each_url : info)
            res.push_back(each_url.toLocalFile());
        if(this->widgetPrivate->getCurrentPicPathNums() == 0)
        {
            this->widgetPrivate->setImageLists(res);
            curViewIndex += res.size() - 1;
            showCurrentPic();
        }
        else
        {
            qDebug() << "InDrugEvent";
            qDebug() << this->widgetPrivate->getTargetPicPath(curViewIndex) << " " << res.first();
            if(this->widgetPrivate->getTargetPicPath(curViewIndex) == res.last())
            {
                qDebug() << "fetch the same pics";
                return; // reject the same input
            }
            long index = this->curViewIndex + 1;
            for(QString& each_path : res)
                this->widgetPrivate->insertToTargetPlace(each_path, index++);
            this->curViewIndex += res.size();
            showCurrentPic();
        }
    }
}
