#ifndef FONTTOOLS_H
#define FONTTOOLS_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>

namespace Ui {
class FontTools;
}

class FontTools : public QWidget
{
    Q_OBJECT

public:
    explicit FontTools(QWidget *parent = nullptr);
    ~FontTools();

    QByteArray Generated(QString font);

private slots:
    void on_bn_generated_pressed();
    void on_Cb_font_currentFontChanged(const QFont &f);
    void on_le_pixSize_textChanged(const QString &arg1);

    void on_bn_offsetDown_pressed();

    void on_bn_offsetLeft_pressed();

    void on_bn_offsetRight_pressed();

    void on_bn_offsetUP_pressed();

    void on_le_char_textChanged(const QString &arg1);

    void on_hs_MonoThreshold_valueChanged(int value);

private:
    Ui::FontTools *ui;
    int _fontSize = 16;
    int offsetX = 0,offsetY = 0;
    int _monoThreshold = 128;
    QString _Str;
    QImage _canvasMap;
};

#endif // FONTTOOLS_H
