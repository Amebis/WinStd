/*
    Copyright 1991-2016 Amebis
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

#pragma once

#include <Windows.h>

#include <assert.h>
#include <stdarg.h>

#include <memory>
#include <string>


///
/// Public function calling convention
///
#if defined(WINSTD_DLL)
#define WINSTD_API      __declspec(dllexport)
#elif defined(WINSTD_DLLIMP)
#define WINSTD_API      __declspec(dllimport)
#else
#define WINSTD_API
#endif


#ifndef WINSTD_STACK_BUFFER_BYTES
///
/// Size of the stack buffer in bytes used for initial system function call
///
/// Some system functions with variable length output data fail for
/// insufficient buffer sizes, and return an exact buffer length required.
/// The function helpers use a fixed size stack buffer first. If the stack
/// buffer really prooved sufficient, the helper allocates the exact length
/// output on heap and copies the data without calling the system function
/// again. Otherwise it allocates the exact length output on heap and retries.
///
/// \note
/// Decrease this value in case of stack overflow.
///
#define WINSTD_STACK_BUFFER_BYTES  1024
#endif


// Do not use _vsnprintf_s/_vsnwprintf_s(), since it terminates string by force even when we explicitly want to write unterminated string.
// Threfore turn off compiler warning instead. ;)
#pragma warning(push)
#pragma warning(disable: 4995)
#pragma warning(disable: 4996)

inline int vsnprintf(_Out_z_cap_(capacity) char *str, _In_ size_t capacity, _In_z_ _Printf_format_string_ const char *format, _In_ va_list arg)
{
    return _vsnprintf(str, capacity, format, arg);
}


inline int vsnprintf(_Out_z_cap_(capacity) wchar_t *str, _In_ size_t capacity, _In_z_ _Printf_format_string_ const wchar_t *format, _In_ va_list arg)
{
    return _vsnwprintf(str, capacity, format, arg);
}


///
/// Formats string using `printf()`.
///
/// \param[out] str     Formatted string
/// \param[in ] format  String template using `printf()` style
/// \param[in ] arg     Arguments to `format`
///
/// \returns Number of characters in result.
///
template<class _Elem, class _Traits, class _Ax>
inline int vsprintf(_Out_ std::basic_string<_Elem, _Traits, _Ax> &str, _In_z_ _Printf_format_string_ const _Elem *format, _In_ va_list arg)
{
    assert(0); // TODO: Test this code.

    _Elem buf[WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem)];

    // Try with stack buffer first.
    int count = vsnprintf(buf, _countof(buf) - 1, format, arg);
    if (count >= 0) {
        // Copy from stack.
        str.assign(buf, count);
    } else {
        for (size_t capacity = 2*WINSTD_STACK_BUFFER_BYTES/sizeof(_Elem);; capacity *= 2) {
            // Allocate on heap and retry.
            auto buf = std::unique_ptr<_Elem[]>(new _Elem[capacity]);
            count = vsnprintf(buf.get(), capacity - 1, format, arg);
            if (count >= 0) {
                str.assign(buf.get(), count);
                break;
            }
        }
    }

    return count;
}

#pragma warning(pop)


///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStrV(_In_ LPCSTR lpOutputString, _In_ va_list arg)
{
    std::string str;
    vsprintf(str, lpOutputString, arg);
    OutputDebugStringA(str.c_str());
}


///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStrV(_In_ LPCWSTR lpOutputString, _In_ va_list arg)
{
    std::wstring str;
    vsprintf(str, lpOutputString, arg);
    OutputDebugStringW(str.c_str());
}


///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStr(_In_ LPCSTR lpOutputString, ...)
{
    va_list arg;
    va_start(arg, lpOutputString);
    OutputDebugStrV(lpOutputString, arg);
    va_end(arg);
}


///
/// Formats and sends a string to the debugger for display.
///
/// \sa [OutputDebugString function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363362.aspx)
///
inline VOID OutputDebugStr(_In_ LPCWSTR lpOutputString, ...)
{
    va_list arg;
    va_start(arg, lpOutputString);
    OutputDebugStrV(lpOutputString, arg);
    va_end(arg);
}


///
/// Formats string using `printf()`.
///
/// \param[out] str     Formatted string
/// \param[in ] format  String template using `printf()` style
///
/// \returns Number of characters in result.
///
template<class _Elem, class _Traits, class _Ax>
inline int sprintf(_Out_ std::basic_string<_Elem, _Traits, _Ax> &str, _In_z_ _Printf_format_string_ const _Elem *format, ...)
{
    va_list arg;
    va_start(arg, format);
    int res = vsprintf(str, format, arg);
    va_end(arg);
    return res;
}


namespace winstd
{
#ifdef _UNICODE
    typedef std::wstring tstring;
#else
    typedef std::string tstring;
#endif


    ///
    /// Deleter for unique_ptr using LocalFree
    ///
    template <class _Ty> struct LocalFree_delete
    {
        typedef LocalFree_delete<_Ty> _Myt;

        ///
        /// Default construct
        ///
        LocalFree_delete() {}

        ///
        /// Construct from another LocalFree_delete
        ///
        template <class _Ty2> LocalFree_delete(const LocalFree_delete<_Ty2>&) {}

        ///
        /// Delete a pointer
        ///
        void operator()(_Ty *_Ptr) const
        {
            LocalFree(_Ptr);
        }
    };


    ///
    /// Deleter for unique_ptr to array of unknown size using LocalFree
    ///
    template <class _Ty> struct LocalFree_delete<_Ty[]>
    {
        typedef LocalFree_delete<_Ty> _Myt;

        ///
        /// Default construct
        ///
        LocalFree_delete() {}

        ///
        /// Delete a pointer
        ///
        void operator()(_Ty *_Ptr) const
        {
            LocalFree(_Ptr);
        }

        ///
        /// Delete a pointer of another type
        ///
        template<class _Other>
        void operator()(_Other *) const
        {
            LocalFree(_Ptr);
        }
    };


    ///
    /// \defgroup WinStdSysHandles System Handles
    /// Simplifies work with object handles of various type
    ///
    /// @{

    ///
    /// Base abstract template class to support generic object handle keeping
    ///
    /// It provides basic operators and methods common to all descendands of this class establishing a base to ease the replacement of native object handle type with classes in object-oriented approach.
    ///
    template <class T> class handle
    {
    public:
        ///
        /// Datatype of the object handle this template class handles
        ///
        typedef T handle_type;

        ///
        /// Initializes a new class instance with the object handle set to NULL.
        ///
        inline handle() : m_h(NULL)
        {
        }

        ///
        /// Initializes a new class instance with an available object handle.
        ///
        /// \param[in] h  Initial object handle value
        ///
        inline handle(_In_opt_ handle_type h) : m_h(h)
        {
        }

        /// \name Operators for transparent usage of this class
        /// @{

        ///
        /// Auto-typecasting operator
        ///
        /// \return Object handle
        ///
        inline operator handle_type() const
        {
            return m_h;
        }

        ///
        /// Returns the object handle value when the object handle is a pointer to a value (class, struct, etc.).
        ///
        /// \return Object handle value
        ///
        inline handle_type*& operator*() const
        {
            assert(m_h != NULL);
            return *m_h;
        }

        ///
        /// Returns the object handle reference.
        /// \return Object handle reference
        ///
        inline handle_type* operator&()
        {
            assert(m_h == NULL);
            return &m_h;
        }

        ///
        /// Provides object handle member access when the object handle is a pointer to a class or struct.
        ///
        /// \return Object handle
        ///
        inline handle_type operator->() const
        {
            assert(m_h != NULL);
            return m_h;
        }

        /// @}

        /// \name Comparison operators
        /// @{

        ///
        /// Tests if the object handle is NULL.
        ///
        /// \return
        /// - Non zero when object handle is NULL;
        /// - Zero otherwise.
        ///
        inline bool operator!() const
        {
            return m_h == NULL;
        }

        ///
        /// Is handle less than?
        ///
        /// \param[in] h  Object handle to compare against
        /// \return
        /// - Non zero when object handle is less than h;
        /// - Zero otherwise.
        ///
        inline bool operator<(_In_opt_ handle_type h) const
        {
            return m_h < h;
        }

        ///
        /// Is handle less than or equal to?
        ///
        /// \param[in] h  Object handle to compare against
        /// \return
        /// - Non zero when object handle is less than or equal to h;
        /// - Zero otherwise.
        ///
        inline bool operator<=(_In_opt_ handle_type h) const
        {
            return m_h <= h;
        }

        ///
        /// Is handle greater than or equal to?
        ///
        /// \param[in] h  Object handle to compare against
        /// \return
        /// - Non zero when object handle is greater than or equal to h;
        /// - Zero otherwise.
        ///
        inline bool operator>=(_In_opt_ handle_type h) const
        {
            return m_h >= h;
        }

        ///
        /// Is handle greater than?
        ///
        /// \param[in] h  Object handle to compare against
        /// \return
        /// - Non zero when object handle is greater than h;
        /// - Zero otherwise.
        ///
        inline bool operator>(_In_opt_ handle_type h) const
        {
            return m_h > h;
        }

        ///
        /// Is handle not equal to?
        ///
        /// \param[in] h  Object handle to compare against
        /// \return
        /// - Non zero when object handle is not equal to h;
        /// - Zero otherwise.
        ///
        inline bool operator!=(_In_opt_ handle_type h) const
        {
            return !operator==(h);
        }

        ///
        /// Is handle equal to?
        ///
        /// \param[in] h  Object handle to compare against
        /// \return
        /// - Non zero when object handle is equal to h;
        /// - Zero otherwise.
        ///
        inline bool operator==(_In_opt_ handle_type h) const
        {
            return m_h == h;
        }

        /// @}

        ///
        /// Sets a new object handle for the class
        ///
        /// When the current object handle of the class is non-NULL, the object is destroyed first.
        ///
        /// \param[in] h  New object handle
        ///
        inline void attach(_In_opt_ handle_type h)
        {
            if (m_h)
                free_internal();
            m_h = h;
        }

        ///
        /// Dismisses the object handle from this class
        ///
        /// \return Object handle
        ///
        inline handle_type detach()
        {
            handle_type h = m_h;
            m_h = NULL;
            return h;
        }

        ///
        /// Destroys the object
        ///
        inline void Free()
        {
            if (m_h) {
                free_internal();
                m_h = NULL;
            }
        }

        /// @}

    protected:
        ///
        /// Abstract member function that must be implemented by child classes to do the actual object destruction.
        ///
        virtual void free_internal() = 0;

    protected:
        handle_type m_h; ///< Object handle
    };


    ///
    /// Base abstract template class to support object handle keeping for objects that support handle duplication
    ///
    template <class T>
    class dplhandle : public handle<T>
    {
    public:
        ///
        /// Duplicates and returns a new object handle.
        ///
        /// \return Duplicated object handle
        ///
        inline handle_type duplicate() const
        {
            return m_h ? duplicate_internal(m_h) : NULL;
        }

        ///
        /// Duplicates an object handle and sets a new object handle.
        ///
        /// \param[in] h  Object handle of existing object
        ///
        /// \return
        /// - true when duplication succeeds;
        /// - false when duplication fails. In case of failure obtaining the extended error information is object type specific (for example: `GetLastError()`).
        ///
        inline bool attach_duplicated(_In_opt_ handle_type h)
        {
            if (m_h)
                free_internal();

            return h ? (m_h = duplicate_internal(h)) != NULL : (m_h = NULL, true);
        }

        //
        // Do not allow = operators. They are semantically ambigious:
        // Do they attach the class to the existing instance of object, or do they duplicate it?
        // To avoid confusion, user should use attach() and Duplicate() methods explicitly.
        //
        //inline const dplhandle<T>& operator=(_In_ const handle_type src)
        //{
        //    attach(src ? duplicate_internal(src) : NULL);
        //    return *this;
        //}

        //inline const dplhandle<T>& operator=(_In_ const dplhandle<T> &src)
        //{
        //    attach(src.m_h ? duplicate_internal(src.m_h) : NULL);
        //    return *this;
        //}

    protected:
        ///
        /// Abstract member function that must be implemented by child classes to do the actual object handle duplication.
        ///
        /// \param[in] h  Object handle of existing object
        ///
        /// \return Duplicated object handle
        ///
        virtual handle_type duplicate_internal(_In_ handle_type h) const = 0;
    };

    /// @}

    ///
    /// \defgroup WinStdStrFormat String Formatting
    /// Formatted string generation
    ///
    /// \par Example
    /// \code
    /// // Please note the PCSTR typecasting invokes an operator to return
    /// // pointer to formatted buffer rather than class reference itself.
    /// cout << (PCSTR)(string_printf("%i is less than %i.\n", 1, 5));
    /// \endcode
    ///
    /// @{

    ///
    /// Base template class to support string formatting using `printf()` style templates
    ///
    template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
    class basic_string_printf : public std::basic_string<_Elem, _Traits, _Ax>
    {
    public:
        /// \name Initializing string using template in memory
        /// @{

        ///
        /// Initializes a new string and formats its contents using `printf()` style template.
        ///
        /// \param[in] format  String template using `printf()` style
        ///
        basic_string_printf(_In_z_ _Printf_format_string_ const _Elem *format, ...)
        {
            va_list arg;
            va_start(arg, format);
            vsprintf(*this, format, arg);
            va_end(arg);
        }

        /// @}

        /// \name Initializing string using template in resources
        /// @{

        ///
        /// Initializes a new string and formats its contents using `printf()` style template in resources.
        ///
        /// \param[in] hInstance  Resource module handle
        /// \param[in] nFormatID  Resource ID of the string template using `printf()` style
        ///
        basic_string_printf(_In_ HINSTANCE hInstance, _In_ UINT nFormatID, ...)
        {
            _Myt format;
            ATLENSURE(format.LoadString(hInstance, nFormatID));

            va_list arg;
            va_start(arg, nFormatID);
            vsprintf(*this, format, arg);
            va_end(arg);
        }

        ///
        /// Initializes a new string and formats its contents using `printf()` style template in resources.
        ///
        /// \param[in] hInstance    Resource module handle
        /// \param[in] wLanguageID  Resource language
        /// \param[in] nFormatID    Resource ID of the string template using `printf()` style
        ///
        basic_string_printf(_In_ HINSTANCE hInstance, _In_ WORD wLanguageID, _In_ UINT nFormatID, ...)
        {
            _Myt format;
            ATLENSURE(format.LoadString(hInstance, nFormatID, wLanguageID));

            va_list arg;
            va_start(arg, nFormatID);
            vsprintf(*this, format, arg);
            va_end(arg);
        }

        /// }@
    };


    ///
    /// Single-byte character implementation of a class to support string formatting using `printf()` style templates
    ///
    typedef basic_string_printf<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > string_printf;


    ///
    /// Wide character implementation of a class to support string formatting using `printf()` style templates
    ///
    typedef basic_string_printf<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> wstring_printf;


#ifdef _UNICODE
    typedef wstring_printf tstring_printf;
#else
    typedef string_printf tstring_printf;
#endif


    ///
    /// Base template class to support string formatting using `FormatMessage()` style templates
    ///
    template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
    class basic_string_msg : public std::basic_string<_Elem, _Traits, _Ax>
    {
    public:
        /// \name Initializing string using template in memory
        /// @{

        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style template.
        ///
        /// \param[in] format  String template using `FormatMessage()` style
        ///
        basic_string_msg(_In_z_ _FormatMessage_format_string_ const _Elem *format, ...)
        {
            va_list arg;
            va_start(arg, format);
            FormatMessageV(format, &arg);
            va_end(arg);
        }

        /// @}

        /// \name Initializing string using template in resources
        /// @{

        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style template in resources.
        ///
        /// \param[in] hInstance  Resource module handle
        /// \param[in] nFormatID  Resource ID of the string template using `FormatMessage()` style
        ///
        basic_string_msg(_In_ HINSTANCE hInstance, _In_ UINT nFormatID, ...)
        {
            _Myt format(GetManager());
            ATLENSURE(format.LoadString(hInstance, nFormatID));

            va_list arg;
            va_start(arg, nFormatID);
            FormatMessageV(format, &arg);
            va_end(arg);
        }

        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style template in resources.
        ///
        /// \param[in] hInstance    Resource module handle
        /// \param[in] wLanguageID  Resource language
        /// \param[in] nFormatID    Resource ID of the string template using `FormatMessage()` style
        ///
        basic_string_msg(_In_ HINSTANCE hInstance, _In_ WORD wLanguageID, _In_ UINT nFormatID, ...)
        {
            _Myt format(GetManager());
            ATLENSURE(format.LoadString(hInstance, nFormatID, wLanguageID));

            va_list arg;
            va_start(arg, nFormatID);
            FormatMessageV(format, &arg);
            va_end(arg);
        }

        /// @}
    };


    ///
    /// Single-byte character implementation of a class to support string formatting using `FormatMessage()` style templates
    ///
    typedef basic_string_msg<char, std::char_traits<char>, std::allocator<char> > string_msg;


    ///
    /// Wide character implementation of a class to support string formatting using `FormatMessage()` style templates
    ///
    typedef basic_string_msg<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > wstring_msg;


#ifdef _UNICODE
    typedef wstring_msg tstring_msg;
#else
    typedef string_msg tstring_msg;
#endif


    /// @}

    /// \defgroup WinStdMemSanitize Auto-sanitize Memory Management
    /// Sanitizes memory before dismissed
    ///
    /// @{

    // winstd::sanitizing_allocator::destroy() member generates _Ptr parameter not used warning for primitive datatypes _Ty.
    #pragma warning(push)
    #pragma warning(disable: 4100)

    ///
    /// An allocator template that sanitizes each memory block before it is destroyed or reallocated
    ///
    /// \note
    /// `sanitizing_allocator` introduces a performance penalty. However, it provides an additional level of security.
    /// Use for security sensitive data memory storage only.
    ///
    template<class _Ty>
    class sanitizing_allocator : public std::_Allocator_base<_Ty>
    {
    public:
        typedef std::_Allocator_base<_Ty> _Mybase;
        typedef typename _Mybase::value_type value_type;

        typedef value_type _FARQ *pointer;
        typedef value_type _FARQ& reference;
        typedef const value_type _FARQ *const_pointer;
        typedef const value_type _FARQ& const_reference;

        typedef _SIZT size_type;
        typedef _PDFT difference_type;


        ///
        /// Convert this type to sanitizing_allocator<_Other>
        ///
        template<class _Other>
        struct rebind
        {
            typedef sanitizing_allocator<_Other> other;
        };


        ///
        /// Return address of mutable _Val
        ///
        inline pointer address(_In_ reference _Val) const
        {
            return ((pointer) &(char&)_Val);
        }


        ///
        /// Return address of nonmutable _Val
        ///
        inline const_pointer address(_In_ const_reference _Val) const
        {
            return ((const_pointer) &(char&)_Val);
        }


        ///
        /// Construct default allocator
        ///
        inline sanitizing_allocator()
        {
        }


        ///
        /// Construct by copying
        ///
        inline sanitizing_allocator(_In_ const sanitizing_allocator<_Ty>&)
        {
        }


        ///
        /// Construct from a related allocator
        template<class _Other>
        inline sanitizing_allocator(_In_ const sanitizing_allocator<_Other>&)
        {
        }


        ///
        /// Assign from a related allocator
        ///
        template<class _Other>
        inline sanitizing_allocator<_Ty>& operator=(_In_ const sanitizing_allocator<_Other>&)
        {
            return (*this);
        }


        ///
        /// Deallocate object at _Ptr sanitizing its content first
        ///
        inline void deallocate(_In_ pointer _Ptr, _In_ size_type _Count)
        {
            // Sanitize then free.
            SecureZeroMemory(_Ptr, sizeof(_Ty)*_Count);
            ::operator delete(_Ptr);
        }


        ///
        /// Allocate array of _Count elements
        ///
        inline pointer allocate(_In_ size_type _Count)
        {
            void *_Ptr = 0;

            if (_Count <= 0)
                _Count = 0;
            else if (((_SIZT)(-1)/sizeof(_Ty) < _Count) || (_Ptr = ::operator new(_Count * sizeof(_Ty))) == 0)
                _THROW_NCEE(bad_alloc, 0);

            return ((_Ty _FARQ *)_Ptr);
        }


        ///
        /// Allocate array of _Count elements
        ///
        inline pointer allocate(_In_ size_type _Count, _In_ const void _FARQ *)
        {
            return (allocate(_Count));
        }


        ///
        /// Construct object at _Ptr with value _Val
        ///
        inline void construct(_In_ pointer _Ptr, _In_ const _Ty& _Val)
        {
            void _FARQ *_Vptr = _Ptr;
            ::new (_Vptr)_Ty1(_STD forward<_Ty2>(_Val));
        }


        ///
        /// Construct object at _Ptr with value _Val
        ///
        inline void construct(_In_ pointer _Ptr, _In_ _Ty&& _Val)
        {
            ::new ((void _FARQ *)_Ptr) _Ty(_STD forward<_Ty>(_Val));
        }


        ///
        /// Construct object at _Ptr with value _Val
        ///
        template<class _Other>
        inline void construct(_In_ pointer _Ptr, _In_ _Other&& _Val)
        {
            ::new ((void _FARQ *)_Ptr) _Ty(_STD forward<_Other>(_Val));
        }


        ///
        /// Destroy object at _Ptr
        ///
        inline void destroy(_In_ pointer _Ptr)
        {
            _Ptr->~_Ty();
        }


        ///
        /// Estimate maximum array size
        ///
        inline _SIZT max_size() const
        {
            _SIZT _Count = (_SIZT)(-1) / sizeof (_Ty);
            return (0 < _Count ? _Count : 1);
        }
    };

    #pragma warning(pop)

    /// @}
}
