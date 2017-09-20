#ifndef PUSHPACK_IS_DEFINED
#error pushpack.h must be included after poppack.h
#endif

// reset packing to the original value
#pragma pack( pop )

#undef PUSHPACK_IS_DEFINED
