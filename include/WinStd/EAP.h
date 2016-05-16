/*
    Copyright 1991-2016 Amebis

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

#include <eaptypes.h>


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
    class eap_attr : public EAP_ATTRIBUTE
    {
    public:
        ///
        /// Initializes a new EAP attribute set to eatReserved.
        ///
        eap_attr()
        {
            eaType   = eatReserved;
            dwLength = 0;
            pValue   = NULL;
        }

        ///
        /// Destroys the EAP attribute.
        ///
        ~eap_attr()
        {
            if (pValue)
                delete pValue;
        }
    };

    /// @}
}
