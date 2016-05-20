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

#include "StdAfx.h"


//////////////////////////////////////////////////////////////////////
// winstd::library
//////////////////////////////////////////////////////////////////////

winstd::library::~library()
{
    if (m_h)
        FreeLibrary(m_h);
}


void winstd::library::free_internal()
{
    FreeLibrary(m_h);
}


//////////////////////////////////////////////////////////////////////
// winstd::heap
//////////////////////////////////////////////////////////////////////

winstd::heap::~heap()
{
    if (m_h)
        HeapDestroy(m_h);
}


void winstd::heap::free_internal()
{
    HeapDestroy(m_h);
}
