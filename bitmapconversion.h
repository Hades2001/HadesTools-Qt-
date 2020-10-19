#ifndef BITMAPCONVERSION_H
#define BITMAPCONVERSION_H

#include <QWidget>
#include <QImage>
#include <QMap>
#include <QFile>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class BitmapConversion;
}

class BitmapConversion : public QWidget
{
    Q_OBJECT

public:
    typedef struct {
        QString path;
        QImage *image;
        int mode;
        int ThresholdMode;
        int Threshold;
    }imageConversion_t;

    explicit BitmapConversion(QWidget *parent = nullptr);
    ~BitmapConversion();

    QSize getWidgetSize(){ return QSize(600,400);}

    void showImage(int imgIndex);
    QImage Threshold_pro(QImage img);

    QPixmap ConversionGrayHistogram(QImage img, int* HistGram);

    QImage ConversionRGB2RGB(QImage img);
    QImage ConversionRGB2Gray(QImage img);
    QImage ConversionRGB2Mono(QImage img);
    int GetCustomThreshold(int* HistGram);
    int GetIterativeBestThreshold(int* HistGram);
    int GetOSTUThreshold(int* HistGram);

    QString GeneratMonoFile(QImage* image, QString path);

signals:
    void setWidgetSize(QSize);

private slots:
    void on_bn_openFile_pressed();
    void on_bn_Front_pressed();
    void on_bn_Next_pressed();
    void on_cb_mode_currentIndexChanged(int index);
    void on_hs_MonoThreshold_valueChanged(int value);
    void on_cb_MonoMode_currentIndexChanged(int index);
    void on_bn_Generated_pressed();

private:
    Ui::BitmapConversion *ui;
    QList<imageConversion_t*> _imageList;

    int _indexNow;
    typedef int (BitmapConversion::*ThresholdFunptr)(int*);
    QMap<int,QSize> WidgetSizeMap;
    QMap<int,ThresholdFunptr> ThresholdFunMap;

    bool ThresholdSetFlag = false;
    int _ThresholdValue = 0;

    bool changeImageFlag = false;
    bool loadImageArgFlag = false;

    bool _showImageNow = false;

    qreal qrealMap(qreal x, qreal in_min, qreal in_max, qreal out_min, qreal out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
};

#endif // BITMAPCONVERSION_H
