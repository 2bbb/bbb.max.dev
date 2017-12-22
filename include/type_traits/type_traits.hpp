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
        
        template <typename type>
        using gimme_method_t = void(type::*)(long inlet, t_symbol * s, long ac, t_atom * av);
        
        template <typename type, gimme_method_t<type> f>
        struct gimme_method_caller {
            inline static void call(type *obj, t_symbol *sym, long argc, t_atom *argv) {
                (obj->*f)(proxy_getinlet((t_object *)obj), sym, argc, argv);
            }
        };
        
        template <typename type>
        using bang_method_t = void(type::*)(t_object *);
        
        template <typename check_type>
        struct has_bang_method {
            template <typename, bang_method_t<check_type>> class checker;
            
            template <typename checkee>
            static std::true_type test(checker<checkee, &checkee::bang> *);
            
            template <typename>
            static std::false_type test(...);
            
            using type = decltype(test<check_type>(nullptr));
            static constexpr bool value = decltype(test<check_type>(nullptr))::value;
        };

        template <typename type, bang_method_t<type> f>
        struct bang_method_caller {
            inline static void call(type *obj, void *b) {
                (obj->*f)(b);
            }
        };
        
        template <typename type>
        using assist_method_t = void(type::*)(void *, long, long, char *);
        
        template <typename check_type>
        struct has_assist_method {
            template <typename, assist_method_t<check_type>> class checker;
            
            template <typename checkee>
            static std::true_type test(checker<checkee, &checkee::assist> *);
            
            template <typename>
            static std::false_type test(...);
            
            using type = decltype(test<check_type>(nullptr));
            static constexpr bool value = decltype(test<check_type>(nullptr))::value;
        };
        
        template <typename type, assist_method_t<type> f>
        struct assist_method_caller {
            inline static void call(type *obj, void *b, long msg, long index, char *s) {
                (obj->*f)(b, msg, index, s);
            }
        };

        template <typename type>
        using loadbang_method_t = void (type::*)(void *);
        
        template <typename check_type>
        struct has_loadbang_method {
            template <typename, loadbang_method_t<check_type>> class checker;
            
            template <typename checkee>
            static std::true_type test(checker<checkee, &checkee::loadbang> *);
            
            template <typename>
            static std::false_type test(...);
            
            using type = decltype(test<check_type>(nullptr));
            static constexpr bool value = decltype(test<check_type>(nullptr))::value;
        };
        
        template <typename type, loadbang_method_t<type> f>
        struct loadbang_method_caller {
            inline static void call(type *obj, void *b) {
                (obj->*f)(b);
            }
        };
    };
};

#endif /* bbb_max_dev_type_traits_hpp */
