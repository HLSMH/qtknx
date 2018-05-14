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

#include "qknxnetiproutingindication.h"
#include "qknxlinklayerframebuilder.h"
QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpRoutingIndicationProxy

    \inmodule QtKnx
    \brief The QKnxNetIpRoutingIndicationProxy class provides the means to read
    a routing indication from the generic \l QKnxNetIpFrame class and to
    create a KNXnet/IP frame based on the information.

    A routing indication is sent by a KNXnet/IP router or device to transmit KNX
    link layer frames over IP networks.

    The following code sample illustrates how to read the routing indication
    information sent by a KNXnet/IP router or device:

    \code
        auto netIpFrame = QKnxNetIpFrame::fromBytes(...);

        const QKnxNetIpRoutingIndicationProxy routingIndication(netIpFrame);
        if (!routingIndication.isValid())
            return;

        auto linkFrame = routingIndication.linkLayerFrame();
    \endcode

    \sa builder(), QKnxNetIpRoutingLostMessageProxy, QKnxNetIpRoutingBusyProxy
*/

/*!
    \fn QKnxNetIpRoutingIndicationProxy::QKnxNetIpRoutingIndicationProxy()
    \internal
*/

/*!
    \fn QKnxNetIpRoutingIndicationProxy::~QKnxNetIpRoutingIndicationProxy()
    \internal
*/

/*!
    \fn QKnxNetIpRoutingIndicationProxy::QKnxNetIpRoutingIndicationProxy(const QKnxNetIpFrame &&)
    \internal
*/

/*!
    Constructs a proxy object to read the routing indication information carried
    by the specified KNXnet/IP frame \a frame.
*/
QKnxNetIpRoutingIndicationProxy::QKnxNetIpRoutingIndicationProxy(const QKnxNetIpFrame &frame)
    : m_frame(frame)
{}

/*!
    Returns the link layer frame within the routing indication frame.
*/
QKnxLinkLayerFrame QKnxNetIpRoutingIndicationProxy::linkLayerFrame() const
{
    return QKnxLinkLayerFrame::builder()
        .setMedium(QKnx::MediumType::NetIP)
        .setData(m_frame.constData())
        .createFrame();
}

/*!
    Returns \c true if the frame contains initialized values and is in itself
    valid, otherwise returns \c false. A valid KNXnet/IP frame consist of
    at least a valid header and a size in bytes corresponding to the total size
    of the KNXnet/IP frame header.

    \sa QKnxNetIpFrameHeader::totalSize()
*/
bool QKnxNetIpRoutingIndicationProxy::isValid() const
{
    // TODO: fix size check once the minimum CEMI frame size is known
    return m_frame.isValid() && m_frame.size() >= 8
        && m_frame.serviceType() == QKnxNetIp::ServiceType::RoutingIndication;
}

/*!
    Returns a builder object to create a KNXnet/IP routing indication frame.
*/
QKnxNetIpRoutingIndicationProxy::Builder QKnxNetIpRoutingIndicationProxy::builder()
{
    return QKnxNetIpRoutingIndicationProxy::Builder();
}


/*!
    \class QKnxNetIpRoutingIndicationProxy::Builder

    \inmodule QtKnx
    \brief The QKnxNetIpRoutingIndicationProxy::Builder class provides the means
    to create a routing indication frame.

    A routing indication is sent by a KNXnet/IP router or device to transmit KNX
    link layer frames over IP networks.

    The common way to create a routing indication is:

    \code
        QKnxLinkLayerFrame linkFrame = // create a link frame ....;
        auto netIpframe = QKnxNetIpRoutingIndicationProxy::builder()
            .setLinkLayerFrame(linkFrame)
            .create();
    \endcode

    If a KNXnet/IP router or device receives a router busy message from another
    router or device, it immediately stops sending routing indication frames and
    waits until the time specified in the router busy message has elapsed to
    resume sending frames.
*/

/*!
    Sets the link layer frame within the routing indication frame to \a llf and
    returns a reference to the builder.
*/
QKnxNetIpRoutingIndicationProxy::Builder &
    QKnxNetIpRoutingIndicationProxy::Builder::setLinkLayerFrame(const QKnxLinkLayerFrame &llf)
{
    m_llf = llf;
    return *this;
}

/*!
    Creates and returns a KNXnet/IP routing indication frame.

    \note The returned frame may be invalid depending on the values used during
    setup.

    \sa isValid()
*/
QKnxNetIpFrame QKnxNetIpRoutingIndicationProxy::Builder::create() const
{
    return { QKnxNetIp::ServiceType::RoutingIndication, m_llf.bytes() };
}

QT_END_NAMESPACE
