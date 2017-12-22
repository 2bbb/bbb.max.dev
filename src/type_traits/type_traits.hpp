//
//  type_traits.hpp
//  bbb.max.dev
//
//  Created by ISHII 2bit on 2017/12/21.
//

#ifndef bbb_max_dev_type_traits_hpp
#define bbb_max_dev_type_traits_hpp

#include <type_traits>

#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "commonsyms.h"
#include "jpatcher_api.h"
#include "jgraphics.h"
#include "z_dsp.h"

namespace bbb {
    namespace max {
        template <bool condition_value, typename result_type = void>
        using enable_if_t = typename std::enable_if<condition_value, result_type>::type;
        
        template <template <typename> class method_def, typename check_type>
        struct sfinae_method_checker {
            template <typename, typename method_def<check_type>::type> class checker;
            
            template <typename checkee>
            static std::true_type test(checker<checkee, method_def<checkee>::value> *);
            
            template <typename>
            static std::false_type test(...);
            
            using type = decltype(test<check_type>(nullptr));
            static constexpr bool value = decltype(test<check_type>(nullptr))::value;
        };
        
        template <typename type>
        using general_gimme_method = void(type::*)(long inlet, t_symbol * s, long ac, t_atom * av);
        template <typename type, general_gimme_method<type> f>
        struct general_gimme_method_caller {
            inline static void call(type *obj, t_symbol *sym, long argc, t_atom *argv) {
                (obj->*f)(proxy_getinlet((t_object *)obj), sym, argc, argv);
            }
        };
        
        template <typename check_type>
        struct bang_method_def {
            using type = void(check_type::*)(t_object *);
            type value = &check_type::bang;
        };
        
        template <typename check_type>
        struct assist_method_def {
            using type = void(check_type::*)(void *, long, long, char *s);
            type value = &check_type::assist;
        };
        
        template <typename check_type>
        struct loadbang_method_def {
            using type = void (check_type::*)(void *);
            type value = &check_type::loadbang;
        };
    };
};

#endif /* bbb_max_dev_type_traits_hpp */
