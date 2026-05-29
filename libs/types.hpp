#pragma once

#include <QtQml/QQmlComponent>

namespace SignCert {

class Types
{
    Q_GADGET
    QML_ELEMENT

public:
    enum OutputFormat
    {
        PEM,
        DER
    };
    Q_ENUM(OutputFormat)
};

} // namespace SignCert
