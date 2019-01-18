/*
    Copyright 1991-2019 Amebis
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

#include "StdAfx.h"


#if defined(SECURITY_WIN32) || defined(SECURITY_KERNEL) || defined(SECURITY_MAC)

//////////////////////////////////////////////////////////////////////
// winstd::sec_credentials
//////////////////////////////////////////////////////////////////////

winstd::sec_credentials::~sec_credentials()
{
    if (m_h) {
        FreeCredentialsHandle(m_h);
        delete m_h;
    }
}


void winstd::sec_credentials::free_internal()
{
    FreeCredentialsHandle(m_h);
    delete m_h;
}


//////////////////////////////////////////////////////////////////////
// winstd::sec_context
//////////////////////////////////////////////////////////////////////

winstd::sec_context::~sec_context()
{
    if (m_h) {
        DeleteSecurityContext(m_h);
        delete m_h;
    }
}


void winstd::sec_context::free_internal()
{
    DeleteSecurityContext(m_h);
    delete m_h;
}


//////////////////////////////////////////////////////////////////////
// winstd::sec_buffer_desc
//////////////////////////////////////////////////////////////////////

winstd::sec_buffer_desc::~sec_buffer_desc()
{
    for (ULONG i = 0; i < cBuffers; i++) {
        if (pBuffers[i].pvBuffer)
            FreeContextBuffer(pBuffers[i].pvBuffer);
    }
}

#endif
