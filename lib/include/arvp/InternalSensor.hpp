#ifndef ARVP_INTERNAL_SENSOR_HPP_INCLUDED
#define ARVP_INTERNAL_SENSOR_HPP_INCLUDED

#include <uavcan/build_config.hpp>
#include <uavcan/node/global_data_type_registry.hpp>
#include <uavcan/marshal/types.hpp>

#undef temperature
#undef pressure
#undef humidity

namespace arvp
{

template <int _tmpl>
struct UAVCAN_EXPORT InternalSensor_
{
    typedef const InternalSensor_<_tmpl>& ParameterType;
    typedef InternalSensor_<_tmpl> &ReferenceType;

    struct ConstantTypes
    {
    };

    struct FieldTypes
    {
        typedef ::uavcan::FloatSpec< 16, ::uavcan::CastModeSaturate > temperature;
        typedef ::uavcan::FloatSpec< 16, ::uavcan::CastModeSaturate > pressure;
        typedef ::uavcan::FloatSpec< 16, ::uavcan::CastModeSaturate > humidity;
    };

    enum
    {
        MinBitLen
            = FieldTypes::temperature::MinBitLen
            + FieldTypes::pressure::MinBitLen
            + FieldTypes::humidity::MinBitLen
    };

    enum
    {
        MaxBitLen
            = FieldTypes::temperature::MaxBitLen
            + FieldTypes::pressure::MaxBitLen
            + FieldTypes::humidity::MaxBitLen
    };

    // Constants

    // Fields
    typename ::uavcan::StorageType< typename FieldTypes::temperature >::Type temperature;
    typename ::uavcan::StorageType< typename FieldTypes::pressure >::Type pressure;
    typename ::uavcan::StorageType< typename FieldTypes::humidity >::Type humidity;

    InternalSensor_()
        : temperature(), pressure(), humidity()
    {
        ::uavcan::StaticAssert<_tmpl == 0>::check();  // Usage check

#if UAVCAN_DEBUG
        /*
         * Cross-checking MaxBitLen provided by the DSDL compiler.
         * This check shall never be performed in user code because MaxBitLen value
         * actually depends on the nested types, thus it is not invariant.
         */
        ::uavcan::StaticAssert<48 == MaxBitLen>::check();
#endif
    }

    bool operator==(ParameterType rhs) const;
    bool operator!=(ParameterType rhs) const { return !operator==(rhs); }

    /**
     * This comparison is based on @ref uavcan::areClose(), which ensures proper comparison of
     * floating point fields at any depth.
     */
    bool isClose(ParameterType rhs) const;

    static int encode(ParameterType self, ::uavcan::ScalarCodec& codec,
                      ::uavcan::TailArrayOptimizationMode tao_mode = ::uavcan::TailArrayOptEnabled);

    static int decode(ReferenceType self, ::uavcan::ScalarCodec& codec,
                      ::uavcan::TailArrayOptimizationMode tao_mode = ::uavcan::TailArrayOptEnabled);

    /*
     * Static type info
     */
    enum { DataTypeKind = ::uavcan::DataTypeKindMessage };
    enum { DefaultDataTypeID = 2048 };

    static const char* getDataTypeFullName()
    {
        return "arvp.InternalSensor";
    }

    static void extendDataTypeSignature(::uavcan::DataTypeSignature& signature)
    {
        signature.extend(getDataTypeSignature());
    }

    static ::uavcan::DataTypeSignature getDataTypeSignature();

};

/*
 * Out of line struct method definitions
 */

template <int _tmpl>
bool InternalSensor_<_tmpl>::operator==(ParameterType rhs) const
{
    return
        temperature == rhs.temperature &&
        pressure == rhs.pressure &&
        humidity == rhs.humidity;
}

template <int _tmpl>
bool InternalSensor_<_tmpl>::isClose(ParameterType rhs) const
{
    return
        ::uavcan::areClose(temperature, rhs.temperature) &&
        ::uavcan::areClose(pressure, rhs.pressure) &&
        ::uavcan::areClose(humidity, rhs.humidity);
}

template <int _tmpl>
int InternalSensor_<_tmpl>::encode(ParameterType self, ::uavcan::ScalarCodec& codec,
    ::uavcan::TailArrayOptimizationMode tao_mode)
{
    (void)self;
    (void)codec;
    (void)tao_mode;
    int res = 1;
    res = FieldTypes::temperature::encode(self.temperature, codec,  ::uavcan::TailArrayOptDisabled);
    if (res <= 0)
    {
        return res;
    }
    res = FieldTypes::pressure::encode(self.pressure, codec,  ::uavcan::TailArrayOptDisabled);
    if (res <= 0)
    {
        return res;
    }
    res = FieldTypes::humidity::encode(self.humidity, codec,  tao_mode);
    return res;
}

template <int _tmpl>
int InternalSensor_<_tmpl>::decode(ReferenceType self, ::uavcan::ScalarCodec& codec,
    ::uavcan::TailArrayOptimizationMode tao_mode)
{
    (void)self;
    (void)codec;
    (void)tao_mode;
    int res = 1;
    res = FieldTypes::temperature::decode(self.temperature, codec,  ::uavcan::TailArrayOptDisabled);
    if (res <= 0)
    {
        return res;
    }
    res = FieldTypes::pressure::decode(self.pressure, codec,  ::uavcan::TailArrayOptDisabled);
    if (res <= 0)
    {
        return res;
    }
    res = FieldTypes::humidity::decode(self.humidity, codec,  tao_mode);
    return res;
}

/*
 * Out of line type method definitions
 */
template <int _tmpl>
::uavcan::DataTypeSignature InternalSensor_<_tmpl>::getDataTypeSignature()
{
    ::uavcan::DataTypeSignature signature(0xCDC7C43412BDC89AULL);

    FieldTypes::temperature::extendDataTypeSignature(signature);
    FieldTypes::pressure::extendDataTypeSignature(signature);
    FieldTypes::humidity::extendDataTypeSignature(signature);

    return signature;
}

/*
 * Out of line constant definitions
 */

/*
 * Final typedef
 */
typedef InternalSensor_<0> InternalSensor;

namespace
{

const ::uavcan::DefaultDataTypeRegistrator< ::arvp::InternalSensor > _uavcan_gdtr_registrator_InternalSensor;

}

} // Namespace arvp

/*
 * YAML streamer specialization
 */
namespace uavcan
{

template <>
class UAVCAN_EXPORT YamlStreamer< ::arvp::InternalSensor >
{
public:
    template <typename Stream>
    static void stream(Stream& s, ::arvp::InternalSensor::ParameterType obj, const int level);
};

template <typename Stream>
void YamlStreamer<::arvp::InternalSensor>::stream(Stream &s, ::arvp::InternalSensor::ParameterType obj, const int level)
{
    (void)s;
    (void)obj;
    (void)level;
    if (level > 0)
    {
        s << '\n';
        for (int pos = 0; pos < level; pos++)
        {
            s << "  ";
        }
    }
    s << "temperture: ";
    YamlStreamer< ::arvp::InternalSensor::FieldTypes::temperature >::stream(s, obj.temperature, level + 1);
    s << '\n';
    for (int pos = 0; pos < level; pos++)
    {
        s << "  ";
    }
    s << "pressure: ";
    YamlStreamer< ::arvp::InternalSensor::FieldTypes::pressure >::stream(s, obj.pressure, level + 1);
    s << '\n';
    for (int pos = 0; pos < level; pos++)
    {
        s << "  ";
    }
    s << "humidity: ";
    YamlStreamer< ::arvp::InternalSensor::FieldTypes::humidity >::stream(s, obj.humidity, level + 1);
}

}

namespace uavcan
{

template <typename Stream>
inline Stream& operator<<(Stream& s, ::arvp::InternalSensor::ParameterType obj)
{
    ::uavcan::YamlStreamer< ::arvp::InternalSensor >::stream(s, obj, 0);
    return s;
}

} // Namespace arvp

#endif // ARVP_INTERNAL_SENSOR_HPP_INCLUDED