//
//  convert.hpp
//  bbb.max.dev
//
//  Created by ISHII 2bit on 2017/12/22.
//

#ifndef bbb_max_dev_convert_hpp
#define bbb_max_dev_convert_hpp

#include "../type_traits/type_traits.hpp"

namespace bbb {
    namespace max {
        template <typename type>
        inline auto as(t_atom *v)
            -> enable_if_t<std::is_integral<type>::value, type>
        {
            return static_cast<type>(atom_getlong(v));
        }
        
        template <typename type>
        inline auto as(t_atom *v)
            -> enable_if_t<std::is_floating_point<type>::value, type>
        {
            return static_cast<type>(atom_getfloat(v));
        }
        
        template <typename type>
        inline auto as(t_atom *v)
            -> enable_if_t<std::is_same<type, std::string>::value, type>
        {
            return {atom_getsym(v)->s_name};
        }
        
        template <typename type>
        inline auto as(t_atom *v)
            -> enable_if_t<std::is_same<type, char *>::value, type>
        {
            return atom_getsym(v)->s_name;
        }
    };
};

#endif /* bbb_max_dev_convert_hpp */
