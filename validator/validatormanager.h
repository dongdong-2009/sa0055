#ifndef VALIDATORMANAGER_H
#define VALIDATORMANAGER_H

class QValidator;

class ValidatorManager
{
public:
    ValidatorManager();

    static QValidator *ipValidator();

private:
    static QValidator *_ipValidator;
};

#endif // VALIDATORMANAGER_H
