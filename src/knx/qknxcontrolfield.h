/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QKNXCONTROLFIELD_H
#define QKNXCONTROLFIELD_H

#include <QtCore/qdebug.h>
#include <QtKnx/qknxbytearray.h>
#include <QtKnx/qtknxglobal.h>

#include <bitset>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxControlField final
{
    Q_GADGET

    static constexpr bool testBit(quint8 byteToTest, quint8 bit) noexcept
    {
        return (byteToTest & (quint8(1) << bit)) != 0;
    }

    // ### Qt6: pass byteToSet as reference
    static constexpr quint8 setBit(quint8 byteToSet, bool value, quint8 bit) noexcept
    {
        return (value ? byteToSet | (quint8(1) << bit) : byteToSet & ~(quint8(1) << bit));
    }

public:
    QKnxControlField() = default;
    explicit QKnxControlField(quint8 data);
    explicit QKnxControlField(const QKnxByteArray &data);

    enum class FrameFormat : quint8
    {
        Extended = 0x00,
        Standard = 0x01
    };
    Q_ENUM(FrameFormat)
    QKnxControlField::FrameFormat frameFormat() const // ### Qt6: Replace byte() with m_ctrl1
        { return static_cast<FrameFormat> (quint8(testBit(byte(), 7))); }
    void setFrameFormat(QKnxControlField::FrameFormat type) // ### Qt6: Replace byte() with m_ctrl1
        { m_ctrl1 = setBit(byte(), bool(static_cast<int> (type)), 7); }

    enum class Repeat : quint8
    {
        Repeat = 0x00,
        DoNotRepeat = 0x01
    };
    Q_ENUM(Repeat)
    QKnxControlField::Repeat repeat() const // ### Qt6: Replace byte() with m_ctrl1
        { return static_cast<Repeat> (quint8(testBit(byte(), 5))); }
    void setRepeat(QKnxControlField::Repeat rep) // ### Qt6: Replace byte() with m_ctrl1
        { m_ctrl1 = setBit(byte(), bool(static_cast<int> (rep)), 5); }


    enum class Broadcast : quint8
    {
        System = 0x00,
        Domain = 0x01
    };
    Q_ENUM(Broadcast)
    QKnxControlField::Broadcast broadcast() const // ### Qt6: Replace byte() with m_ctrl1
        { return static_cast<Broadcast> (quint8(testBit(byte(), 4))); }
    void setBroadcast(QKnxControlField::Broadcast bcst) // ### Qt6: Replace byte() with m_ctrl1
        { m_ctrl1 = setBit(byte(), bool(static_cast<int> (bcst)), 4); }

    enum class Priority : quint8
    {
        System = 0x00,
        Normal = 0x01,
        Urgent = 0x02,
        Low = 0x03
    };
    Q_ENUM(Priority)
    QKnxControlField::Priority priority() const;
    void setPriority(QKnxControlField::Priority priority);

    enum class Acknowledge : quint8
    {
        NotRequested = 0x00,
        Requested = 0x01
    };
    Q_ENUM(Acknowledge)
    QKnxControlField::Acknowledge acknowledge() const // ### Qt6: Replace byte() with m_ctrl1
        { return Acknowledge(quint8(testBit(byte(), 1))); }
    void setAcknowledge(QKnxControlField::Acknowledge ack) // ### Qt6: Replace byte() with m_ctrl1
        { m_ctrl1 = setBit(byte(), bool(static_cast<int> (ack)), 1); }

    enum class Confirm : quint8
    {
        NoError = 0x00,
        Error = 0x01
    };
    Q_ENUM(Confirm)
    QKnxControlField::Confirm confirm() const // ### Qt6: Replace byte() with m_ctrl1
        { return static_cast<Confirm> (quint8(testBit(byte(), 0))); }
    void setConfirm(QKnxControlField::Confirm conf) // ### Qt6: Replace byte() with m_ctrl1
        { m_ctrl1 = setBit(byte(), bool(static_cast<int> (conf)), 0); }

    quint8 byte() const { return quint8(m_ctrl1.to_ulong()); }
    QKnxByteArray bytes() const { return { byte() }; }

    quint8 size() const { return 1; }

    bool operator==(const QKnxControlField &other) const;
    bool operator!=(const QKnxControlField &other) const;

    class Q_KNX_EXPORT Builder
    {
    public:
        Builder &setFrameFormat(QKnxControlField::FrameFormat type);
        Builder &setRepeat(QKnxControlField::Repeat repeat);
        Builder &setBroadcast(QKnxControlField::Broadcast broadcast);
        Builder &setPriority(QKnxControlField::Priority priority);
        Builder &setAcknowledge(QKnxControlField::Acknowledge acknowledge);
        Builder &setConfirm(QKnxControlField::Confirm errorStatus);

        QKnxControlField create() const;

    private:
        QKnxControlField::FrameFormat m_frameFormat { FrameFormat::Standard };
        QKnxControlField::Repeat m_repeat { Repeat::DoNotRepeat };
        QKnxControlField::Broadcast m_broad { Broadcast::Domain };
        QKnxControlField::Priority m_priority { Priority::Low };
        QKnxControlField::Acknowledge m_acknowledge { Acknowledge::NotRequested };
        QKnxControlField::Confirm m_errorStatus { Confirm::NoError };
    };
    static QKnxControlField::Builder builder();

private:
    std::bitset<8> m_ctrl1; // ### Qt6: Replace with quint8
};
Q_KNX_EXPORT QDebug operator<<(QDebug debug, const QKnxControlField &ctrl);

Q_DECLARE_TYPEINFO(QKnxControlField::FrameFormat, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Repeat, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Broadcast, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Priority, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Acknowledge, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QKnxControlField::Confirm, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
