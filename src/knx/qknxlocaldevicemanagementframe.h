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

#ifndef QKNXLOCALDEVICEMANAGEMENTFRAME_H
#define QKNXLOCALDEVICEMANAGEMENTFRAME_H

#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxinterfaceobjectproperty.h>
#include <QtKnx/qknxinterfaceobjecttype.h>
#include <QtKnx/qknxnetippayload.h>
#include <QtKnx/qknxnamespace.h>
#include <QtKnx/qknxutils.h>

QT_BEGIN_NAMESPACE
using  QKnxLocalDeviceManagementPayLoad = QKnxNetIpPayload;// TODO remove the QKnxNetIpPayLoad dependency
using  QKnxLocalDeviceManagementPayLoadRef = QKnxByteStoreRef;

class Q_KNX_EXPORT QKnxLocalDeviceManagementFrame final
{
    friend class QKnxLocalDeviceManagementFrameFactory;
    Q_GADGET

public:
    enum class MessageCode : quint8
    {
        Unknown = 0x00,
        // Device Management
        PropertyReadRequest = 0xfc,                     // M_PropRead.req
        PropertyReadConfirmation = 0xfb,                // M_PropRead.con

        PropertyWriteRequest = 0xf6,                    // M_PropWrite.req
        PropertyWriteConfirmation = 0xf5,               // M_PropWrite.con

        PropertyInfoIndication = 0xf7,                  // M_PropInfo.ind

        FunctionPropertyCommandRequest = 0xf8,          // M_FuncPropCommand.req
        FunctionPropertyStateReadRequest = 0xf9,        // M_FuncPropStateRead.req
        FunctionPropertyCommandConfirmation = 0xfa,     // M_FuncPropCommand.con
        FunctionPropertyStateReadConfirmation = 0xfa,   // M_FuncPropStateRead.con

        ResetRequest = 0xf1,                            // M_Reset.req
        ResetIndication = 0xf0,                         // M_Reset.ind
    };
    Q_ENUM(MessageCode)
    MessageCode messageCode() const;
    void setMessageCode(MessageCode code);

    QKnxLocalDeviceManagementFrame() = default;
    ~QKnxLocalDeviceManagementFrame() = default;

    explicit QKnxLocalDeviceManagementFrame(MessageCode code);
    QKnxLocalDeviceManagementFrame(const QKnxLocalDeviceManagementFrame &other);

    quint16 size() const;
    QString toString() const;
    bool isValid() const;
    bool isNegativeConfirmation() const;

    QKnxInterfaceObjectType objectType() const;
    void setObjectType(QKnxInterfaceObjectType type);

    quint8 objectInstance() const;
    void setObjectInstance(quint8 instance);

    QKnxInterfaceObjectProperty property() const;
    void setProperty(QKnxInterfaceObjectProperty pid);

    quint8 numberOfElements() const;
    void setNumberOfElements(quint8 count);

    quint16 startIndex() const;
    void setStartIndex(quint16 index);

    QKnx::CemiServer::Error error() const;
    void setError(QKnx::CemiServer::Error error);

    QKnx::CemiServer::ReturnCode returnCode() const;
    void setReturnCode(QKnx::CemiServer::ReturnCode code);

    QKnxLocalDeviceManagementPayLoad serviceInformation() const;
    QKnxLocalDeviceManagementPayLoadRef serviceInformationRef(quint16 index = 0) const;

    QKnxByteArray bytes() const
    {
        return QKnxByteArray { quint8(m_code) } + m_serviceInformation.ref().bytes(0);
    }

    static QKnxLocalDeviceManagementFrame fromBytes(const QKnxByteArray &type, quint16 index,
        quint16 size)
    {
        if (type.size() < 1)
            return {};

        QKnxLocalDeviceManagementPayLoad payload;
        auto begin = std::next(std::begin(type), index);
        payload.setBytes(std::next(begin, 1), std::next(begin, size));
        return { MessageCode(QKnxUtils::QUint8::fromBytes(type, index)), payload };
    }

    QKnxByteArray data() const
    {
        return serviceInformationRef(6).bytes(0);
    }

    void setData(const QKnxByteArray &newData)
    {
        auto sf = serviceInformation();
        sf.resize(6);
        sf.appendBytes(newData);
        setServiceInformation(sf);
    }

protected:
    QKnxLocalDeviceManagementFrame(QKnxLocalDeviceManagementFrame::MessageCode messageCode,
        const QKnxLocalDeviceManagementPayLoad &payload);
    void setServiceInformation(const QKnxLocalDeviceManagementPayLoad &serviceInformation);

private:
    QKnxLocalDeviceManagementFrame(MessageCode code, QKnxInterfaceObjectType type,
            quint8 instance, QKnxInterfaceObjectProperty pid, quint8 noe, quint16 index,
            const QKnxByteArray &payload = {})
        : QKnxLocalDeviceManagementFrame(code)
    {
        auto si = serviceInformation();
        si.setBytes(QKnxUtils::QUint16::bytes(quint16(type)));
        si.setByte(2, instance);
        si.setByte(3, pid);
        si.replaceBytes(4, QKnxUtils::QUint16::bytes((quint16(noe) << 12) | index));
        si.appendBytes(payload);
        setServiceInformation(si);
    }

    QKnxLocalDeviceManagementFrame(MessageCode code, QKnxInterfaceObjectType type,
            quint8 instance, QKnxInterfaceObjectProperty pid, const QKnxByteArray &payload = {})
        : QKnxLocalDeviceManagementFrame(code)
    {
        auto si = serviceInformation();
        si.setBytes(QKnxUtils::QUint16::bytes(quint16(type)));
        si.setByte(2, instance);
        si.setByte(3, pid);
        si.appendBytes(payload);
        setServiceInformation(si);
    }

private:
    // TODO: d_ptr
    MessageCode m_code;
    QKnxLocalDeviceManagementPayLoad m_serviceInformation;
};

QT_END_NAMESPACE

#endif
