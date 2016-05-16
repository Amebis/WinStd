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

#pragma once

#include "Common.h"

#include <string>
#include <vector>


namespace winstd
{
    ///
    /// \defgroup WinStdBase64 Base64 conversion
    /// Provides Base64 conversion for WinStd classes
    ///
    /// @{

    ///
    /// Base64 encoding session
    ///
    class base64_enc
    {
    public:
        inline base64_enc() : num(0) {};


        ///
        /// Encodes one block of information, and _appends_ it to the output
        ///
        /// \param[out] out      Output
        /// \param[in ] data     Data to encode
        /// \param[in ] size     Length of `data` in bytes
        /// \param[in ] is_last  Is this the last block of data?
        ///
        template<class _Elem, class _Traits, class _Ax>
        inline void encode(_Out_ std::basic_string<_Elem, _Traits, _Ax> &out, _In_bytecount_(size) const void *data, _In_ size_t size, _In_opt_ bool is_last = true)
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
        template<class _Elem, class _Traits, class _Ax>
        static inline void encode(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &out, _In_ const unsigned char buf[3])
        {
            out += lookup[                  buf[0] >> 2         ];
            out += lookup[((buf[0] << 4) | (buf[1] >> 4)) & 0x3f];
            out += lookup[((buf[1] << 2) | (buf[2] >> 6)) & 0x3f];
            out += lookup[                  buf[2]        & 0x3f];
        }


        template<class _Elem, class _Traits, class _Ax>
        static inline void encode(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &out, _In_count_(size) const unsigned char buf[3], _In_ size_t size)
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
                    out += "==";
                }
            } else
                out += "====";
        }


    protected:
        unsigned char buf[3];
        size_t num;

        static const char lookup[64];
    };


    ///
    /// Base64 decoding session
    ///
    class WINSTD_API base64_dec
    {
    public:
        inline base64_dec() : num(0) {};


        ///
        /// Decodes one block of information, and _appends_ it to the output
        ///
        /// \param[out] out      Output
        /// \param[in ] data     Data to decode
        /// \param[in ] size     Length of `data` in bytes
        /// \param[in ] is_last  Was this the last block of data?
        ///
        template <class T>
        inline void decode(_Out_ std::vector<unsigned char> &out, _Out_ bool &is_last, _In_z_count_(size) const T *data, _In_ size_t size)
        {
            size_t i = 0, j = 0, nibbles;

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


        ///
        /// Resets decoding session
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
        inline size_t dec_size(size_t size) const
        {
            return ((num + size + 3)/4)*3;
        }


    protected:
        static inline size_t decode(_Inout_ std::vector<unsigned char> &out, _In_ const unsigned char buf[4])
        {
            out.push_back((unsigned char)(((buf[0] << 2) | (buf[1] >> 4)) & 0xff));
            if (buf[2] < 64) {
                out.push_back((unsigned char)(((buf[1] << 4) | (buf[2] >> 2)) & 0xff));
                if (buf[3] < 64) {
                    out.push_back((unsigned char)(((buf[2] << 6) | buf[3]) & 0xff));
                    return 3;
                } else
                    return 2;
            } else
                return 1;
        }


    protected:
        unsigned char buf[4];
        size_t num;

        static const unsigned char lookup[256];
    };

    /// @}
}

