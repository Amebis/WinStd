/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

#pragma once

#include "Common.h"
#include <unknwn.h>
#include <string>

namespace winstd
{
    /// \addtogroup WinStdExceptions
    /// @{

    ///
    /// COM runtime error
    ///
    /// \note Must be defined as derived class from num_runtime_error<> to allow correct type info for dynamic typecasting and prevent folding with other derivates of num_runtime_error<>.
    ///
    class com_runtime_error : public num_runtime_error<HRESULT>
    {
    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] num  COM error code
        /// \param[in] msg  Error message
        ///
        com_runtime_error(_In_ error_type num, _In_ const std::string& msg) : num_runtime_error<HRESULT>(num, msg)
        {
        }

        ///
        /// Constructs an exception
        ///
        /// \param[in] num  COM error code
        /// \param[in] msg  Error message
        ///
        com_runtime_error(_In_ error_type num, _In_opt_z_ const char *msg = nullptr) : num_runtime_error<HRESULT>(num, msg)
        {
        }
    };

    /// @}

    ///
    /// \defgroup WinStdCOM COM object management
    /// Provides helper templates for Windows COM object manipulation
    ///
    /// @{

    ///
    /// Deleter for unique_ptr using CoTaskMemFree
    ///
    struct CoTaskMemFree_delete
    {
        ///
        /// Default constructor
        ///
        CoTaskMemFree_delete() noexcept {}

        ///
        /// Delete a pointer
        ///
        /// \sa [CoTaskMemFree function](https://docs.microsoft.com/en-us/windows/desktop/api/combaseapi/nf-combaseapi-cotaskmemfree)
        ///
        template <class _T>
        void operator()(_T *_Ptr) const
        {
            CoTaskMemFree(_Ptr);
        }
    };

    ///
    /// COM object wrapper template
    ///
    /// \sa [CoCreateInstance function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615.aspx)
    ///
    template <class T>
    class com_obj : public dplhandle<T*, NULL>
    {
        WINSTD_DPLHANDLE_IMPL(com_obj, NULL)

    public:
        ///
        /// Queries the object for another interface and creates new class with it
        ///
        /// \sa [IUnknown::QueryInterface method](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682521.aspx)
        ///
        template <class _Other>
        com_obj(_In_ _Other *other)
        {
            assert(other);
            other->QueryInterface(__uuidof(T), (void**)&m_h);
        }

        ///
        /// Queries the object for another interface and creates new class with it
        ///
        /// \sa [IUnknown::QueryInterface method](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682521.aspx)
        ///
        template <class _Other>
        com_obj(_In_ com_obj<_Other> &other)
        {
            other->QueryInterface(__uuidof(T), (void**)&m_h);
        }

        ///
        /// Releases object
        ///
        virtual ~com_obj()
        {
            if (m_h != invalid)
                free_internal();
        }

        ///
        /// Queries the object for another interface
        ///
        /// \sa [IUnknown::QueryInterface method](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682521.aspx)
        ///
        template <class _Other>
        HRESULT query_interface(_Out_ _Other **h) const
        {
            assert(h);
            assert(m_h);
            return m_h->QueryInterface(__uuidof(_Other), (void**)h);
        }

        ///
        /// Queries the object for another interface
        ///
        /// \sa [IUnknown::QueryInterface method](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682521.aspx)
        ///
        template <class _Other>
        HRESULT query_interface(_Out_ com_obj<_Other> &h) const
        {
            assert(m_h);
            _Other *_h;
            HRESULT hr = m_h->QueryInterface(__uuidof(_Other), (void**)&_h);
            if (SUCCEEDED(hr))
                h.attach(_h);
            return hr;
        }

    protected:
        ///
        /// Releases the object by decrementing reference counter
        ///
        /// \sa [IUnknown::Release method](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682317.aspx)
        ///
        void free_internal() noexcept override
        {
            m_h->Release();
        }

        ///
        /// Duplicates the object by incrementing the reference counter
        ///
        /// \sa [IUnknown::AddRef method](https://msdn.microsoft.com/en-us/library/windows/desktop/ms691379.aspx)
        ///
        /// \param[in] h  Object handle of existing object
        ///
        /// \return Duplicated object handle
        ///
        handle_type duplicate_internal(_In_ handle_type h) const noexcept override
        {
            h->AddRef();
            return h;
        }
    };

    ///
    /// BSTR string wrapper
    ///
    class bstr : public dplhandle<BSTR, NULL>
    {
        WINSTD_DPLHANDLE_IMPL(bstr, NULL)

    public:
        ///
        /// Constructs BSTR from OLE string
        ///
        bstr(_In_ LPCOLESTR src) noexcept
        {
            m_h = SysAllocString(src);
        }

        ///
        /// Constructs BSTR from OLE string with length
        ///
        bstr(_In_ LPCOLESTR src, _In_ UINT len) noexcept
        {
            m_h = SysAllocStringLen(src, len);
        }

        ///
        /// Constructs BSTR from std::basic_string
        ///
        template<class _Traits, class _Ax>
        bstr(_In_ const std::basic_string<wchar_t, _Traits, _Ax> &src) noexcept
        {
            m_h = SysAllocStringLen(src.c_str(), (UINT)src.length());
        }

        ///
        /// Destroys the string
        ///
        /// \sa [SysFreeString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221481.aspx)
        ///
        virtual ~bstr()
        {
            if (m_h != invalid)
                free_internal();
        }

        ///
        /// Returns the length of the string
        ///
        /// \sa [SysStringLen function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221240.aspx)
        ///
        UINT length() const noexcept
        {
            return SysStringLen(m_h);
        }

    protected:
        ///
        /// Destroys the string
        ///
        /// \sa [SysFreeString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221481.aspx)
        ///
        void free_internal() noexcept override
        {
            SysFreeString(m_h);
        }

        ///
        /// Duplicates the string
        ///
        /// \param[in] h  Existing string
        ///
        /// \return Duplicated string
        ///
        /// \sa [SysAllocString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221458.aspx)
        ///
        handle_type duplicate_internal(_In_ handle_type h) const noexcept override
        {
            return SysAllocStringLen(h, SysStringLen(h));
        }
    };

    ///
    /// VARIANT struct wrapper
    ///
    #pragma warning(push)
    #pragma warning(disable: 26432) // Copy constructor and assignment operator are also present, but not detected by code analysis as they are using base type source object reference.
    class variant : public VARIANT
    {
    public:
        ///
        /// Constructs blank VARIANT
        ///
        variant() noexcept
        {
            VariantInit(this);
        }

        ///
        /// Constructs VARIANT from another
        ///
        variant(_In_ const VARIANT& varSrc)
        {
            vt = VT_EMPTY;
            const HRESULT hr = VariantCopy(this, &varSrc);
            if (FAILED(hr))
                throw winstd::com_runtime_error(hr, "VariantCopy failed.");
        }

        ///
        /// Moves VARIANT from another
        ///
        #pragma warning(suppress: 26495) // vt member is initialized as a result of memcpy()
        variant(_Inout_ VARIANT&& varSrc) noexcept
        {
            memcpy(this, &varSrc, sizeof(VARIANT));
            varSrc.vt = VT_EMPTY;
        }

        ///
        /// Constructs VARIANT from bool
        ///
        variant(_In_ bool bSrc) noexcept
        {
            vt = VT_BOOL;
            boolVal = bSrc ? VARIANT_TRUE : VARIANT_FALSE;
        }

        ///
        /// Constructs VARIANT from character
        ///
        variant(_In_ char cSrc) noexcept
        {
            vt = VT_I1;
            cVal = cSrc;
        }

        ///
        /// Constructs VARIANT from byte
        ///
        variant(_In_ unsigned char nSrc) noexcept
        {
            vt = VT_UI1;
            bVal = nSrc;
        }

        ///
        /// Constructs VARIANT from short
        ///
        variant(_In_ short nSrc) noexcept
        {
            vt = VT_I2;
            iVal = nSrc;
        }

        ///
        /// Constructs VARIANT from unsigned short
        ///
        variant(_In_ unsigned short nSrc) noexcept
        {
            vt = VT_UI2;
            uiVal = nSrc;
        }

        ///
        /// Constructs VARIANT from integer
        ///
        variant(_In_ int nSrc, _In_ VARTYPE vtSrc = VT_I4) noexcept
        {
            assert(vtSrc == VT_I4 || vtSrc == VT_INT);
            vt = vtSrc;
            intVal = nSrc;
        }

        ///
        /// Constructs VARIANT from unsigned integer
        ///
        variant(_In_ unsigned int nSrc, _In_ VARTYPE vtSrc = VT_UI4) noexcept
        {
            assert(vtSrc == VT_UI4 || vtSrc == VT_UINT);
            vt = vtSrc;
            uintVal= nSrc;
        }

        ///
        /// Constructs VARIANT from long
        ///
        variant(_In_ long nSrc, _In_ VARTYPE vtSrc = VT_I4) noexcept
        {
            assert(vtSrc == VT_I4 || vtSrc == VT_ERROR);
            vt = vtSrc;
            lVal = nSrc;
        }

        ///
        /// Constructs VARIANT from unsigned long
        ///
        variant(_In_ unsigned long nSrc) noexcept
        {
            vt = VT_UI4;
            ulVal = nSrc;
        }

        ///
        /// Constructs VARIANT from float
        ///
        variant(_In_ float fltSrc) noexcept
        {
            vt = VT_R4;
            fltVal = fltSrc;
        }

        ///
        /// Constructs VARIANT from double or variant date
        ///
        variant(_In_ double dblSrc, _In_ VARTYPE vtSrc = VT_R8) noexcept
        {
            assert(vtSrc == VT_R8 || vtSrc == VT_DATE);
            vt = vtSrc;
            dblVal = dblSrc;
        }

        ///
        /// Constructs VARIANT from 64-bit integer
        ///
        variant(_In_ long long nSrc) noexcept
        {
            vt = VT_I8;
            llVal = nSrc;
        }

        ///
        /// Constructs VARIANT from unsigned integer
        ///
        variant(_In_ unsigned long long nSrc) noexcept
        {
            vt = VT_UI8;
            ullVal = nSrc;
        }

        ///
        /// Constructs VARIANT from CY (64-bit integer)
        ///
        variant(_In_ CY cySrc) noexcept
        {
            vt = VT_CY;
            cyVal.Hi = cySrc.Hi;
            cyVal.Lo = cySrc.Lo;
        }

        ///
        /// Constructs VARIANT from OLE string
        ///
        variant(_In_z_ LPCOLESTR lpszSrc) noexcept
        {
            vt = VT_EMPTY;
            *this = lpszSrc;
        }

        ///
        /// Constructs VARIANT from BSTR
        ///
        variant(_In_z_ BSTR bstr) noexcept
        {
            vt = VT_EMPTY;
            *this = bstr;
        }

        ///
        /// Constructs VARIANT from IDispatch
        ///
        variant(_In_opt_ IDispatch* pSrc)
        {
            vt = VT_DISPATCH;
            pdispVal = pSrc;

            if (pdispVal != NULL)
                pdispVal->AddRef();
        }

        ///
        /// Constructs VARIANT from IUnknown
        ///
        variant(_In_opt_ IUnknown* pSrc)
        {
            vt = VT_UNKNOWN;
            punkVal = pSrc;

            if (punkVal != NULL)
                punkVal->AddRef();
        }

        ///
        /// Constructs VARIANT from SAFEARRAY
        ///
        variant(_In_ const SAFEARRAY *pSrc)
        {
            assert(pSrc != NULL);

            LPSAFEARRAY pCopy;
            const HRESULT hr = SafeArrayCopy(const_cast<LPSAFEARRAY>(pSrc), &pCopy);
            if (FAILED(hr))
                throw winstd::com_runtime_error(hr, "SafeArrayCopy failed.");

            SafeArrayGetVartype(const_cast<LPSAFEARRAY>(pSrc), &vt);
            vt |= VT_ARRAY;
            parray = pCopy;
        }

        ///
        /// Destroys VARIANT
        ///
        virtual ~variant()
        {
            VariantClear(this);
        }

        ///
        /// Copy from another VARIANT
        ///
        variant& operator=(_In_ const VARIANT& varSrc)
        {
            if (this != &varSrc) {
                const HRESULT hr = VariantCopy(this, &varSrc);
                if (FAILED(hr))
                    throw winstd::com_runtime_error(hr, "VariantCopy failed.");
            }
            return *this;
        }

        ///
        /// Moves from another VARIANT
        ///
        variant& operator=(_Inout_ VARIANT&& varSrc) noexcept
        {
            if (this != &varSrc) {
                VariantClear(this);
                memcpy(this, &varSrc, sizeof(VARIANT));
                varSrc.vt = VT_EMPTY;
            }
            return *this;
        }

        ///
        /// Copy from bool value
        ///
        variant& operator=(_In_ bool bSrc) noexcept
        {
            if (vt != VT_BOOL) {
                VariantClear(this);
                vt = VT_BOOL;
            }
            boolVal = bSrc ? VARIANT_TRUE : VARIANT_FALSE;
            return *this;
        }

        ///
        /// Copy from char value
        ///
        variant& operator=(_In_ char cSrc) noexcept
        {
            if (vt != VT_I1) {
                VariantClear(this);
                vt = VT_I1;
            }
            cVal = cSrc;
            return *this;
        }

        ///
        /// Copy from unsigned char value
        ///
        variant& operator=(_In_ unsigned char nSrc) noexcept
        {
            if (vt != VT_UI1) {
                VariantClear(this);
                vt = VT_UI1;
            }
            bVal = nSrc;
            return *this;
        }

        ///
        /// Copy from short value
        ///
        variant& operator=(_In_ short nSrc) noexcept
        {
            if (vt != VT_I2) {
                VariantClear(this);
                vt = VT_I2;
            }
            iVal = nSrc;
            return *this;
        }

        ///
        /// Copy from unsigned short value
        ///
        variant& operator=(_In_ unsigned short nSrc) noexcept
        {
            if (vt != VT_UI2) {
                VariantClear(this);
                vt = VT_UI2;
            }
            uiVal = nSrc;
            return *this;
        }

        ///
        /// Copy from int value
        ///
        variant& operator=(_In_ int nSrc) noexcept
        {
            if (vt != VT_I4) {
                VariantClear(this);
                vt = VT_I4;
            }
            intVal = nSrc;
            return *this;
        }

        ///
        /// Copy from unsigned int value
        ///
        variant& operator=(_In_ unsigned int nSrc) noexcept
        {
            if (vt != VT_UI4) {
                VariantClear(this);
                vt = VT_UI4;
            }
            uintVal= nSrc;
            return *this;
        }

        ///
        /// Copy from long value
        ///
        variant& operator=(_In_ long nSrc) noexcept
        {
            if (vt != VT_I4) {
                VariantClear(this);
                vt = VT_I4;
            }
            lVal = nSrc;
            return *this;
        }

        ///
        /// Copy from unsigned long value
        ///
        variant& operator=(_In_ unsigned long nSrc) noexcept
        {
            if (vt != VT_UI4) {
                VariantClear(this);
                vt = VT_UI4;
            }
            ulVal = nSrc;
            return *this;
        }

        ///
        /// Copy from long long value
        ///
        variant& operator=(_In_ long long nSrc) noexcept
        {
            if (vt != VT_I8) {
                VariantClear(this);
                vt = VT_I8;
            }
            llVal = nSrc;
            return *this;
        }

        ///
        /// Copy from unsigned long long value
        ///
        variant& operator=(_In_ unsigned long long nSrc) noexcept
        {
            if (vt != VT_UI8) {
                VariantClear(this);
                vt = VT_UI8;
            }
            ullVal = nSrc;

            return *this;
        }

        ///
        /// Copy from float value
        ///
        variant& operator=(_In_ float fltSrc) noexcept
        {
            if (vt != VT_R4) {
                VariantClear(this);
                vt = VT_R4;
            }
            fltVal = fltSrc;
            return *this;
        }

        ///
        /// Copy from double value
        ///
        variant& operator=(_In_ double dblSrc) noexcept
        {
            if (vt != VT_R8) {
                VariantClear(this);
                vt = VT_R8;
            }
            dblVal = dblSrc;
            return *this;
        }

        ///
        /// Copy from CY value
        ///
        variant& operator=(_In_ CY cySrc) noexcept
        {
            if (vt != VT_CY) {
                VariantClear(this);
                vt = VT_CY;
            }
            cyVal.Hi = cySrc.Hi;
            cyVal.Lo = cySrc.Lo;
            return *this;
        }

        ///
        /// Copy from OLE string value
        ///
        variant& operator=(_In_z_ LPCOLESTR lpszSrc) noexcept
        {
            VariantClear(this);
            vt = VT_BSTR;
            bstrVal = SysAllocString(lpszSrc);
            return *this;
        }

        ///
        /// Copy from IDispatch
        ///
        variant& operator=(_Inout_opt_ IDispatch* pSrc)
        {
            VariantClear(this);
            vt = VT_DISPATCH;
            pdispVal = pSrc;
            if (pdispVal != NULL)
                pdispVal->AddRef();
            return *this;
        }

        ///
        /// Copy from IUnknown
        ///
        variant& operator=(_Inout_opt_ IUnknown* pSrc)
        {
            VariantClear(this);
            vt = VT_UNKNOWN;
            punkVal = pSrc;
            if (punkVal != NULL)
                punkVal->AddRef();
            return *this;
        }

        ///
        /// Copy from unsigned char reference
        ///
        variant& operator=(_In_ unsigned char* pbSrc) noexcept
        {
            if (vt != (VT_UI1|VT_BYREF)) {
                VariantClear(this);
                vt = VT_UI1|VT_BYREF;
            }
            pbVal = pbSrc;
            return *this;
        }

        ///
        /// Copy from short reference
        ///
        variant& operator=(_In_ short* pnSrc) noexcept
        {
            if (vt != (VT_I2|VT_BYREF)) {
                VariantClear(this);
                vt = VT_I2|VT_BYREF;
            }
            piVal = pnSrc;
            return *this;
        }

        ///
        /// Copy from unsigned short reference
        ///
        variant& operator=(_In_ unsigned short* pnSrc) noexcept
        {
            if (vt != (VT_UI2|VT_BYREF)) {
                VariantClear(this);
                vt = VT_UI2|VT_BYREF;
            }
            puiVal = pnSrc;
            return *this;
        }

        ///
        /// Copy from int reference
        ///
        variant& operator=(_In_ int* pnSrc) noexcept
        {
            if (vt != (VT_I4|VT_BYREF)) {
                VariantClear(this);
                vt = VT_I4|VT_BYREF;
            }
            pintVal = pnSrc;
            return *this;
        }

        ///
        /// Copy from unsigned int reference
        ///
        variant& operator=(_In_ unsigned int* pnSrc) noexcept
        {
            if (vt != (VT_UI4|VT_BYREF)) {
                VariantClear(this);
                vt = VT_UI4|VT_BYREF;
            }
            puintVal = pnSrc;
            return *this;
        }

        ///
        /// Copy from long reference
        ///
        variant& operator=(_In_ long* pnSrc) noexcept
        {
            if (vt != (VT_I4|VT_BYREF)) {
                VariantClear(this);
                vt = VT_I4|VT_BYREF;
            }
            plVal = pnSrc;
            return *this;
        }

        ///
        /// Copy from unsigned long reference
        ///
        variant& operator=(_In_ unsigned long* pnSrc) noexcept
        {
            if (vt != (VT_UI4|VT_BYREF)) {
                VariantClear(this);
                vt = VT_UI4|VT_BYREF;
            }
            pulVal = pnSrc;
            return *this;
        }

        ///
        /// Copy from long long reference
        ///
        variant& operator=(_In_ long long* pnSrc) noexcept
        {
            if (vt != (VT_I8|VT_BYREF)) {
                VariantClear(this);
                vt = VT_I8|VT_BYREF;
            }
            pllVal = pnSrc;
            return *this;
        }

        ///
        /// Copy from unsigned long long reference
        ///
        variant& operator=(_In_ unsigned long long* pnSrc) noexcept
        {
            if (vt != (VT_UI8|VT_BYREF)) {
                VariantClear(this);
                vt = VT_UI8|VT_BYREF;
            }
            pullVal = pnSrc;
            return *this;
        }

        ///
        /// Copy from float reference
        ///
        variant& operator=(_In_ float* pfSrc) noexcept
        {
            if (vt != (VT_R4|VT_BYREF)) {
                VariantClear(this);
                vt = VT_R4|VT_BYREF;
            }
            pfltVal = pfSrc;
            return *this;
        }

        ///
        /// Copy from double reference
        ///
        variant& operator=(_In_ double* pfSrc) noexcept
        {
            if (vt != (VT_R8|VT_BYREF)) {
                VariantClear(this);
                vt = VT_R8|VT_BYREF;
            }
            pdblVal = pfSrc;
            return *this;
        }

        ///
        /// Copy from SAFEARRAY
        ///
        variant& operator=(_In_ const SAFEARRAY *pSrc) noexcept
        {
            assert(pSrc != NULL);
            VariantClear(this);

            LPSAFEARRAY pCopy;
            const HRESULT hr = SafeArrayCopy(const_cast<LPSAFEARRAY>(pSrc), &pCopy);
            if (SUCCEEDED(hr)) {
                SafeArrayGetVartype(const_cast<LPSAFEARRAY>(pSrc), &vt);
                vt |= VT_ARRAY;
                parray = pCopy;
            } else
                assert(0);

            return *this;
        }

    public:
        ///
        /// Is variant equal to?
        ///
        /// \param[in] varSrc  Variant to compare against
        /// \return
        /// - Non zero when variant is equal to \p varSrc;
        /// - Zero otherwise.
        ///
        bool operator==(_In_ const VARIANT& varSrc) const noexcept
        {
            if (vt == VT_NULL && varSrc.vt == VT_NULL) return true;
            if (vt != varSrc.vt) return false;
            return compare(static_cast<const VARIANT&>(*this), varSrc, LOCALE_USER_DEFAULT, 0) == static_cast<HRESULT>(VARCMP_EQ);
        }

        ///
        /// Is variant not equal to?
        ///
        /// \param[in] varSrc  Variant to compare against
        /// \return
        /// - Non zero when variant is not equal to \p varSrc;
        /// - Zero otherwise.
        ///
        bool operator!=(_In_ const VARIANT& varSrc) const noexcept
        {
            return !operator==(varSrc);
        }

        ///
        /// Is variant less than?
        ///
        /// \param[in] varSrc  Variant to compare against
        /// \return
        /// - Non zero when variant is less than \p varSrc;
        /// - Zero otherwise.
        ///
        bool operator<(_In_ const VARIANT& varSrc) const noexcept
        {
            if (vt == VT_NULL && varSrc.vt == VT_NULL) return false;
            return compare(static_cast<const VARIANT&>(*this), varSrc, LOCALE_USER_DEFAULT, 0)== static_cast<HRESULT>(VARCMP_LT);
        }

        ///
        /// Is variant greater than?
        ///
        /// \param[in] varSrc  Variant to compare against
        /// \return
        /// - Non zero when variant is greater than \p varSrc;
        /// - Zero otherwise.
        ///
        bool operator>(_In_ const VARIANT& varSrc) const noexcept
        {
            if (vt == VT_NULL && varSrc.vt == VT_NULL) return false;
            return compare(static_cast<const VARIANT&>(*this), varSrc, LOCALE_USER_DEFAULT, 0)== static_cast<HRESULT>(VARCMP_GT);
        }

        ///
        /// Is variant less than or equal to?
        ///
        /// \param[in] varSrc  Variant to compare against
        /// \return
        /// - Non zero when variant is less than or equal to \p varSrc;
        /// - Zero otherwise.
        ///
        bool operator<=(_In_ const VARIANT& varSrc) const noexcept
        {
            return !operator>(varSrc);
        }

        ///
        /// Is variant greater than or equal to?
        ///
        /// \param[in] varSrc  Variant to compare against
        /// \return
        /// - Non zero when variant is greater than or equal to \p varSrc;
        /// - Zero otherwise.
        ///
        bool operator>=(_In_ const VARIANT& varSrc) const noexcept
        {
            return !operator<(varSrc);
        }

        ///
        /// Converts a variant from one type to another.
        ///
        /// \sa [VariantChangeType function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221258.aspx)
        ///
        HRESULT change_type(_In_ VARTYPE _vt, _In_opt_ USHORT wFlags = 0) noexcept
        {
            return VariantChangeType(this, this, wFlags, _vt);
        }

    private:
        /// \cond internal
        HRESULT compare(_In_ const VARIANT &varLeft, _In_ const VARIANT &varRight, _In_ LCID lcid, _In_ ULONG dwFlags) const noexcept
        {
            switch(vt) {
                case VT_I1:  return varLeft.cVal    == varRight.cVal    ? VARCMP_EQ : varLeft.cVal    > varRight.cVal    ? VARCMP_GT : VARCMP_LT;
                case VT_UI2: return varLeft.uiVal   == varRight.uiVal   ? VARCMP_EQ : varLeft.uiVal   > varRight.uiVal   ? VARCMP_GT : VARCMP_LT;
                case VT_UI4: return varLeft.uintVal == varRight.uintVal ? VARCMP_EQ : varLeft.uintVal > varRight.uintVal ? VARCMP_GT : VARCMP_LT;
                case VT_UI8: return varLeft.ullVal  == varRight.ullVal  ? VARCMP_EQ : varLeft.ullVal  > varRight.ullVal  ? VARCMP_GT : VARCMP_LT;
                default:     return VarCmp(const_cast<LPVARIANT>(&varLeft), const_cast<LPVARIANT>(&varRight), lcid, dwFlags);
            }
        }
        /// \endcond
    };
    #pragma warning(pop)

    ///
    /// Context scope automatic COM (un)initialization
    ///
    class com_initializer
    {
        WINSTD_NONCOPYABLE(com_initializer)
        WINSTD_NONMOVABLE(com_initializer)

    public:
        ///
        /// Initializes the COM library on the current thread and identifies the concurrency model as single-thread apartment (STA).
        ///
        /// \sa [CoInitialize function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms678543.aspx)
        ///
        com_initializer(_In_opt_ LPVOID pvReserved) noexcept
        {
            m_result = CoInitialize(pvReserved);
        }

        ///
        /// Initializes the COM library for use by the calling thread, sets the thread's concurrency model, and creates a new apartment for the thread if one is required.
        ///
        /// \sa [CoInitializeEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms695279.aspx)
        ///
        com_initializer(_In_opt_ LPVOID pvReserved, _In_ DWORD dwCoInit) noexcept
        {
            m_result = CoInitializeEx(pvReserved, dwCoInit);
        }

        ///
        /// Uninitializes COM.
        ///
        /// \sa [CoUninitialize function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms688715.aspx)
        ///
        virtual ~com_initializer()
        {
            if (SUCCEEDED(m_result))
                CoUninitialize();
        }

        ///
        /// Return result of `CoInitialize()` call.
        ///
        /// \sa [CoInitialize function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms678543.aspx)
        ///
        HRESULT status() const noexcept
        {
            return m_result;
        }

    protected:
        HRESULT m_result;   ///< Result of CoInitialize call
    };

    /// @}
}

/// \addtogroup WinStdCOM
/// @{

///
/// Creates and default-initializes a single object of the class associated with a specified CLSID
///
/// \sa [CoCreateInstance function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615.aspx)
///
template <class T>
static _Check_return_ HRESULT CoCreateInstance(_In_ REFCLSID rclsid, _In_opt_ LPUNKNOWN pUnkOuter, _In_ DWORD dwClsContext, _Inout_ winstd::com_obj<T> &v)
{
    T* ppv;
    HRESULT hr = CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (LPVOID*)&ppv);
    if (SUCCEEDED(hr))
        v.attach(ppv);
    return hr;
}

/// @}
