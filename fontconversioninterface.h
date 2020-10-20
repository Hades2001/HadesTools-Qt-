#ifndef FONTCONVERSIONINTERFACE_H
#define FONTCONVERSIONINTERFACE_H

#include <QObject>
#include <QFile>

class fontConversionInterface : public QObject
{
    Q_OBJECT
public:
    fontConversionInterface() {}
    virtual ~fontConversionInterface() {}
    virtual QString getConversionName();
    virtual QImage FontImageConversion(QImage);
    virtual int FontGenerat(QImage virtualCanvas,
                            QString FileName,
                            QString fontName,int fontSize,
                            QFile *Header,QFile *Source);
    virtual QWidget* getFontSetQWidger();
};

Q_DECLARE_METATYPE(fontConversionInterface*)

#endif // FONTCONVERSIONINTERFACE_H
