#pragma once

struct Variant
{
    enum VariantValueType
    {
        VAR_BOOL,
        VAR_INT8,
        VAR_UINT8,
        VAR_INT16,
        VAR_UINT16,
        VAR_INT32,
        VAR_UINT32,
        VAR_FLOAT,
        VAR_VEC2,
        VAR_VEC3
    };
    
    VariantValueType type;

    union
    {
        uint8 byteAcess[12];

        bool b;
        int8 i8;
        uint8 ui8;
        int16 i16;
        uint16 ui16;
        int32 i32;
        uint32 ui32;

        float f;
        vec2 f2;
        vec3 f3;
    };

    template<typename T>
    T GetValue()
    {
        switch (type)
        {
            case VAR_BOOL:
            return b;

            case VAR_INT8:
            return i8;

            case VAR_UINT8:
            return ui8;

            case VAR_INT16:
            return i16;

            case VAR_UINT16:
            return ui16;

            case VAR_INT32:
            return i32;

            case VAR_UINT32:
            return ui32;

            case VAR_FLOAT:
            return f;

            case VAR_VEC2:
            return f2;

            case VAR_VEC3:
            return f3;

            default:
            fail1();
        }

        return nullptr;
    }
};