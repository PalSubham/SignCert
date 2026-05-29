// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <memory>
#include <unordered_map>

#include <QFile>
#include <QObject>

#include <botan/x509_ca.h>
#include <botan/x509cert.h>
#include <botan/pkcs10.h>
#include <botan/pkcs8.h>
#include <botan/rng.h>

#include "types.hpp"

using namespace std;
using namespace Botan;

namespace SignCert
{
    class CSRSigner : public QObject
    {
        Q_OBJECT

    public:
        explicit CSRSigner(QObject *parent = nullptr);

        void sign(
            const QString &csrFile,
            const QString &caFile,
            const QString &caKeyFile,
            const unsigned int days,
            const QString &outDir,
            const QString &outFileName,
            const Types::OutputFormat outFileType
        );

    public slots:
        void providePassword(const QString &password);

    signals:
        void info(const QString &info);
        void warn(const QString &warn);
        void error(const QString &error);
        void debug(const QString &debug);
        void needPassword();
        void finished();

        void passwordReady(const QString &password, QPrivateSignal);

    private:
        unique_ptr<RandomNumberGenerator> rng;

        unique_ptr<PKCS10_Request> load_csr(const QString &csrFile);
        unique_ptr<X509_Certificate> load_ca(const QString &caFile);
        unique_ptr<Private_Key> load_ca_key(const QString &caKeyFile);

        unique_ptr<X509_CA> make_ca(const X509_Certificate &ca, const Private_Key &ca_key);
        const string fetch_password();

        static void file_close(QFile *f);

        inline const static unordered_map<OID, string> oidToHashAlgoMap = {
            {OID({1, 2, 840, 113549, 1, 1, 11}), "SHA-256"}, // SHA256withRSA
            {OID({1, 2, 840, 113549, 1, 1, 12}), "SHA-384"}, // SHA384withRSA
            {OID({1, 2, 840, 113549, 1, 1, 13}), "SHA-256"}, // SHA512withRSA

            {OID({1, 2, 840, 10045, 4, 3, 2}), "SHA-256"}, // ECDSA-with-SHA256
            {OID({1, 2, 840, 10045, 4, 3, 3}), "SHA-384"}, // ECDSA-with-SHA384
            {OID({1, 2, 840, 10045, 4, 3, 4}), "SHA-512"}, // ECDSA-with-SHA512

            {OID({1, 3, 101, 112}), "Pure"}, // Ed25519
            {OID({1, 3, 101, 113}), "Pure"}, // Ed448
        };
    };
} // namespace SignCert
