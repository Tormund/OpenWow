#ifdef PUSHPACK_IS_DEFINED
#error poppack.h must be included after pushpack.h
#endif

#pragma pack(push,1)

// C4103: Packing was changed after the inclusion of the header, probably missing #pragma pop
#pragma warning (disable : 4103) 

#define PUSHPACK_IS_DEFINED


