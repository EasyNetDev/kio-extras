/*
This file is part of KDE

 SPDX-FileCopyrightText: 2000 Waldo Bastian <bastian@kde.org>
 SPDX-FileCopyrightText: 2008 David Faure <faure@kde.org>

SPDX-License-Identifier: MIT
*/

#ifndef __filter_h__
#define __filter_h__

#include <KIO/WorkerBase>

#include <QObject>

class KFilterBase;

class FilterProtocol : /*public QObject, */ public KIO::WorkerBase
{
    //    Q_OBJECT

public:
    FilterProtocol(const QByteArray &protocol, const QByteArray &pool, const QByteArray &app);

    KIO::WorkerResult get(const QUrl &url) override;

private:
    const QString m_protocol;
    KFilterBase *filter;
};

#endif
