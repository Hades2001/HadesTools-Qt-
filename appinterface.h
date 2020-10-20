#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <QObject>

class AppInterface : public QObject
{
    Q_OBJECT
public:
    AppInterface() {}
    virtual ~AppInterface() {}
};

#endif // APPINTERFACE_H
