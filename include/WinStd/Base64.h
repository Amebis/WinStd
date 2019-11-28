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

///
/// \defgroup WinStdBase64 Base64 conversion
/// Provides Base64 conversion for WinStd classes
///

#include "Common.h"

#include <string>
#include <vector>

namespace winstd
{
    class WINSTD_API base64_enc;
    class WINSTD_API base64_dec;
}

#pragma once


namespace winstd
{
    /// \addtogroup WinStdBase64
    /// @{

    ///
    /// Base64 encoding session
    ///
    class WINSTD_API base64_enc
    {
    public:
        ///
        /// Constructs blank encoding session
        ///
        inline base64_enc() : num(0)
        {
            buf[0] = 0;
            buf[1] = 0;
            buf[2] = 0;
        }


        ///
        /// Encodes one block of information, and _appends_ it to the output
        ///
        /// \param[out] out      Output
        /// \param[in ] data     Data to encode
        /// \param[in ] size     Length of `data` in bytes
        /// \param[in ] is_last  Is this the last block of data?
        ///
        template<class _Elem, class _Traits, class _Ax>
        inline void encode(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &out, _In_bytecount_(size) const void *data, _In_ size_t size, _In_opt_ bool is_last = true)
        {
            assert(data || !size);

            // Preallocate output
            out.reserve(out.size() + enc_size(size));

            // Convert data character by character.
            for (size_t i = 0;; i++) {
                if (num >= 3) {
                    encode(out);
                    num = 0;
                }

                if (i >= size)
                    break;

                buf[num++] = reinterpret_cast<const unsigned char*>(data)[i];
            }

            // If this is the last block, flush the buffer.
            if (is_last && num) {
                encode(out, num);
                num = 0;
            }
        }


        ///
        /// Resets encoding session
        ///
        inline void clear()
        {
            num = 0;
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
            return ((num + size + 2)/3)*4;
        }


    protected:
        ///
        /// Encodes one complete internal buffer of data
        ///
        template<class _Elem, class _Traits, class _Ax>
        inline void encode(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &out)
        {
            out += lookup[                  buf[0] >> 2         ];
            out += lookup[((buf[0] << 4) | (buf[1] >> 4)) & 0x3f];
            out += lookup[((buf[1] << 2) | (buf[2] >> 6)) & 0x3f];
            out += lookup[                  buf[2]        & 0x3f];
        }


        ///
        /// Encodes partial internal buffer of data
        ///
        template<class _Elem, class _Traits, class _Ax>
        inline void encode(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &out, _In_ size_t size)
        {
            if (size > 0) {
                out += lookup[buf[0] >> 2];
                if (size > 1) {
                    out += lookup[((buf[0] << 4) | (buf[1] >> 4)) & 0x3f];
                    if (size > 2) {
                        out += lookup[((buf[1] << 2) | (buf[2] >> 6)) & 0x3f];
                        out += lookup[buf[2] & 0x3f];
                    } else {
                        out += lookup[(buf[1] << 2) & 0x3f];
                        out += '=';
                    }
                } else {
                    out += lookup[(buf[0] << 4) & 0x3f];
                    out += '=';
                    out += '=';
                }
            } else {
                out += '=';
                out += '=';
                out += '=';
                out += '=';
            }
        }


    protected:
        unsigned char buf[3];           ///< Internal buffer
        size_t num;                     ///< Number of bytes used in `buf`

        /// \cond internal
        static const char lookup[64];
        /// \endcond
    };


    ///
    /// Base64 decoding session
    ///
    class WINSTD_API base64_dec
    {
    public:
        ///
        /// Constructs blank decoding session
        ///
        inline base64_dec() : num(0)
        {
            buf[0] = 0;
            buf[1] = 0;
            buf[2] = 0;
            buf[3] = 0;
        }


        ///
        /// Decodes one block of information, and _appends_ it to the output
        ///
        /// \param[out] out      Output
        /// \param[in ] is_last  Was this the last block of data?
        /// \param[in ] data     Data to decode
        /// \param[in ] size     Length of `data` in bytes
        ///
        template<class _Ty, class _Ax, class _Tchr>
        inline void decode(_Inout_ std::vector<_Ty, _Ax> &out, _Out_ bool &is_last, _In_z_count_(size) const _Tchr *data, _In_ size_t size)
        {
            is_last = false;

            // Trim data size to first terminator.
            for (size_t k = 0; k < size; k++)
                if (!data[k]) { size = k; break; }

            // Preallocate output
            out.reserve(out.size() + dec_size(size));

            for (size_t i = 0;; i++) {
                if (num >= 4) {
                    // Buffer full; decode it.
                    size_t nibbles = decode(out);
                    num = 0;
                    if (nibbles < 3) {
                        is_last = true;
                        break;
                    }
                }

                if (i >= size)
                    break;

                int x = data[i];
                if ((buf[num] = x < _countof(lookup) ? lookup[x] : 255) != 255)
                    num++;
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
            return ((num + size + 3)/4)*3;
        }


    protected:
        ///
        /// Decodes one complete internal buffer of data
        ///
        template<class _Ty, class _Ax>
        inline size_t decode(_Inout_ std::vector<_Ty, _Ax> &out)
        {
            out.push_back((_Ty)(((buf[0] << 2) | (buf[1] >> 4)) & 0xff));
            if (buf[2] < 64) {
                out.push_back((_Ty)(((buf[1] << 4) | (buf[2] >> 2)) & 0xff));
                if (buf[3] < 64) {
                    out.push_back((_Ty)(((buf[2] << 6) | buf[3]) & 0xff));
                    return 3;
                } else
                    return 2;
            } else
                return 1;
        }


    protected:
        unsigned char buf[4];                   ///< Internal buffer
        size_t num;                             ///< Number of bytes used in `buf`

        /// \cond internal
        static const unsigned char lookup[256];
        /// \endcond
    };

    /// @}
}
