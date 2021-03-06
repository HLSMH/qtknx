/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include <QtCore/qdebug.h>
#include <QtTest/qtest.h>
#include <QtKnx/qknxnetipconnectionstaterequest.h>
#include <QtKnx/qknxnetip.h>

static QString s_msg;
static void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    s_msg = msg;
}

class tst_QKnxNetIpConnectionStateRequest: public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testDebugStream();
    void testProxyMethods();
};

void tst_QKnxNetIpConnectionStateRequest::testDefaultConstructor()
{
    QKnxNetIpFrame frame;
    QKnxNetIpConnectionStateRequestProxy connectionStateRequest(frame);
    QCOMPARE(connectionStateRequest.isValid(), false);
    QCOMPARE(connectionStateRequest.channelId(), quint8(0));
    QCOMPARE(connectionStateRequest.controlEndpoint().isValid(), false);

    frame = QKnxNetIpConnectionStateRequestProxy::builder().create();
    const QKnxNetIpConnectionStateRequestProxy req(frame);
    QCOMPARE(req.isValid(), false);
}

void tst_QKnxNetIpConnectionStateRequest::testConstructor()
{
    const auto hpai = QKnxNetIpHpaiProxy::builder()
        .setHostAddress(QHostAddress::LocalHost)
        .setPort(3671)
        .create();

    auto frame = QKnxNetIpConnectionStateRequestProxy::builder()
        .setChannelId(255)
        .setControlEndpoint(hpai)
        .create();
    QKnxNetIpConnectionStateRequestProxy connectionStateRequest(frame);

    QCOMPARE(connectionStateRequest.isValid(), true);
    QCOMPARE(frame.size(), quint16(16));
    QCOMPARE(frame.bytes(), QKnxByteArray({ 0x06, 0x10, 0x02, 0x07, 0x00, 0x10, 0xff, 0x00, 0x08,
        0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e, 0x57 }));
    QCOMPARE(frame.data().size(), quint16(10));
    QCOMPARE(frame.data(), QKnxByteArray({ 0xff, 0x00, 0x08, 0x01, 0x7f, 0x00, 0x00, 0x01, 0x0e,
        0x57 }));

    QCOMPARE(connectionStateRequest.channelId(), quint8(255));
    QCOMPARE(connectionStateRequest.controlEndpoint().bytes(), hpai.bytes());
}

void tst_QKnxNetIpConnectionStateRequest::testProxyMethods()
{
    quint8 channelId = 255;
    {
        auto frame = QKnxNetIpConnectionStateRequestProxy::builder()
                     .setChannelId(channelId)
                     .setControlEndpoint(QKnxNetIpHpaiProxy::builder()
                                         .setHostAddress(QHostAddress::LocalHost)
                                         .setPort(3671)
                                         .create())
                     .create();
        QCOMPARE(frame.isValid(), true);
        const QKnxNetIpConnectionStateRequestProxy view(frame);
        QCOMPARE(view.isValid(), true);
        // Setting an invalid service type that should invalidate the proxy
        frame.setServiceType(QKnxNetIp::ServiceType::TunnelingFeatureSet);
        QCOMPARE(view.controlEndpoint().isValid(), true);
        QCOMPARE(view.channelId(), channelId);
        QCOMPARE(view.isValid(), false);
    }
    {
        // Check channelId(), status() and isValid() on a ivalid connection state request
        // with no channelId and status.
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::ConnectionStateRequest };
        const QKnxNetIpConnectionStateRequestProxy view(frame);
        QCOMPARE(frame.header().size(), 6);
        QCOMPARE(frame.dataSize(), 0);
        QCOMPARE(view.isValid(), false);
    }
    {
        // Check channelId(), status() and isValid() on a ivalid connection state request
        // missing control end point bytes.
        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::ConnectionStateRequest,
                                 QKnxByteArray { channelId, 0x00 }
                               };
        const QKnxNetIpConnectionStateRequestProxy view(frame);
        QCOMPARE(frame.header().size(), 6);
        QCOMPARE(frame.dataSize(), 2);
        QCOMPARE(frame.size(), 8);
        QCOMPARE(view.isValid(), false);
    }
    {
        // Valid connection state request
        auto ctrlEndpoint = QKnxNetIpHpaiProxy::builder().create();
        QCOMPARE(ctrlEndpoint.isValid(), true);

        QKnxNetIpFrame frame = { QKnxNetIp::ServiceType::ConnectionStateRequest,
                                 QKnxByteArray { channelId, 0x00 }
                                 + ctrlEndpoint.bytes()
                               };
        const QKnxNetIpConnectionStateRequestProxy view(frame);
        QCOMPARE(view.controlEndpoint().isValid(), true);
        QCOMPARE(view.channelId(), channelId);
        QCOMPARE(frame.size(), 16);
        QCOMPARE(view.isValid(), true);
    }
}

void tst_QKnxNetIpConnectionStateRequest::testDebugStream()
{
    struct DebugHandler
    {
        explicit DebugHandler(QtMessageHandler newMessageHandler)
            : oldMessageHandler(qInstallMessageHandler(newMessageHandler)) {}
        ~DebugHandler() {
            qInstallMessageHandler(oldMessageHandler);
        }
        QtMessageHandler oldMessageHandler;
    } _(myMessageHandler);

    qDebug() << QKnxNetIpConnectionStateRequestProxy::builder().create();
    QCOMPARE(s_msg, QString::fromLatin1("0x0610020700080000"));

    qDebug() << QKnxNetIpConnectionStateRequestProxy::builder()
        .setChannelId(255)
        .setControlEndpoint(QKnxNetIpHpaiProxy::builder()
            .setHostAddress(QHostAddress::LocalHost)
            .setPort(3671).create())
        .create();
    QCOMPARE(s_msg, QString::fromLatin1("0x061002070010ff0008017f0000010e57"));
}


QTEST_APPLESS_MAIN(tst_QKnxNetIpConnectionStateRequest)

#include "tst_qknxnetipconnectionstaterequest.moc"
