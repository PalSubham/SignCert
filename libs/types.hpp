#pragma once

#include <QtQml/QQmlComponent>

namespace SignCert
{
    namespace Types
    {
        Q_NAMESPACE
        QML_ELEMENT

        enum OutputFormat
        {
            PEM,
            DER
        };
        Q_ENUM_NS(OutputFormat)
    }
} // namespace SignCert
