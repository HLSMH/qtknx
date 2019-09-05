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

#include "qknxdevicemanagementframe.h"
#include "qknxdevicemanagementframefactory.h"
#include "qknxutils.h"

QT_BEGIN_NAMESPACE

class QKnxDeviceManagementFramePrivate : public QSharedData
{
public:
    QKnxByteArray m_serviceInformation;
    QKnxDeviceManagementFrame::MessageCode m_code { QKnxDeviceManagementFrame
        ::MessageCode::Unknown };
};

/*!
    \class QKnxDeviceManagementFrame

    \inmodule QtKnx
    \ingroup qtknx-device-management

    \brief The QKnxDeviceManagementFrame class represents a cEMI frame dedicated
    to KNXnet/IP device management.

    A device management connection is established between a common external
    message interface (cEMI) client and a cEMI server to transmit cEMI device
    management frames.
    A cEMI frame specifies the service to use and the \l MessageCode that
    determines whether the frame carries a request, confirmation, or indication
    related to the selected service.

    The following services are available:

    \list
        \li Property read (\c {M_PropRead.req} and \c {M_PropRead.con})
        \li Property write (\c {M_PropWrite.req} and \c {M_PropWrite.con})
        \li Property info indication service (\c {M_PropInfo.ind})
        \li Function property command (\c {M_FuncPropCommand.req} and
            \c {M_FuncPropCommand.con})
        \li Function property state read (\c {M_FuncPropStateRead.req} and
            \c {M_FuncPropStateRead.con})
        \li Reset and restart service (\c {M_Reset.req})
    \endlist

    Application layer services are invoked by the transport layer frames
    \e {request (.req)}, \e {indication (.ind)}, and \e {confirmation (.con)}.
    The same frames are used by the remote device to respond to a remote
    confirmed service.

    Typically, a device management frame contains at least the following
    arguments:

    \list
        \li The code describing the service to use,
            \l QKnxDeviceManagementFrame::MessageCode.
        \li The type of the interface object holding the property to access,
            \l QKnxInterfaceObjectType.
        \li The instance of this interface object, because it is possible to
            have more than one realization of an interface object in a given
            device.
        \li The property of the interface object to access,
           \l QKnxInterfaceObjectProperty.
    \endlist

    The \l QKnxDeviceManagementFrame::Builder class can be used to construct
    local device management cEMI frames. The following more specialized versions
    of the builder are also provided, and it is recommended to prefer them over
    the generic version:

    \list
        \li \l QKnxDeviceManagementFrame::PropertyReadBuilder
        \li \l QKnxDeviceManagementFrame::PropertyWriteBuilder
        \li \l QKnxDeviceManagementFrame::PropertyInfoBuilder
        \li \l QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder
        \li \l QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder
        \li \l QKnxDeviceManagementFrame::ResetBuilder
    \endlist

    \sa {Qt KNX Device Management Classes}
*/

/*!
    \enum QKnxDeviceManagementFrame::MessageCode

    This enum holds the message codes of a local device management frame.

    \value Unknown                          Unknown message code, do not use.
    \value PropertyReadRequest
           Message code for a property read request frame, \c {M_PropRead.req}.
    \value PropertyReadConfirmation
           Message code for a property read confirmation frame,
           \c {M_PropRead.con}.
    \value PropertyWriteRequest
           Message code for a property write request frame,
           \c {M_PropWrite.req}.
    \value PropertyWriteConfirmation
           Message code for a property write confirmation frame,
           \c {M_PropWrite.con}.
    \value PropertyInfoIndication
           Message code for a property info indication frame,
           \c {M_PropInfo.ind}.
    \value FunctionPropertyCommandRequest
           Message code for a function property command frame,
           \c {M_FuncPropCommand.req}.
    \value FunctionPropertyStateReadRequest
           Message code for a function property state read request frame,
           \c {M_FuncPropStateRead.req}.
    \value FunctionPropertyCommandConfirmation
           Message code for a function property command confirmation frame,
           \c {M_FuncPropCommand.con}.
    \value FunctionPropertyStateReadConfirmation
           Message code for a function property state read confirmation frame,
           \c {M_FuncPropStateRead.con}.
    \value ResetRequest
           Message code for a reset request frame, \c {M_Reset.req}.
    \value ResetIndication
           Message code for a reset indication frame, \c {M_Reset.ind}.
*/

/*!
    Constructs an empty invalid local device management frame.
*/
QKnxDeviceManagementFrame::QKnxDeviceManagementFrame()
    : d_ptr(new QKnxDeviceManagementFramePrivate)
{}

/*!
    Destroys the local device management frame and frees all allocated resources.
*/
QKnxDeviceManagementFrame::~QKnxDeviceManagementFrame()
{}

/*!
    Constructs an empty local device management frame with the message code set
    to \a code.
*/
QKnxDeviceManagementFrame::QKnxDeviceManagementFrame(MessageCode code)
    : d_ptr(new QKnxDeviceManagementFramePrivate)
{
    d_ptr->m_code = code;
    if (code != MessageCode::ResetRequest && code != MessageCode::ResetIndication)
        d_ptr->m_serviceInformation = { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
}

/*!
    Constructs a local device management frame with the message code set to
    \a code and service information set to \a serviceInfo.
*/
QKnxDeviceManagementFrame::QKnxDeviceManagementFrame(
        QKnxDeviceManagementFrame::MessageCode code, const QKnxByteArray &serviceInfo)
    : d_ptr(new QKnxDeviceManagementFramePrivate)
{
    d_ptr->m_code = code;
    d_ptr->m_serviceInformation = serviceInfo;
}

/*!
    Returns \c true if this is a default constructed device management frame,
    otherwise returns \c false. A frame is considered null if it contains no
    initialized values.
*/
bool QKnxDeviceManagementFrame::isNull() const
{
    return d_ptr->m_code == MessageCode::Unknown && d_ptr->m_serviceInformation.isNull();
}

/*!
    Returns \c true if the local device management frame is valid, returns
    \c false otherwise.
*/
bool QKnxDeviceManagementFrame::isValid() const
{
    switch (messageCode()) {
    case MessageCode::PropertyReadRequest:
        if (size() != 7) // 4.1.7.3.2 The request shall not contain any further data.
            return false;
        break;
    case MessageCode::PropertyReadConfirmation:  // 4.1.7.3.3
    case MessageCode::PropertyWriteRequest:      // 4.1.7.3.4
        if (size() < 8)     // The frame shall contain further data, at minimum one quint8.
            return false;   // For 4.1.7.3.3 it shall be (NoE) or error code (negative response).
        break;
    case MessageCode::PropertyWriteConfirmation:
        if ((numberOfElements() >= 1 && size() != 7) || (numberOfElements() == 0 && size() != 8))
            return false; // 4.1.7.3.5 size == (write request - data) or negative confirmation
        break;
    case MessageCode::PropertyInfoIndication:
        if (size() < 7)
            return false;
        break;
    case MessageCode::FunctionPropertyCommandRequest:
    case MessageCode::FunctionPropertyStateReadRequest:
        if (size() < 6)
            return false;
        break;
    case MessageCode::FunctionPropertyCommandConfirmation:
//  case MessageCode::FunctionPropertyStateReadConfirmation: (same value as above)
        if (size() < 5)     // 4.1.7.4.5 Error and exception handling for cEMI Function Properties
            return false;   // return code and data are omitted on error.
        break;
    case MessageCode::ResetRequest:
    case MessageCode::ResetIndication:
        return size() == 1;
    default:
        return false;
    }

    auto type = objectType();
    if (!QKnxInterfaceObjectType::isObjectType(type))
        return false;

    if (objectInstance() < 1)
        return false;

    if (!QKnxInterfaceObjectType::isMatch(type, property()))
        return false;
    return true; // TODO: Find other definitions of validity.
}

/*!
    Returns the number of bytes of the local device management frame.
*/
quint16 QKnxDeviceManagementFrame::size() const
{
    return d_ptr->m_serviceInformation.size() + 1 /* message code */;
}

/*!
    Returns \c true if this local device management frame is a negative
    confirmation; otherwise returns \c false.

    For property read or write confirmation frames, the data field of a
    negative confirmation contains the error information.

    For function property command or function property state read confirmation
    frames, neither a return code nor data are transmitted with the frame.

    \sa data(), error(), returnCode()
*/
bool QKnxDeviceManagementFrame::isNegativeConfirmation() const
{
    switch (messageCode()) {
    case MessageCode::PropertyReadConfirmation:
    case MessageCode::PropertyWriteConfirmation:
        // 4.1.7.3.3 Property read / 4.1.7.3.5 Property write confirmation
        // The confirmation indicates an error with number of elements == 0.
        return numberOfElements() == 0;

    //case MessageCode::FunctionPropertyStateReadConfirmation:
    case MessageCode::FunctionPropertyCommandConfirmation:
        // 4.1.7.4.5 The confirmation indicates error by omitting return code
        return size() == 5; // and data
    default:
        break;
    }
    return false;
}

/*!
    Returns the message code of this local device management frame.
*/
QKnxDeviceManagementFrame::MessageCode QKnxDeviceManagementFrame::messageCode() const
{
    return d_ptr->m_code;
}

/*!
    Sets the message code of this local device management frame to \a code.
*/
void QKnxDeviceManagementFrame::setMessageCode(QKnxDeviceManagementFrame::MessageCode code)
{
    d_ptr->m_code = code;
}

/*!
    Returns the object type carried by the frame if available;
    otherwise returns \l QKnxInterfaceObjectType::Invalid.
*/
QKnxInterfaceObjectType QKnxDeviceManagementFrame::objectType() const
{
    if (d_ptr->m_serviceInformation.size() >= 2)
        return QKnxUtils::QUint16::fromBytes(d_ptr->m_serviceInformation);
    return QKnxInterfaceObjectType::Invalid;
}

/*!
    Sets the interface object type to \a type if the argument is a valid;
    otherwise does nothing.
*/
void QKnxDeviceManagementFrame::setObjectType(QKnxInterfaceObjectType type)
{
    if (QKnxInterfaceObjectType::isObjectType(type))
        d_ptr->m_serviceInformation.replace(0, 2, QKnxUtils::QUint16::bytes(quint16(type)));
}

/*!
    Returns the object instance if available; otherwise returns \c 0.
*/
quint8 QKnxDeviceManagementFrame::objectInstance() const
{
    return d_ptr->m_serviceInformation.value(2);
}

/*!
    Sets the object instance to \a instance if the argument is valid; does
    nothing otherwise.

    \note The range for the object instance is from \c 1 to \c 255.
*/
void QKnxDeviceManagementFrame::setObjectInstance(quint8 instance)
{
    if (instance < 1)
        return;
    d_ptr->m_serviceInformation.resize(3);
    d_ptr->m_serviceInformation.set(2, instance);
}

/*!
    Returns the interface object property if available; otherwise returns
    \l QKnxInterfaceObjectProperty::Invalid.
*/
QKnxInterfaceObjectProperty QKnxDeviceManagementFrame::property() const
{
    if (d_ptr->m_serviceInformation.size() >= 4)
        return d_ptr->m_serviceInformation.at(3);
    return QKnxInterfaceObjectProperty::Invalid;
}

/*!
    Sets the interface object property identifier to \a pid if the argument is
    valid; otherwise does nothing.
*/
void QKnxDeviceManagementFrame::setProperty(QKnxInterfaceObjectProperty pid)
{
    if (QKnxInterfaceObjectProperty::isProperty(pid)) {
        d_ptr->m_serviceInformation.resize(4);
        d_ptr->m_serviceInformation.set(3, quint8(pid));
    }
}

/*!
    Returns the number of elements for an array-structured property. A return
    value of \c 0 indicates a negative response.

    \sa isNegativeConfirmation()
*/
quint8 QKnxDeviceManagementFrame::numberOfElements() const
{
    return quint8(d_ptr->m_serviceInformation.value(4) & 0xf0) >> 4;
}

/*!
    Sets the number of elements for an array-structured property to
    \a numOfElements. A value of \c 0 is used to indicate a negative response.

    \note The range for the number of elements is from \c 0 to \c 15.
*/
void QKnxDeviceManagementFrame::setNumberOfElements(quint8 numOfElements)
{
    if (numOfElements > 0x0f)
        return;

    d_ptr->m_serviceInformation.resize(5);
    d_ptr->m_serviceInformation.set(4,
        (d_ptr->m_serviceInformation.value(4) & 0x0f) | (numOfElements << 4));
}

/*!
    Returns the start index within an array-structured property if available;
    otherwise returns \c 0.
*/
quint16 QKnxDeviceManagementFrame::startIndex() const
{
    return QKnxUtils::QUint16::fromBytes(d_ptr->m_serviceInformation, 4) & 0x0fff;
}

/*!
    Sets the start \a index within an array-structured property. The first
    element is placed at the index \c 1. The array element \c 0 contains the
    current number of valid array elements.

    \note The range for the number of elements is from \c 0 to \c 4095.
*/
void QKnxDeviceManagementFrame::setStartIndex(quint16 index)
{
    if (index > 0x0fff)
        return;

    d_ptr->m_serviceInformation.replace(4, 2,
        QKnxUtils::QUint16::bytes((quint16(numberOfElements()) << 12) | index));
}

/*!
    Returns the data field of the local device management frame as
    an array of bytes.

    The length of the data depends on the property datatype of the property
    and in case of an array-structured property value also on the number of
    array elements that are accessed.

    In case of an error, the data field stores the error code as a single byte.
*/
QKnxByteArray QKnxDeviceManagementFrame::data() const
{
    return d_ptr->m_serviceInformation.mid(6);
}

/*!
    Sets the data field of the local device management frame to \a newData.
    No validation checks are done on the function argument.

    \sa isValid()
*/
void QKnxDeviceManagementFrame::setData(const QKnxByteArray &newData)
{
    d_ptr->m_serviceInformation.resize(6);
    d_ptr->m_serviceInformation += newData;
}

/*!
    Returns the error code stored in the local device management frame if
    the frame is a \l PropertyReadConfirmation or \l PropertyWriteConfirmation
    frame and the number of elements is set to \c 0. Otherwise returns \c None.

    \sa data(), isNegativeConfirmation()
*/
QKnxNetIpCemiServer::Error QKnxDeviceManagementFrame::error() const
{
    switch (messageCode()) {
    case MessageCode::PropertyReadConfirmation:
    case MessageCode::PropertyWriteConfirmation:
        if (numberOfElements() == 0) {
            auto err = data();
            if (err.size() < 1)
                return QKnxNetIpCemiServer::Error::Unspecified;
            return QKnxNetIpCemiServer::Error(err.at(0));
        }
    default:
        break;
    }
    return QKnxNetIpCemiServer::Error::None;
}

/*!
    Sets the error code stored in the frame to \a error if the frame is a
    \l PropertyReadConfirmation or \l PropertyWriteConfirmation frame; does
    nothing otherwise.
*/
void QKnxDeviceManagementFrame::setError(QKnxNetIpCemiServer::Error error)
{
    // Set error code on confirmed messages only. See paragraph 4.1.7.3.7.1
    switch (messageCode()) {
    case MessageCode::PropertyReadConfirmation:
    case MessageCode::PropertyWriteConfirmation: {
        d_ptr->m_serviceInformation.resize(7);
        d_ptr->m_serviceInformation.set(6, quint8(error));
    }
    default:
        break;
    }
}

/*!
    Returns the code set by the cEMI server after a cEMI function property
    service request if the frame is a \l FunctionPropertyCommandConfirmation
    or \l FunctionPropertyStateReadConfirmation and is also not a negative
    confirmation. Otherwise returns \c NoError.

    \note A return code larger than \c NoError indicates the negative result of
    a function property command or function property state read execution.

    \sa data(), isNegativeConfirmation()
*/
QKnxNetIpCemiServer::ReturnCode QKnxDeviceManagementFrame::returnCode() const
{
    switch (messageCode()) {
    //case MessageCode::FunctionPropertyStateReadConfirmation:
    case MessageCode::FunctionPropertyCommandConfirmation:
        if (size() >= 6)
            return QKnxNetIpCemiServer::ReturnCode(d_ptr->m_serviceInformation.value(5));
    default:
        break;
    }
    return QKnxNetIpCemiServer::ReturnCode::NoError;
}

/*!
    Sets the return code stored in the frame to \a code if the frame is a
    \l FunctionPropertyCommandConfirmation or
    \l FunctionPropertyStateReadConfirmation frame; does nothing otherwise.
*/
void QKnxDeviceManagementFrame::setReturnCode(QKnxNetIpCemiServer::ReturnCode code)
{
    switch (messageCode()) {
    //case MessageCode::FunctionPropertyStateReadConfirmation:
    case MessageCode::FunctionPropertyCommandConfirmation:
        break;
    default:
        return;
    }

    d_ptr->m_serviceInformation.resize(6);
    d_ptr->m_serviceInformation.set(5, quint8(code));
}

/*!
    Returns the service information of the local device management frame as
    an array of bytes.
*/
QKnxByteArray QKnxDeviceManagementFrame::serviceInformation() const
{
    return d_ptr->m_serviceInformation;
}

/*!
    Sets the service information of the local device management frame to
    \a serviceInfo. No validation checks are done on the function argument.

    \sa isValid()
*/
void QKnxDeviceManagementFrame::setServiceInformation(const QKnxByteArray &serviceInfo)
{
    d_ptr->m_serviceInformation = serviceInfo;
}

/*!
    Returns an array of bytes that represent the local device management frame.
*/
QKnxByteArray QKnxDeviceManagementFrame::bytes() const
{
    return QKnxByteArray { quint8(d_ptr->m_code) } + d_ptr->m_serviceInformation;
}

/*!
    Constructs the local device management frame from the byte array \a data
    starting at the position \a index inside the array with the size \a size.
*/
QKnxDeviceManagementFrame QKnxDeviceManagementFrame::fromBytes(const QKnxByteArray &data,
    quint16 index, quint16 size)
{
    if (data.size() < 1)
        return {};
    return { MessageCode(data.at(index)), data.mid(index + 1, size - 1) };
}

/*!
    Constructs a copy of \a other.
*/
QKnxDeviceManagementFrame::QKnxDeviceManagementFrame(const QKnxDeviceManagementFrame &other)
    : d_ptr(other.d_ptr)
{}

/*!
    Assigns the specified \a other to this object.
*/
QKnxDeviceManagementFrame &
    QKnxDeviceManagementFrame::operator=(const QKnxDeviceManagementFrame &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Move-constructs an object instance, making it point to the same object that
    \a other was pointing to.
*/
QKnxDeviceManagementFrame::QKnxDeviceManagementFrame(QKnxDeviceManagementFrame &&other) Q_DECL_NOTHROW
    : d_ptr(other.d_ptr)
{
    other.d_ptr = Q_NULLPTR;
}

/*!
    Move-assigns \a other to this object instance.
*/
QKnxDeviceManagementFrame &
    QKnxDeviceManagementFrame::operator=(QKnxDeviceManagementFrame &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Swaps \a other with this object. This operation is very fast and never fails.
*/
void QKnxDeviceManagementFrame::swap(QKnxDeviceManagementFrame &other) Q_DECL_NOTHROW
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/
bool QKnxDeviceManagementFrame::operator==(const QKnxDeviceManagementFrame &other) const
{
    return d_ptr == other.d_ptr
        || (d_ptr->m_code == other.d_ptr->m_code
            && d_ptr->m_serviceInformation == other.d_ptr->m_serviceInformation);
}

/*!
    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/
bool QKnxDeviceManagementFrame::operator!=(const QKnxDeviceManagementFrame &other) const
{
    return !operator==(other);
}

/*!
    Returns an instance of a generic device management frame builder.
*/
QKnxDeviceManagementFrame::Builder QKnxDeviceManagementFrame::builder()
{
    return QKnxDeviceManagementFrame::Builder();
}

/*!
    Returns an instance of a device management property read frame builder.
*/
QKnxDeviceManagementFrame::PropertyReadBuilder QKnxDeviceManagementFrame::propertyReadBuilder()
{
    return QKnxDeviceManagementFrame::PropertyReadBuilder();
}

/*!
    Returns an instance of a device management property write frame builder.
*/
QKnxDeviceManagementFrame::PropertyWriteBuilder QKnxDeviceManagementFrame::propertyWriteBuilder()
{
    return QKnxDeviceManagementFrame::PropertyWriteBuilder();
}

/*!
    Returns an instance of a device management property info frame builder.
*/
QKnxDeviceManagementFrame::PropertyInfoBuilder QKnxDeviceManagementFrame::propertyInfoBuilder()
{
    return QKnxDeviceManagementFrame::PropertyInfoBuilder();
}

/*!
    Returns an instance of a device management function property command frame
    builder.
*/
QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder
    QKnxDeviceManagementFrame::functionPropertyCommandBuilder()
{
    return QKnxDeviceManagementFrame::FunctionPropertyCommandBuilder();
}

/*!
    Returns an instance of a device management function property state read
    frame builder.
*/
QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder
    QKnxDeviceManagementFrame::functionPropertyStateReadBuilder()
{
    return QKnxDeviceManagementFrame::FunctionPropertyStateReadBuilder();
}

/*!
    Returns an instance of a device management reset frame builder.
*/
QKnxDeviceManagementFrame::ResetBuilder QKnxDeviceManagementFrame::resetBuilder()
{
    return QKnxDeviceManagementFrame::ResetBuilder();
}

/*!
    \internal
*/
QKnxDeviceManagementFrame::QKnxDeviceManagementFrame(MessageCode code,
        QKnxInterfaceObjectType type, quint8 instance, QKnxInterfaceObjectProperty pid,
        const QKnxByteArray &payload)
    : QKnxDeviceManagementFrame(code)
{
    d_ptr->m_serviceInformation = QKnxUtils::QUint16::bytes(quint16(type));
    d_ptr->m_serviceInformation.append(instance);
    d_ptr->m_serviceInformation.append(pid);
    d_ptr->m_serviceInformation + payload;
}

/*!
    \relates QKnxDeviceManagementFrame

    Writes the local device management frame \a frame to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxDeviceManagementFrame &frame)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << frame.bytes().toHex();
}

QT_END_NAMESPACE
