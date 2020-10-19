#include "fonttools.h"
#include "ui_fonttools.h"

FontTools::FontTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FontTools)
{
    ui->setupUi(this);
    ui->le_pixSize->setValidator(new QIntValidator(0,65536));

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
    //canvasPainter.drawRect(QRect(0, 0, _fontSize, _fontSize));
    canvasPainter.drawText(QRect(0 + offsetX, 0 + offsetY, _fontSize, _fontSize),Qt::AlignBottom | Qt::AlignHCenter,font);

    canvasPainter.end();

    QImage virtualCanvas = QImage(_fontSize,_fontSize,QImage::Format_RGB888);
    virtualCanvas.fill(QColor(255,255,255));

    QPainter virtualcanvasPainter(&virtualCanvas);

    virtualcanvasPainter.setPen(QColor(0,0,0));
    for ( int y = 0; y < _fontSize; y++) {
        for ( int x = 0; x < _fontSize; x++ ) {
            QRgb pix = _canvasMap.pixel(x,y);
            int pixGray = qGray(pix);
            if( pixGray < _monoThreshold )
            {
                virtualcanvasPainter.drawPoint(x,y);
            }
        }
    }
    virtualcanvasPainter.end();
    QImage labImage = virtualCanvas.scaled(ui->label->width(),
                                         ui->label->height(),
                                         Qt::KeepAspectRatio, Qt::FastTransformation);


    ui->label->setPixmap(QPixmap::fromImage(labImage));

    /*
    QPixmap canvasMap(ui->label->width(),ui->label->width());
    canvasMap.fill(QColor(0,0,0));

    QPainter canvasPainter(&canvasMap);

    if( _fontSize > 46 )
    {

    }
    else
    {
        qreal canvasPixsize = qreal(ui->label->width()) / qreal(_fontSize);
        qreal canvasPointSize = canvasPixsize * 0.8;
        for( int y = 0; y < _fontSize; y++ )
        {
            for( int x = 0; x < _fontSize; x++ )
            {
                canvasPainter.fillRect(QRectF(x * canvasPixsize + ( canvasPixsize - canvasPointSize) / 2,
                                              y * canvasPixsize + ( canvasPixsize - canvasPointSize) / 2,
                                              canvasPointSize,
                                              canvasPointSize),QBrush(QColor(0,255,0)));
            }
        }
    }


    canvasPainter.setPen(QColor(255,255,255));

    QFont SeleFont = ui->Cb_font->currentFont();
    SeleFont.setPixelSize(_fontSize);
    canvasPainter.setFont(SeleFont);
    canvasPainter.drawText(QRect(0, 0, 128, 128),Qt::AlignBottom | Qt::AlignLeft,font);

    canvasPainter.end();

    ui->label->setPixmap(canvasMap);
    */
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
