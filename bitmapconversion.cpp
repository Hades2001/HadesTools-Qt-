#include "bitmapconversion.h"
#include "ui_bitmapconversion.h"

BitmapConversion::BitmapConversion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BitmapConversion)
{
    ui->setupUi(this);

    ui->cb_mode->clear();
    ui->cb_mode->addItem("RGB");
    ui->cb_mode->addItem("Gray");
    ui->cb_mode->addItem("Mono");

    WidgetSizeMap.insert(0,QSize(600,400));
    WidgetSizeMap.insert(1,QSize(600,400));
    WidgetSizeMap.insert(2,QSize(600,400));

    ui->cb_MonoMode->insertItem(0,"Custom");
    ui->cb_MonoMode->insertItem(1,"IterativeBest");
    ui->cb_MonoMode->insertItem(2,"OSTU");

    ThresholdFunMap.insert(0,&BitmapConversion::GetCustomThreshold);
    ThresholdFunMap.insert(1,&BitmapConversion::GetIterativeBestThreshold);
    ThresholdFunMap.insert(2,&BitmapConversion::GetOSTUThreshold);

    ui->cb_MonoMode->setCurrentIndex(1);
}

BitmapConversion::~BitmapConversion()
{
    delete ui;
}

void BitmapConversion::on_bn_openFile_pressed()
{
    if( !_imageList.isEmpty())
    {
        foreach(imageConversion_t* imageConversionStr,_imageList)
        {
            delete imageConversionStr->image;
            delete imageConversionStr;
        }
    }
    _imageList.clear();

    QStringList _imagePathList;
    _imagePathList = QFileDialog::getOpenFileNames(this,
                                                  "open",
                                                  "../",
                                                  "image(*.bmp *.jpeg *jpg *.png)");
    if( _imagePathList.isEmpty())
    {
        return;
    }

    foreach(QString path,_imagePathList)
    {
        imageConversion_t* imageConversionPtr = new imageConversion_t;
        imageConversionPtr->path = path;
        imageConversionPtr->image = new QImage;
        imageConversionPtr->image->load(path);
        imageConversionPtr->ThresholdMode = 1;
        imageConversionPtr->Threshold = 128;
        _imageList.append(imageConversionPtr);
    }
    _indexNow = 0;
    changeImageFlag = true;
    showImage(_indexNow);
}

void BitmapConversion::showImage(int imgIndex)
{
    Q_UNUSED(imgIndex)

    if(( _imageList.size() < imgIndex )||( imgIndex < 0 ))
    {
        return;
    }

    QImage *image = _imageList.at(imgIndex)->image;

    ui->lab_image->width();

    if( changeImageFlag == true )
    {
        qInfo("Change Image ThresholdMode %d",_imageList.at(imgIndex)->ThresholdMode);
        qInfo("Change Image Threshold Value %d",_imageList.at(imgIndex)->Threshold);

        ui->cb_MonoMode->setCurrentIndex(_imageList.at(imgIndex)->ThresholdMode);
        ui->hs_MonoThreshold->setValue(_imageList.at(imgIndex)->Threshold);
        ui->le_Threshold->setText(QString::number(_imageList.at(imgIndex)->Threshold));
    }

    QImage labImage = image->scaled(ui->lab_image->width(),
                                    ui->lab_image->height(),
                                    Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lab_image->setPixmap(QPixmap::fromImage(Threshold_pro(labImage)));

    if( changeImageFlag == true )
    {
        changeImageFlag = false;
    }
    else
    {
        _imageList.at(imgIndex)->ThresholdMode = ui->cb_MonoMode->currentIndex();
        _imageList.at(imgIndex)->Threshold = _ThresholdValue;
    }

    ui->lab_imageNum->setText(QString("%1/%2").arg(imgIndex + 1).arg(_imageList.size()));
}
QPixmap BitmapConversion::ConversionGrayHistogram(QImage img, int* HistGram)
{
    int w = img.width();
    int h = img.height();

    QRgb pixRGB;
    int  pixGray;
    int  pixGrayHistogram[256];
    memset(pixGrayHistogram,0,sizeof (int) * 256);

    for( int y = 0; y < h; y++ )
    {
        for( int x = 0; x < w; x++ )
        {
            pixRGB = img.pixel(x,y);
            pixGray = qGray(pixRGB);
            pixGrayHistogram[pixGray]++;
        }
    }

    int HistogramMax = 0;
    for( int i = 0; i< 256; i++ )
    {
        HistogramMax = ( pixGrayHistogram[i] > HistogramMax ) ? pixGrayHistogram[i] : HistogramMax;
        if( HistGram != nullptr )
        {
            HistGram[i] = pixGrayHistogram[i];
        }
    }

    int histogramHeight = ui->lab_histogram->height();
    QPixmap histogramImage( 256,histogramHeight );
    histogramImage.fill(QColor(170,170,170));

    QPainter histogramPainter(&histogramImage);
    histogramPainter.setPen(QColor(0,0,0));
    for( int i = 0; i < 256; i++)
    {
        int pixSize = w * h / 50;
        pixGrayHistogram[i] = ( pixGrayHistogram[i] > pixSize ) ? pixSize : pixGrayHistogram[i];
        int HistogramGray = int(qrealMap(pixGrayHistogram[i],0,pixSize,0,histogramHeight));
        histogramPainter.drawLine(QLine(i,histogramHeight,i,histogramHeight - HistogramGray));
    }
    histogramPainter.end();


    return histogramImage;
}
QImage BitmapConversion::ConversionRGB2RGB(QImage img)
{
    return img;
}
QImage BitmapConversion::ConversionRGB2Gray(QImage img)
{
    QImage newimage(img.width(),img.height(),
                    QImage::Format_RGB888);

    int w = img.width();
    int h = img.height();

    int  pixGrayHistogram[256];
    ui->lab_histogram->setPixmap(ConversionGrayHistogram(img,pixGrayHistogram));

    QRgb pixRGB;
    int  pixGray;
    for( int y = 0; y < h; y++ )
    {
        for( int x = 0; x < w; x++ )
        {
            pixRGB = img.pixel(x,y);
            pixGray = qGray(pixRGB);
            newimage.setPixel(x,y,QColor(pixGray,pixGray,pixGray).rgb());
        }
    }
    return newimage;
}

QImage BitmapConversion::ConversionRGB2Mono(QImage img)
{
    QImage newimage(img.width(),img.height(),
                    QImage::Format_RGB888);

    int w = img.width();
    int h = img.height();

    QRgb pixRGB;
    int  pixGray;
    int  pixGrayHistogram[256];
    int Threshold = 0;
    int ThresholdModeIndex = 0;

    QPixmap HistogramImg = ConversionGrayHistogram(img,pixGrayHistogram);

    if( changeImageFlag == true )
    {
        ThresholdModeIndex = _imageList.at(_indexNow)->ThresholdMode;
    }
    else
    {
        ThresholdModeIndex = ui->cb_MonoMode->currentIndex();
    }

    ThresholdFunptr funPtr = ThresholdFunMap[ThresholdModeIndex];

    if(( changeImageFlag == true )&&( _imageList.at(_indexNow)->ThresholdMode == 0 ))
    {
        Threshold = _imageList.at(_indexNow)->Threshold;
    }
    else
    {
        Threshold = (this->*funPtr)(pixGrayHistogram);
        _imageList.at(_indexNow)->Threshold = Threshold;
    }

    if( Threshold < 0 )
    {
        qInfo("Error %d",Threshold);
        Threshold = 128;
    }

    ThresholdSetFlag = true;
    _ThresholdValue = Threshold;

    ui->hs_MonoThreshold->setValue(Threshold);
    ui->le_Threshold->setText(QString::number(Threshold));


    QPainter painter(&HistogramImg);
    painter.setPen(QColor(255,200,0,128));
    painter.drawLine(QLine(Threshold,0,Threshold,ui->lab_histogram->height()));
    painter.end();
    ui->lab_histogram->setPixmap(HistogramImg);


    for( int y = 0; y < h; y++ )
    {
        for( int x = 0; x < w; x++ )
        {
            pixRGB = img.pixel(x,y);
            pixGray = qGray(pixRGB);
            pixGrayHistogram[pixGray]++;
            pixGray = ( pixGray >= Threshold ) ? 255 : 0;
            newimage.setPixel(x,y,QColor(pixGray,pixGray,pixGray).rgb());
        }
    }
    return newimage;
}

QImage BitmapConversion::Threshold_pro(QImage img)
{
    Q_UNUSED(img);
    int modeIndex = ui->cb_mode->currentIndex();
    QImage (BitmapConversion::*ConversionFunPtr)(QImage) = nullptr;
    switch( modeIndex )
    {
        case 0: ConversionFunPtr = &BitmapConversion::ConversionRGB2RGB;  break;
        case 1: ConversionFunPtr = &BitmapConversion::ConversionRGB2Gray; break;
        case 2: ConversionFunPtr = &BitmapConversion::ConversionRGB2Mono; break;
    }

    return (this->*ConversionFunPtr)(img);
}
int BitmapConversion::GetCustomThreshold(int* HistGram)
{
    Q_UNUSED(HistGram);
    return ui->hs_MonoThreshold->value();
}

int BitmapConversion::GetIterativeBestThreshold(int* HistGram)
{
    int X, Iter = 0;
    int MeanValueOne, MeanValueTwo, SumOne, SumTwo, SumIntegralOne, SumIntegralTwo;

    int MinValue, MaxValue;
    for( MinValue = 0; MinValue < 256 &&( HistGram[MinValue] == 0); MinValue++ );
    for( MaxValue = 255; MaxValue > MinValue && ( HistGram[MaxValue] == 0 ); MaxValue--);

    qInfo("MinValue %d",MinValue);
    qInfo("MaxValue %d",MaxValue);

    if( MaxValue == MinValue ) return MaxValue;
    if( MaxValue + 1 == MinValue ) return MinValue;

    int Threshold = MinValue;
    int NewThreshold = (MaxValue + MinValue) >> 1;

    while( Threshold != NewThreshold )    // 当前后两次迭代的获得阈值相同时，结束迭代
    {
        SumOne = 0; SumIntegralOne = 0;
        SumTwo = 0; SumIntegralTwo = 0;
        Threshold = NewThreshold;
        for (X = MinValue; X <= Threshold; X++)         //根据阈值将图像分割成目标和背景两部分，求出两部分的平均灰度值
        {
            SumIntegralOne += HistGram[X] * X;
            SumOne += HistGram[X];
        }
        //if( SumOne == 0 ) return -2;
        MeanValueOne = ( SumOne == 0 ) ? SumIntegralOne : SumIntegralOne / SumOne;

        for (X = Threshold + 1; X <= MaxValue; X++)
        {
            SumIntegralTwo += HistGram[X] * X;
            SumTwo += HistGram[X];
        }
        //if( SumTwo == 0 ) return -3;
        MeanValueTwo = ( SumTwo == 0 ) ? SumIntegralTwo : SumIntegralTwo / SumTwo;
        NewThreshold = (MeanValueOne + MeanValueTwo) >> 1;
        Iter++;
        if (Iter >= 1000) return -1;
    }
    return Threshold;
}

int BitmapConversion::GetOSTUThreshold(int* HistGram)
{
    int Y, Amount = 0;
    int PixelBack = 0, PixelFore = 0, PixelIntegralBack = 0, PixelIntegralFore = 0, PixelIntegral = 0;
    double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma;              // 类间方差;
    int MinValue, MaxValue;
    int Threshold = 0;

    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ;
    if (MaxValue == MinValue) return MaxValue;          // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue) return MinValue;      // 图像中只有二个颜色

    for (Y = MinValue; Y <= MaxValue; Y++) Amount += HistGram[Y];        //  像素总数

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++) PixelIntegral += HistGram[Y] * Y;
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
        PixelBack = PixelBack + HistGram[Y];
        PixelFore = Amount - PixelBack;
        OmegaBack = double(PixelBack) / Amount;
        OmegaFore = double(PixelFore) / Amount;
        PixelIntegralBack += HistGram[Y] * Y;
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;
        MicroBack = double(PixelIntegralBack) / double(PixelBack);
        MicroFore = double(PixelIntegralFore) / double(PixelFore);
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);
        if (Sigma > SigmaB)
        {
            SigmaB = Sigma;
            Threshold = Y;
        }
    }
    return Threshold;
}

QString BitmapConversion::GeneratMonoFile(QImage *image,QString path)
{
    QByteArray sendarray;

    char pixData = 0;
    int pixCount = 0;

    for( int y = 0; y < image->height(); y++ )
    {
        for( int x = 0; x < image->width(); x++ )
        {
            int pix = qRed(image->pixel(x,y));
            pixData <<= 1;
            if( pix > 0 )
            {
                pixData |= 0x01;
            }
            pixCount++;
            if( pixCount >= 8 )
            {
                sendarray.append(pixData);
                pixData = 0;
                pixCount = 0;
            }
        }
    }

    if( pixCount != 0 )
    {
        pixData <<= ( 8 - pixCount );
        sendarray.append(pixData);
    }

    QString filestr;

    QString name = path.mid(path.lastIndexOf('/') + 1,
                            path.lastIndexOf('.') - path.lastIndexOf('/') - 1);

    filestr.append("unsigned char image_" + QString(name) + QString("[%1] = { \n").arg(sendarray.size()));


    for( int i = 0 ; i < sendarray.size(); i++ )
    {
        if(( i != 0 )&&( i % 32 == 31 ))
        {
            filestr.append(QString("0x%1, \n").arg(sendarray.at(i)&0x00ff,2,16,QLatin1Char('0')));
        }
        else
        {
            filestr.append(QString("0x%1, ").arg(sendarray.at(i)&0x00ff,2,16,QLatin1Char('0')));
        }
    }

    filestr.append( QString("\n};\n"));

    return filestr;
}

void BitmapConversion::on_bn_Front_pressed()
{
    _indexNow --;
    _indexNow = ( _indexNow < 0 ) ? 0 : _indexNow;

    changeImageFlag = true;
    showImage(_indexNow);
}

void BitmapConversion::on_bn_Next_pressed()
{
    _indexNow ++;
    _indexNow = ( _indexNow > (_imageList.size() - 1)) ? ( _imageList.size() - 1 ) : _indexNow;

    changeImageFlag = true;
    showImage(_indexNow);
}

void BitmapConversion::on_cb_mode_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    this->resize(WidgetSizeMap[index]);
    emit setWidgetSize(WidgetSizeMap[index]);
    showImage(_indexNow);
}

void BitmapConversion::on_hs_MonoThreshold_valueChanged(int value)
{
    Q_UNUSED(value);
    if( changeImageFlag == true ) return;
    if( ThresholdSetFlag == true ){ ThresholdSetFlag = false; return; }

    if( ui->cb_MonoMode->currentIndex() == 0 )
    {
        showImage(_indexNow);
    }
    else
    {
        ui->cb_MonoMode->setCurrentIndex(0);
    }
}

void BitmapConversion::on_cb_MonoMode_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if( changeImageFlag == true ) return;
    showImage(_indexNow);
}

void BitmapConversion::on_bn_Generated_pressed()
{
    QString saveFilePath = QFileDialog::getSaveFileName(this,
                                                        "save",
                                                        "../",
                                                        "c file(*.c)");
    if(saveFilePath.isEmpty()) return;
    changeImageFlag = true;

    QString allFileStr;

    _indexNow = 0;

    foreach(imageConversion_t* imagePtr, _imageList)
    {
        //showImage(_indexNow);
        QImage imageOut = Threshold_pro(*imagePtr->image);
        QString path = imagePtr->path;
        QString fileStr = GeneratMonoFile(&imageOut,path);
        allFileStr.append(fileStr);
        //_indexNow++;
    }

    QFile savefile( saveFilePath );
    savefile.open( QFile::WriteOnly | QFile::Append );
    savefile.write( allFileStr.toLatin1());
    savefile.close();

    QMessageBox::information(this,
                             "Image Generat",
                             "Image Generat Successful");

    changeImageFlag = false;

}
