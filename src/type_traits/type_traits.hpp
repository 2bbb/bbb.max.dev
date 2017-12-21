//
//  type_traits.hpp
//  bbb.max.dev
//
//  Created by ISHII 2bit on 2017/12/21.
//

#ifndef bbb_max_dev_type_traits_hpp
#define bbb_max_dev_type_traits_hpp

namespace bbb {
    namespace max {
        template <typename check_type>
        struct has_assist {
            template <typename, typename> class checker;
            
            template <typename checkee>
            static std::true_type test(checker<checkee, decltype(&checkee::assist)> *);
            
            template <typename>
            static std::false_type test(...);
            
            using type = decltype(test<check_type>(nullptr));
            static constexpr bool value = decltype(test<check_type>(nullptr))::value;
        };
        
        template <typename check_type>
        struct has_loadbang {
            template <typename, typename> class checker;
            
            template <typename checkee>
            static std::true_type test(checker<checkee, decltype(&checkee::loadbang)> *);
            
            template <typename>
            static std::false_type test(...);
            
            using type = decltype(test<check_type>(nullptr));
            static constexpr bool value = decltype(test<check_type>(nullptr))::value;
        };
    };
};

#endif /* bbb_max_dev_type_traits_hpp */
