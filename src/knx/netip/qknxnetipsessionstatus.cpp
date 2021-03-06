/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qknxnetipsessionstatus.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpSessionStatusProxy

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpSessionStatusProxy class provides the means to
    introspect secure session status data inside the generic \l QKnxNetIpFrame
    class and to create a KNXnet/IP secure session status frame from provided
    data.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame may be sent by the KNXnet/IP secure server to the KNXnet/IP
    secure client or by the KNXnet/IP secure client to the KNXnet/IP secure
    server at any stage of the secure session handshake to indicate an error
    condition or to convey status information.

    \note When using QKnxNetIpSessionStatusProxy, care must be taken to ensure
    that the referenced KNXnet/IP frame outlives the proxy on all code paths,
    lest the proxy ends up referencing deleted data.

    The following code sample illustrates how to read the session status
    information:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpSessionStatusProxy proxy(netIpFrame);
        if (!proxy.isValid())
            return;

        auto currentStatus = proxy.status();
    \endcode

    \sa builder(), {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn QKnxNetIpSessionStatusProxy::QKnxNetIpSessionStatusProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSessionStatusProxy::~QKnxNetIpSessionStatusProxy()
    \internal
*/

/*!
    \fn QKnxNetIpSessionStatusProxy::QKnxNetIpSessionStatusProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the session status information
    carried by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpSessionStatusProxy::QKnxNetIpSessionStatusProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consists of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \note KNXnet/IP session status frames currently have a fixed size of \c 8
    bytes.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpSessionStatusProxy::isValid() const
{
    return m_frame.isValid() && m_frame.serviceType() == QKnxNetIp::ServiceType::SessionStatus
        && m_frame.size() == 8 && status() <= QKnxNetIp::SecureSessionStatus::Close;
}

/*!
    Returns the session status from the generic KNXnet/IP session status
    frame.
*/
QKnxNetIp::SecureSessionStatus QKnxNetIpSessionStatusProxy::status() const
{
    return QKnxNetIp::SecureSessionStatus(m_frame.constData().value(0, 0xff));
}

/*!
    Returns a builder object to create a KNXnet/IP session status frame.
*/
QKnxNetIpSessionStatusProxy::Builder QKnxNetIpSessionStatusProxy::builder()
{
    return QKnxNetIpSessionStatusProxy::Builder();
}


/*!
    \class QKnxNetIpSessionStatusProxy::Builder

    \inmodule QtKnx
    \inheaderfile QKnxNetIpSessionStatusProxy

    \brief The QKnxNetIpSessionStatusProxy::Builder class provides the
    means to create a KNXnet/IP secure session status frame.

    This class is part of the Qt KNX module and currently available as a
    Technology Preview, and therefore the API and functionality provided
    by the class may be subject to change at any time without prior notice.

    This frame may be sent by the KNXnet/IP secure server to the KNXnet/IP
    secure client or by the KNXnet/IP secure client to the KNXnet/IP secure
    server at any stage of the secure session handshake to indicate an error
    condition or to convey status information.

    The common way to create a session status frame is:

    \code
        auto netIpFrame = QKnxNetIpSessionStatusProxy::builder()
            .setStatus(QKnxNetIp::SecureSessionStatus::Close)
            .create();
    \endcode
*/

class QKnxNetIpSessionStatusBuilderPrivate : public QSharedData
{
public:
    QKnxNetIpSessionStatusBuilderPrivate() = default;
    ~QKnxNetIpSessionStatusBuilderPrivate() = default;

    QKnxNetIp::SecureSessionStatus m_status = QKnxNetIp::SecureSessionStatus::Unknown;
};

/*!
    Creates a new empty session status frame builder object.
*/
QKnxNetIpSessionStatusProxy::Builder::Builder()
    : d_ptr(new QKnxNetIpSessionStatusBuilderPrivate)
{}

/*!
    Destroys the object and frees any allocated resources.
*/
QKnxNetIpSessionStatusProxy::Builder::~Builder() = default;

/*!
    Sets the status of the KNXnet/IP session status frame to \a status
    and returns a reference to the builder.
*/
QKnxNetIpSessionStatusProxy::Builder &
    QKnxNetIpSessionStatusProxy::Builder::setStatus(QKnxNetIp::SecureSessionStatus status)
{
    d_ptr->m_status = status;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP session status frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpSessionStatusProxy::Builder::create() const
{
    if (d_ptr->m_status > QKnxNetIp::SecureSessionStatus::Close)
        return { QKnxNetIp::ServiceType::SessionStatus };

    return { QKnxNetIp::ServiceType::SessionStatus,
        QKnxUtils::QUint16::bytes(quint16(d_ptr->m_status) << 8) };
}

/*!
    Constructs a copy of \a other.
*/
QKnxNetIpSessionStatusProxy::Builder::Builder(const Builder &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxNetIpSessionStatusProxy::Builder &
    QKnxNetIpSessionStatusProxy::Builder::operator=(const Builder &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

QT_END_NAMESPACE
