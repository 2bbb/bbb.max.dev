//
//  bbb.max.dev.hpp
//  bbb.max.dev
//
//  Created by ISHII 2bit on 2017/12/18.
//

#ifndef bbb_max_dev_hpp
#define bbb_max_dev_hpp

#include <vector>
#include <string>
#include <type_traits>

#include "type_traits/type_traits.hpp"

#include "maxcpp6.h"

namespace bbb {
    namespace max {
        // register util
        template <typename method_type, method_type f>
        struct register_helper;
        
        template <typename obj_class, typename ... argument_types, void(obj_class::*f)(argument_types ...)>
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
                            (method)helper::call,
                            message_name.c_str(),
                            std::forward<argument_types>(args) ...);
        }
        
        // assist
        template <typename obj_class>
        static auto register_assist(obj_class * = nullptr)
            -> typename std::enable_if<has_assist<obj_class>::value>::type
        {
            using method_class = typename obj_class::template MaxMethodAssist<&obj_class::assist>;
            class_addmethod(static_cast<t_class *>(obj_class::m_class),
                            (method)method_class::call,
                            "assist",
                            A_CANT,
                            0);
//            register_method<obj_class, decltype(&obj_class::assist), &obj_class::assist>("assist", A_CANT, 0);
        }
        template <typename obj_class>
        static auto register_assist(obj_class * = nullptr)
            -> typename std::enable_if<!has_assist<obj_class>::value>::type
        {}
        
        // loadbang
        template <typename obj_class>
        static auto register_loadbang(obj_class * = nullptr)
            -> typename std::enable_if<has_loadbang<obj_class>::value>::type
        {
            using method_class = typename obj_class::template MaxMethodLoadBang<&obj_class::loadbang>;
            class_addmethod(static_cast<t_class *>(obj_class::m_class),
                            (method)method_class::call,
                            "loadbang",
                            A_CANT,
                            0);
//            register_method<obj_class, decltype(&obj_class::loadbang), &obj_class::loadbang>("loadbang", A_CANT, 0);
        }
        template <typename obj_class>
        static auto register_loadbang(obj_class * = nullptr)
            -> typename std::enable_if<!has_loadbang<obj_class>::value>::type
        {}

        // convert
        template <typename type>
        inline auto as(t_atom *v)
            -> typename std::enable_if<std::is_integral<type>::value, type>::type
        {
            return static_cast<type>(atom_getlong(v));
        }
        
        template <typename type>
        inline auto as(t_atom *v)
            -> typename std::enable_if<std::is_floating_point<type>::value, type>::type
        {
            return static_cast<type>(atom_getfloat(v));
        }
        
        template <typename type>
        inline auto as(t_atom *v)
            -> typename std::enable_if<std::is_same<type, std::string>::value, type>::type
        {
            return {atom_getsym(v)->s_name};
        }

        template <typename type>
        inline auto as(t_atom *v)
            -> typename std::enable_if<std::is_same<type, char *>::value, type>::type
        {
            return atom_getsym(v)->s_name;
        }
        
        template <typename obj_class>
        class obj : public MaxCpp6<obj_class> {
        protected:
            std::vector<t_atom> args;

        public:
            void copyArgs(t_symbol *sym, std::size_t argc, const t_atom * const argv) {
                args.reserve(argc);
                for(std::size_t i = 0; i < argc; i++) {
                    args.emplace_back(argv[i]);
                }
            }
            
            template <typename type>
            inline auto setAtom(t_atom &atom, type v) const
                -> typename std::enable_if<std::is_integral<type>::value>::type
            {
                atom_setlong(&atom, v);
            }
            template <typename type>
            inline auto setAtom(t_atom &atom, type v) const
                -> typename std::enable_if<std::is_floating_point<type>::value>::type
            {
                atom_setfloat(&atom, v);
            }
            inline void setAtom(t_atom &atom, const char *str) const {
                atom_setsym(&atom, gensym(str));
            }
            inline void setAtom(t_atom &atom, const std::string &str) const {
                setAtom(atom, str.c_str());
            }
            
            void setAtoms(std::vector<t_atom> &atoms) const {}

            template <typename argument, typename ... arguments>
            inline auto setAtoms(std::vector<t_atom> &atoms, argument &&arg, arguments && ... args) const
            -> typename std::enable_if<
                    !std::is_same<
                        typename std::remove_const<argument>::type,
                        std::vector<t_atom>
                    >::value
                >::type
            {
                atoms.emplace_back();
                setAtom(atoms.back(), std::forward<argument>(arg));
                setAtoms(atoms, std::forward<arguments>(args) ...);
            }
            template <typename argument, typename ... arguments>
            void setAtoms(std::vector<t_atom> &atoms, const std::vector<t_atom> &source, arguments && ... args) const {
                atoms.insert(atoms.end(), source.begin(), source.end());
                setAtoms(atoms, std::forward<arguments>(args) ...);
            }

            inline void outlet(std::size_t index) const {
                outlet_bang(this->m_outlets[index]);
            }
            
            inline void outlet(std::size_t index, std::size_t argc, const t_atom *argv) const {
                outlet_list(this->m_outlets[index], nullptr, argc, const_cast<t_atom *>(argv));
            }
            inline void outlet(std::size_t index, const std::vector<t_atom> &args) const {
                outlet(index, args.size(), &args[0]);
            }
            
            inline void outlet(std::size_t index, t_symbol *sym, std::size_t argc, t_atom *argv) const {
                outlet_anything(this->m_outlets[index], sym, argc, argv);
            }
            inline void outlet(std::size_t index, t_symbol *sym, const std::vector<t_atom> &argv) const {
                outlet(index, sym, argv.size(), &argv[0]);
            }
            
            template <typename type>
            inline auto outlet(std::size_t index, type v) const
                -> typename std::enable_if<std::is_integral<type>::value>::type
            {
                outlet_int(this->m_outlets[index], v);
            }
            template <typename type>
            inline auto outlet(std::size_t index, const std::vector<type> &v) const
                -> typename std::enable_if<std::is_integral<type>::value>::type
            {
                std::vector<t_atom> atoms{v.size()};
                for(std::size_t i = 0; i < v.size(); i++) {
                    atom_setlong(&atoms[i], static_cast<t_atom_long>(v[i]));
                }
                outlet(index, atoms);
            }
            
            template <typename type>
            inline auto outlet(std::size_t index, type v) const
                -> typename std::enable_if<std::is_floating_point<type>::value>::type
            {
                outlet_float(this->m_outlets[index], v);
            }
            template <typename type>
            inline auto outlet(std::size_t index, const std::vector<type> &v) const
                -> typename std::enable_if<std::is_floating_point<type>::value>::type
            {
                std::vector<t_atom> atoms{v.size()};            atoms.resize(v.size());
                for(std::size_t i = 0; i < v.size(); i++) {
                    atom_setlong(&atoms[i], static_cast<t_atom_float>(v[i]));
                }
                outlet(index, atoms);
            }

            inline void outlet(std::size_t index, const char * const str) const {
                t_atom atom;
                atom_setsym(&atom, gensym(str));
                outlet(index, 1, &atom);
            }
            inline void outlet(std::size_t index, const std::string &str) const {
                outlet(index, str.c_str());
            }
            
            template <typename ... arguments>
            inline auto outlet(std::size_t index, arguments && ... args) const
                -> typename std::enable_if<1 < sizeof...(arguments)>::type
            {
                std::vector<t_atom> atoms;
                setAtoms(atoms, std::forward<arguments>(args) ...);
                outlet(index, atoms);
            }
            
            //
            
            inline static void registerStandardFunctions() {
                register_assist(static_cast<obj_class *>(nullptr));
                register_loadbang(static_cast<obj_class *>(nullptr));
            }
            
            template <typename MaxCppBase<obj_class>::maxmethodgimme method_ptr>
            inline static void registerGimme(const std::string &method_name) {
                using method_class = typename obj_class::template MaxMethodGimme<method_ptr>;
                class_addmethod((t_class *)obj_class::m_class,
                                (method)method_class::call,
                                method_name.c_str(),
                                A_GIMME,
                                0);
            }
        };
    };
    template <typename obj_class>
    using max_obj = max::obj<obj_class>;
};

#endif /* bbb_max_dev_hpp */
