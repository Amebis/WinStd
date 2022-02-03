/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 1991-2022 Amebis
    Copyright © 2016 GÉANT
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
    class base64_enc;
    class base64_dec;
}

#pragma once


namespace winstd
{
    /// \addtogroup WinStdBase64
    /// @{

    ///
    /// Base64 encoding session
    ///
    class base64_enc
    {
    public:
        ///
        /// Constructs blank encoding session
        ///
        inline base64_enc() noexcept : num(0)
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
        inline void clear() noexcept
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
        inline size_t enc_size(size_t size) const noexcept
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
            out += base64_enc_lookup[                  buf[0] >> 2         ];
            out += base64_enc_lookup[((buf[0] << 4) | (buf[1] >> 4)) & 0x3f];
            out += base64_enc_lookup[((buf[1] << 2) | (buf[2] >> 6)) & 0x3f];
            out += base64_enc_lookup[                  buf[2]        & 0x3f];
        }


        ///
        /// Encodes partial internal buffer of data
        ///
        template<class _Elem, class _Traits, class _Ax>
        inline void encode(_Inout_ std::basic_string<_Elem, _Traits, _Ax> &out, _In_ size_t size)
        {
            if (size > 0) {
                out += base64_enc_lookup[buf[0] >> 2];
                if (size > 1) {
                    out += base64_enc_lookup[((buf[0] << 4) | (buf[1] >> 4)) & 0x3f];
                    if (size > 2) {
                        out += base64_enc_lookup[((buf[1] << 2) | (buf[2] >> 6)) & 0x3f];
                        out += base64_enc_lookup[buf[2] & 0x3f];
                    } else {
                        out += base64_enc_lookup[(buf[1] << 2) & 0x3f];
                        out += '=';
                    }
                } else {
                    out += base64_enc_lookup[(buf[0] << 4) & 0x3f];
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
    };


    /// \cond internal
    static const char base64_enc_lookup[64] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };
    /// \endcond


    ///
    /// Base64 decoding session
    ///
    class base64_dec
    {
    public:
        ///
        /// Constructs blank decoding session
        ///
        inline base64_dec() noexcept : num(0)
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
                if ((buf[num] = x < _countof(base64_dec_lookup) ? base64_dec_lookup[x] : 255) != 255)
                    num++;
            }
        }


        ///
        /// Resets decoding session
        ///
        inline void clear() noexcept
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
        inline size_t dec_size(size_t size) const noexcept
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
    };


    /// \cond internal
    static const unsigned char base64_dec_lookup[256] = {
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
    /// \endcond

    /// @}
}
