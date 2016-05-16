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
// winstd::base64_enc
//////////////////////////////////////////////////////////////////////

const char winstd::base64_enc::lookup[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};


void winstd::base64_enc::encode(_Out_ std::string &out, _In_bytecount_(size) const void *data, _In_ size_t size, _In_opt_ bool is_last)
{
    assert(data || !size);

    size_t i = 0, j = 0;

    // Convert data character by character.
    for (;;) {
        if (num >= 3) {
            encode(out, buf);
            num = 0;
            j += 4;
        }

        if (i >= size)
            break;

        buf[num++] = ((unsigned char*)data)[i++];
    }

    // If this is the last block, flush the buffer.
    if (is_last && num) {
        encode(out, buf, num);
        num = 0;
        j += 4;
    }
}


//////////////////////////////////////////////////////////////////////
// winstd::base64_dec
//////////////////////////////////////////////////////////////////////

const unsigned char winstd::base64_dec::lookup[256] = {
/*           0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F  */
/* 0 */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* 1 */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* 2 */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
/* 3 */     52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255, 255,  64, 255, 255,
/* 4 */    255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
/* 5 */     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
/* 6 */    255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
/* 7 */     41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255,
/* 8 */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* 9 */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* A */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* B */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* C */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* D */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* E */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/* F */    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};


void winstd::base64_dec::decode(_Out_ std::vector<unsigned char> &out, _Out_ bool &is_last, _In_z_count_(size) const char *data, _In_ size_t size)
{
    size_t i = 0, j = 0;
    size_t nibbles;

    is_last = false;

    for (;;) {
        if (num >= 4) {
            // Buffer full; decode it.
            nibbles = decode(out, buf);
            j += nibbles;
            num = 0;
            if (nibbles < 3) {
                is_last = true;
                break;
            }
        }

        if (i >= size || !data[i])
            break;

        if ((buf[num] = lookup[(unsigned char)data[i++]]) != 255)
            num++;
    }
}
