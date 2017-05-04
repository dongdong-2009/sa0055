#include "validatormanager.h"

#include <QRegExp>
#include <QRegExpValidator>

QValidator *ValidatorManager::_ipValidator = 0;

const QString IpAddrRegExp = "(?:(?:25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]?\\d)\\.){3}(?:25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]?\\d)";

ValidatorManager::ValidatorManager()
{
}

QValidator *ValidatorManager::ipValidator()
{
    if (!_ipValidator)
    {
        QRegExp regExp(IpAddrRegExp);
        _ipValidator = new QRegExpValidator(regExp);
    }

    return _ipValidator;
}
