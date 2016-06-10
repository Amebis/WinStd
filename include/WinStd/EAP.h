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

#include "Common.h"

#include <Windows.h>
#include <eaptypes.h> // Must include after <Windows.h>

namespace winstd
{
    class WINSTD_API eap_attr;
}

#pragma once


namespace winstd
{
    ///
    /// \defgroup WinStdEAPAPI Extensible Authentication Protocol API
    /// Integrates WinStd classes with Microsoft EAP API
    ///
    /// @{

    ///
    /// EAP_ATTRIBUTE wrapper class
    ///
    class WINSTD_API eap_attr : public EAP_ATTRIBUTE
    {
    public:
        ///
        /// Initializes a new EAP attribute set to eatReserved.
        ///
        inline eap_attr()
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
        inline eap_attr(_Inout_ EAP_ATTRIBUTE &&a)
        {
            eaType   = a.eaType;
            dwLength = a.dwLength;
            if (a.dwLength) {
                pValue = a.pValue;
                a.pValue = NULL;
            } else
                pValue = NULL;
        }

        ///
        /// Destroys the EAP attribute.
        ///
        virtual ~eap_attr();

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
        inline eap_attr& operator=(_Inout_ EAP_ATTRIBUTE &&a)
        {
            if (this != &a) {
                eaType   = a.eaType;
                dwLength = a.dwLength;
                if (pValue)
                    delete [] pValue;
                if (a.dwLength) {
                    pValue = a.pValue;
                    a.pValue = NULL;
                } else
                    pValue = NULL;
            }
            return *this;
        }
    };

    /// @}
}
