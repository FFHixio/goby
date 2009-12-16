// copyright 2009 t. schneider tes@mit.edu
// 
// this file is part of goby-acomms, a collection of libraries for acoustic underwater networking
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this software.  If not, see <http://www.gnu.org/licenses/>.

#ifndef AcommsConstants20091122H
#define AcommsConstants20091122H

#include <string>
#include <limits>
#include <bitset>

namespace acomms_util
{
    const unsigned BITS_IN_BYTE = 8;
    // one hex char is a nibble (4 bits), two nibbles per byte
    const unsigned NIBS_IN_BYTE = 2;
    
    const unsigned BROADCAST_ID = 0;

    const std::string DCCL_CCL_HEADER_STR = "20";
    const std::bitset<BITS_IN_BYTE> DCCL_CCL_HEADER(32);
    
    // 00000111
    const unsigned BYTE_MASK = 7; 

    const double NaN = std::numeric_limits<double>::quiet_NaN();

    // number of frames for a given packet type
    const unsigned PACKET_FRAME_COUNT [] = { 1, 3, 3, 2, 2, 8 };

    // data size
    const unsigned PACKET_SIZE [] = { 32, 32, 64, 256, 256, 256 };

    const unsigned NUM_HEADER_BYTES = 2;

}

#endif
