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

#include "qknxnetipstruct.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxNetIpHpai
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpHpai class stores the address information required to
    uniquely identify a communication channel on the host protocol.

    The size of a KNXnet/IP host protocol address information (HPAI) structure
    varies depending on the host protocol.

    \sa QKnxNetIpConnectResponseProxy, QKnxNetIpHpaiProxy,
        {Qt KNXnet/IP Connection Classes}
*/

/*!
    \class QKnxNetIpCri
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpCri class stores the additional information needed by
    communication channels to fulfill a connection request.

    A KNXnet/IP connection request information (CRI) structure encapsulates two
    substructures that contain both host protocol independent data and host
    protocol dependent data.

    \sa QKnxNetIpConnectResponseProxy, QKnxNetIpCriProxy,
        {Qt KNXnet/IP Connection Classes}
*/

/*!
    \class QKnxNetIpCrd
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpCrd class stores the data block returned with a
    KNXnet/IP connection request frame.

    A KNXnet/IP connection response data (CRD) structure encapsulates two
    substructures that contain both host protocol independent data and host
    protocol dependent data.

    \sa QKnxNetIpConnectResponseProxy, QKnxNetIpCrdProxy,
        {Qt KNXnet/IP Connection Classes}
*/

/*!
    \class QKnxNetIpDib
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpDib class stores a specific block of device information
    that is used when responding to a KNXnet/IP description request.

    At least two KNXnet/IP description information block (DIB) structures are
    returned in response to \l QKnxNetIpDescriptionRequestProxy. They contain
    information about the device capabilities on device hardware and the
    supported service families. More than two DIB structures may be returned
    in one \l QKnxNetIpDescriptionResponseProxy frame.

    The first octet of each DIB contains the length of the DIB structure.
    The second octet declares the DIB structure type. Then the actual data of
    the DIB follows.

    \sa QKnxNetIpConfigDibProxy, QKnxNetIpCurrentConfigDibProxy,
        QKnxNetIpDeviceDibProxy, QKnxNetIpKnxAddressesDibProxy,
        QKnxNetIpManufacturerDibProxy, QKnxNetIpServiceFamiliesDibProxy,
        {Qt KNXnet/IP Connection Classes}
*/

/*!
    \class QKnxNetIpSrp

    \since 5.12
    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpSrp class stores an extended search request parameter
    (SRP).

    A KNXnet/IP client may include zero or more search request parameter
    blocks (SRP) to transfer additional information regarding the search.

    A KNXnet/IP server interprets SRPs using these rules: if a KNXnet/IP server
    supports the SRP, it applies the selection criteria as specified for the
    specific SRP type. Otherwise, if it is not supported and the M (Mandatory)
    bit is set, the server shall not respond to the search request. Moreover,
    if it is not supported and the M bit is not set, the server
    ignores this SRP and react as if this SRP were not present. If the
    evaluation of any of the SRPs leads to the decision to not respond to the
    search request, then the following SRPs are skipped.

    A KNXnet/IP client cannot use an invalid SRP type code \c 00h. The KNXnet/IP
    server handles an SRP with the invalid code \c 00h as any other unknown SRP
    code.

    \sa QKnxNetIpSearchRequestProxy
*/

/*!
    \class QKnxNetIpStruct

    \inmodule QtKnx
    \ingroup qtknx-netip

    \brief The QKnxNetIpStruct class represents a generic data structure
    encapsulated in a KNXnet/IP frame.

    The following types of KNXnet/IP structures can be encapsulated in
    \l QKnxNetIpFrame: \l QKnxNetIpHpai for host protocol address information
    (HPAI), \l QKnxNetIpCri for connection request information (CRI),
    \l QKnxNetIpCrd for connection response data (CRD) and \l QKnxNetIpDib for
    a description information block (DIB).

    \sa {Qt KNXnet/IP Connection Classes}
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::QKnxNetIpStruct()

    Creates an empty invalid KNXnet/IP structure.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::QKnxNetIpStruct(CodeType codeType, const QKnxByteArray &dataField = {})

    Creates a new KNXnet/IP structure with the specified \a codeType and payload
    \a dataField.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::QKnxNetIpStruct(const QKnxNetIpStructHeader<CodeType> &headerField, const QKnxByteArray &dataField = {})

    Creates a new KNXnet/IP structure with the specified \a headerField and payload
    \a dataField.

    The header is expected to be fully set up and all values describing the
    payload are expected to match the specified payload.

    \note No adjustments are made to the function arguments.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::code() const

    Returns the generic code stored in the KNXnet/IP structure.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::bytes() const

    Returns an array of bytes representing the KNXnet/IP structure including
    the header and the payload.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::constData() const

    Returns the data stored in the KNXnet/IP structure.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::data() const

    Returns the data stored in the KNXnet/IP structure.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::dataSize() const

    Returns the size of the data stored in the KNXnet/IP structure.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::fromBytes(const QKnxByteArray &bytes, quint16 index)

    Constructs the KNXnet/IP structure from the byte array \a bytes starting at
    the position \a index inside the array.

    \sa isNull(), isValid()
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::header() const

    Returns the header stored in the KNXnet/IP structure.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::isNull() const

    Returns \c true if this is a default constructed structure; otherwise
    returns \c false.
*/

/*!

    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::isValid() const

    Returns \c true if the structure contains initialized values and is in
    itself valid; otherwise returns \c false.

    \sa isNull()
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::setData(const QKnxByteArray &dataField)

    Sets the data stored in the KNXnet/IP structure to \a dataField and updates the
    data size accordingly.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::setHeader(const QKnxNetIpStructHeader<CodeType> &headerField)

    Sets the header stored in the KNXnet/IP structure to \a headerField.
*/

/*!
    \fn template <typename CodeType> QKnxNetIpStruct<CodeType>::size() const

    Returns the size of the KNXnet/IP structure.
*/

/*!
    \fn template <typename CodeType> bool QKnxNetIpStruct<CodeType>::operator==(const QKnxNetIpStruct &other) const

    Returns \c true if this object and the given \a other are equal; otherwise
    returns \c false.
*/

/*!
    \fn template <typename CodeType> bool QKnxNetIpStruct<CodeType>::operator!=(const QKnxNetIpStruct &other) const

    Returns \c true if this object and the given \a other are not equal;
    otherwise returns \c false.
*/

/*!
    \relates QKnxNetIpStruct

    Writes the KNXnet/IP host protocol address information structure \a hpai to
    the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpStruct<QKnxNetIp::HostProtocol> &hpai)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << hpai.bytes().toHex();
}

/*!
    \relates QKnxNetIpStruct

    Writes the KNXnet/IP connection request information structure \a cr to the
    \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpStruct<QKnxNetIp::ConnectionType> &cr)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << cr.bytes().toHex();
}

/*!
    \relates QKnxNetIpStruct

    Writes the KNXnet/IP description information block structure \a dib to the
    \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpStruct<QKnxNetIp::DescriptionType> &dib)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << dib.bytes().toHex();
}

/*!
    \since 5.12
    \relates QKnxNetIpStruct

    Writes the KNXnet/IP extended search parameter request \a srp to the
    \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxNetIpStruct<QKnxNetIp::SearchParameterType> &srp)
{
    QDebugStateSaver _(debug);
    return debug.nospace().noquote() << "0x" << srp.bytes().toHex();
}

QT_END_NAMESPACE
