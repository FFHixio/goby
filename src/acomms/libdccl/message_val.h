// copyright 2008, 2009 t. schneider tes@mit.edu
// 
// this file is part of the Dynamic Compact Control Language (DCCL),
// the goby-acomms codec. goby-acomms is a collection of libraries 
// for acoustic underwater networking
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

#ifndef MESSAGE_VAL20091211H
#define MESSAGE_VAL20091211H

#include <iostream>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp> 

#include "util/tes_utils.h"
#include "dccl_constants.h"

namespace dccl
{
    
/// defines a DCCL value
    class MessageVal
    {
      public:
        /// \name Constructors/Destructor
        //@{         
        /// empty
      MessageVal()
          : sval_(""),
            dval_(0),
            lval_(0),
            bval_(false),
            precision_(15),
            type_(cpp_notype)
        {}

        /// construct with string value
      MessageVal(const std::string& s)
          : sval_(s),
            dval_(0),
            lval_(0),
            bval_(false),
            precision_(15),
            type_(cpp_string)
        {}

        /// construct with double value
      MessageVal(double d, int p = 15)
          : sval_(""),
            dval_(d),
            lval_(0),
            bval_(false),
            precision_(p),
            type_(cpp_double)
        {}

        /// construct with long value
      MessageVal(long l)
          : sval_(""),
            dval_(0),
            lval_(l),
            bval_(false),
            precision_(15),
            type_(cpp_long)
        {}

        /// construct with bool value
      MessageVal(bool b)
          : sval_(""),
            dval_(0),
            lval_(0),
            bval_(b),
            precision_(15),
            type_(cpp_bool)
        {}
        //@}

        /// \name Setters
        //@{  
        /// set the value with a string (overwrites previous value regardless of type)
        void set(std::string sval);
        /// \brief set the value with a double (overwrites previous value regardless of type)
        /// \param dval values to set
        /// \param precision decimal places of precision to preserve if this is cast to a string
        void set(double dval, int precision = 15);
        /// set the value with a long (overwrites previous value regardless of type)
        void set(long lval);
        /// set the value with a bool (overwrites previous value regardless of type)
        void set(bool bval);
        //@}
        
        /// \name Getters
        //@{       
        /// \brief extract as std::string (all reasonable casts are done)
        /// \param s std::string to store value in
        /// \return successfully extracted (and if necessary successfully cast to this type)
        bool val(std::string& s) const;
        /// \brief extract as bool (all reasonable casts are done)
        /// \param b bool to store value in
        /// \return successfully extracted (and if necessary successfully cast to this type)
        bool val(bool& b) const;
        /// \brief extract as long (all reasonable casts are done)
        /// \param t long to store value in
        /// \return successfully extracted (and if necessary successfully cast to this type)
        bool val(long& t) const;        
        /// \brief extract as double (all reasonable casts are done)
        /// \param d double to store value in
        /// \return successfully extracted (and if necessary successfully cast to this type)
        bool val(double& d) const;

        /// what type is the original type of this MessageVal?
        DCCLCppType type() const { return type_; }        
        //@}

        bool operator==(const std::string& s)
        {
            std::string us;
            val(us);
            return us == s;
        }
        bool operator==(double d)
        {
            double us;
            val(us);
            return us == d;
        }
        bool operator==(long l)
        {
            long us;
            val(us);
            return us == l;
        }
        bool operator==(bool b)
        {
            bool us;
            val(us);
            return us == b;
        }
        
        
      private:
        friend std::ostream& operator<<(std::ostream& os,  const MessageVal& mv);        
        
      private:
        std::string sval_;
        double dval_;
        long lval_;
        bool bval_;

        unsigned precision_;

        DCCLCppType type_;
    };

    inline std::ostream& operator<<(std::ostream& os, const MessageVal& mv)
    {
        os << "sval: " << mv.sval_ << std::endl;
        os << "dval: " << mv.dval_ << std::endl;
        os << "lval: " << mv.lval_ << std::endl;
        os << "bval: " << mv.bval_;
        return os;
    }

}


#endif
