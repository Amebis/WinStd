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

///
/// Public function calling convention
///
#ifndef WINSTD_API
#if defined(WINSTD_DLL)
#define WINSTD_API      __declspec(dllexport)
#elif defined(WINSTD_DLLIMP)
#define WINSTD_API      __declspec(dllimport)
#else
#define WINSTD_API
#endif
#endif

#ifndef __L
#define __L(x)  L ## x
#endif
#ifndef _L
#define _L(x)   __L(x)
#endif

#include <Windows.h>

#include <stdarg.h>

#include <memory>
#include <stdexcept>
#include <string>

inline int vsnprintf(_Out_z_cap_(capacity) char *str, _In_ size_t capacity, _In_z_ _Printf_format_string_ const char *format, _In_ va_list arg);
inline int vsnprintf(_Out_z_cap_(capacity) wchar_t *str, _In_ size_t capacity, _In_z_ _Printf_format_string_ const wchar_t *format, _In_ va_list arg);
template<class _Elem, class _Traits, class _Ax> inline int vsprintf(_Out_ std::basic_string<_Elem, _Traits, _Ax> &str, _In_z_ _Printf_format_string_ const _Elem *format, _In_ va_list arg);
template<class _Elem, class _Traits, class _Ax> inline int sprintf(_Out_ std::basic_string<_Elem, _Traits, _Ax> &str, _In_z_ _Printf_format_string_ const _Elem *format, ...);

template<class _Traits, class _Ax> inline DWORD FormatMessage(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _Out_ std::basic_string<char, _Traits, _Ax> &str, _In_opt_ va_list *Arguments);
template<class _Traits, class _Ax> inline DWORD FormatMessage(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &str, _In_opt_ va_list *Arguments);

namespace winstd
{
    ///
    /// Multi-byte / Wide-character string (according to _UNICODE)
    ///
#ifdef _UNICODE
    typedef std::wstring tstring;
#else
    typedef std::string tstring;
#endif

    template <class _Ty> struct LocalFree_delete;
    template <class _Ty> struct LocalFree_delete<_Ty[]>;
    template <class T> class handle;
    template <class T> class dplhandle;

    ///
    /// \defgroup WinStdExceptions Exceptions
    /// Additional exceptions
    ///
    /// @{

    template <typename _Tn> class num_runtime_error;
    class WINSTD_API win_runtime_error;

    /// @}

    /// \addtogroup WinStdStrFormat
    /// @{

    template<class _Elem, class _Traits = std::char_traits<_Elem>, class _Ax = std::allocator<_Elem> > class basic_string_printf;

    ///
    /// Single-byte character implementation of a class to support string formatting using `printf()` style templates
    ///
    typedef basic_string_printf<char, std::char_traits<char>, std::allocator<char> > string_printf;

    ///
    /// Wide character implementation of a class to support string formatting using `printf()` style templates
    ///
    typedef basic_string_printf<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> wstring_printf;

    ///
    /// Multi-byte / Wide-character formatted string (according to _UNICODE)
    ///
#ifdef _UNICODE
    typedef wstring_printf tstring_printf;
#else
    typedef string_printf tstring_printf;
#endif

    template<class _Elem, class _Traits = std::char_traits<_Elem>, class _Ax = std::allocator<_Elem> > class basic_string_msg;

    ///
    /// Single-byte character implementation of a class to support string formatting using `FormatMessage()` style templates
    ///
    typedef basic_string_msg<char, std::char_traits<char>, std::allocator<char> > string_msg;

    ///
    /// Wide character implementation of a class to support string formatting using `FormatMessage()` style templates
    ///
    typedef basic_string_msg<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > wstring_msg;

    ///
    /// Multi-byte / Wide-character formatted string (according to _UNICODE)
    ///
#ifdef _UNICODE
    typedef wstring_msg tstring_msg;
#else
    typedef string_msg tstring_msg;
#endif

    /// @}

    /// \addtogroup WinStdMemSanitize
    /// @{

    template<class _Ty> class sanitizing_allocator;

    ///
    /// A sanitizing variant of std::string
    ///
    /// \note
    /// `sanitizing_string` introduces a performance penalty. However, it provides an additional level of security.
    /// Use for security sensitive data memory storage only.
    ///
    typedef std::basic_string<char, std::char_traits<char>, sanitizing_allocator<char> > sanitizing_string;

    ///
    /// A sanitizing variant of std::wstring
    ///
    /// \note
    /// `sanitizing_wstring` introduces a performance penalty. However, it provides an additional level of security.
    /// Use for security sensitive data memory storage only.
    ///
    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, sanitizing_allocator<wchar_t> > sanitizing_wstring;

    ///
    /// Multi-byte / Wide-character sanitizing string (according to _UNICODE)
    ///
#ifdef _UNICODE
    typedef sanitizing_wstring sanitizing_tstring;
#else
    typedef sanitizing_string sanitizing_tstring;
#endif

    /// @}
}

#pragma once

#include <assert.h>

#include <memory>
#include <vector>


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


///
/// Formats a message string.
///
/// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
///
template<class _Traits, class _Ax>
inline DWORD FormatMessage(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _Out_ std::basic_string<char, _Traits, _Ax> &str, _In_opt_ va_list *Arguments)
{
    std::unique_ptr<CHAR[], winstd::LocalFree_delete<CHAR[]> > lpBuffer;
    DWORD dwResult = FormatMessageA(dwFlags | FORMAT_MESSAGE_ALLOCATE_BUFFER, lpSource, dwMessageId, dwLanguageId, (LPSTR)&lpBuffer, 0, Arguments);
    if (dwResult)
        str.assign(lpBuffer.get(), dwResult);
    return dwResult;
}


///
/// Formats a message string.
///
/// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
///
template<class _Traits, class _Ax>
inline DWORD FormatMessage(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _Out_ std::basic_string<wchar_t, _Traits, _Ax> &str, _In_opt_ va_list *Arguments)
{
    std::unique_ptr<WCHAR[], winstd::LocalFree_delete<WCHAR[]> > lpBuffer;
    DWORD dwResult = FormatMessageW(dwFlags | FORMAT_MESSAGE_ALLOCATE_BUFFER, lpSource, dwMessageId, dwLanguageId, (LPWSTR)&lpBuffer, 0, Arguments);
    if (dwResult)
        str.assign(lpBuffer.get(), dwResult);
    return dwResult;
}


namespace winstd
{
    ///
    /// Deleter for unique_ptr using LocalFree
    ///
    template <class _Ty> struct LocalFree_delete
    {
        typedef LocalFree_delete<_Ty> _Myt; ///< This type

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
        typedef LocalFree_delete<_Ty> _Myt; ///< This type

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
    template <class T>
    class handle
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
        /// Initializes a new class instance with an already available object handle.
        ///
        /// \param[in] h  Initial object handle value
        ///
        inline handle(_In_opt_ handle_type h) : m_h(h)
        {
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        inline handle<handle_type>(_Inout_ handle<handle_type> &&h)
        {
            // Transfer handle.
            m_h   = h.m_h;
            h.m_h = NULL;
        }

    private:
        // This class is noncopyable.
        handle(_In_ const handle<handle_type> &h);
        handle<handle_type>& operator=(_In_ const handle<handle_type> &h);

    public:
        ///
        /// Move assignment
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        handle<handle_type>& operator=(_Inout_ handle<handle_type> &&h)
        {
            if (this != std::addressof(h)) {
                // Transfer handle.
                if (m_h)
                    free_internal();
                m_h   = h.m_h;
                h.m_h = NULL;
            }
            return *this;
        }

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
            return !operator>(h);
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
            return !operator<(h);
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
            return h < m_h;
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
        inline void free()
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
        /// Duplicates the object handle.
        ///
        /// \param[in] h  Object handle value
        ///
        inline dplhandle<handle_type>& operator=(_In_ handle_type h)
        {
            if (m_h != h) {
                handle_type h_new = duplicate_internal(h);
                if (h_new) {
                    if (m_h)
                        free_internal();

                    m_h = h_new;
                } else if (h)
                    assert(0); // Could not duplicate the handle
            }
            return *this;
        }

        ///
        /// Duplicates the object.
        ///
        /// \param[in] h  Object
        ///
        inline dplhandle<handle_type>& operator=(_In_ const dplhandle<handle_type> &h)
        {
            if (this != &h) {
                handle_type h_new = duplicate_internal(h.m_h);
                if (h_new) {
                    if (m_h)
                        free_internal();

                    m_h = h_new;
                } else if (h.m_h)
                    assert(0); // Could not duplicate the handle
            }
            return *this;
        }

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
    /// \defgroup WinStdExceptions Exceptions
    /// Additional exceptions
    ///
    /// @{

    ///
    /// Windows runtime error
    ///
    template <typename _Tn>
    class num_runtime_error : public std::runtime_error
    {
    public:
        typedef _Tn error_type;

    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] error  Numeric error code
        /// \param[in] msg    Error message
        ///
        inline num_runtime_error(_In_ error_type num, _In_ const std::string& msg) :
            m_num(num),
            runtime_error(msg.c_str())
        {
        }


        ///
        /// Constructs an exception
        ///
        /// \param[in] num  Numeric error code
        /// \param[in] msg  Error message
        ///
        inline num_runtime_error(_In_ error_type num, _In_z_ const char *msg) :
            m_num(num),
            runtime_error(msg)
        {
        }


        ///
        /// Copies an exception
        ///
        /// \param[in] other  Exception to copy from
        ///
        inline num_runtime_error(const num_runtime_error<_Tn> &other) :
            m_num(other.m_num),
            runtime_error(other)
        {
        }


        ///
        /// Copies an exception
        ///
        /// \param[in] other  Exception to copy from
        ///
        inline num_runtime_error& operator=(const num_runtime_error<_Tn> &other)
        {
            if (this != addressof(other)) {
                *(runtime_error*)this = other;
                m_num                 = other.m_num;
            }

            return *this;
        }


        ///
        /// Returns the Windows error number
        ///
        inline error_type number() const
        {
            return m_num;
        }

    protected:
        error_type m_num;  ///< Numeric error code
    };


    ///
    /// Windows runtime error
    ///
    class WINSTD_API win_runtime_error : public num_runtime_error<DWORD>
    {
    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] error  Windows error code
        /// \param[in] msg    Error message
        ///
        inline win_runtime_error(_In_ error_type num, _In_ const std::string& msg) : num_runtime_error<DWORD>(num, msg.c_str())
        {
        }


        ///
        /// Constructs an exception
        ///
        /// \param[in] num  Numeric error code
        /// \param[in] msg  Error message
        ///
        inline win_runtime_error(_In_ error_type num, _In_z_ const char *msg) : num_runtime_error<DWORD>(num, msg)
        {
        }


        ///
        /// Constructs an exception using `GetLastError()`
        ///
        /// \param[in] msg  Error message
        ///
        inline win_runtime_error(_In_ const std::string& msg) : num_runtime_error<DWORD>(GetLastError(), msg.c_str())
        {
        }


        ///
        /// Constructs an exception using `GetLastError()`
        ///
        /// \param[in] msg  Error message
        ///
        inline win_runtime_error(_In_z_ const char *msg) : num_runtime_error<DWORD>(GetLastError(), msg)
        {
        }


        ///
        /// Copies an exception
        ///
        /// \param[in] other  Exception to copy from
        ///
        inline win_runtime_error(const win_runtime_error &other) : num_runtime_error<DWORD>(other)
        {
        }
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
    /// cout << (PCSTR)(winstd::string_printf("%i is less than %i.\n", 1, 5));
    /// \endcode
    ///
    /// @{

    ///
    /// Base template class to support string formatting using `printf()` style templates
    ///
    template<class _Elem, class _Traits, class _Ax>
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
        inline basic_string_printf(_In_z_ _Printf_format_string_ const _Elem *format, ...)
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
        inline basic_string_printf(_In_ HINSTANCE hInstance, _In_ UINT nFormatID, ...)
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
        inline basic_string_printf(_In_ HINSTANCE hInstance, _In_ WORD wLanguageID, _In_ UINT nFormatID, ...)
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
    /// Base template class to support string formatting using `FormatMessage()` style templates
    ///
    template<class _Elem, class _Traits, class _Ax>
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
        inline basic_string_msg(_In_z_ _FormatMessage_format_string_ const _Elem *format, ...)
        {
            va_list arg;
            va_start(arg, format);
            FormatMessage(FORMAT_MESSAGE_FROM_STRING, format, 0, 0, *this, &arg);
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
        inline basic_string_msg(_In_ HINSTANCE hInstance, _In_ UINT nFormatID, ...)
        {
            _Myt format(GetManager());
            ATLENSURE(format.LoadString(hInstance, nFormatID));

            va_list arg;
            va_start(arg, nFormatID);
            FormatMessage(FORMAT_MESSAGE_FROM_STRING, format, 0, 0, *this, &arg);
            va_end(arg);
        }

        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style template in resources.
        ///
        /// \param[in] hInstance    Resource module handle
        /// \param[in] wLanguageID  Resource language
        /// \param[in] nFormatID    Resource ID of the string template using `FormatMessage()` style
        ///
        inline basic_string_msg(_In_ HINSTANCE hInstance, _In_ WORD wLanguageID, _In_ UINT nFormatID, ...)
        {
            _Myt format(GetManager());
            ATLENSURE(format.LoadString(hInstance, nFormatID, wLanguageID));

            va_list arg;
            va_start(arg, nFormatID);
            FormatMessage(FORMAT_MESSAGE_FROM_STRING, format, 0, 0, *this, &arg);
            va_end(arg);
        }

        /// @}


        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style.
        ///
        /// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
        ///
        inline basic_string_msg(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _In_opt_ va_list *Arguments)
        {
            FormatMessage(dwFlags & ~FORMAT_MESSAGE_ARGUMENT_ARRAY, lpSource, dwMessageId, dwLanguageId, *this, Arguments);
        }


        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style.
        ///
        /// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
        ///
        inline basic_string_msg(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _In_opt_ DWORD_PTR *Arguments)
        {
            FormatMessage(dwFlags | FORMAT_MESSAGE_ARGUMENT_ARRAY, lpSource, dwMessageId, dwLanguageId, *this, (va_list*)Arguments);
        }


        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style.
        ///
        /// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
        ///
        inline basic_string_msg(_In_ DWORD dwFlags, _In_z_ LPCTSTR pszFormat, _In_opt_ va_list *Arguments)
        {
            FormatMessage(dwFlags & ~FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_STRING, pszFormat, 0, 0, *this, Arguments);
        }


        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style.
        ///
        /// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
        ///
        inline basic_string_msg(_In_ DWORD dwFlags, _In_z_ LPCTSTR pszFormat, _In_opt_ DWORD_PTR *Arguments)
        {
            FormatMessage(dwFlags | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_STRING, pszFormat, 0, 0, *this, (va_list*)Arguments);
        }
    };

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
    class sanitizing_allocator : public std::allocator<_Ty>
    {
    public:
        typedef std::allocator<_Ty> _Mybase; ///< Base type

        ///
        /// Convert this type to sanitizing_allocator<_Other>
        ///
        template<class _Other>
        struct rebind
        {
            typedef sanitizing_allocator<_Other> other; ///< Other type
        };


        ///
        /// Construct default allocator
        ///
        inline sanitizing_allocator() : _Mybase()
        {
        }


        ///
        /// Construct by copying
        ///
        inline sanitizing_allocator(_In_ const sanitizing_allocator<_Ty> &_Othr) : _Mybase(_Othr)
        {
        }


        ///
        /// Construct from a related allocator
        ///
        template<class _Other>
        inline sanitizing_allocator(_In_ const sanitizing_allocator<_Other> &_Othr) : _Mybase(_Othr)
        {
        }


        ///
        /// Deallocate object at _Ptr sanitizing its content first
        ///
        inline void deallocate(_In_ pointer _Ptr, _In_ size_type _Size)
        {
            // Sanitize then free.
            SecureZeroMemory(_Ptr, _Size);
            _Mybase::deallocate(_Ptr, _Size);
        }
    };

    #pragma warning(pop)

    /// @}
}
