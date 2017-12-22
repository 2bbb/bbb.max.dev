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
#include "utility/convert.hpp"
#include "utility/register_methods.hpp"

#include "maxcpp6.h"

namespace bbb {
    namespace max {
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
                -> enable_if_t<std::is_integral<type>::value>
            {
                atom_setlong(&atom, v);
            }
            template <typename type>
            inline auto setAtom(t_atom &atom, type v) const
                -> enable_if_t<std::is_floating_point<type>::value>
            {
                atom_setfloat(&atom, v);
            }
            inline void setAtom(t_atom &atom, const char *str) const {
                atom_setsym(&atom, gensym(str));
            }
            inline void setAtom(t_atom &atom, const std::string &str) const {
                setAtom(atom, str.c_str());
            }
            
            inline void setAtoms(std::vector<t_atom> &atoms) const {}

            template <typename argument, typename ... arguments>
            inline auto setAtoms(std::vector<t_atom> &atoms, argument &&arg, arguments && ... args) const
            -> enable_if_t<
                    !std::is_same<
                        typename std::remove_const<argument>::type,
                        std::vector<t_atom>
                    >::value
                >
            {
                atoms.emplace_back();
                setAtom(atoms.back(), std::forward<argument>(arg));
                setAtoms(atoms, std::forward<arguments>(args) ...);
            }
            template <typename argument, typename ... arguments>
            inline void setAtoms(std::vector<t_atom> &atoms, const std::vector<t_atom> &source, arguments && ... args) const {
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
                -> enable_if_t<std::is_integral<type>::value>
            {
                outlet_int(this->m_outlets[index], v);
            }
            template <typename type>
            inline auto outlet(std::size_t index, const std::vector<type> &v) const
                -> enable_if_t<std::is_integral<type>::value>
            {
                std::vector<t_atom> atoms{v.size()};
                for(std::size_t i = 0; i < v.size(); i++) {
                    atom_setlong(&atoms[i], static_cast<t_atom_long>(v[i]));
                }
                outlet(index, atoms);
            }
            
            template <typename type>
            inline auto outlet(std::size_t index, type v) const
                -> enable_if_t<std::is_floating_point<type>::value>
            {
                outlet_float(this->m_outlets[index], v);
            }
            template <typename type>
            inline auto outlet(std::size_t index, const std::vector<type> &v) const
                -> enable_if_t<std::is_floating_point<type>::value>
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
                -> enable_if_t<1 < sizeof...(arguments)>
            {
                std::vector<t_atom> atoms;
                setAtoms(atoms, std::forward<arguments>(args) ...);
                outlet(index, atoms);
            }
            
            inline static void registerStandardFunctions() {
                register_standard_functions_helper<obj_class>::call_register();
            }
            
            template <general_gimme_method<obj_class> method_ptr>
            inline static void registerGimme(const std::string &method_name) {
                using caller = general_gimme_method_caller<obj_class, method_ptr>;
                class_addmethod((t_class *)(obj_class::m_class),
                                (method)(caller::call),
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
