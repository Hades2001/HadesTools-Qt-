#include "fonttools.h"
#include "ui_fonttools.h"

FontTools::FontTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FontTools)
{
    ui->setupUi(this);
    ui->le_pixSize->setValidator(new QIntValidator(0,65536));

    //fontMonoConversion* fontMonoConversion;
    //QVariant var = QVariant::fromValue(fontMonoConversion);
    //ui->cb_colorMode->addItem("Mono",QVariant::fromValue(fontMonoConversion));
    //QVariant value = ui->cb_colorMode->currentData();
    //fontConversionInterface* ptr = value.value<fontConversionInterface*>();


    _Str = "F";
    Generated(_Str);
}

FontTools::~FontTools()
{
    delete ui;
}

QByteArray FontTools::Generated(QString font)
{
    _canvasMap = QImage(_fontSize,_fontSize,QImage::Format_RGB888);
    _canvasMap.fill(QColor(255,255,255));

    QPainter canvasPainter(&_canvasMap);

    canvasPainter.setPen(QColor(0,0,0));

    QFont SeleFont = ui->Cb_font->currentFont();
    SeleFont.setPixelSize(_fontSize);
    canvasPainter.setFont(SeleFont);
    canvasPainter.drawText(QRect(0 + offsetX, 0 + offsetY, _fontSize, _fontSize),Qt::AlignBottom | Qt::AlignHCenter,font);

    canvasPainter.end();

    //QVariant value = ui->cb_colorMode->currentData();
    //fontConversionInterface* ConversionPtr = value.value<fontConversionInterface*>();
    //if( ConversionPtr == nullptr ) return QByteArray();
    //QImage virtualCanvas = ConversionPtr->FontImageConversion(_canvasMap);
    //QImage labImage = virtualCanvas.scaled(ui->label->width(),
    //                                     ui->label->height(),
    //                                     Qt::KeepAspectRatio, Qt::FastTransformation);


    //ui->label->setPixmap(QPixmap::fromImage(labImage));
    return QByteArray();

}

void FontTools::on_bn_generated_pressed()
{
    Generated(_Str);
}

void FontTools::on_Cb_font_currentFontChanged(const QFont &f)
{
    Q_UNUSED(f)
    Generated(_Str);
}

void FontTools::on_le_pixSize_textChanged(const QString &arg1)
{

    _fontSize = ( arg1.toInt() <= 0 ) ? 6 : arg1.toInt();
    Generated(_Str);
}

void FontTools::on_bn_offsetDown_pressed()
{
    offsetY ++;
    Generated(_Str);
}

void FontTools::on_bn_offsetLeft_pressed()
{
    offsetX --;
    Generated(_Str);
}

void FontTools::on_bn_offsetRight_pressed()
{
    offsetX ++;
    Generated(_Str);
}

void FontTools::on_bn_offsetUP_pressed()
{
    offsetY --;
    Generated(_Str);
}

void FontTools::on_le_char_textChanged(const QString &arg1)
{
    _Str = arg1;
    Generated(_Str);
}

void FontTools::on_hs_MonoThreshold_valueChanged(int value)
{
    _monoThreshold = value;
    Generated(_Str);
}
/*
QImage fontMonoConversion::FontImageConversion(QImage virtualCanvas)
{
    int width = virtualCanvas.width();
    int height = virtualCanvas.height();
    QImage Canvas = QImage(width,height,QImage::Format_RGB888);
    Canvas.fill(QColor(255,255,255));

    QPainter virtualcanvasPainter(&virtualCanvas);

    virtualcanvasPainter.setPen(QColor(0,0,0));
    for ( int y = 0; y < height; y++) {
        for ( int x = 0; x < width; x++ ) {
            QRgb pix = virtualCanvas.pixel(x,y);
            int pixGray = qGray(pix);
            if( pixGray < 128 )
            {
                virtualcanvasPainter.drawPoint(x,y);
            }
        }
    }
    virtualcanvasPainter.end();
    return Canvas;
}

int fontMonoConversion::FontGenerat(QImage virtualCanvas,
                        QString FileName,
                        QString fontName,int fontSize,
                        QFile *Header,QFile *Source)
{
    Q_UNUSED(virtualCanvas)
    Q_UNUSED(FileName)
    Q_UNUSED(fontName)
    Q_UNUSED(fontSize)
    Q_UNUSED(Header)
    Q_UNUSED(Source)
    return 0;
}

QWidget* fontMonoConversion::getFontSetQWidger()
{
    return nullptr;
}
*/
