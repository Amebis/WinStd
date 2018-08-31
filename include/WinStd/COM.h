/*
    Copyright 1991-2017 Amebis
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
/// \defgroup WinStdCOM COM object management
/// Provides helper templates for Windows COM object manipulation
///

#include "Common.h"

#include <string>

namespace winstd
{
    template <class T> class com_obj;
    class WINSTD_API bstr;
    class WINSTD_API variant;
    class WINSTD_API com_initializer;
    class WINSTD_API com_runtime_error;
}

#pragma once


namespace winstd
{
    /// \addtogroup WinStdCOM
    /// @{

    ///
    /// COM object wrapper template
    ///
    template <class T>
    class com_obj : public dplhandle<T*>
    {
        DPLHANDLE_IMPL(com_obj)

    public:
        ///
        /// Constructs a new object and creates a new class with it
        ///
        /// \sa [CoCreateInstance function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615.aspx)
        ///
        inline com_obj(_In_ REFCLSID rclsid, _In_opt_ LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
        {
            CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (LPVOID*)&m_h);
        }


        ///
        /// Queries the object for another interface and creates new class with it
        ///
        /// \sa [IUnknown::QueryInterface method](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682521.aspx)
        ///
        template <class _Other>
        inline com_obj(_Out_ _Other **other)
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
        inline com_obj(_Out_ com_obj<_Other> &other)
        {
            assert(other);
            other->QueryInterface(__uuidof(T), (void**)&m_h);
        }


        ///
        /// Releases object
        ///
        virtual ~com_obj()
        {
            if (m_h)
                m_h->Release();
        }


        ///
        /// Creates a new object
        ///
        /// \sa [CoCreateInstance function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615.aspx)
        ///
        inline HRESULT create(_In_ REFCLSID rclsid, _In_opt_ LPUNKNOWN pUnkOuter = NULL, _In_ DWORD dwClsContext = CLSCTX_ALL)
        {
            handle_type h;
            HRESULT hr = CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&h);
            if (SUCCEEDED(hr))
                attach(h);
            return hr;
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
        virtual void free_internal()
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
        virtual handle_type duplicate_internal(_In_ handle_type h) const
        {
            h->AddRef();
            return h;
        }
    };


    ///
    /// BSTR string wrapper
    ///
    class WINSTD_API bstr : public dplhandle<BSTR>
    {
        DPLHANDLE_IMPL(bstr)

    public:
        ///
        /// Constructs BSTR from OLE string
        ///
        inline bstr(_In_ LPCOLESTR src)
        {
            m_h = SysAllocString(src);
        }

        ///
        /// Constructs BSTR from OLE string with length
        ///
        inline bstr(_In_ LPCOLESTR src, _In_ UINT len)
        {
            m_h = SysAllocStringLen(src, len);
        }

        ///
        /// Constructs BSTR from std::basic_string
        ///
        template<class _Traits, class _Ax>
        inline bstr(_In_ const std::basic_string<wchar_t, _Traits, _Ax> &src)
        {
            m_h = SysAllocStringLen(src.c_str(), (UINT)src.length());
        }

        ///
        /// Destroys the string
        ///
        /// \sa [SysFreeString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221481.aspx)
        ///
        virtual ~bstr();

        ///
        /// Returns the length of the string
        ///
        /// \sa [SysStringLen function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221240.aspx)
        ///
        inline UINT length() const
        {
            return SysStringLen(m_h);
        }

    protected:
        ///
        /// Destroys the string
        ///
        /// \sa [SysFreeString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221481.aspx)
        ///
        virtual void free_internal();

        ///
        /// Duplicates the string
        ///
        /// \param[in] h  Existing string
        ///
        /// \return Duplicated string
        ///
        /// \sa [SysAllocString function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221458.aspx)
        ///
        virtual handle_type duplicate_internal(_In_ handle_type h) const;
    };


    ///
    /// VARIANT struct wrapper
    ///
    class WINSTD_API variant : public VARIANT
    {
    public:
        ///
        /// Constructs blank VARIANT
        ///
        inline variant()
        {
            VariantInit(this);
        }

        ///
        /// Constructs VARIANT from another
        ///
        inline variant(_In_ const VARIANT& varSrc)
        {
            vt = VT_EMPTY;
            VariantCopy(this, &varSrc);
        }

        ///
        /// Moves VARIANT from another
        ///
        inline variant(_Inout_ VARIANT&& varSrc)
        {
            memcpy(this, &varSrc, sizeof(VARIANT));
            varSrc.vt = VT_EMPTY;
        }

        ///
        /// Constructs VARIANT from bool
        ///
        inline variant(_In_ bool bSrc)
        {
            vt = VT_BOOL;
            boolVal = bSrc ? VARIANT_TRUE : VARIANT_FALSE;
        }

        ///
        /// Constructs VARIANT from character
        ///
        inline variant(_In_ char cSrc)
        {
            vt = VT_I1;
            cVal = cSrc;
        }

        ///
        /// Constructs VARIANT from byte
        ///
        inline variant(_In_ unsigned char nSrc)
        {
            vt = VT_UI1;
            bVal = nSrc;
        }

        ///
        /// Constructs VARIANT from short
        ///
        inline variant(_In_ short nSrc)
        {
            vt = VT_I2;
            iVal = nSrc;
        }

        ///
        /// Constructs VARIANT from unsigned short
        ///
        inline variant(_In_ unsigned short nSrc)
        {
            vt = VT_UI2;
            uiVal = nSrc;
        }

        ///
        /// Constructs VARIANT from integer
        ///
        inline variant(_In_ int nSrc, _In_ VARTYPE vtSrc = VT_I4)
        {
            assert(vtSrc == VT_I4 || vtSrc == VT_INT);
            vt = vtSrc;
            intVal = nSrc;
        }

        ///
        /// Constructs VARIANT from unsigned integer
        ///
        inline variant(_In_ unsigned int nSrc, _In_ VARTYPE vtSrc = VT_UI4)
        {
            assert(vtSrc == VT_UI4 || vtSrc == VT_UINT);
            vt = vtSrc;
            uintVal= nSrc;
        }

        ///
        /// Constructs VARIANT from long
        ///
        inline variant(_In_ long nSrc, _In_ VARTYPE vtSrc = VT_I4)
        {
            assert(vtSrc == VT_I4 || vtSrc == VT_ERROR);
            vt = vtSrc;
            lVal = nSrc;
        }

        ///
        /// Constructs VARIANT from unsigned long
        ///
        inline variant(_In_ unsigned long nSrc)
        {
            vt = VT_UI4;
            ulVal = nSrc;
        }

        ///
        /// Constructs VARIANT from float
        ///
        inline variant(_In_ float fltSrc)
        {
            vt = VT_R4;
            fltVal = fltSrc;
        }

        ///
        /// Constructs VARIANT from double or variant date
        ///
        inline variant(_In_ double dblSrc, _In_ VARTYPE vtSrc = VT_R8)
        {
            assert(vtSrc == VT_R8 || vtSrc == VT_DATE);
            vt = vtSrc;
            dblVal = dblSrc;
        }

        ///
        /// Constructs VARIANT from 64-bit integer
        ///
        inline variant(_In_ long long nSrc)
        {
            vt = VT_I8;
            llVal = nSrc;
        }

        ///
        /// Constructs VARIANT from unsigned integer
        ///
        inline variant(_In_ unsigned long long nSrc)
        {
            vt = VT_UI8;
            ullVal = nSrc;
        }

        ///
        /// Constructs VARIANT from CY (64-bit integer)
        ///
        inline variant(_In_ CY cySrc)
        {
            vt = VT_CY;
            cyVal.Hi = cySrc.Hi;
            cyVal.Lo = cySrc.Lo;
        }

        ///
        /// Constructs VARIANT from OLE string
        ///
        inline variant(_In_z_ LPCOLESTR lpszSrc)
        {
            vt = VT_EMPTY;
            *this = lpszSrc;
        }

        ///
        /// Constructs VARIANT from BSTR
        ///
        inline variant(_In_z_ BSTR bstr)
        {
            vt = VT_EMPTY;
            *this = bstr;
        }

        ///
        /// Constructs VARIANT from IDispatch
        ///
        inline variant(_In_opt_ IDispatch* pSrc)
        {
            vt = VT_DISPATCH;
            pdispVal = pSrc;

            if (pdispVal != NULL)
                pdispVal->AddRef();
        }

        ///
        /// Constructs VARIANT from IUnknown
        ///
        inline variant(_In_opt_ IUnknown* pSrc)
        {
            vt = VT_UNKNOWN;
            punkVal = pSrc;

            if (punkVal != NULL)
                punkVal->AddRef();
        }

        ///
        /// Constructs VARIANT from SAFEARRAY
        ///
        inline variant(_In_ const SAFEARRAY *pSrc)
        {
            assert(pSrc != NULL);

            LPSAFEARRAY pCopy;
            HRESULT hRes = SafeArrayCopy((LPSAFEARRAY)pSrc, &pCopy);
            if (SUCCEEDED(hRes)) {
                SafeArrayGetVartype((LPSAFEARRAY)pSrc, &vt);
                vt |= VT_ARRAY;
                parray = pCopy;
            } else
                assert(0);
        }

        ///
        /// Destroys VARIANT
        ///
        virtual ~variant();

        ///
        /// Copy from another VARIANT
        ///
        inline variant& operator=(_In_ const VARIANT& varSrc)
        {
            if (this != &varSrc)
                VariantCopy(this, &varSrc);
            return *this;
        }

        ///
        /// Moves from another VARIANT
        ///
        inline variant& operator=(_Inout_ VARIANT&& varSrc)
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
        inline variant& operator=(_In_ bool bSrc)
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
        inline variant& operator=(_In_ char cSrc)
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
        inline variant& operator=(_In_ unsigned char nSrc)
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
        inline variant& operator=(_In_ short nSrc)
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
        inline variant& operator=(_In_ unsigned short nSrc)
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
        inline variant& operator=(_In_ int nSrc)
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
        inline variant& operator=(_In_ unsigned int nSrc)
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
        inline variant& operator=(_In_ long nSrc)
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
        inline variant& operator=(_In_ unsigned long nSrc)
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
        inline variant& operator=(_In_ long long nSrc)
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
        inline variant& operator=(_In_ unsigned long long nSrc)
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
        inline variant& operator=(_In_ float fltSrc)
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
        inline variant& operator=(_In_ double dblSrc)
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
        inline variant& operator=(_In_ CY cySrc)
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
        inline variant& operator=(_In_z_ LPCOLESTR lpszSrc)
        {
            VariantClear(this);
            vt = VT_BSTR;
            bstrVal = SysAllocString(lpszSrc);
            return *this;
        }

        ///
        /// Copy from IDispatch
        ///
        inline variant& operator=(_Inout_opt_ IDispatch* pSrc)
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
        inline variant& operator=(_Inout_opt_ IUnknown* pSrc)
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
        inline variant& operator=(_In_ unsigned char* pbSrc)
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
        inline variant& operator=(_In_ short* pnSrc)
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
        inline variant& operator=(_In_ unsigned short* pnSrc)
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
        inline variant& operator=(_In_ int* pnSrc)
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
        inline variant& operator=(_In_ unsigned int* pnSrc)
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
        inline variant& operator=(_In_ long* pnSrc)
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
        inline variant& operator=(_In_ unsigned long* pnSrc)
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
        inline variant& operator=(_In_ long long* pnSrc)
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
        inline variant& operator=(_In_ unsigned long long* pnSrc)
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
        inline variant& operator=(_In_ float* pfSrc)
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
        inline variant& operator=(_In_ double* pfSrc)
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
        inline variant& operator=(_In_ const SAFEARRAY *pSrc)
        {
            assert(pSrc != NULL);
            VariantClear(this);

            LPSAFEARRAY pCopy;
            HRESULT hr = SafeArrayCopy((LPSAFEARRAY)pSrc, &pCopy);
            if (SUCCEEDED(hr)) {
                SafeArrayGetVartype((LPSAFEARRAY)pSrc, &vt);
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
        inline bool operator==(_In_ const VARIANT& varSrc) const
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
        inline bool operator!=(_In_ const VARIANT& varSrc) const
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
        inline bool operator<(_In_ const VARIANT& varSrc) const
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
        inline bool operator>(_In_ const VARIANT& varSrc) const
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
        inline bool operator<=(_In_ const VARIANT& varSrc) const
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
        inline bool operator>=(_In_ const VARIANT& varSrc) const
        {
            return !operator<(varSrc);
        }

        ///
        /// Converts a variant from one type to another.
        ///
        /// \sa [VariantChangeType function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms221258.aspx)
        ///
        inline HRESULT change_type(_In_ VARTYPE _vt, _In_opt_ USHORT wFlags = 0)
        {
            return VariantChangeType(this, this, wFlags, _vt);
        }

    private:
        /// \cond internal
        inline HRESULT compare(_In_ const VARIANT &varLeft, _In_ const VARIANT &varRight, _In_ LCID lcid, _In_ ULONG dwFlags) const
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


    ///
    /// Context scope automatic COM (un)initialization
    ///
    class WINSTD_API com_initializer
    {
    public:
        ///
        /// Initializes the COM library on the current thread and identifies the concurrency model as single-thread apartment (STA).
        ///
        /// \sa [CoInitialize function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms678543.aspx)
        ///
        inline com_initializer(_In_opt_ LPVOID pvReserved)
        {
            m_result = CoInitialize(pvReserved);
        }


        ///
        /// Initializes the COM library for use by the calling thread, sets the thread's concurrency model, and creates a new apartment for the thread if one is required.
        ///
        /// \sa [CoInitializeEx function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms695279.aspx)
        ///
        inline com_initializer(_In_opt_ LPVOID pvReserved, _In_ DWORD dwCoInit)
        {
            m_result = CoInitializeEx(pvReserved, dwCoInit);
        }


        ///
        /// Uninitializes COM.
        ///
        /// \sa [CoUninitialize function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms688715.aspx)
        ///
        virtual ~com_initializer();


        ///
        /// Return result of `CoInitialize()` call.
        ///
        /// \sa [CoInitialize function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms678543.aspx)
        ///
        inline HRESULT status() const
        {
            return m_result;
        }

    protected:
        HRESULT m_result;   ///< Result of CoInitialize call
    };

    /// @}

    ///
    /// \defgroup WinStdExceptions Exceptions
    /// Additional exceptions
    ///
    /// @{

    ///
    /// COM runtime error
    ///
    /// \note Must be defined as derived class from num_runtime_error<> to allow correct type info for dynamic typecasting and prevent folding with other derivates of num_runtime_error<>.
    ///
    class WINSTD_API com_runtime_error : public num_runtime_error<HRESULT>
    {
    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] num  COM error code
        /// \param[in] msg  Error message
        ///
        inline com_runtime_error(_In_ error_type num, _In_ const std::string& msg) : num_runtime_error<HRESULT>(num, msg.c_str())
        {
        }


        ///
        /// Constructs an exception
        ///
        /// \param[in] num  COM error code
        /// \param[in] msg  Error message
        ///
        inline com_runtime_error(_In_ error_type num, _In_z_ const char *msg) : num_runtime_error<HRESULT>(num, msg)
        {
        }


        ///
        /// Copies an exception
        ///
        /// \param[in] other  Exception to copy from
        ///
        inline com_runtime_error(const com_runtime_error &other) : num_runtime_error<HRESULT>(other)
        {
        }
    };

    /// @}
}
