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
        template <typename method_type, method_type f>
        struct register_helper;
        
        template <typename obj_class, typename ... argument_types, void(obj_class::*f)(argument_types && ...)>
        struct register_helper<void(obj_class::*)(argument_types ...), f> {
            using method_type = void(obj_class::*)(argument_types ...);
            static void call(obj_class *obj, argument_types && ... args) {
                (obj->*f)(std::forward<argument_types>(args) ...);
            }
        };
        
        template <typename obj_class, typename method_type, method_type f, typename ... argument_types>
        void register_method(const std::string &message_name, argument_types && ... args) {
            using helper = register_helper<method_type, f>;
            class_addmethod(static_cast<t_class *>(obj_class::m_class),
                            static_cast<method>(helper::call),
                            message_name.c_str(),
                            std::forward<argument_types>(args) ...);
        }
        
        template <typename obj_class, template <typename> class method_def, typename ... argument_types>
        inline auto register_method(const std::string &message_name, argument_types && ... args)
            -> enable_if_t<sfinae_method_checker<method_def, obj_class>::value>
        {
            using helper = register_helper<typename method_def<obj_class>::type, method_def<obj_class>::value>;
            class_addmethod(static_cast<t_class *>(obj_class::m_class),
                            static_cast<method>(helper::call),
                            message_name.c_str(),
                            std::forward<argument_types>(args) ...);
        }

        template <typename obj_class, template <typename> class method_def, typename ... argument_types>
        inline auto register_method(const std::string &message_name, argument_types && ... args)
            -> enable_if_t<!sfinae_method_checker<method_def, obj_class>::value>
        {}
        
        template <typename obj_class>
        struct register_standard_functions_helper {
            inline static void call_register() {
                register_method<obj_class, bang_method_def>("bang", A_CANT, 0);
                register_method<obj_class, assist_method_def>("assist", A_CANT, 0);
                register_method<obj_class, loadbang_method_def>("loadbang", A_CANT, 0);
            }
        };
    };
};

#endif /* bbb_max_dev_register_methods_hpp */
