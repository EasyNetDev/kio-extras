/*
    This file is part of the MTP KIOD module, part of the KDE project.

    SPDX-FileCopyrightText: 2018 Andreas Krutzler <andreas.krutzler@gmx.net>
    SPDX-FileCopyrightText: 2022 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mtpdevice.h"

#include <QDBusConnection>

#include <Solid/Device>
#include <Solid/DeviceNotifier>
#include <Solid/GenericInterface>

#include "kiod_kmtpd_debug.h"
#include "mtpstorage.h"

// D-Bus adaptors
#include "deviceadaptor.h"

/**
 * Creates a Cached Device that has a predefined lifetime (default: 10000 msec)s
 * The lifetime is reset every time the device is accessed. After it expires it
 * will be released.
 *
 * @param device The LIBMTP_mtpdevice_t pointer to cache
 * @param udi The UDI of the new device to cache
 */
MTPDevice::MTPDevice(const QString &dbusObjectPath, LIBMTP_mtpdevice_t *device, LIBMTP_raw_device_t *rawdevice, const QString &udi, QObject *parent)
    : QObject(parent)
    , m_dbusObjectName(dbusObjectPath)
    , m_mtpdevice(device)
    , m_rawdevice(*rawdevice)
    , m_udi(udi)
    , m_devicesUpdated(true)
{
    const char *deviceName = LIBMTP_Get_Friendlyname(device);
    const char *deviceModel = LIBMTP_Get_Modelname(device);

    // prefer friendly devicename over model
    if (!deviceName || strlen(deviceName) == 0) {
        m_friendlyName = QString::fromUtf8(deviceModel);
    } else {
        m_friendlyName = QString::fromUtf8(deviceName);
    }

    qCDebug(LOG_KIOD_KMTPD) << "Created device " << m_friendlyName << "  with udi=" << udi;

    new DeviceAdaptor(this);
    QDBusConnection::sessionBus().registerObject(m_dbusObjectName, this);

    int index = 0;
    for (LIBMTP_devicestorage_t *storage = device->storage; storage != nullptr; storage = storage->next) {
        m_storages.append(new MTPStorage(QStringLiteral("%1/storage%2").arg(m_dbusObjectName).arg(index++), storage, this));
    }
}

MTPDevice::~MTPDevice()
{
    qCDebug(LOG_KIOD_KMTPD) << "release device:" << m_friendlyName;
    LIBMTP_Release_Device(m_mtpdevice);
}

void MTPDevice::setDevicesUpdatedStatus(bool value)
{
    m_devicesUpdated = value;
}

bool MTPDevice::devicesUpdated() const
{
    return m_devicesUpdated;
}

LIBMTP_mtpdevice_t *MTPDevice::getDevice()
{
    return m_mtpdevice;
}

QString MTPDevice::dbusObjectName() const
{
    return m_dbusObjectName;
}

QString MTPDevice::udi() const
{
    return m_udi;
}

QString MTPDevice::friendlyName() const
{
    return m_friendlyName;
}

int MTPDevice::setFriendlyName(const QString &friendlyName)
{
    if (m_friendlyName == friendlyName) {
        return 1;
    }

    const int result = LIBMTP_Set_Friendlyname(m_mtpdevice, friendlyName.toUtf8().constData());
    if (!result) {
        m_friendlyName = friendlyName;
        Q_EMIT friendlyNameChanged(m_friendlyName);
    }
    return result;
}

QList<QDBusObjectPath> MTPDevice::listStorages()
{
    QList<QDBusObjectPath> list;
    list.reserve(m_storages.count());
    for (const MTPStorage *storage : m_storages) {
        list.append(QDBusObjectPath(storage->dbusObjectPath()));
    }

    // New storages have been collected. No new changes anymore.
    setDevicesUpdatedStatus(false);

    return list;
}

QUrl MTPDevice::url() const
{
    QUrl friendlyUrl;
    friendlyUrl.setScheme(QStringLiteral("mtp"));
    friendlyUrl.setPath(QLatin1Char('/') + friendlyName());
    return friendlyUrl;
}

#include "moc_mtpdevice.cpp"
