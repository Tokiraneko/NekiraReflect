
#pragma once

#include <type_traits>

namespace NekiraReflect
{
    // =============================================== 成员变量指针剥离 =============================================== //
    // 成员变量指针的剥离
    template <typename T>
    struct member_variable_traits
    {
        using Type = T;
        using ObjectType = void;
    };

    template <typename ClassType, typename MemberType>
    struct member_variable_traits<MemberType ClassType::*>
    {
        using Type = MemberType;
        using ObjectType = ClassType;
    };

    // ================================================== 类型剥离 ================================================== //
    // 类型剥离，获取去除引用、指针、const等修饰符的类型
    template <typename T>
    struct raw_type
    {
        using Type = T;
    };

    // 剥离const
    template <typename T>
    struct raw_type<const T> : raw_type<T>
    {
    };

    // 剥离volatile
    template <typename T>
    struct raw_type<volatile T> : raw_type<T>
    {
    };

    // 剥离引用
    template <typename T>
    struct raw_type<T&> : raw_type<T>
    {
    };

    // 剥离右值引用
    template <typename T>
    struct raw_type<T&&> : raw_type<T>
    {
    };

    // 剥离指针
    template <typename T>
    struct raw_type<T*> : raw_type<T>
    {
    };

    // 剥离数组
    template <typename T, std::size_t N>
    struct raw_type<T[ N ]> : raw_type<T>
    {
    };

    // =================================================== 变量萃取器 ================================================== //
    // 基础的变量萃取器
    template <typename T>
    struct variable_traits_base
    {
        // 去除引用，指针，const等修饰符的类型
        using RawType = typename raw_type<typename member_variable_traits<T>::Type>::Type;

        using ValueType = T;

        using ClassType = typename member_variable_traits<T>::ObjectType;
    };

    // 变量萃取器，默认实现
    template <typename T>
    struct variable_traits : variable_traits_base<T>
    {
    };

    // ================================================== 便捷别名和常量 ================================================== //
    // 获取去除引用、指针、const等修饰符的类型
    template <typename T>
    using variable_traits_RawType = typename variable_traits<T>::RawType;

    // 获取值类型
    template <typename T>
    using variable_traits_ValueType = typename variable_traits<T>::ValueType;

    // 获取对象类型（仅对成员变量有效）
    template <typename T>
    using variable_traits_ObjectType = typename variable_traits<T>::ClassType;

} // namespace NekiraReflect