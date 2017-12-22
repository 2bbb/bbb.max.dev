//
//  register_methods.hpp
//  bbb.max.dev
//
//  Created by ISHII 2bit on 2017/12/22.
//

#ifndef bbb_max_dev_register_methods_hpp
#define bbb_max_dev_register_methods_hpp

#include "../type_traits/type_traits.hpp"

namespace bbb {
    namespace max {
        template <typename obj_class>
        inline static auto register_bang()
            -> enable_if_t<has_bang_method<obj_class>::value>
        {
            using caller = bang_method_caller<obj_class, &obj_class::bang>;
            class_addmethod((t_class *)(obj_class::m_class),
                            (method)(caller::call),
                            "bang",
                            A_CANT,
                            0);
        }
        template <typename obj_class>
        inline static auto register_bang()
            -> enable_if_t<!has_bang_method<obj_class>::value>
        {}

        template <typename obj_class>
        inline static auto register_assist()
            -> enable_if_t<has_assist_method<obj_class>::value>
        {
            using caller = assist_method_caller<obj_class, &obj_class::assist>;
            class_addmethod((t_class *)(obj_class::m_class),
                            (method)(caller::call),
                            "assist",
                            A_CANT,
                            0);
        }
        template <typename obj_class>
        inline static auto register_assist()
            -> enable_if_t<!has_assist_method<obj_class>::value>
        {}

        template <typename obj_class>
        inline static auto register_loadbang()
            -> enable_if_t<has_loadbang_method<obj_class>::value>
        {
            using caller = loadbang_method_caller<obj_class, &obj_class::loadbang>;
            class_addmethod((t_class *)(obj_class::m_class),
                            (method)(caller::call),
                            "loadbang",
                            A_CANT,
                            0);
            
        }
        template <typename obj_class>
        inline static auto register_loadbang()
            -> enable_if_t<!has_loadbang_method<obj_class>::value>
        {}

        template <typename obj_class>
        struct standard_functions_registrar {
            inline static void call_register() {
                register_bang<obj_class>();
                register_assist<obj_class>();
                register_loadbang<obj_class>();
            }
        };
    };
};

#endif /* bbb_max_dev_register_methods_hpp */
