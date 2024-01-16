/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2024 Amebis
    Copyright © 2016 GÉANT
*/

/// \defgroup WinStdCOM COM Object Management

#pragma once

#include "Common.h"
#include <assert.h>
#include <unknwn.h>
#include <stdexcept>
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
        {}

        ///
        /// Constructs an exception
        ///
        /// \param[in] num  COM error code
        /// \param[in] msg  Error message
        ///
        com_runtime_error(_In_ error_type num, _In_opt_z_ const char *msg = nullptr) : num_runtime_error<HRESULT>(num, msg)
        {}
    };

    /// @}

    /// \addtogroup WinStdCOM
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
        WINSTD_DPLHANDLE_IMPL(com_obj, T*, NULL)

    public:
        ///
        /// Creates a new instance of a class
        ///
        /// \sa [CoCreateInstance function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615.aspx)
        ///
        com_obj(
            _In_ REFCLSID rclsid,
            _In_opt_ LPUNKNOWN pUnkOuter,
            _In_ DWORD dwClsContext)
        {
            HRESULT hr = CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (LPVOID*)&m_h);
            if (FAILED(hr))
                throw com_runtime_error(hr, "CoCreateInstance failed");
        }

        ///
        /// Queries the object for another interface and creates new class with it
        ///
        /// \sa [IUnknown::QueryInterface method](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682521.aspx)
        ///
        template <class _Other>
        com_obj(_In_ _Other *other)
        {
            assert(other);
            HRESULT hr = other->QueryInterface(__uuidof(T), (void**)&m_h);
            if (FAILED(hr))
                throw com_runtime_error(hr, "QueryInterface failed");
        }

        ///
        /// Queries the object for another interface and creates new class with it
        ///
        /// \sa [IUnknown::QueryInterface method](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682521.aspx)
        ///
        template <class _Other>
        com_obj(_In_ com_obj<_Other> &other)
        {
            HRESULT hr = other->QueryInterface(__uuidof(T), (void**)&m_h);
            if (FAILED(hr))
                throw com_runtime_error(hr, "QueryInterface failed");
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
        T* duplicate_internal(_In_ T* h) const override
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
        WINSTD_DPLHANDLE_IMPL(bstr, BSTR, NULL)

    public:
        ///
        /// Constructs BSTR from OLE string
        ///
        bstr(_In_opt_z_ LPCOLESTR src)
        {
            m_h = SysAllocString(src);
            if (!m_h)
                throw std::bad_alloc();
        }

        ///
        /// Constructs BSTR from OLE string with length
        ///
        bstr(_In_reads_opt_(len) LPCOLESTR src, _In_ UINT len)
        {
            m_h = SysAllocStringLen(src, len);
            if (!m_h)
                throw std::bad_alloc();
        }

        ///
        /// Constructs BSTR from std::basic_string
        ///
        template<class _Traits, class _Ax>
        bstr(_In_ const std::basic_string<OLECHAR, _Traits, _Ax> &src)
        {
            size_t len = src.length();
            if (len > UINT_MAX)
                throw std::invalid_argument("string too long");
            m_h = SysAllocStringLen(src.c_str(), static_cast<UINT>(len));
            if (!m_h)
                throw std::bad_alloc();
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
        handle_type duplicate_internal(_In_ handle_type h) const override
        {
            handle_type h_new = SysAllocStringLen(h, SysStringLen(h));
            if (h_new != invalid)
                return h_new;
            throw std::bad_alloc();
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
                throw winstd::com_runtime_error(hr, "VariantCopy failed");
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
                throw winstd::com_runtime_error(hr, "SafeArrayCopy failed");

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
                    throw winstd::com_runtime_error(hr, "VariantCopy failed");
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
        variant& operator=(_In_ const SAFEARRAY *pSrc)
        {
            assert(pSrc != NULL);
            VariantClear(this);

            LPSAFEARRAY pCopy;
            const HRESULT hr = SafeArrayCopy(const_cast<LPSAFEARRAY>(pSrc), &pCopy);
            if (SUCCEEDED(hr)) {
                SafeArrayGetVartype(const_cast<LPSAFEARRAY>(pSrc), &vt);
                vt |= VT_ARRAY;
                parray = pCopy;
                return *this;
            }
            throw com_runtime_error(hr, "SafeArrayCopy failed");
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
    /// SAFEARRAY string wrapper
    ///
    class safearray : public dplhandle<SAFEARRAY*, NULL>
    {
        WINSTD_DPLHANDLE_IMPL(safearray, SAFEARRAY*, NULL)

    public:
        ///
        /// Destroys the array
        ///
        /// \sa [SafeArrayDestroy function](https://learn.microsoft.com/en-us/windows/win32/api/oleauto/nf-oleauto-safearraydestroy)
        ///
        virtual ~safearray()
        {
            if (m_h != invalid)
                free_internal();
        }

    protected:
        ///
        /// Destroys the array
        ///
        /// \sa [SafeArrayDestroy function](https://learn.microsoft.com/en-us/windows/win32/api/oleauto/nf-oleauto-safearraydestroy)
        ///
        void free_internal() noexcept override
        {
            SafeArrayDestroy(m_h);
        }

        ///
        /// Duplicates the array
        ///
        /// \param[in] h  Existing array
        ///
        /// \return Duplicated array
        ///
        /// \sa [SysAllocString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221458.aspx)
        ///
        handle_type duplicate_internal(_In_ handle_type h) const override
        {
            handle_type h_new;
            HRESULT hr = SafeArrayCopy(h, &h_new);
            if (SUCCEEDED(hr))
                return h_new;
            throw com_runtime_error(hr, "SafeArrayCopy failed");
        }
    };

    ///
    /// Context scope automatic SAFEARRAY (un)access
    ///
    template <class T>
    class safearray_accessor
    {
        WINSTD_NONCOPYABLE(safearray_accessor)
        WINSTD_NONMOVABLE(safearray_accessor)

    public:
        ///
        /// Increments the lock count of an array, and retrieves a pointer to the array data.
        ///
        /// \sa [SafeArrayAccessData function](https://learn.microsoft.com/en-us/windows/win32/api/oleauto/nf-oleauto-safearrayaccessdata)
        ///
        safearray_accessor(_In_ SAFEARRAY* psa) : m_sa(psa)
        {
            HRESULT hr = SafeArrayAccessData(psa, (void HUGEP**)&m_data);
            if (FAILED(hr))
                throw com_runtime_error(hr, "SafeArrayAccessData failed");
        }

        ///
        /// Decrements the lock count of an array.
        ///
        /// \sa [CoUninitialize function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms688715.aspx)
        ///
        virtual ~safearray_accessor()
        {
            SafeArrayUnaccessData(m_sa);
        }

        ///
        /// Return SAFEARRAY data pointer.
        ///
        T HUGEP* data() const noexcept
        {
            return m_data;
        }

    protected:
        SAFEARRAY* m_sa; ///< SAFEARRAY
        T HUGEP* m_data; ///< SAFEARRAY data
    };

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
        com_initializer(_In_opt_ LPVOID pvReserved)
        {
            HRESULT hr = CoInitialize(pvReserved);
            if (FAILED(hr))
                throw com_runtime_error(hr, "CoInitialize failed");
        }

        ///
        /// Initializes the COM library for use by the calling thread, sets the thread's concurrency model, and creates a new apartment for the thread if one is required.
        ///
        /// \sa [CoInitializeEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms695279.aspx)
        ///
        com_initializer(_In_opt_ LPVOID pvReserved, _In_ DWORD dwCoInit)
        {
            HRESULT hr = CoInitializeEx(pvReserved, dwCoInit);
            if (FAILED(hr))
                throw com_runtime_error(hr, "CoInitializeEx failed");
        }

        ///
        /// Uninitializes COM.
        ///
        /// \sa [CoUninitialize function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms688715.aspx)
        ///
        virtual ~com_initializer()
        {
            CoUninitialize();
        }
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

///
/// Converts a display name into a moniker that identifies the object named, and then binds to the object identified by the moniker.
///
/// \sa [CoGetObject function](https://learn.microsoft.com/en-us/windows/win32/api/objbase/nf-objbase-cogetobject)
///
template <class T>
static _Check_return_ HRESULT CoGetObject(_In_ LPCWSTR pszName, _In_opt_ BIND_OPTS* pBindOptions, _In_ REFIID riid, _Inout_ winstd::com_obj<T>& v)
{
    T* ppv;
    HRESULT hr = CoGetObject(pszName, pBindOptions, riid, (LPVOID*)&ppv);
    if (SUCCEEDED(hr))
        v.attach(ppv);
    return hr;
}

/// @}

/// \addtogroup WinStdCOMHelpers
/// @{

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ IDispatch* value)
{
    V_VT(&v) = VT_DISPATCH;
    V_DISPATCH(&v) = value;
    value->AddRef();
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const CHAR value)
{
    V_VT(&v) = VT_I1;
    V_I1(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const BYTE value)
{
    V_VT(&v) = VT_UI1;
    V_UI1(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const SHORT value)
{
    V_VT(&v) = VT_I2;
    V_I2(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const USHORT value)
{
    V_VT(&v) = VT_UI2;
    V_UI2(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const LONG value)
{
    V_VT(&v) = VT_I4;
    V_I4(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const ULONG value)
{
    V_VT(&v) = VT_UI4;
    V_UI4(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const LONGLONG value)
{
    V_VT(&v) = VT_I8;
    V_I8(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const ULONGLONG value)
{
    V_VT(&v) = VT_UI8;
    V_UI8(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const FLOAT value)
{
    V_VT(&v) = VT_R4;
    V_R4(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const DOUBLE value)
{
    V_VT(&v) = VT_R8;
    V_R8(&v) = value;
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const BOOL value)
{
    return v << (value ? VARIANT_TRUE : VARIANT_FALSE);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ LPCOLESTR value)
{
    V_VT(&v) = VT_BSTR;
    V_BSTR(&v) = SysAllocString(value);
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
template<class _Traits, class _Ax>
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const std::basic_string<OLECHAR, _Traits, _Ax>& value)
{
    if (value.size() > UINT_MAX)
        throw std::invalid_argument("string too long");
    V_VT(&v) = VT_BSTR;
    V_BSTR(&v) = SysAllocStringLen(value.data(), static_cast<UINT>(value.size()));
    return *(&v + 1);
}

///
/// Saves value to VARIANT
///
/// \note This operator saves GUID as BSTR string in "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}" notation.
///
/// \param[out] v      VARIANT to save value to. For performance reasons, no VariantClear is called before saving. Thus, v must not contain referenced data.
/// \param[in]  value  Value to save
///
/// \return Reference to VARIANT successor in array of VARIANT(s)
///
inline VARIANT& operator <<(_Out_ VARIANT& v, _In_ const GUID& value)
{
    OLECHAR guid_niz[38 + 1];
    V_VT(&v) = VT_BSTR;
    V_BSTR(&v) = SysAllocStringLen(guid_niz, _swprintf_s_l(
        guid_niz, _countof(guid_niz),
        L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        NULL,
        value.Data1,
        value.Data2,
        value.Data3,
        value.Data4[0], value.Data4[1],
        value.Data4[2], value.Data4[3], value.Data4[4], value.Data4[5], value.Data4[6], value.Data4[7]));
    return *(&v + 1);
}

/// @}

namespace winstd
{
    /// \addtogroup WinStdCOMHelpers
    /// @{

    ///
    /// Builds VBARRAY of uniform data
    ///
    /// \param[in] vt       Type of array element
    /// \param[in] array    Pointer to data
    /// \param[in] columns  Number of columns. When 1, SafeArrayCreateVector is used; when >1, SafeArrayCreate is used.
    /// \param[in] rows     Number of rows
    ///
    /// \return Returns VBARRAY
    ///
    inline VARIANT BuildVBARRAY(_In_ VARTYPE vt, _In_opt_ LPCVOID array, _In_ ULONG columns, _In_ ULONG rows)
    {
        LPSAFEARRAY sa;
        size_t n;
        if (columns == 1) {
            // Make vector when one column only.
            sa = SafeArrayCreateVector(VT_VARIANT, 0, rows);
            n = rows;
        }
        else {
            // Make 2-dimensional array when more columns.
            SAFEARRAYBOUND dim[2] = {
                { columns, 0 },
                { rows, 0 }
            };
            sa = SafeArrayCreate(VT_VARIANT, 2, dim);
            n = columns * rows;
        }
        if (!sa)
            throw std::bad_alloc();
        assert(array || !n);

        // Support VARIANT types that may be used for SAFEARRAY
        // Source: https://learn.microsoft.com/en-us/windows/win32/api/wtypes/ne-wtypes-varenum#remarks
        size_t delta_src;
        VARIANT* v = NULL;
        switch (vt) {
        case VT_BOOL: delta_src = sizeof(V_BOOL(v)); break;
        case VT_BSTR: delta_src = sizeof(V_BSTR(v)); break;
        case VT_CY: delta_src = sizeof(V_CY(v)); break;
        case VT_DATE: delta_src = sizeof(V_DATE(v)); break;
        case VT_DECIMAL: delta_src = sizeof(V_DECIMAL(v)); break;
        case VT_DISPATCH: delta_src = sizeof(V_DISPATCH(v)); break;
        case VT_ERROR: delta_src = sizeof(V_ERROR(v)); break;
        case VT_I1: delta_src = sizeof(V_I1(v)); break;
        case VT_I2: delta_src = sizeof(V_I2(v)); break;
        case VT_I4: delta_src = sizeof(V_I4(v)); break;
        case VT_INT: delta_src = sizeof(V_INT(v)); break;
        case VT_R4: delta_src = sizeof(V_R4(v)); break;
        case VT_R8: delta_src = sizeof(V_R8(v)); break;
        case VT_RECORD: delta_src = sizeof(V_RECORD(v)); break;
        case VT_UI1: delta_src = sizeof(V_UI1(v)); break;
        case VT_UI2: delta_src = sizeof(V_UI2(v)); break;
        case VT_UI4: delta_src = sizeof(V_UI4(v)); break;
        case VT_UINT: delta_src = sizeof(V_UINT(v)); break;
        case VT_UNKNOWN: delta_src = sizeof(V_UNKNOWN(v)); break;
        default: throw std::invalid_argument("unsupported VARIANT type");
        };
        size_t delta_dst = SafeArrayGetElemsize(sa);

        {
            safearray_accessor<BYTE> ssa(sa);
            LPBYTE dst = ssa.data();
            LPCBYTE src = reinterpret_cast<LPCBYTE>(array);
            for (size_t i = 0; i < n; ++i, src += delta_src, dst += delta_dst) {
                v = reinterpret_cast<VARIANT*>(dst);

                // No VariantInit, since SafeArrayCreate(Vector) zero-initializes all elements.
                // VariantInit(v);
                V_VT(v) = vt;
                memcpy(&(v->byref), src, delta_src);
            }
        }

        VARIANT var;
        V_VT(&var) = VT_ARRAY | VT_VARIANT;
        V_ARRAY(&var) = sa;
        return var;
    }

    ///
    /// Builds VBARRAY of uniform data
    ///
    /// This template is using operator <<(VARIANT&, const T&) to write data to VBARRAY.
    /// Such operator must be declared for given datatype T.
    ///
    /// \tparam T        Element type
    /// \tparam columns  Number of columns. When 1, SafeArrayCreateVector is used; when >1, SafeArrayCreate is used.
    ///
    /// \param array     Pointer to data
    /// \param rows      Number of rows of data
    ///
    /// \return Returns VBARRAY
    ///
    template <class T, ULONG columns = 1>
    VARIANT BuildVBARRAY(_In_reads_opt_(rows) const T* array, _In_ ULONG rows)
    {
        assert(array || !rows);

        LPSAFEARRAY sa;
        if constexpr (columns == 1) {
            // Make vector when one column only.
            sa = SafeArrayCreateVector(VT_VARIANT, 0, rows);
        }
        else {
            // Make 2-dimensional array when more columns.
            SAFEARRAYBOUND dim[2] = {
                { columns, 0 },
                { rows, 0 }
            };
            sa = SafeArrayCreate(VT_VARIANT, _countof(dim), dim);
        }
        if (!sa)
            throw std::bad_alloc();

        assert(SafeArrayGetElemsize(sa) == sizeof(VARIANT));

        {
            safearray_accessor<VARIANT> ssa(sa);
            VARIANT* dst = ssa.data();
            for (size_t i = 0; i < rows; ++i) {
                VARIANT* dst_next = &(*dst << array[i]);
                assert(dst + columns == dst_next);
                dst = dst_next;
            }
        }

        VARIANT var;
        V_VT(&var) = VT_ARRAY | VT_VARIANT;
        V_ARRAY(&var) = sa;
        return var;
    }

    ///
    /// Builds VBARRAY containing BMP image
    ///
    /// \param[in] dc    Drawing context
    /// \param[in] rows  Bitmap handle
    ///
    /// \return Returns VBARRAY
    ///
    inline VARIANT BuildVBARRAY(_In_ HDC dc, _In_ HBITMAP pic)
    {
        // Get picture parameters.
        BITMAP bmp;
        GetObject(pic, sizeof(bmp), reinterpret_cast<LPSTR>(&bmp));

        // Estimate file parameters.
        BITMAPINFOHEADER bmh = { sizeof(bmh) };
        GetDIBits(dc, pic, 0, bmp.bmHeight, NULL, reinterpret_cast<LPBITMAPINFO>(&bmh), DIB_RGB_COLORS);

        // Allocate.
        size_t pallete_size = sizeof(RGBQUAD) * bmh.biClrUsed;
        LPSAFEARRAY sa = SafeArrayCreateVector(VT_UI1, 0, static_cast<ULONG>(sizeof(BITMAPFILEHEADER) + sizeof(bmh) + pallete_size + bmh.biSizeImage));
        if (!sa)
            throw std::bad_alloc();

        // Locate BITMAPFILEHEADER, BITMAPINFO and pixel map.
        safearray_accessor<BYTE> ssa(sa);
        auto header = reinterpret_cast<LPBITMAPFILEHEADER>(ssa.data());
        auto info = reinterpret_cast<LPBITMAPINFO>(ssa.data() + sizeof(*header));
        auto raster = ssa.data() + sizeof(*header) + sizeof(bmh) + pallete_size;

        // Fill in BITMAPFILEHEADER.
        memset(header, 0, sizeof(*header));
#pragma warning(push)
#pragma warning(disable: 6276) // "BM" is not an UTF16 char.
        header->bfType = *reinterpret_cast<WORD*>("BM");
#pragma warning(pop)
        header->bfSize = static_cast<DWORD>(sizeof(*header) + sizeof(bmh) + pallete_size + bmh.biSizeImage);
        header->bfOffBits = static_cast<DWORD>(sizeof(*header) + sizeof(bmh) + pallete_size);

        // Fill in BITMAPINFO.
        memcpy(&(info->bmiHeader), &bmh, sizeof(bmh));
        memset(&(info->bmiColors), 0, pallete_size);

        // Set pallete and pixel map.
        GetDIBits(dc, pic, 0, bmp.bmHeight, raster, info, DIB_RGB_COLORS);

        VARIANT var;
        V_VT(&var) = VT_ARRAY | VT_UI1;
        V_ARRAY(&var) = sa;
        return var;
    }

    ///
    /// Calls IDispatch::Invoke
    ///
    /// \tparam T  Original interface type
    ///
    /// \param[in] cp      Any interface of the class that implements IDispatch
    /// \param[in] id      ID of method to invoke
    /// \param[in] param   Parameters for the method to invoke
    /// \param[in] locale  Locale for the invoke call
    ///
    template <class T>
    void IDispatchInvoke(_In_ T* cp, _In_ DISPID id, _In_opt_ DISPPARAMS* param, _In_ LCID locale = LOCALE_USER_DEFAULT)
    {
        assert(cp);
        com_obj<IEnumConnections> e;
        HRESULT hr = cp->EnumConnections(&e);
        if (FAILED(hr))
            throw com_runtime_error(hr, "IDispatch::EnumConnections failed");

        CONNECTDATA cd;
        while (e->Next(1, &cd, NULL) == S_OK) {
            com_obj<IDispatch> d(cd.pUnk);
            d->Invoke(id, IID_NULL, locale, DISPATCH_METHOD, param, NULL, NULL, NULL);
        }
    }

    ///
    /// Check VARIANT value for integer value
    ///
    /// \tparam T        Integral type
    ///
    /// \param[in] var       Pointer to VARIANT value
    /// \param[in] fallback  Value to return, for non-integral types.
    ///
    /// \return Value of VARIANT if integral; fallback otherwise.
    ///
    template <class T>
    T VariantAsInteger(_In_ const VARIANT* var, _In_ T fallback = 0)
    {
        assert(var);
        switch (V_VT(var)) {
        case VT_UINT_PTR: return static_cast<T>(V_UINT_PTR(var));
        case VT_INT_PTR: return static_cast<T>(V_INT_PTR(var));
        case VT_UINT: return static_cast<T>(V_UINT(var));
        case VT_INT: return static_cast<T>(V_INT(var));
        case VT_UI4: return static_cast<T>(V_UI4(var));
        case VT_I4: return static_cast<T>(V_I4(var));
        case VT_UI2: return static_cast<T>(V_UI2(var));
        case VT_I2: return static_cast<T>(V_I2(var));
        case VT_UI1: return static_cast<T>(V_UI1(var));
        case VT_I1: return static_cast<T>(V_I1(var));
        }
        return fallback;
    }

    ///
    /// Check VARIANT value for boolean value
    ///
    /// \param[in] var       Pointer to VARIANT value
    /// \param[in] fallback  Value to return, for non-boolean and non-numeric VARIANT types.
    ///
    /// \return TRUE if var is VT_BOOL and not VARIANT_FALSE, or VT_<numeric> and not zero; FALSE if var is VT_BOOL and VARIANT_FALSE, or VT_<numeric> and zero; fallback otherwise.
    ///
    inline BOOL VariantAsBoolean(_In_ const VARIANT* var, _In_ BOOL fallback = FALSE)
    {
        assert(var);
        switch (V_VT(var)) {
        case VT_BOOL: return V_BOOL(var) != VARIANT_FALSE;
        case VT_UI4: return V_UI4(var) != 0;
        case VT_I4: return V_I4(var) != 0;
        case VT_UI2: return V_UI2(var) != 0;
        case VT_I2: return V_I2(var) != 0;
        case VT_UI1: return V_UI1(var) != 0;
        case VT_I1: return V_I1(var) != 0;
        case VT_UINT: return V_UINT(var) != 0;
        case VT_INT: return V_INT(var) != 0;
        case VT_UINT_PTR: return V_UINT_PTR(var) != 0;
        case VT_INT_PTR: return V_INT_PTR(var) != 0;
        }
        return fallback;
    }

    /// @}
}