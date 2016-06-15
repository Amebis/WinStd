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

#include <string>
#include <vector>

namespace winstd
{
    class WINSTD_API hex_enc;
    class WINSTD_API hex_dec;
}

#pragma once


namespace winstd
{
    ///
    /// \defgroup WinStdHexadecimal Hexadecimal conversion
    /// Provides Hexadecimal conversion for WinStd classes
    ///
    /// @{

    ///
    /// Hexadecimal encoding session
    ///
    class WINSTD_API hex_enc
    {
    public:
        ///
        /// Constructs blank encoding session
        ///
        inline hex_enc()
        {
        }


        ///
        /// Encodes one block of information, and _appends_ it to the output
        ///
        /// \param[out] out   Output
        /// \param[in ] data  Data to encode
        /// \param[in ] size  Length of `data` in bytes
        ///
        template<class _Elem, class _Traits, class _Ax>
        inline void encode(_Out_ std::basic_string<_Elem, _Traits, _Ax> &out, _In_bytecount_(size) const void *data, _In_ size_t size)
        {
            assert(data || !size);

            // Preallocate output
            out.reserve(out.size() + enc_size(size));

            // Convert data character by character.
            for (size_t i = 0; i < size; i++) {
                unsigned char
                    x   = ((unsigned char*)data)[i],
                    x_h = ((x & 0xf0) >> 4),
                    x_l = ((x & 0x0f)     );

                out += x_h < 10 ? '0' + x_h : 'A' - 10 + x_h;
                out += x_l < 10 ? '0' + x_l : 'A' - 10 + x_l;
            }
        }


        ///
        /// Returns maximum encoded size
        ///
        /// \param size  Number of bytes to encode
        ///
        /// \returns Maximum number of bytes for the encoded data of `size` length
        ///
        inline size_t enc_size(size_t size) const
        {
            return size*2;
        }
    };


    ///
    /// Hexadecimal decoding session
    ///
    class WINSTD_API hex_dec
    {
    public:
        ///
        /// Constructs blank decoding session
        ///
        inline hex_dec() : num(0)
        {
        }


        ///
        /// Decodes one block of information, and _appends_ it to the output
        ///
        /// \param[out] out      Output
        /// \param[in ] data     Data to decode
        /// \param[in ] size     Length of `data` in bytes
        /// \param[in ] is_last  Was this the last block of data? Actually, is this block of data complete?
        ///
        template<class _Ty, class _Ax, class _Tchr>
        inline void decode(_Out_ std::vector<_Ty, _Ax> &out, _Out_ bool &is_last, _In_z_count_(size) const _Tchr *data, _In_ size_t size)
        {
            is_last = false;

            // Trim data size to first terminator.
            for (size_t k = 0; k < size; k++)
                if (!data[k]) { size = k; break; }

            // Preallocate output
            out.reserve(out.size() + dec_size(size));

            for (size_t i = 0;; i++) {
                if (num >= 2) {
                    // Buffer full.
                    out.push_back(buf);
                    num = 0;
                    is_last = true;
                } else
                    is_last = false;

                if (i >= size)
                    break;

                int x = data[i];
                if ('0' <= x && x <= '9') {
                    buf = ((buf & 0xf) << 4) | (unsigned char)(x - '0');
                    num++;
                } else if ('A' <= x && x <= 'F') {
                    buf = ((buf & 0xf) << 4) | (unsigned char)(x - ('A' - 10));
                    num++;
                } else if ('a' <= x && x <= 'f') {
                    buf = ((buf & 0xf) << 4) | (unsigned char)(x - ('a' - 10));
                    num++;
                }
            }
        }


        ///
        /// Resets decoding session
        ///
        inline void clear()
        {
            num = 0;
        }


        ///
        /// Returns maximum decoded size
        ///
        /// \param size  Number of bytes to decode
        ///
        /// \returns Maximum number of bytes for the decoded data of `size` length
        ///
        inline size_t dec_size(size_t size) const
        {
            return (size + 1)/2;
        }


    protected:
        unsigned char buf;  ///< Internal buffer
        size_t num;         ///< Number of nibbles used in `buf`
    };

    /// @}
}
