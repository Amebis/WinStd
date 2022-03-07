/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

/// \file
/// Integrates WinStd classes with Microsoft EAP API
///
/// \defgroup WinStdEAPAPI Extensible Authentication Protocol API
/// Integrates WinStd classes with Microsoft EAP API

#pragma once

#include "Common.h"
#include <eaphostpeerconfigapis.h>
#include <eaptypes.h>
#include <EapHostPeerTypes.h>
#include <eapmethodtypes.h>
#include <eappapis.h>
#include <WinSock2.h>
#include <memory>

#pragma warning(push)
#pragma warning(disable: 26812) // Windows EAP API is using unscoped enums

#pragma warning(push)
#pragma warning(disable: 4505) // Don't warn on unused code

/// \addtogroup WinStdEAPAPI
/// @{

///
/// Are EAP method types equal?
///
/// \param[in] a  First EAP method type
/// \param[in] b  Second EAP method type
///
/// \returns
/// - Non zero when \p a is equal to \p b;
/// - Zero otherwise.
///
static bool operator==(_In_ const EAP_METHOD_TYPE &a, _In_ const EAP_METHOD_TYPE &b) noexcept
{
    return
        a.eapType.type         == b.eapType.type         &&
        a.eapType.dwVendorId   == b.eapType.dwVendorId   &&
        a.eapType.dwVendorType == b.eapType.dwVendorType &&
        a.dwAuthorId           == a.dwAuthorId;
}

///
/// Are EAP method types non-equal?
///
/// \param[in] a  First EAP method type
/// \param[in] b  Second EAP method type
///
/// \returns
/// - Non zero when \p a is not equal to \p b;
/// - Zero otherwise.
///
static bool operator!=(_In_ const EAP_METHOD_TYPE &a, _In_ const EAP_METHOD_TYPE &b) noexcept
{
    return !operator==(a, b);
}

/// @}

#pragma warning(pop)

namespace winstd
{
    /// \addtogroup WinStdEAPAPI
    /// @{

    ///
    /// EAP method numbers
    ///
    /// \sa [Extensible Authentication Protocol (EAP) Registry (Chapter: Method Types)](https://www.iana.org/assignments/eap-numbers/eap-numbers.xhtml#eap-numbers-4)
    ///
    #pragma warning(suppress: 4480)
    enum class eap_type_t : unsigned char {
        undefined       =   0,          ///< Undefined EAP type
        identity        =   1,          ///< Identity
        notification    =   2,          ///< Notification
        nak             =   3,          ///< Legacy Nak
        md5_challenge   =   4,          ///< MD5-Challenge
        otp             =   5,          ///< One-Time Password (OTP)
        gtc             =   6,          ///< Generic Token Card (GTC)
        tls             =  13,          ///< EAP-TLS
        ttls            =  21,          ///< EAP-TTLS
        peap            =  25,          ///< PEAP
        mschapv2        =  26,          ///< EAP-MSCHAPv2
        ms_auth_tlv     =  33,          ///< MS-Authentication-TLV

        gtcp            = 128 + gtc,    ///< EAP-GTC using a password

        legacy_pap      = 192,          ///< PAP (Not actually an EAP method; Moved to the Unassigned area)
        legacy_mschapv2 = 193,          ///< MSCHAPv2 (Not actually an EAP method; Moved to the Unassigned area)

        start           =   1,          ///< Start of EAP methods
        end             = 192,          ///< End of EAP methods (non-inclusive)
        noneap_start    = 192,          ///< Start of non-EAP methods
        noneap_end      = 254,          ///< End of non-EAP methods (non-inclusive)
    };

    ///
    /// Deleter for unique_ptr using EapHostPeerFreeMemory
    ///
    struct EapHostPeerFreeMemory_delete
    {
        ///
        /// Default constructor
        ///
        EapHostPeerFreeMemory_delete() noexcept {}

        ///
        /// Delete a pointer
        ///
        /// \sa [EapHostPeerFreeMemory function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363558.aspx)
        ///
        template <class _T>
        void operator()(_T *_Ptr) const
        {
            EapHostPeerFreeMemory((BYTE*)_Ptr);
        }
    };

    ///
    /// EapHost BLOB wrapper class
    ///
    typedef std::unique_ptr<BYTE[], EapHostPeerFreeMemory_delete> eap_blob;

    ///
    /// Deleter for unique_ptr using EapHostPeerFreeRuntimeMemory
    ///
    struct EapHostPeerFreeRuntimeMemory_delete
    {
        ///
        /// Default constructor
        ///
        EapHostPeerFreeRuntimeMemory_delete() noexcept {}

        ///
        /// Delete a pointer
        ///
        template <class _T>
        void operator()(_T *_Ptr) const
        {
            EapHostPeerFreeRuntimeMemory((BYTE*)_Ptr);
        }
    };

    ///
    /// EapHost BLOB wrapper class
    ///
    typedef std::unique_ptr<BYTE[], EapHostPeerFreeRuntimeMemory_delete> eap_blob_runtime;

    ///
    /// Deleter for unique_ptr to EAP_ERROR using EapHostPeerFreeErrorMemory
    ///
    struct EapHostPeerFreeErrorMemory_delete
    {
        ///
        /// Default constructor
        ///
        EapHostPeerFreeErrorMemory_delete() noexcept {}

        ///
        /// Delete a pointer
        ///
        /// \sa [EapHostPeerFreeErrorMemory function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363557.aspx)
        ///
        void operator()(EAP_ERROR *_Ptr) const noexcept
        {
            EapHostPeerFreeErrorMemory(_Ptr);
        }
    };

    ///
    /// EAP_ERROR wrapper class
    ///
    typedef std::unique_ptr<EAP_ERROR, EapHostPeerFreeErrorMemory_delete> eap_error;

    ///
    /// Deleter for unique_ptr to EAP_ERROR using EapHostPeerFreeEapError
    ///
    struct EapHostPeerFreeEapError_delete
    {
        ///
        /// Default constructor
        ///
        EapHostPeerFreeEapError_delete() noexcept {}

        ///
        /// Delete a pointer
        ///
        /// \sa [EapHostPeerFreeEapError function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363556.aspx)
        ///
        void operator()(EAP_ERROR *_Ptr) const noexcept
        {
            EapHostPeerFreeEapError(_Ptr);
        }
    };

    ///
    /// EAP_ERROR wrapper class
    ///
    typedef std::unique_ptr<EAP_ERROR, EapHostPeerFreeEapError_delete> eap_error_runtime;

    ///
    /// EAP_ATTRIBUTE wrapper class
    ///
    #pragma warning(push)
    #pragma warning(disable: 26432) // Copy constructor and assignment operator are also present, but not detected by code analysis as they are using base type source object reference.
    class eap_attr : public EAP_ATTRIBUTE
    {
    public:
        ///
        /// Initializes a new EAP attribute set to eatReserved.
        ///
        eap_attr() noexcept
        {
            eaType   = eatReserved;
            dwLength = 0;
            pValue   = NULL;
        }

        ///
        /// Copies an existing EAP attribute.
        ///
        eap_attr(_In_ const EAP_ATTRIBUTE &a)
        {
            eaType   = a.eaType;
            dwLength = a.dwLength;
            if (a.dwLength) {
                pValue = new BYTE[a.dwLength];
                assert(pValue);
                memcpy(pValue, a.pValue, a.dwLength);
            } else
                pValue = NULL;
        }

        ///
        /// Moves an existing EAP attribute.
        ///
        eap_attr(_Inout_ eap_attr &&a) noexcept
        {
            eaType   = a.eaType;
            dwLength = a.dwLength;
            if (a.dwLength) {
                pValue     = a.pValue;
                a.dwLength = 0;
                a.pValue   = NULL;
            } else
                pValue = NULL;
        }

        ///
        /// Destroys the EAP attribute.
        ///
        ~eap_attr()
        {
            if (pValue)
                delete [] pValue;
        }

        ///
        /// Copies an existing EAP attribute.
        ///
        eap_attr& operator=(_In_ const EAP_ATTRIBUTE &a)
        {
            if (this != &a) {
                eaType   = a.eaType;
                dwLength = a.dwLength;
                if (a.dwLength) {
                    BYTE *pValueNew = new BYTE[a.dwLength];
                    if (pValueNew) {
                        if (pValue)
                            delete [] pValue;
                        memcpy(pValueNew, a.pValue, a.dwLength);
                        pValue = pValueNew;
                    } else
                        assert(0); // Could not allocate memory
                } else
                    pValue = NULL;
            }
            return *this;
        }

        ///
        /// Moves an existing EAP attribute.
        ///
        eap_attr& operator=(_Inout_ eap_attr &&a) noexcept
        {
            if (this != &a) {
                eaType   = a.eaType;
                dwLength = a.dwLength;
                if (pValue)
                    delete [] pValue;
                if (a.dwLength) {
                    pValue     = a.pValue;
                    a.dwLength = 0;
                    a.pValue   = NULL;
                } else
                    pValue = NULL;
            }
            return *this;
        }

        ///
        /// Creates MS-MPPE-Send-Key or MS-MPPE-Recv-Key
        ///
        /// \sa [RADIUS Vendor-Specific](https://tools.ietf.org/html/rfc2865#section-5.26)
        /// \sa [MS-MPPE-Send-Key](https://tools.ietf.org/html/rfc2548#section-2.4.2)
        /// \sa [MS-MPPE-Recv-Key](https://tools.ietf.org/html/rfc2548#section-2.4.3)
        ///
        void create_ms_mppe_key(_In_ BYTE bVendorType, _In_count_(nKeySize) LPCBYTE pbKey, _In_ BYTE nKeySize)
        {
            const BYTE nPaddingLength = static_cast<BYTE>((16 - (1 + static_cast<DWORD>(nKeySize))) % 16);
            const DWORD dwLengthNew =
                4              + // Vendor-Id
                1              + // Vendor type
                1              + // Vendor length
                2              + // Salt
                1              + // Key-Length
                nKeySize       + // Key
                nPaddingLength;  // Padding

        #pragma warning(push)
        #pragma warning(disable: 6386)
            LPBYTE p = new BYTE[dwLengthNew];
            p[0] = 0x00;                                    // Vendor-Id (0x137 = 311 = Microsoft)
            p[1] = 0x00;                                    // --|
            p[2] = 0x01;                                    // --|
            p[3] = 0x37;                                    // --^
            p[4] = bVendorType;                             // Vendor type
            p[5] = static_cast<BYTE>(dwLengthNew - 4);      // Vendor length
            p[6] = 0x00;                                    // Salt
            p[7] = 0x00;                                    // --^
            p[8] = nKeySize;                                // Key-Length
        #pragma warning(pop)
            memcpy(p + 9, pbKey, nKeySize);                 // Key
            memset(p + 9 + nKeySize, 0, nPaddingLength);    // Padding

            if (pValue)
                delete [] pValue;

            #pragma warning(suppress: 26812) // EAP_ATTRIBUTE_TYPE is unscoped.
            eaType   = eatVendorSpecific;
            dwLength = dwLengthNew;
            pValue   = p;
        }
    };
    #pragma warning(pop)

    ///
    /// Blank EAP attribute
    ///
    static const EAP_ATTRIBUTE blank_eap_attr = {};

    ///
    /// EAP_METHOD_PROPERTY wrapper class
    ///
    class eap_method_prop : public EAP_METHOD_PROPERTY
    {
    public:
        ///
        /// Constructs a BOOL method property
        ///
        /// \param[in] type   EAP method property type
        /// \param[in] value  Property value
        ///
        eap_method_prop(_In_ EAP_METHOD_PROPERTY_TYPE type, _In_ BOOL value) noexcept
        {
            eapMethodPropertyType                  = type;
            eapMethodPropertyValueType             = empvtBool;
            eapMethodPropertyValue.empvBool.length = sizeof(BOOL);
            eapMethodPropertyValue.empvBool.value  = value;
        }

        ///
        /// Constructs a DWORD method property
        ///
        /// \param[in] type   EAP method property type
        /// \param[in] value  Property value
        ///
        eap_method_prop(_In_ EAP_METHOD_PROPERTY_TYPE type, _In_ DWORD value) noexcept
        {
            eapMethodPropertyType                   = type;
            eapMethodPropertyValueType              = empvtDword;
            eapMethodPropertyValue.empvDword.length = sizeof(DWORD);
            eapMethodPropertyValue.empvDword.value  = value;
        }

        ///
        /// Constructs a Unicode string method property
        ///
        /// \param[in] type   EAP method property type
        /// \param[in] value  Property value
        ///
        eap_method_prop(_In_ EAP_METHOD_PROPERTY_TYPE type, _In_z_ LPCWSTR value) noexcept
        {
            eapMethodPropertyType                    = type;
            eapMethodPropertyValueType               = empvtString;
            eapMethodPropertyValue.empvString.length = static_cast<DWORD>(sizeof(WCHAR)*(wcslen(value) + 1));
            eapMethodPropertyValue.empvString.value  = const_cast<BYTE*>(reinterpret_cast<const BYTE*>(value));
        }
    };

    ///
    /// EapPacket wrapper class
    ///
    class eap_packet : public dplhandle<EapPacket*, NULL>
    {
        WINSTD_DPLHANDLE_IMPL(eap_packet, NULL)

    public:
        ///
        /// Destroys the EAP packet.
        ///
        virtual ~eap_packet()
        {
            if (m_h != invalid)
                free_internal();
        }

        ///
        /// Create new EAP packet
        ///
        /// \param[in] code  EAP code (one of EapCode enum values)
        /// \param[in] id    Packet ID
        /// \param[in] size  Total packet size in bytes, including header. Must be at least 4B.
        ///
        /// \note Packet data (beyond first 4B) is not initialized.
        ///
        /// \return
        /// - true when creation succeeds;
        /// - false when creation fails. For extended error information, call `GetLastError()`.
        ///
        bool create(_In_ EapCode code, _In_ BYTE id, _In_ WORD size) noexcept
        {
            assert(size >= 4); // EAP packets must contain at least Code, Id, and Length fields: 4B.

            handle_type h = static_cast<handle_type>(HeapAlloc(GetProcessHeap(), 0, size));
            if (h != NULL) {
                h->Code = static_cast<BYTE>(code);
                h->Id = id;
                *reinterpret_cast<WORD*>(h->Length) = htons(size);

                attach(h);
                return true;
            } else {
                SetLastError(ERROR_OUTOFMEMORY);
                return false;
            }
        }

        ///
        /// Returns total EAP packet size in bytes.
        ///
        WORD size() const noexcept
        {
            return m_h != NULL ? ntohs(*(WORD*)m_h->Length) : 0;
        }

    protected:
        ///
        /// Destroys the EAP packet.
        ///
        void free_internal() noexcept override
        {
            HeapFree(GetProcessHeap(), 0, m_h);
        }

        ///
        /// Duplicates the EAP packet.
        ///
        handle_type duplicate_internal(_In_ handle_type h) const noexcept override
        {
            const WORD n = ntohs(*reinterpret_cast<WORD*>(h->Length));
            handle_type h2 = static_cast<handle_type>(HeapAlloc(GetProcessHeap(), 0, n));
            if (h2 == NULL) {
                SetLastError(ERROR_OUTOFMEMORY);
                return NULL;
            }
            memcpy(h2, h, n);
            return h2;
        }
    };

    ///
    /// EAP_METHOD_INFO_ARRAY wrapper class
    ///
    class eap_method_info_array : public EAP_METHOD_INFO_ARRAY
    {
        WINSTD_NONCOPYABLE(eap_method_info_array)

    public:
        ///
        /// Constructs an empty array
        ///
        eap_method_info_array() noexcept
        {
            dwNumberOfMethods = 0;
            pEapMethods       = NULL;
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] other  A rvalue reference of another object
        ///
        eap_method_info_array(_Inout_ eap_method_info_array &&other) noexcept
        {
            dwNumberOfMethods       = other.dwNumberOfMethods;
            pEapMethods             = other.pEapMethods;
            other.dwNumberOfMethods = 0;
            other.pEapMethods       = NULL;
        }

        ///
        /// Destructor
        ///
        ~eap_method_info_array()
        {
            if (pEapMethods)
                free_internal();
        }

        ///
        /// Move assignment
        ///
        /// \param[inout] other  A rvalue reference of another object
        ///
        eap_method_info_array& operator=(_Inout_ eap_method_info_array &&other) noexcept
        {
            if (this != std::addressof(other)) {
                if (pEapMethods)
                    free_internal();
                dwNumberOfMethods       = other.dwNumberOfMethods;
                pEapMethods             = other.pEapMethods;
                other.dwNumberOfMethods = 0;
                other.pEapMethods       = NULL;
            }
            return *this;
        }

    protected:
        /// \cond internal

        void free_internal() noexcept
        {
            for (DWORD i = 0; i < dwNumberOfMethods; i++)
                free_internal(pEapMethods + i);

            EapHostPeerFreeMemory(reinterpret_cast<BYTE*>(pEapMethods));
        }

        static void free_internal(_In_ EAP_METHOD_INFO *pMethodInfo) noexcept
        {
            if (pMethodInfo->pInnerMethodInfo)
                free_internal(pMethodInfo->pInnerMethodInfo);

            EapHostPeerFreeMemory(reinterpret_cast<BYTE*>(pMethodInfo->pwszAuthorName));
            EapHostPeerFreeMemory(reinterpret_cast<BYTE*>(pMethodInfo->pwszFriendlyName));
        }

        /// \endcond
    };

    /// @}

    /// \addtogroup WinStdExceptions
    /// @{

    ///
    /// EapHost runtime error
    ///
    /// \sa [EAP_ERROR structure](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363699.aspx)
    ///
    class eap_runtime_error : public win_runtime_error
    {
    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] err  EapHost error descriptor
        /// \param[in] msg  Error message
        ///
        eap_runtime_error(_In_ const EAP_ERROR &err, _In_ const std::string& msg) :
            m_type           (err.type                   ),
            m_reason         (err.dwReasonCode           ),
            m_root_cause_id  (err.rootCauseGuid          ),
            m_root_cause_desc(err.pRootCauseString       ),
            m_repair_id      (err.repairGuid             ),
            m_repair_desc    (err.pRepairString          ),
            m_help_link_id   (err.helpLinkGuid           ),
            win_runtime_error(err.dwWinError, msg.c_str())
        {
        }

        ///
        /// Constructs an exception
        ///
        /// \param[in] err  EapHost error descriptor
        /// \param[in] msg  Error message
        ///
        eap_runtime_error(_In_ const EAP_ERROR &err, _In_opt_z_ const char *msg = nullptr) :
            m_type           (err.type            ),
            m_reason         (err.dwReasonCode    ),
            m_root_cause_id  (err.rootCauseGuid   ),
            m_root_cause_desc(err.pRootCauseString),
            m_repair_id      (err.repairGuid      ),
            m_repair_desc    (err.pRepairString   ),
            m_help_link_id   (err.helpLinkGuid    ),
            win_runtime_error(err.dwWinError, msg )
        {
        }

        ///
        /// Returns EAP method type
        ///
        const EAP_METHOD_TYPE& type() const noexcept
        {
            return m_type;
        }

        ///
        /// Returns the reason code for error
        ///
        DWORD reason() const noexcept
        {
            return m_reason;
        }

        ///
        /// Returns root cause ID
        ///
        const GUID& root_cause_id() const noexcept
        {
            return m_root_cause_id;
        }

        ///
        /// Returns root cause ID
        ///
        const wchar_t* root_cause() const noexcept
        {
            return m_root_cause_desc.c_str();
        }

        ///
        /// Returns repair ID
        ///
        const GUID& repair_id() const noexcept
        {
            return m_repair_id;
        }

        ///
        /// Returns root cause ID
        ///
        const wchar_t* repair() const noexcept
        {
            return m_repair_desc.c_str();
        }

        ///
        /// Returns help_link ID
        ///
        const GUID& help_link_id() const noexcept
        {
            return m_help_link_id;
        }

    protected:
        EAP_METHOD_TYPE m_type;         ///< Structure that identifies the EAP method that raised the error

        DWORD m_reason;                 ///< The reason code for the error

        GUID m_root_cause_id;           ///< A unique ID that identifies cause of error in EAPHost
        std::wstring m_root_cause_desc; ///< A localized and readable string that describes the root cause of the error

        GUID m_repair_id;               ///< A unique ID that maps to a localizable string that identifies the repair action that can be taken to fix the reported error
        std::wstring m_repair_desc;     ///< A localized and readable string that describes the possible repair action

        GUID m_help_link_id;            ///< A unique ID that maps to a localizable string that specifies an URL for a page that contains additional information about an error or repair message
    };

    /// @}
}

#pragma warning(pop)
