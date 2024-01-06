/*
    SPDX-License-Identifier: MIT
    Copyright © 2022-2024 Amebis
*/

/// \defgroup WinStdSDDL SDDL

#pragma once

#include "Common.h"
#include <sddl.h>

namespace winstd
{
    /// \addtogroup WinStdSDDL
    /// @{

    class security_attributes : public SECURITY_ATTRIBUTES
    {
        WINSTD_NONCOPYABLE(security_attributes)

    public:
        ///
        /// Initializes a new SECURITY_ATTRIBUTES.
        ///
        security_attributes() noexcept
        {
            nLength              = sizeof(SECURITY_ATTRIBUTES);
            lpSecurityDescriptor = NULL;
            bInheritHandle       = FALSE;
        }

        ///
        /// Moves an existing SECURITY_ATTRIBUTES.
        ///
        security_attributes(_Inout_ security_attributes &&a) noexcept
        {
            nLength                = sizeof(SECURITY_ATTRIBUTES);
            lpSecurityDescriptor   = a.lpSecurityDescriptor;
            bInheritHandle         = a.bInheritHandle;
            a.lpSecurityDescriptor = NULL;
        }

        ///
        /// Destroys the SECURITY_ATTRIBUTES.
        ///
        ~security_attributes()
        {
            if (lpSecurityDescriptor)
                LocalFree(lpSecurityDescriptor);
        }

        ///
        /// Moves an existing SECURITY_ATTRIBUTES.
        ///
        security_attributes& operator=(_Inout_ security_attributes &&a) noexcept
        {
            if (this != &a) {
                nLength                = sizeof(SECURITY_ATTRIBUTES);
                if (lpSecurityDescriptor)
                    LocalFree(lpSecurityDescriptor);
                lpSecurityDescriptor   = a.lpSecurityDescriptor;
                bInheritHandle         = a.bInheritHandle;
                a.lpSecurityDescriptor = NULL;
            }
            return *this;
        }
    };

    /// @}
}

/// \addtogroup WinStdSDDL
/// @{

#pragma warning(push)
#pragma warning(disable: 4505) // Don't warn on unused code

/// @copydoc ConvertStringSecurityDescriptorToSecurityDescriptorW()
_Success_(return != FALSE) static BOOL WINAPI ConvertStringSecurityDescriptorToSecurityDescriptorA(_In_ LPCSTR StringSecurityDescriptor, _In_ DWORD StringSDRevision, _Inout_ winstd::security_attributes &sa, _Out_opt_ PULONG SecurityDescriptorSize)
{
    PSECURITY_DESCRIPTOR sd;
    BOOL bResult = ConvertStringSecurityDescriptorToSecurityDescriptorA(StringSecurityDescriptor, StringSDRevision, &sd, SecurityDescriptorSize);
    if (bResult) {
        if (sa.lpSecurityDescriptor)
            LocalFree(sa.lpSecurityDescriptor);
        sa.lpSecurityDescriptor = sd;
    }
    return bResult;
}

///
/// Converts a string-format security descriptor into a valid, functional security descriptor.
///
/// \sa [ConvertStringSecurityDescriptorToSecurityDescriptor function](https://docs.microsoft.com/en-us/windows/win32/api/sddl/nf-sddl-convertstringsecuritydescriptortosecuritydescriptorw)
///
_Success_(return != FALSE) static BOOL WINAPI ConvertStringSecurityDescriptorToSecurityDescriptorW(_In_ LPCWSTR StringSecurityDescriptor, _In_ DWORD StringSDRevision, _Inout_ winstd::security_attributes &sa, _Out_opt_ PULONG SecurityDescriptorSize)
{
    PSECURITY_DESCRIPTOR sd;
    BOOL bResult = ConvertStringSecurityDescriptorToSecurityDescriptorW(StringSecurityDescriptor, StringSDRevision, &sd, SecurityDescriptorSize);
    if (bResult) {
        if (sa.lpSecurityDescriptor)
            LocalFree(sa.lpSecurityDescriptor);
        sa.lpSecurityDescriptor = sd;
    }
    return bResult;
}

#pragma warning(pop)

/// @}
