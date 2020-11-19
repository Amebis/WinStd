/*
    Copyright 1991-2020 Amebis
    Copyright 2016 GÉANT

    This file is part of WinStd.

    Setup is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Setup is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Setup. If not, see <http://www.gnu.org/licenses/>.
*/

///
/// \defgroup WinStdEAPAPI Extensible Authentication Protocol API
/// Integrates WinStd classes with Microsoft EAP API
///

#include "Common.h"

#include <Windows.h>
#include <eaphostpeerconfigapis.h>
#include <eaptypes.h> // Must include after <Windows.h>

#include <memory>

namespace winstd
{
    enum class eap_type_t : unsigned char;
    struct WINSTD_API EapHostPeerFreeMemory_delete;
    struct WINSTD_API EapHostPeerFreeRuntimeMemory_delete;
    struct WINSTD_API EapHostPeerFreeErrorMemory_delete;
    struct WINSTD_API EapHostPeerFreeEapError_delete;
    class WINSTD_API WINSTD_NOVTABLE eap_attr;
    class WINSTD_API WINSTD_NOVTABLE eap_method_prop;
    class WINSTD_API eap_packet;
    class WINSTD_API WINSTD_NOVTABLE eap_method_info_array;
    class WINSTD_API eap_runtime_error;

    /// \addtogroup WinStdEAPAPI
    /// @{

    ///
    /// EapHost BLOB wrapper class
    ///
    typedef std::unique_ptr<BYTE[], EapHostPeerFreeMemory_delete> WINSTD_API eap_blob;

    ///
    /// EapHost BLOB wrapper class
    ///
    typedef std::unique_ptr<BYTE[], EapHostPeerFreeRuntimeMemory_delete> WINSTD_API eap_blob_runtime;

    ///
    /// EAP_ERROR wrapper class
    ///
    typedef std::unique_ptr<EAP_ERROR, EapHostPeerFreeErrorMemory_delete> WINSTD_API eap_error;

    ///
    /// EAP_ERROR wrapper class
    ///
    typedef std::unique_ptr<EAP_ERROR, EapHostPeerFreeEapError_delete> WINSTD_API eap_error_runtime;

    /// @}
}


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
inline bool operator==(_In_ const EAP_METHOD_TYPE &a, _In_ const EAP_METHOD_TYPE &b) noexcept;

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
inline bool operator!=(_In_ const EAP_METHOD_TYPE &a, _In_ const EAP_METHOD_TYPE &b) noexcept;

/// @}

#pragma once

#include <EapHostPeerTypes.h>
#include <eapmethodtypes.h>
#include <eappapis.h>
#include <WinSock2.h>

#pragma warning(push)
#pragma warning(disable: 26812) // Windows EAP API is using unscoped enums

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
    struct WINSTD_API EapHostPeerFreeMemory_delete
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
    /// Deleter for unique_ptr using EapHostPeerFreeRuntimeMemory
    ///
    struct WINSTD_API EapHostPeerFreeRuntimeMemory_delete
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
    /// Deleter for unique_ptr to EAP_ERROR using EapHostPeerFreeErrorMemory
    ///
    struct WINSTD_API EapHostPeerFreeErrorMemory_delete
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
    /// Deleter for unique_ptr to EAP_ERROR using EapHostPeerFreeEapError
    ///
    struct WINSTD_API EapHostPeerFreeEapError_delete
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
    /// EAP_ATTRIBUTE wrapper class
    ///
    #pragma warning(push)
    #pragma warning(disable: 26432) // Copy constructor and assignment operator are also present, but not detected by code analysis as they are using base type source object reference.
    class WINSTD_API WINSTD_NOVTABLE eap_attr : public EAP_ATTRIBUTE
    {
    public:
        ///
        /// Initializes a new EAP attribute set to eatReserved.
        ///
        inline eap_attr() noexcept
        {
            eaType   = eatReserved;
            dwLength = 0;
            pValue   = NULL;
        }

        ///
        /// Copies an existing EAP attribute.
        ///
        inline eap_attr(_In_ const EAP_ATTRIBUTE &a)
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
        inline eap_attr(_Inout_ eap_attr &&a) noexcept
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
        ~eap_attr();

        ///
        /// Copies an existing EAP attribute.
        ///
        inline eap_attr& operator=(_In_ const EAP_ATTRIBUTE &a)
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
        inline eap_attr& operator=(_Inout_ eap_attr &&a) noexcept
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
        void create_ms_mppe_key(_In_ BYTE bVendorType, _In_count_(nKeySize) LPCBYTE pbKey, _In_ BYTE nKeySize);

    public:
        static const EAP_ATTRIBUTE blank;   ///< Blank EAP attribute
    };
    #pragma warning(pop)


    ///
    /// EAP_METHOD_PROPERTY wrapper class
    ///
    class WINSTD_API WINSTD_NOVTABLE eap_method_prop : public EAP_METHOD_PROPERTY
    {
    public:
        ///
        /// Constructs a BOOL method property
        ///
        /// \param[in] type   EAP method property type
        /// \param[in] value  Property value
        ///
        inline eap_method_prop(_In_ EAP_METHOD_PROPERTY_TYPE type, _In_ BOOL value) noexcept
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
        inline eap_method_prop(_In_ EAP_METHOD_PROPERTY_TYPE type, _In_ DWORD value) noexcept
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
        inline eap_method_prop(_In_ EAP_METHOD_PROPERTY_TYPE type, _In_z_ LPCWSTR value) noexcept
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
    class WINSTD_API eap_packet : public dplhandle<EapPacket*, NULL>
    {
        DPLHANDLE_IMPL(eap_packet, NULL)

    public:
        ///
        /// Destroys the EAP packet.
        ///
        virtual ~eap_packet();


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
        inline bool create(_In_ EapCode code, _In_ BYTE id, _In_ WORD size) noexcept
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
        inline WORD size() const noexcept
        {
            return m_h != NULL ? ntohs(*(WORD*)m_h->Length) : 0;
        }


    protected:
        ///
        /// Destroys the EAP packet.
        ///
        void free_internal() noexcept override;

        ///
        /// Duplicates the EAP packet.
        ///
        handle_type duplicate_internal(_In_ handle_type h) const noexcept override;
    };


    ///
    /// EAP_METHOD_INFO_ARRAY wrapper class
    ///
    class WINSTD_API WINSTD_NOVTABLE eap_method_info_array : public EAP_METHOD_INFO_ARRAY
    {
        WINSTD_NONCOPYABLE(eap_method_info_array)

    public:
        ///
        /// Constructs an empty array
        ///
        inline eap_method_info_array() noexcept
        {
            dwNumberOfMethods = 0;
            pEapMethods       = NULL;
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] other  A rvalue reference of another object
        ///
        inline eap_method_info_array(_Inout_ eap_method_info_array &&other) noexcept
        {
            dwNumberOfMethods       = other.dwNumberOfMethods;
            pEapMethods             = other.pEapMethods;
            other.dwNumberOfMethods = 0;
            other.pEapMethods       = NULL;
        }

        ///
        /// Destructor
        ///
        ~eap_method_info_array();

        ///
        /// Move assignment
        ///
        /// \param[inout] other  A rvalue reference of another object
        ///
        inline eap_method_info_array& operator=(_Inout_ eap_method_info_array &&other) noexcept
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
        void free_internal() noexcept;
        static void free_internal(_In_ EAP_METHOD_INFO *pMethodInfo) noexcept;
        /// \endcond
    };

    /// @}

    ///
    /// \defgroup WinStdExceptions Exceptions
    /// Additional exceptions
    ///
    /// @{

    ///
    /// EapHost runtime error
    ///
    /// \sa [EAP_ERROR structure](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363699.aspx)
    ///
    class WINSTD_API eap_runtime_error : public win_runtime_error
    {
    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] err  EapHost error descriptor
        /// \param[in] msg  Error message
        ///
        inline eap_runtime_error(_In_ const EAP_ERROR &err, _In_ const std::string& msg) :
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
        inline eap_runtime_error(_In_ const EAP_ERROR &err, _In_opt_z_ const char *msg = nullptr) :
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
        inline const EAP_METHOD_TYPE& type() const noexcept
        {
            return m_type;
        }


        ///
        /// Returns the reason code for error
        ///
        inline DWORD reason() const noexcept
        {
            return m_reason;
        }


        ///
        /// Returns root cause ID
        ///
        inline const GUID& root_cause_id() const noexcept
        {
            return m_root_cause_id;
        }


        ///
        /// Returns root cause ID
        ///
        inline const wchar_t* root_cause() const noexcept
        {
            return m_root_cause_desc.c_str();
        }


        ///
        /// Returns repair ID
        ///
        inline const GUID& repair_id() const noexcept
        {
            return m_repair_id;
        }


        ///
        /// Returns root cause ID
        ///
        inline const wchar_t* repair() const noexcept
        {
            return m_repair_desc.c_str();
        }


        ///
        /// Returns help_link ID
        ///
        inline const GUID& help_link_id() const noexcept
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


inline bool operator==(_In_ const EAP_METHOD_TYPE &a, _In_ const EAP_METHOD_TYPE &b) noexcept
{
    return
        a.eapType.type         == b.eapType.type         &&
        a.eapType.dwVendorId   == b.eapType.dwVendorId   &&
        a.eapType.dwVendorType == b.eapType.dwVendorType &&
        a.dwAuthorId           == a.dwAuthorId;
}


inline bool operator!=(_In_ const EAP_METHOD_TYPE &a, _In_ const EAP_METHOD_TYPE &b) noexcept
{
    return !operator==(a, b);
}

#pragma warning(pop)
