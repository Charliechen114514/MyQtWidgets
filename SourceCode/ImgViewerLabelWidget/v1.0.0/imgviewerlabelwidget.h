#ifndef IMGVIEWERLABELWIDGET_H
#define IMGVIEWERLABELWIDGET_H

#include <QWidget>

class ImgViewerLabelWidgetPrivate;


namespace Ui {
class ImgViewerLabelWidget;
}

class  Q_DECL_EXPORT ImgViewerLabelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImgViewerLabelWidget(QWidget *parent = nullptr);
    const ImgViewerLabelWidget& operator=(const ImgViewerLabelWidget&) = delete;
    ImgViewerLabelWidget(const ImgViewerLabelWidget&) = delete;
    void importPics();
    void moveToPrevious();
    void moveToAfterWard();
    void showCurrentPic();
    void addPicsToCurrentPlace();
    void deleteCurrentPic();
    void deleteAllPics();
    void showTargetPic(long index);
    ~ImgViewerLabelWidget();

private slots:

    void on_btn_backward_clicked();

    void on_btn_forward_clicked();


    void on_btn_insertHere_clicked();

    void on_btn_deleteThis_clicked();

private:
    Ui::ImgViewerLabelWidget        *ui;
    ImgViewerLabelWidgetPrivate*    widgetPrivate;
    bool isExistedPic();
    long curViewIndex;
};

#endif // IMGVIEWERLABELWIDGET_H
