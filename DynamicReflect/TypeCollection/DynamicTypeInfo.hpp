#pragma once

#include <string>
#include <vector>
#include <memory>

// ================================================== 动态类型信息存储 ================================================== //

namespace NekiraReflect
{

    // DynamicTypeInfo 类用于表示动态类型信息的基类
    class DynamicTypeInfo
    {
    public:
        virtual ~DynamicTypeInfo() = default;

        DynamicTypeInfo( const std::string& name )
            : Name( name )
        {
        }

        const std::string GetName() const
        {
            return Name;
        }

    private:
        std::string Name;
    };

} // namespace NekiraReflect

namespace NekiraReflect
{

    // NumericTypeInfo 类用于表示数值类型信息
    class NumericTypeInfo : public DynamicTypeInfo
    {
    public:
        // 数值类别枚举
        enum class NumericCategory
        {
            Unknown,
            Int8,
            Int16,
            Int32,
            Int64,
            Float,
            Double
        };

        // 拷贝构造函数
        NumericTypeInfo( const NumericTypeInfo& other )
            : DynamicTypeInfo( GetCategoryName( other.Category ) ), Category( other.Category ), IsSigned( other.IsSigned )
        {
        }

        // 构造函数，接受数值类别和是否有符号的标志
        NumericTypeInfo( NumericCategory category, bool isSigned )
            : DynamicTypeInfo( GetCategoryName( category ) ), Category( category ), IsSigned( isSigned )
        {
        }

        // 创建 NumericTypeInfo 实例的静态方法

        // 获取类型类别
        const NumericCategory GetTypeCategory() const
        {
            return Category;
        }

        // 类型是否有符号
        const bool IsSignedType() const
        {
            return IsSigned;
        }

    private:
        NumericCategory Category;
        bool IsSigned;

        // 根据数值类别返回对应的名称
        static std::string GetCategoryName( NumericCategory category )
        {
            switch ( category )
            {
            case NumericCategory::Int8:
                return "int8";
            case NumericCategory::Int16:
                return "int16";
            case NumericCategory::Int32:
                return "int32";
            case NumericCategory::Int64:
                return "int64";
            case NumericCategory::Float:
                return "float";
            case NumericCategory::Double:
                return "double";
            default:
                return "Unknown";
            }
        }

        // 根据类型 T 返回对应的数值类别
        template <typename T>
        static NumericCategory type_category_v()
        {
            if constexpr ( std::is_same_v<T, char> )
            {
                return NumericCategory::Int8;
            }
            else if constexpr ( std::is_same_v<T, short> )
            {
                return NumericCategory::Int16;
            }
            else if constexpr ( std::is_same_v<T, int> )
            {
                return NumericCategory::Int32;
            }
            else if constexpr ( std::is_same_v<T, long long> )
            {
                return NumericCategory::Int64;
            }
            else if constexpr ( std::is_same_v<T, float> )
            {
                return NumericCategory::Float;
            }
            else if constexpr ( std::is_same_v<T, double> )
            {
                return NumericCategory::Double;
            }
            else
            {
                return NumericCategory::Unknown;
            }
        }
    };

} // namespace NekiraReflect

namespace NekiraReflect
{

    // EnumTypeInfo 类用于表示枚举类型信息
    class EnumTypeInfo : public DynamicTypeInfo
    {
    public:
        // 枚举对结构体，用于存储枚举成员名称和值
        struct EnumPair
        {
            std::string Name;
            size_t Value;
        };

        // 构造函数，接受枚举类型的名称
        EnumTypeInfo( const std::string& EnumTypeName )
            : DynamicTypeInfo( EnumTypeName )
        {
        }

        // 静态方法创建 EnumTypeInfo 实例
        static std::shared_ptr<EnumTypeInfo> Create( const std::string& EnumTypeName )
        {
            return std::make_shared<EnumTypeInfo>( EnumTypeName );
        }

        // 添加枚举对
        void AddEnumPair( const std::string& name, size_t value )
        {
            Enums.push_back( std::move( EnumPair{ name, value } ) );
        }

        // 获取枚举对的数量
        const size_t GetEnumCount() const
        {
            return Enums.size();
        }

        // 通过枚举成员名称获取枚举值
        const size_t GetEnumValueByName( const std::string& name ) const
        {
            size_t Result = -1;

            for ( const auto& pair : Enums )
            {
                if ( pair.Name == name )
                {
                    Result = pair.Value;
                    break;
                }
            }

            return Result;
        }

        // 通过值获取枚举成员名称
        const std::string GetEnumNameByValue( size_t value ) const
        {
            std::string Result = "Unknown";

            for ( const auto& pair : Enums )
            {
                if ( pair.Value == value )
                {
                    Result = pair.Name;
                    break;
                }
            }

            return Result;
        }

    private:
        // 存储枚举对的向量
        std::vector<EnumPair> Enums;
    };

} // namespace NekiraReflect

namespace NekiraReflect
{

    // ClassTypeInfo 类用于表示类的类型信息
    class ClassTypeInfo : public DynamicTypeInfo
    {
    public:
    private:
    };

} // namespace NekiraReflect

// ====================================================== 辅助方法 ====================================================== //
namespace NekiraReflect
{
    // 辅助函数：用于转换DynamicTypeInfo为具体的子类
    // Helper function to cast DynamicTypeInfo to specific derived type.

    template <typename T>
    T* CastDynamicTypeInfo( DynamicTypeInfo* TypeInfo )
    {
        return dynamic_cast< T* >( TypeInfo );
    }

    template <typename T>
    std::shared_ptr<T> CastDynamicTypeInfo( const std::shared_ptr<DynamicTypeInfo>& TypeInfo )
    {
        return std::dynamic_pointer_cast< T >( TypeInfo );
    }

} // namespace NekiraReflect