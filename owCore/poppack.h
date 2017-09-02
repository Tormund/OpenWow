#ifndef PUSHPACK_IS_DEFINED
#error pushpack1.h must be included after poppack1.h
#endif

// reset packing to the original value
#pragma pack( pop )

#undef PACK_STRUCT

#undef PUSHPACK_IS_DEFINED
