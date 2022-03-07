/*
    SPDX-License-Identifier: MIT
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
*/

#pragma once

#include <Windows.h>
#include <assert.h>
#include <stdarg.h>
#include <tchar.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

/// \defgroup WinStdGeneral General
///
/// \defgroup WinStdStrFormat String Formatting
///
/// \par Example
/// \code
/// // Please note the PCSTR typecasting invokes an operator to return
/// // pointer to formatted buffer rather than class reference itself.
/// cout << (PCSTR)(winstd::string_printf("%i is less than %i.\n", 1, 5));
/// \endcode
///
/// \defgroup WinStdSysHandles System Handles
///
/// \defgroup WinStdExceptions Exceptions
///
/// \defgroup WinStdMemSanitize Auto-sanitize Memory Management

/// \addtogroup WinStdGeneral
/// @{

///
/// "L" stringizing macro
///
#ifndef __L
#define __L(x)  L ## x
#endif

///
/// Makes string Unicode
///
#ifndef _L
#define _L(x)   __L(x)
#endif

///
/// Stringizing macro helper
///
#define WINSTD_STRING_IMPL(x)  #x

///
/// Stringizing macro
///
#define WINSTD_STRING(x)       WINSTD_STRING_IMPL(x)

///
/// Declares a class as non-copyable
///
#define WINSTD_NONCOPYABLE(C) \
private: \
       C        (_In_ const C &h) noexcept; \
    C& operator=(_In_ const C &h) noexcept;

///
/// Declares a class as non-movable
///
#define WINSTD_NONMOVABLE(C) \
private: \
       C        (_Inout_ C &&h) noexcept; \
    C& operator=(_Inout_ C &&h) noexcept;

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

/// @}

/// \addtogroup WinStdStrFormat
/// @{

///
/// LPTSTR printf/scanf format specifier
///
#ifdef UNICODE
#define PRINTF_LPTSTR "ls"
#else
#define PRINTF_LPTSTR "s"
#endif

///
/// LPOLESTR printf/scanf format specifier
///
#ifdef OLE2ANSI
#define PRINTF_LPOLESTR  "hs"
#else
#define PRINTF_LPOLESTR  "ls"
#endif

///
/// Standard input stream for TCHAR strings
///
#ifdef _UNICODE
#define _tcin   (std::wcin )
#else
#define _tcin   (std::cin ) ///< Standard input stream for TCHAR strings
#endif

///
/// Standard output stream for TCHAR strings
///
#ifdef _UNICODE
#define _tcout  (std::wcout)
#else
#define _tcout  (std::cout)
#endif

///
/// Standard error stream for TCHAR strings
///
#ifdef _UNICODE
#define _tcerr  (std::wcerr)
#else
#define _tcerr  (std::cerr)
#endif

///
/// Standard logging stream for TCHAR strings
///
#ifdef _UNICODE
#define _tclog  (std::wclog)
#else
#define _tclog  (std::clog)
#endif

/// @}

/// \addtogroup WinStdSysHandles
/// @{

///
/// Implements default constructors and operators to prevent their auto-generation by compiler.
///
#define WINSTD_HANDLE_IMPL(C, INVAL) \
public: \
       C        (                        ) noexcept                                            {                                                                    } \
       C        (_In_opt_ handle_type   h) noexcept : handle<handle_type, INVAL>(          h ) {                                                                    } \
       C        (_Inout_  C           &&h) noexcept : handle<handle_type, INVAL>(std::move(h)) {                                                                    } \
    C& operator=(_In_opt_ handle_type   h) noexcept                                            { handle<handle_type, INVAL>::operator=(          h ); return *this; } \
    C& operator=(_Inout_  C           &&h) noexcept                                            { handle<handle_type, INVAL>::operator=(std::move(h)); return *this; } \
WINSTD_NONCOPYABLE(C)

///
/// Implements default constructors and operators to prevent their auto-generation by compiler.
///
#define WINSTD_DPLHANDLE_IMPL(C, INVAL) \
public: \
       C        (                              ) noexcept                                                            {                                                                       } \
       C        (_In_opt_       handle_type   h) noexcept : dplhandle<handle_type, INVAL>(                   h     ) {                                                                       } \
       C        (_In_     const C            &h) noexcept : dplhandle<handle_type, INVAL>(duplicate_internal(h.m_h)) {                                                                       } \
       C        (_Inout_        C           &&h) noexcept : dplhandle<handle_type, INVAL>(std::move         (h    )) {                                                                       } \
    C& operator=(_In_opt_       handle_type   h) noexcept                                                            { dplhandle<handle_type, INVAL>::operator=(          h ); return *this; } \
    C& operator=(_In_     const C            &h) noexcept                                                            { dplhandle<handle_type, INVAL>::operator=(          h ); return *this; } \
    C& operator=(_Inout_        C           &&h) noexcept                                                            { dplhandle<handle_type, INVAL>::operator=(std::move(h)); return *this; } \
private:

/// @}

#ifndef _FormatMessage_format_string_
#define _FormatMessage_format_string_ _In_z_
#endif

/// \cond internal
#ifndef _LPCBYTE_DEFINED
#define _LPCBYTE_DEFINED
typedef const BYTE *LPCBYTE;
#endif
/// \endcond

#pragma warning(push)
// Do not use _vsnprintf_s/_vsnwprintf_s(), since it terminates string by force even when we explicitly want to write unterminated string.
// Threfore turn off compiler warning instead. ;)
#pragma warning(disable: 4995)
#pragma warning(disable: 4996)
#pragma warning(disable: 4505) // Don't warn on unused code

/// \addtogroup WinStdStrFormat
/// @{

///
/// Formats string using `printf()`.
///
/// \param[out] str       Buffer to receive string
/// \param[in ] capacity  Size of `str` in characters
/// \param[in ] format    String template using `printf()` style
/// \param[in ] arg       Arguments to `format`
///
/// \returns Number of characters in result.
///
#if _MSC_VER <= 1600
static int vsnprintf(_Out_z_cap_(capacity) char *str, _In_ size_t capacity, _In_z_ _Printf_format_string_ const char *format, _In_ va_list arg)
{
    return _vsnprintf(str, capacity, format, arg);
}
#endif

///
/// Formats string using `printf()`.
///
/// \param[out] str       Buffer to receive string
/// \param[in ] capacity  Size of `str` in characters
/// \param[in ] format    String template using `printf()` style
/// \param[in ] arg       Arguments to `format`
///
/// \returns Number of characters in result.
///
static int vsnprintf(_Out_z_cap_(capacity) wchar_t *str, _In_ size_t capacity, _In_z_ _Printf_format_string_ const wchar_t *format, _In_ va_list arg) noexcept
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
static int vsprintf(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &str, _In_z_ _Printf_format_string_ const _Elem *format, _In_ va_list arg)
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
            auto buf_dyn = std::make_unique<_Elem[]>(capacity);
            count = vsnprintf(buf_dyn.get(), capacity - 1, format, arg);
            if (count >= 0) {
                str.assign(buf_dyn.get(), count);
                break;
            }
        }
    }

    return count;
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
static int sprintf(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &str, _In_z_ _Printf_format_string_ const _Elem *format, ...)
{
    va_list arg;
    va_start(arg, format);
    const int res = vsprintf(str, format, arg);
    va_end(arg);
    return res;
}

///
/// Formats a message string.
///
/// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
///
template<class _Traits, class _Ax>
static DWORD FormatMessage(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _Inout_ std::basic_string<char, _Traits, _Ax> &str, _In_opt_ va_list *Arguments)
{
    std::unique_ptr<CHAR[], winstd::LocalFree_delete<CHAR[]> > lpBuffer;
    DWORD dwResult = FormatMessageA(dwFlags | FORMAT_MESSAGE_ALLOCATE_BUFFER, lpSource, dwMessageId, dwLanguageId, reinterpret_cast<LPSTR>((LPSTR*)get_ptr(lpBuffer)), 0, Arguments);
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
static DWORD FormatMessage(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _Inout_ std::basic_string<wchar_t, _Traits, _Ax> &str, _In_opt_ va_list *Arguments)
{
    std::unique_ptr<WCHAR[], winstd::LocalFree_delete<WCHAR[]> > lpBuffer;
    DWORD dwResult = FormatMessageW(dwFlags | FORMAT_MESSAGE_ALLOCATE_BUFFER, lpSource, dwMessageId, dwLanguageId, reinterpret_cast<LPWSTR>((LPWSTR*)get_ptr(lpBuffer)), 0, Arguments);
    if (dwResult)
        str.assign(lpBuffer.get(), dwResult);
    return dwResult;
}

/// @}

#pragma warning(pop)

namespace winstd
{
    /// \addtogroup WinStdGeneral
    /// @{

    ///
    /// Multi-byte / Wide-character string (according to _UNICODE)
    ///
#ifdef _UNICODE
    typedef std::wstring tstring;
#else
    typedef std::string tstring;
#endif

    ///
    /// Deleter for unique_ptr using LocalFree
    ///
    template <class _Ty>
    struct LocalFree_delete
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
        /// \sa [LocalFree function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366730.aspx)
        ///
        void operator()(_Ty *_Ptr) const
        {
            LocalFree(_Ptr);
        }
    };

    ///
    /// Deleter for unique_ptr to array of unknown size using LocalFree
    ///
    template <class _Ty>
    struct LocalFree_delete<_Ty[]>
    {
        typedef LocalFree_delete<_Ty> _Myt; ///< This type

        ///
        /// Default construct
        ///
        LocalFree_delete() noexcept {}

        ///
        /// Delete a pointer
        ///
        void operator()(_Frees_ptr_opt_ _Ty *_Ptr) const noexcept
        {
            LocalFree(_Ptr);
        }

        ///
        /// Delete a pointer of another type
        ///
        /// \sa [LocalFree function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366730.aspx)
        ///
        template<class _Other>
        void operator()(_Other *) const
        {
            LocalFree(_Ptr);
        }
    };

    ///
    /// Helper class for returning pointers to std::unique_ptr
    ///
    template<class _Ty, class _Dx>
    class ref_unique_ptr
    {
    public:
        ///
        /// Takes ownership of the pointer
        ///
        /// \param[inout] owner  Object to attach helper to
        ///
        ref_unique_ptr(_Inout_ std::unique_ptr<_Ty, _Dx> &owner) :
            m_own(owner),
            m_ptr(owner.release())
        {}

        ///
        /// Moves object
        ///
        /// \param[inout] other  Source object
        ///
        ref_unique_ptr(_Inout_ ref_unique_ptr<_Ty, _Dx> &&other) :
            m_own(other.m_own),
            m_ptr(other.m_ptr)
        {
            other.m_ptr = nullptr;
        }

        ///
        /// Returns ownership of the pointer
        ///
        ~ref_unique_ptr()
        {
            if (m_ptr != nullptr)
                m_own.reset(m_ptr);
        }

        ///
        /// Operator for pointer-to-pointer parameters by value use-cases.
        ///
        /// \return Pointer to the pointer
        ///
        operator typename _Ty**()
        {
            return &m_ptr;
        }

        ///
        /// Operator for reverence-to-pointer parameters by value use-cases.
        ///
        /// \return Reference to the pointer
        ///
        operator typename _Ty*&()
        {
            return m_ptr;
        }

    protected:
        std::unique_ptr<_Ty, _Dx> &m_own;   ///< Original owner of the pointer
        _Ty                       *m_ptr;   ///< Pointer
    };

    ///
    /// Helper function template for returning pointers to std::unique_ptr
    ///
    /// \param[inout] owner  Original owner of the pointer
    ///
    /// \returns A helper wrapper class to handle returning a reference to the pointer
    ///
    template<class _Ty, class _Dx>
    ref_unique_ptr<_Ty, _Dx> get_ptr(_Inout_ std::unique_ptr<_Ty, _Dx> &owner) noexcept
    {
        return ref_unique_ptr<_Ty, _Dx>(owner);
    }

    ///
    /// Helper function template for returning pointers to std::unique_ptr
    /// (specialization for arrays)
    ///
    /// \param[inout] owner  Original owner of the pointer
    ///
    /// \returns A helper wrapper class to handle returning a reference to the pointer
    ///
    template<class _Ty, class _Dx>
    ref_unique_ptr<_Ty[], _Dx> get_ptr(_Inout_ std::unique_ptr<_Ty[], _Dx> &owner) noexcept
    {
        return ref_unique_ptr<_Ty[], _Dx>(owner);
    }

    ///
    /// Helper class for returning pointers to std::unique_ptr
    /// (specialization for arrays)
    ///
    #pragma warning(push)
    #pragma warning(disable: 26432) // Copy constructor and assignment operator are also present, but not detected by code analysis as they are using base type source object reference.
    template<class _Ty, class _Dx>
    class ref_unique_ptr<_Ty[], _Dx>
    {
    public:
        ///
        /// Takes ownership of the pointer
        ///
        /// \param[inout] owner  Object to attach helper to
        ///
        ref_unique_ptr(_Inout_ std::unique_ptr<_Ty[], _Dx> &owner) noexcept :
            m_own(owner),
            m_ptr(owner.release())
        {}

        ///
        /// Takes ownership of the pointer
        ///
        /// \param[inout] owner  Object to attach helper to
        ///
        /// \returns Reference to this object
        ///
        ref_unique_ptr& operator=(_Inout_ std::unique_ptr<_Ty[], _Dx> &owner) noexcept
        {
            if (this != &other) {
                m_own = owner;
                m_ptr = owner.release();
            }

            return *this;
        }

        ///
        /// Moves object
        ///
        /// \param[inout] other  Source object
        ///
        ref_unique_ptr(_Inout_ ref_unique_ptr<_Ty[], _Dx> &&other) :
            m_own(other.m_own),
            m_ptr(other.m_ptr)
        {
            other.m_ptr = nullptr;
        }

        ///
        /// Moves object
        ///
        /// \param[inout] other  Source object
        ///
        /// \returns Reference to this object
        ///
        ref_unique_ptr& operator=(_Inout_ ref_unique_ptr<_Ty[], _Dx> &&other)
        {
            if (this != &other) {
                m_own = other.m_own;
                m_ptr = other.m_ptr;
                other.m_ptr = nullptr;
            }

            return *this;
        }

        ///
        /// Returns ownership of the pointer
        ///
        virtual ~ref_unique_ptr()
        {
            if (m_ptr != nullptr)
                m_own.reset(m_ptr);
        }

        ///
        /// Operator for pointer-to-pointer parameters by value use-cases.
        ///
        /// \return Pointer to the pointer
        ///
        operator typename _Ty**() noexcept
        {
            return &m_ptr;
        }

        ///
        /// Operator for reverence-to-pointer parameters by value use-cases.
        ///
        /// \return Reference to the pointer
        ///
        operator typename _Ty*&()
        {
            return m_ptr;
        }

    protected:
        std::unique_ptr<_Ty[], _Dx> &m_own;   ///< Original owner of the pointer
        _Ty                         *m_ptr;   ///< Pointer
    };
    #pragma warning(pop)

    /// @}

    /// \addtogroup WinStdSysHandles
    /// @{

    ///
    /// Base abstract template class to support generic object handle keeping
    ///
    /// It provides basic operators and methods common to all descendands of this class establishing a base to ease the replacement of native object handle type with classes in object-oriented approach.
    ///
    template <class T, const T INVAL>
    class handle
    {
    public:
        ///
        /// Datatype of the object handle this template class handles
        ///
        typedef T handle_type;

        ///
        /// Invalid handle value
        ///
        static const T invalid;

        ///
        /// Initializes a new class instance with the object handle set to INVAL.
        ///
        handle() noexcept : m_h(invalid)
        {
        }

        ///
        /// Initializes a new class instance with an already available object handle.
        ///
        /// \param[in] h  Initial object handle value
        ///
        handle(_In_opt_ handle_type h) noexcept : m_h(h)
        {
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        handle(_Inout_ handle<handle_type, INVAL> &&h) noexcept
        {
            // Transfer handle.
            m_h   = h.m_h;
            h.m_h = invalid;
        }

    private:
        // This class is noncopyable.
        handle(_In_ const handle<handle_type, INVAL> &h) noexcept {};
        handle<handle_type, INVAL>& operator=(_In_ const handle<handle_type, INVAL> &h) noexcept {};

    public:
        ///
        /// Attaches already available object handle.
        ///
        /// \param[in] h  Object handle value
        ///
        handle<handle_type, INVAL>& operator=(_In_opt_ handle_type h) noexcept
        {
            attach(h);
            return *this;
        }

        ///
        /// Move assignment
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        #pragma warning(suppress: 26432) // Move constructor is also present, but not detected by code analysis somehow.
        handle<handle_type, INVAL>& operator=(_Inout_ handle<handle_type, INVAL> &&h) noexcept
        {
            if (this != std::addressof(h)) {
                // Transfer handle.
                if (m_h != invalid)
                    free_internal();
                m_h   = h.m_h;
                h.m_h = invalid;
            }
            return *this;
        }

        ///
        /// Auto-typecasting operator
        ///
        /// \return Object handle
        ///
        operator handle_type() const
        {
            return m_h;
        }

        ///
        /// Returns the object handle value when the object handle is a pointer to a value (class, struct, etc.).
        ///
        /// \return Object handle value
        ///
        handle_type*& operator*() const
        {
            assert(m_h != invalid);
            return *m_h;
        }

        ///
        /// Returns the object handle reference.
        /// \return Object handle reference
        ///
        handle_type* operator&()
        {
            assert(m_h == invalid);
            return &m_h;
        }

        ///
        /// Provides object handle member access when the object handle is a pointer to a class or struct.
        ///
        /// \return Object handle
        ///
        handle_type operator->() const
        {
            assert(m_h != invalid);
            return m_h;
        }

        ///
        /// Tests if the object handle is INVAL.
        ///
        /// \return
        /// - Non zero when object handle is INVAL;
        /// - Zero otherwise.
        ///
        bool operator!() const
        {
            return m_h == invalid;
        }

        ///
        /// Is handle less than?
        ///
        /// \param[in] h  Object handle to compare against
        /// \return
        /// - Non zero when object handle is less than h;
        /// - Zero otherwise.
        ///
        bool operator<(_In_opt_ handle_type h) const
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
        bool operator<=(_In_opt_ handle_type h) const
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
        bool operator>=(_In_opt_ handle_type h) const
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
        bool operator>(_In_opt_ handle_type h) const
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
        bool operator!=(_In_opt_ handle_type h) const
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
        bool operator==(_In_opt_ handle_type h) const
        {
            return m_h == h;
        }

        ///
        /// Sets a new object handle for the class
        ///
        /// When the current object handle of the class is non-INVAL, the object is destroyed first.
        ///
        /// \param[in] h  New object handle
        ///
        void attach(_In_opt_ handle_type h) noexcept
        {
            if (m_h != invalid)
                free_internal();
            m_h = h;
        }

        ///
        /// Dismisses the object handle from this class
        ///
        /// \return Object handle
        ///
        handle_type detach()
        {
            handle_type h = m_h;
            m_h = invalid;
            return h;
        }

        ///
        /// Destroys the object
        ///
        void free()
        {
            if (m_h != invalid) {
                free_internal();
                m_h = invalid;
            }
        }

    protected:
        ///
        /// Abstract member function that must be implemented by child classes to do the actual object destruction.
        ///
        virtual void free_internal() noexcept = 0;

    protected:
        handle_type m_h; ///< Object handle
    };

    template <class T, const T INVAL>
    const T handle<T, INVAL>::invalid = INVAL;

    ///
    /// Base abstract template class to support object handle keeping for objects that support trivial handle duplication
    ///
    template <class T, T INVAL>
    class dplhandle : public handle<T, INVAL>
    {
    public:
        ///
        /// Initializes a new class instance with the object handle set to INVAL.
        ///
        dplhandle() noexcept
        {
        }

        ///
        /// Initializes a new class instance with an already available object handle.
        ///
        /// \param[in] h  Initial object handle value
        ///
        dplhandle(_In_opt_ handle_type h) noexcept : handle<handle_type, INVAL>(h)
        {
        }

        ///
        /// Copy constructor
        ///
        /// \param[inout] h  A reference of another object
        ///
        dplhandle<handle_type, INVAL>(_In_ const dplhandle<handle_type, INVAL> &h) noexcept : handle<handle_type, INVAL>(duplicate_internal(h.m_h))
        {
        }

        ///
        /// Move constructor
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        dplhandle<handle_type, INVAL>(_Inout_ dplhandle<handle_type, INVAL> &&h) noexcept : handle<handle_type, INVAL>(std::move(h))
        {
        }

        ///
        /// Attaches already available object handle.
        ///
        /// \param[in] h  Object handle value
        ///
        dplhandle<handle_type, INVAL>& operator=(_In_opt_ handle_type h) noexcept
        {
            handle<handle_type, INVAL>::operator=(h);
            return *this;
        }

        ///
        /// Duplicates the object.
        ///
        /// \param[in] h  Object
        ///
        dplhandle<handle_type, INVAL>& operator=(_In_ const dplhandle<handle_type, INVAL> &h) noexcept
        {
            if (this != std::addressof(h)) {
                if (h.m_h != invalid) {
                    handle_type h_new = duplicate_internal(h.m_h);
                    if (h_new != invalid) {
                        if (m_h != invalid)
                            free_internal();

                        m_h = h_new;
                    } else
                        assert(0); // Could not duplicate the handle
                } else {
                    if (m_h != invalid)
                        free_internal();

                    m_h = invalid;
                }
            }
            return *this;
        }

        ///
        /// Moves the object.
        ///
        /// \param[inout] h  A rvalue reference of another object
        ///
        #pragma warning(disable: 26432) // Move constructor is also present, but not detected by code analysis somehow.
        dplhandle<handle_type, INVAL>& operator=(_Inout_ dplhandle<handle_type, INVAL> &&h) noexcept
        {
            handle<handle_type, INVAL>::operator=(std::move(h));
            return *this;
        }

        ///
        /// Duplicates and returns a new object handle.
        ///
        /// \return Duplicated object handle
        ///
        handle_type duplicate() const
        {
            return m_h != invalid ? duplicate_internal(m_h) : invalid;
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
        bool attach_duplicated(_In_opt_ handle_type h)
        {
            if (m_h != invalid)
                free_internal();

            return h != invalid ? (m_h = duplicate_internal(h)) != invalid : (m_h = invalid, true);
        }

    protected:
        ///
        /// Abstract member function that must be implemented by child classes to do the actual object handle duplication.
        ///
        /// \param[in] h  Object handle of existing object
        ///
        /// \return Duplicated object handle
        ///
        virtual handle_type duplicate_internal(_In_ handle_type h) const noexcept = 0;
    };

    /// @}

    /// \addtogroup WinStdExceptions
    /// @{

    ///
    /// Numerical runtime error
    ///
    template <typename _Tn>
    class num_runtime_error : public std::runtime_error
    {
    public:
        typedef _Tn error_type; ///< Error number type

    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] num  Numeric error code
        /// \param[in] msg  Error message
        ///
        num_runtime_error(_In_ error_type num, _In_ const std::string& msg) :
            m_num(num),
            runtime_error(msg)
        {
        }

        ///
        /// Constructs an exception
        ///
        /// \param[in] num  Numeric error code
        /// \param[in] msg  Error message
        ///
        num_runtime_error(_In_ error_type num, _In_opt_z_ const char *msg = nullptr) :
            m_num(num),
            runtime_error(msg)
        {
        }

        ///
        /// Returns the Windows error number
        ///
        error_type number() const
        {
            return m_num;
        }

    protected:
        error_type m_num;  ///< Numeric error code
    };

    ///
    /// Windows runtime error
    ///
    class win_runtime_error : public num_runtime_error<DWORD>
    {
    public:
        ///
        /// Constructs an exception
        ///
        /// \param[in] num Windows error code
        /// \param[in] msg Error message
        ///
        win_runtime_error(_In_ error_type num, _In_ const std::string& msg) : num_runtime_error<DWORD>(num, msg)
        {
        }

        ///
        /// Constructs an exception
        ///
        /// \param[in] num  Windows error code
        /// \param[in] msg  Error message
        ///
        win_runtime_error(_In_ error_type num, _In_opt_z_ const char *msg = nullptr) : num_runtime_error<DWORD>(num, msg)
        {
        }

        ///
        /// Constructs an exception using `GetLastError()`
        ///
        /// \param[in] msg  Error message
        ///
        win_runtime_error(_In_ const std::string& msg) : num_runtime_error<DWORD>(GetLastError(), msg)
        {
        }

        ///
        /// Constructs an exception using `GetLastError()`
        ///
        /// \param[in] msg  Error message
        ///
        win_runtime_error(_In_opt_z_ const char *msg = nullptr) : num_runtime_error<DWORD>(GetLastError(), msg)
        {
        }

        ///
        /// Returns a user-readable Windows error message
        ///
        /// \sa [FormatMessage function](https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-formatmessage)
        ///
        tstring msg(_In_opt_ DWORD dwLanguageId = 0) const
        {
            tstring str;
            if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, m_num, dwLanguageId, str, NULL)) {
                // Stock Windows error messages contain CRLF. Well... Trim all the trailing white space.
                str.erase(str.find_last_not_of(_T(" \t\n\r\f\v")) + 1);
            } else
                sprintf(str, m_num >= 0x10000 ? _T("Error 0x%X") : _T("Error %u"), m_num);
            return str;
        }
    };

    /// @}

    /// \addtogroup WinStdStrFormat
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
    typedef basic_string_printf<char, std::char_traits<char>, std::allocator<char> > string_printf;

    ///
    /// Wide character implementation of a class to support string formatting using `printf()` style templates
    ///
    typedef basic_string_printf<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > wstring_printf;

    ///
    /// Multi-byte / Wide-character formatted string (according to _UNICODE)
    ///
#ifdef _UNICODE
    typedef wstring_printf tstring_printf;
#else
    typedef string_printf tstring_printf;
#endif

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
        basic_string_msg(_In_z_ _FormatMessage_format_string_ const _Elem *format, ...)
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
        basic_string_msg(_In_ HINSTANCE hInstance, _In_ UINT nFormatID, ...)
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
        basic_string_msg(_In_ HINSTANCE hInstance, _In_ WORD wLanguageID, _In_ UINT nFormatID, ...)
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
        basic_string_msg(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _In_opt_ va_list *Arguments)
        {
            FormatMessage(dwFlags & ~FORMAT_MESSAGE_ARGUMENT_ARRAY, lpSource, dwMessageId, dwLanguageId, *this, Arguments);
        }

        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style.
        ///
        /// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
        ///
        basic_string_msg(_In_ DWORD dwFlags, _In_opt_ LPCVOID lpSource, _In_ DWORD dwMessageId, _In_ DWORD dwLanguageId, _In_opt_ DWORD_PTR *Arguments)
        {
            FormatMessage(dwFlags | FORMAT_MESSAGE_ARGUMENT_ARRAY, lpSource, dwMessageId, dwLanguageId, *this, (va_list*)Arguments);
        }

        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style.
        ///
        /// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
        ///
        basic_string_msg(_In_ DWORD dwFlags, _In_z_ LPCTSTR pszFormat, _In_opt_ va_list *Arguments)
        {
            FormatMessage(dwFlags & ~FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_STRING, pszFormat, 0, 0, *this, Arguments);
        }

        ///
        /// Initializes a new string and formats its contents using `FormatMessage()` style.
        ///
        /// \sa [FormatMessage function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351.aspx)
        ///
        basic_string_msg(_In_ DWORD dwFlags, _In_z_ LPCTSTR pszFormat, _In_opt_ DWORD_PTR *Arguments)
        {
            FormatMessage(dwFlags | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_STRING, pszFormat, 0, 0, *this, (va_list*)Arguments);
        }
    };

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

    ///
    /// Base template class to support converting GUID to string
    ///
    template<class _Elem, class _Traits, class _Ax>
    class basic_string_guid : public std::basic_string<_Elem, _Traits, _Ax>
    {
    public:
        /// \name Initializing string using template in memory
        /// @{

        ///
        /// Initializes a new string and formats its contents to string representation of given GUID.
        ///
        /// \param[in] guid    GUID to convert
        /// \param[in] format  A `printf()` syntax template to convert GUID to string (i.e. `"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"`)
        ///
        basic_string_guid(_In_ const GUID &guid, _In_z_ _Printf_format_string_ const _Elem *format)
        {
            sprintf<_Elem, _Traits, _Ax>(*this, format,
                guid.Data1,
                guid.Data2,
                guid.Data3,
                guid.Data4[0], guid.Data4[1],
                guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        }

        /// @}
    };

    ///
    /// Single-byte character implementation of a class to support converting GUID to string
    ///
    class string_guid : public basic_string_guid<char, std::char_traits<char>, std::allocator<char> >
    {
    public:
        /// \name Initializing string using template in memory
        /// @{

        ///
        /// Initializes a new string and formats its contents to string representation of given GUID.
        ///
        /// \param[in] guid  GUID to convert
        ///
        string_guid(_In_ const GUID &guid) :
            basic_string_guid<char, std::char_traits<char>, std::allocator<char> >(guid, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}")
        {
        }

        /// @}
    };

    ///
    /// Wide character implementation of a class to support converting GUID to string
    ///
    class wstring_guid : public basic_string_guid<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >
    {
    public:
        /// \name Initializing string using template in memory
        /// @{

        ///
        /// Initializes a new string and formats its contents to string representation of given GUID.
        ///
        /// \param[in] guid  GUID to convert
        ///
        wstring_guid(_In_ const GUID &guid) :
            basic_string_guid<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >(guid, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}")
        {
        }

        /// @}
    };

    ///
    /// Multi-byte / Wide-character string GUID (according to _UNICODE)
    ///
#ifdef _UNICODE
    typedef wstring_guid tstring_guid;
#else
    typedef string_guid tstring_guid;
#endif

    /// @}

    /// \addtogroup WinStdMemSanitize
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
        sanitizing_allocator() noexcept : _Mybase()
        {
        }

        ///
        /// Construct by copying
        ///
        sanitizing_allocator(_In_ const sanitizing_allocator<_Ty> &_Othr) : _Mybase(_Othr)
        {
        }

        ///
        /// Construct from a related allocator
        ///
        template<class _Other>
        sanitizing_allocator(_In_ const sanitizing_allocator<_Other> &_Othr) noexcept : _Mybase(_Othr)
        {
        }

        ///
        /// Deallocate object at _Ptr sanitizing its content first
        ///
        void deallocate(_In_ pointer _Ptr, _In_ size_type _Size)
        {
            // Sanitize then free.
            SecureZeroMemory(_Ptr, _Size);
            _Mybase::deallocate(_Ptr, _Size);
        }
    };

    #pragma warning(pop)

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

    ///
    /// Sanitizing BLOB
    ///
    template<size_t N>
    class sanitizing_blob
    {
    public:
        ///
        /// Constructs uninitialized BLOB
        ///
        sanitizing_blob()
        {
            ZeroMemory(m_data, N);
        }

        ///
        /// Sanitizes BLOB
        ///
        ~sanitizing_blob()
        {
            SecureZeroMemory(m_data, N);
        }

    public:
        unsigned char m_data[N];    ///< BLOB data
    };

    /// @}
}
