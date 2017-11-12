#include "stdafx.h"

// General
#include "OpenGL.h"

#include <cstdlib>
#include <cstring>

#pragma region MyRegion

// GL 1.1
PFNGLGETTEXIMAGEPROC glGetTexImage = nullptr;
PFNGLPOLYGONMODEPROC glPolygonMode = nullptr;
PFNGLCLEARDEPTH glClearDepth = nullptr;
PFNGLDRAWBUFFERPROC glDrawBuffer = nullptr;

// GL 1.2
PFNGLBLENDCOLORPROC glBlendColor = nullptr;
PFNGLBLENDEQUATIONPROC glBlendEquation = nullptr;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = nullptr;
PFNGLTEXIMAGE3DPROC glTexImage3D = nullptr;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D = nullptr;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D = nullptr;

// GL 1.3
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage = nullptr;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D = nullptr;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage = nullptr;

// GL 1.4
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays = nullptr;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements = nullptr;
PFNGLPOINTPARAMETERFPROC glPointParameterf = nullptr;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv = nullptr;
PFNGLPOINTPARAMETERIPROC glPointParameteri = nullptr;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv = nullptr;

// GL 1.5
PFNGLGENQUERIESPROC glGenQueries = nullptr;
PFNGLDELETEQUERIESPROC glDeleteQueries = nullptr;
PFNGLISQUERYPROC glIsQuery = nullptr;
PFNGLBEGINQUERYPROC glBeginQuery = nullptr;
PFNGLENDQUERYPROC glEndQuery = nullptr;
PFNGLGETQUERYIVPROC glGetQueryiv = nullptr;
PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv = nullptr;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLISBUFFERPROC glIsBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData = nullptr;
PFNGLMAPBUFFERPROC glMapBuffer = nullptr;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = nullptr;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv = nullptr;
PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv = nullptr;

// GL 2.0
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = nullptr;
PFNGLDRAWBUFFERSPROC glDrawBuffers = nullptr;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = nullptr;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = nullptr;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = nullptr;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = nullptr;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETSHADERSOURCEPROC glGetShaderSource = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLGETUNIFORMFVPROC glGetUniformfv = nullptr;
PFNGLGETUNIFORMIVPROC glGetUniformiv = nullptr;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv = nullptr;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv = nullptr;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv = nullptr;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = nullptr;
PFNGLISPROGRAMPROC glIsProgram = nullptr;
PFNGLISSHADERPROC glIsShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM2FPROC glUniform2f = nullptr;
PFNGLUNIFORM3FPROC glUniform3f = nullptr;
PFNGLUNIFORM4FPROC glUniform4f = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLUNIFORM2IPROC glUniform2i = nullptr;
PFNGLUNIFORM3IPROC glUniform3i = nullptr;
PFNGLUNIFORM4IPROC glUniform4i = nullptr;
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORM1IVPROC glUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC glUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC glUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC glUniform4iv = nullptr;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;

// GL 2.1
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv = nullptr;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv = nullptr;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv = nullptr;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv = nullptr;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv = nullptr;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv = nullptr;

// GL 3.0
PFNGLGETSTRINGIPROC glGetStringi = nullptr;
PFNGLCOLORMASKIPROC glColorMaski = 0;
PFNGLGETBOOLEANI_VPROC glGetBooleani_v = 0;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v = 0;
PFNGLENABLEIPROC glEnablei = 0;
PFNGLDISABLEIPROC glDisablei = 0;
PFNGLISENABLEDIPROC glIsEnabledi = 0;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback = 0;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback = 0;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange = 0;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = 0;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings = 0;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying = 0;
PFNGLCLAMPCOLORPROC glClampColor = 0;
PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender = 0;
PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender = 0;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = 0;
PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv = 0;
PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv = 0;
PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i = 0;
PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i = 0;
PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i = 0;
PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i = 0;
PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui = 0;
PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui = 0;
PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui = 0;
PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui = 0;
PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv = 0;
PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv = 0;
PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv = 0;
PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv = 0;
PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv = 0;
PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv = 0;
PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv = 0;
PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv = 0;
PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv = 0;
PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv = 0;
PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv = 0;
PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv = 0;
PFNGLGETUNIFORMUIVPROC glGetUniformuiv = 0;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation = 0;
PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation = 0;
PFNGLUNIFORM1UIPROC glUniform1ui = 0;
PFNGLUNIFORM2UIPROC glUniform2ui = 0;
PFNGLUNIFORM3UIPROC glUniform3ui = 0;
PFNGLUNIFORM4UIPROC glUniform4ui = 0;
PFNGLUNIFORM1UIVPROC glUniform1uiv = 0;
PFNGLUNIFORM2UIVPROC glUniform2uiv = 0;
PFNGLUNIFORM3UIVPROC glUniform3uiv = 0;
PFNGLUNIFORM4UIVPROC glUniform4uiv = 0;
PFNGLTEXPARAMETERIIVPROC glTexParameterIiv = 0;
PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv = 0;
PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv = 0;
PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv = 0;
PFNGLCLEARBUFFERIVPROC glClearBufferiv = 0;
PFNGLCLEARBUFFERUIVPROC glClearBufferuiv = 0;
PFNGLCLEARBUFFERFVPROC glClearBufferfv = 0;
PFNGLCLEARBUFFERFIPROC glClearBufferfi = 0;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer = 0;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = 0;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = 0;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = 0;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = 0;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv = 0;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer = 0;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = 0;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = 0;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = 0;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = 0;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D = 0;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = 0;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D = 0;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = 0;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv = 0;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = 0;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = 0;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = 0;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer = 0;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange = 0;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange = 0;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLISVERTEXARRAYPROC glIsVertexArray = 0;

// GL 3.1
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = 0;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = 0;
PFNGLTEXBUFFERPROC glTexBuffer = 0;
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex = 0;
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = 0;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = 0;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = 0;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = 0;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = 0;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv = 0;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName = 0;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = 0;

// GL 3.2
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = 0;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex = 0;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex = 0;
PFNGLPROVOKINGVERTEXPROC glProvokingVertex = 0;
PFNGLFENCESYNCPROC glFenceSync = 0;
PFNGLISSYNCPROC glIsSync = 0;
PFNGLDELETESYNCPROC glDeleteSync = 0;
PFNGLCLIENTWAITSYNCPROC glClientWaitSync = 0;
PFNGLWAITSYNCPROC glWaitSync = 0;
PFNGLGETINTEGER64VPROC glGetInteger64v = 0;
PFNGLGETSYNCIVPROC glGetSynciv = 0;
PFNGLGETINTEGER64I_VPROC glGetInteger64i_v = 0;
PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v = 0;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture = 0;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample = 0;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample = 0;
PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv = 0;
PFNGLSAMPLEMASKIPROC glSampleMaski = 0;

// GL 3.3
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed = 0;
PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex = 0;
PFNGLGENSAMPLERSPROC glGenSamplers = 0;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = 0;
PFNGLISSAMPLERPROC glIsSampler = 0;
PFNGLBINDSAMPLERPROC glBindSampler = 0;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = 0;
PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv = 0;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf = 0;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv = 0;
PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv = 0;
PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv = 0;
PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv = 0;
PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv = 0;
PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv = 0;
PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv = 0;
PFNGLQUERYCOUNTERPROC glQueryCounter = 0;
PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v = 0;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v = 0;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = 0;
PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui = 0;
PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv = 0;
PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui = 0;
PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv = 0;
PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui = 0;
PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv = 0;
PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui = 0;
PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv = 0;

// GL 4.0
PFNGLMINSAMPLESHADINGPROC glMinSampleShading = 0;
PFNGLBLENDEQUATIONIPROC glBlendEquationi = 0;
PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei = 0;
PFNGLBLENDFUNCIPROC glBlendFunci = 0;
PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei = 0;
PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect = 0;
PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect = 0;
PFNGLUNIFORM1DPROC glUniform1d = 0;
PFNGLUNIFORM2DPROC glUniform2d = 0;
PFNGLUNIFORM3DPROC glUniform3d = 0;
PFNGLUNIFORM4DPROC glUniform4d = 0;
PFNGLUNIFORM1DVPROC glUniform1dv = 0;
PFNGLUNIFORM2DVPROC glUniform2dv = 0;
PFNGLUNIFORM3DVPROC glUniform3dv = 0;
PFNGLUNIFORM4DVPROC glUniform4dv = 0;
PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv = 0;
PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv = 0;
PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv = 0;
PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv = 0;
PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv = 0;
PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv = 0;
PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv = 0;
PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv = 0;
PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv = 0;
PFNGLGETUNIFORMDVPROC glGetUniformdv = 0;
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation = 0;
PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex = 0;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv = 0;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName = 0;
PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName = 0;
PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv = 0;
PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv = 0;
PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv = 0;
PFNGLPATCHPARAMETERIPROC glPatchParameteri = 0;
PFNGLPATCHPARAMETERFVPROC glPatchParameterfv = 0;
PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback = 0;
PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks = 0;
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks = 0;
PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback = 0;
PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback = 0;
PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback = 0;
PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback = 0;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream = 0;
PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed = 0;
PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed = 0;
PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv = 0;

// GL 4.1
PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler = 0;
PFNGLSHADERBINARYPROC glShaderBinary = 0;
PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat = 0;
PFNGLDEPTHRANGEFPROC glDepthRangef = 0;
PFNGLCLEARDEPTHFPROC glClearDepthf = 0;
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary = 0;
PFNGLPROGRAMBINARYPROC glProgramBinary = 0;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = 0;
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages = 0;
PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram = 0;
PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv = 0;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline = 0;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines = 0;
PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines = 0;
PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline = 0;
PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv = 0;
PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i = 0;
PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv = 0;
PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f = 0;
PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv = 0;
PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d = 0;
PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv = 0;
PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui = 0;
PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv = 0;
PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i = 0;
PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv = 0;
PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f = 0;
PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv = 0;
PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d = 0;
PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv = 0;
PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui = 0;
PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv = 0;
PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i = 0;
PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv = 0;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f = 0;
PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv = 0;
PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d = 0;
PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv = 0;
PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui = 0;
PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv = 0;
PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i = 0;
PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv = 0;
PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f = 0;
PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv = 0;
PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d = 0;
PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv = 0;
PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui = 0;
PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv = 0;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv = 0;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv = 0;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv = 0;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv = 0;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv = 0;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv = 0;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv = 0;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv = 0;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv = 0;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline = 0;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog = 0;
PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d = 0;
PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d = 0;
PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d = 0;
PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d = 0;
PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv = 0;
PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv = 0;
PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv = 0;
PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv = 0;
PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer = 0;
PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv = 0;
PFNGLVIEWPORTARRAYVPROC glViewportArrayv = 0;
PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf = 0;
PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv = 0;
PFNGLSCISSORARRAYVPROC glScissorArrayv = 0;
PFNGLSCISSORINDEXEDPROC glScissorIndexed = 0;
PFNGLSCISSORINDEXEDVPROC glScissorIndexedv = 0;
PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv = 0;
PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed = 0;
PFNGLGETFLOATI_VPROC glGetFloati_v = 0;
PFNGLGETDOUBLEI_VPROC glGetDoublei_v = 0;

// GL 4.2
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance = 0;
PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ = 0;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv = 0;
PFNGLBINDIMAGETEXTUREPROC glBindImageTexture = 0;
PFNGLMEMORYBARRIERPROC glMemoryBarrier = 0;
PFNGLTEXSTORAGE1DPROC glTexStorage1D = 0;
PFNGLTEXSTORAGE2DPROC glTexStorage2D = 0;
PFNGLTEXSTORAGE3DPROC glTexStorage3D = 0;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced = 0;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced = 0;

// GL 4.3
PFNGLCLEARBUFFERDATAPROC glClearBufferData = 0;
PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData = 0;
PFNGLDISPATCHCOMPUTEPROC glDispatchCompute = 0;
PFNGLDISPATCHCOMPUTEINDIRECTPROC glDispatchComputeIndirect = 0;
PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData = 0;
PFNGLFRAMEBUFFERPARAMETERIPROC glFramebufferParameteri = 0;
PFNGLGETFRAMEBUFFERPARAMETERIVPROC glGetFramebufferParameteriv = 0;
PFNGLGETINTERNALFORMATI64VPROC glGetInternalformati64v = 0;
PFNGLINVALIDATETEXSUBIMAGEPROC glInvalidateTexSubImage = 0;
PFNGLINVALIDATETEXIMAGEPROC glInvalidateTexImage = 0;
PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData = 0;
PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData = 0;
PFNGLINVALIDATEFRAMEBUFFERPROC glInvalidateFramebuffer = 0;
PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer = 0;
PFNGLMULTIDRAWARRAYSINDIRECTPROC glMultiDrawArraysIndirect = 0;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect = 0;
PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv = 0;
PFNGLGETPROGRAMRESOURCEINDEXPROC glGetProgramResourceIndex = 0;
PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName = 0;
PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv = 0;
PFNGLGETPROGRAMRESOURCELOCATIONPROC glGetProgramResourceLocation = 0;
PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex = 0;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC glShaderStorageBlockBinding = 0;
PFNGLTEXBUFFERRANGEPROC glTexBufferRange = 0;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC glTexStorage2DMultisample = 0;
PFNGLTEXSTORAGE3DMULTISAMPLEPROC glTexStorage3DMultisample = 0;
PFNGLTEXTUREVIEWPROC glTextureView = 0;
PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer = 0;
PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat = 0;
PFNGLVERTEXATTRIBIFORMATPROC glVertexAttribIFormat = 0;
PFNGLVERTEXATTRIBLFORMATPROC glVertexAttribLFormat = 0;
PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding = 0;
PFNGLVERTEXBINDINGDIVISORPROC glVertexBindingDivisor = 0;
PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl = 0;
PFNGLDEBUGMESSAGEINSERTPROC glDebugMessageInsert = 0;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = 0;
PFNGLGETDEBUGMESSAGELOGPROC glGetDebugMessageLog = 0;
PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup = 0;
PFNGLPOPDEBUGGROUPPROC glPopDebugGroup = 0;
PFNGLOBJECTLABELPROC glObjectLabel = 0;
PFNGLGETOBJECTLABELPROC glGetObjectLabel = 0;
PFNGLOBJECTPTRLABELPROC glObjectPtrLabel = 0;
PFNGLGETOBJECTPTRLABELPROC glGetObjectPtrLabel = 0;

// GL 4.4
PFNGLBUFFERSTORAGEPROC glBufferStorage = 0;
PFNGLCLEARTEXIMAGEPROC glClearTexImage = 0;
PFNGLCLEARTEXSUBIMAGEPROC glClearTexSubImage = 0;
PFNGLBINDBUFFERSBASEPROC glBindBuffersBase = 0;
PFNGLBINDBUFFERSRANGEPROC glBindBuffersRange = 0;
PFNGLBINDTEXTURESPROC glBindTextures = 0;
PFNGLBINDSAMPLERSPROC glBindSamplers = 0;
PFNGLBINDIMAGETEXTURESPROC glBindImageTextures = 0;
PFNGLBINDVERTEXBUFFERSPROC glBindVertexBuffers = 0;

// Extensions

// GL_ARB_timer_query
PFNGLQUERYCOUNTERPROC glQueryCounterARB = nullptr;
PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64vARB = nullptr;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64vARB = nullptr;

// GL_OES_EGL_image
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES = nullptr;

#pragma endregion

bool isExtensionSupported(const char *extName)
{
	GLint numExts;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExts);
	for (int i = 0; i < numExts; ++i)
	{
		if (strcmp(extName, (char*)glGetStringi(GL_EXTENSIONS, i)) == 0)
		{
			return true;
		}
	}

	return false;
}

void getOpenGLVersion()
{
	char version[8];
	size_t len = strlen((char*)glGetString(GL_VERSION));
	if (len >= 8)
	{
		len = 7;
	}

	strncpy(version, (char*)glGetString(GL_VERSION), len);
	version[len] = '\0';

	char* pos1 = strtok(version, ".");
	if (pos1)
	{
		_Config.OpenGL.majorVersion = atoi(pos1);
		char *pos2 = strtok(nullptr, ". ");
		if (pos2)
		{
            _Config.OpenGL.minorVersion = atoi(pos2);
		}
	}
}

void* platGetProcAddress(const char *funcName)
{
	void* retVal = (void *)wglGetProcAddress(funcName);

	// For some reason legacy OpenGL functions are not resolved by wglGetProcAddress
	if (!retVal)
	{
		static HMODULE lib = LoadLibraryW(L"opengl32.dll");
		retVal = (void*)GetProcAddress(lib, funcName);
	}
	return retVal;
}

bool initOpenGLExtensions()
{
	bool r = true;

	getOpenGLVersion();
	assert1(_Config.OpenGL.majorVersion >= 3);


	// GL 1.1
	r &= (glPolygonMode = (PFNGLPOLYGONMODEPROC)platGetProcAddress("glPolygonMode")) != nullptr;
	r &= (glGetTexImage = (PFNGLGETTEXIMAGEPROC)platGetProcAddress("glGetTexImage")) != nullptr;
	r &= (glClearDepth = (PFNGLCLEARDEPTH)platGetProcAddress("glClearDepth")) != nullptr;
	r &= (glDrawBuffer = (PFNGLDRAWBUFFERPROC)platGetProcAddress("glDrawBuffer")) != nullptr;

	// GL 1.2
	r &= (glBlendColor = (PFNGLBLENDCOLORPROC)platGetProcAddress("glBlendColor")) != nullptr;
	r &= (glBlendEquation = (PFNGLBLENDEQUATIONPROC)platGetProcAddress("glBlendEquation")) != nullptr;
	r &= (glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)platGetProcAddress("glDrawRangeElements")) != nullptr;
	r &= (glTexImage3D = (PFNGLTEXIMAGE3DPROC)platGetProcAddress("glTexImage3D")) != nullptr;
	r &= (glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)platGetProcAddress("glTexSubImage3D")) != nullptr;
	r &= (glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)platGetProcAddress("glCopyTexSubImage3D")) != nullptr;

	// GL 1.3
	r &= (glActiveTexture = (PFNGLACTIVETEXTUREPROC)platGetProcAddress("glActiveTexture")) != nullptr;
	r &= (glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)platGetProcAddress("glSampleCoverage")) != nullptr;
	r &= (glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)platGetProcAddress("glCompressedTexImage3D")) != nullptr;
	r &= (glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)platGetProcAddress("glCompressedTexImage2D")) != nullptr;
	r &= (glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)platGetProcAddress("glCompressedTexImage1D")) != nullptr;
	r &= (glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)platGetProcAddress("glCompressedTexSubImage3D")) != nullptr;
	r &= (glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)platGetProcAddress("glCompressedTexSubImage2D")) != nullptr;
	r &= (glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)platGetProcAddress("glCompressedTexSubImage1D")) != nullptr;
	r &= (glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)platGetProcAddress("glGetCompressedTexImage")) != nullptr;

	// GL 1.4
	r &= (glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)platGetProcAddress("glBlendFuncSeparate")) != nullptr;
	r &= (glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)platGetProcAddress("glMultiDrawArrays")) != nullptr;
	r &= (glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)platGetProcAddress("glMultiDrawElements")) != nullptr;
	r &= (glPointParameterf = (PFNGLPOINTPARAMETERFPROC)platGetProcAddress("glPointParameterf")) != nullptr;
	r &= (glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)platGetProcAddress("glPointParameterfv")) != nullptr;
	r &= (glPointParameteri = (PFNGLPOINTPARAMETERIPROC)platGetProcAddress("glPointParameteri")) != nullptr;
	r &= (glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)platGetProcAddress("glPointParameteriv")) != nullptr;

	// GL 1.5
	r &= (glGenQueries = (PFNGLGENQUERIESPROC)platGetProcAddress("glGenQueries")) != nullptr;
	r &= (glDeleteQueries = (PFNGLDELETEQUERIESPROC)platGetProcAddress("glDeleteQueries")) != nullptr;
	r &= (glIsQuery = (PFNGLISQUERYPROC)platGetProcAddress("glIsQuery")) != nullptr;
	r &= (glBeginQuery = (PFNGLBEGINQUERYPROC)platGetProcAddress("glBeginQuery")) != nullptr;
	r &= (glEndQuery = (PFNGLENDQUERYPROC)platGetProcAddress("glEndQuery")) != nullptr;
	r &= (glGetQueryiv = (PFNGLGETQUERYIVPROC)platGetProcAddress("glGetQueryiv")) != nullptr;
	r &= (glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)platGetProcAddress("glGetQueryObjectiv")) != nullptr;
	r &= (glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)platGetProcAddress("glGetQueryObjectuiv")) != nullptr;
	r &= (glBindBuffer = (PFNGLBINDBUFFERPROC)platGetProcAddress("glBindBuffer")) != nullptr;
	r &= (glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)platGetProcAddress("glDeleteBuffers")) != nullptr;
	r &= (glGenBuffers = (PFNGLGENBUFFERSPROC)platGetProcAddress("glGenBuffers")) != nullptr;
	r &= (glIsBuffer = (PFNGLISBUFFERPROC)platGetProcAddress("glIsBuffer")) != nullptr;
	r &= (glBufferData = (PFNGLBUFFERDATAPROC)platGetProcAddress("glBufferData")) != nullptr;
	r &= (glBufferSubData = (PFNGLBUFFERSUBDATAPROC)platGetProcAddress("glBufferSubData")) != nullptr;
	r &= (glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)platGetProcAddress("glGetBufferSubData")) != nullptr;
	r &= (glMapBuffer = (PFNGLMAPBUFFERPROC)platGetProcAddress("glMapBuffer")) != nullptr;
	r &= (glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)platGetProcAddress("glUnmapBuffer")) != nullptr;
	r &= (glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)platGetProcAddress("glGetBufferParameteriv")) != nullptr;
	r &= (glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)platGetProcAddress("glGetBufferPointerv")) != nullptr;

	// GL 2.0
	r &= (glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)platGetProcAddress("glBlendEquationSeparate")) != nullptr;
	r &= (glDrawBuffers = (PFNGLDRAWBUFFERSPROC)platGetProcAddress("glDrawBuffers")) != nullptr;
	r &= (glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)platGetProcAddress("glStencilOpSeparate")) != nullptr;
	r &= (glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)platGetProcAddress("glStencilFuncSeparate")) != nullptr;
	r &= (glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)platGetProcAddress("glStencilMaskSeparate")) != nullptr;
	r &= (glAttachShader = (PFNGLATTACHSHADERPROC)platGetProcAddress("glAttachShader")) != nullptr;
	r &= (glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)platGetProcAddress("glBindAttribLocation")) != nullptr;
	r &= (glCompileShader = (PFNGLCOMPILESHADERPROC)platGetProcAddress("glCompileShader")) != nullptr;
	r &= (glCreateProgram = (PFNGLCREATEPROGRAMPROC)platGetProcAddress("glCreateProgram")) != nullptr;
	r &= (glCreateShader = (PFNGLCREATESHADERPROC)platGetProcAddress("glCreateShader")) != nullptr;
	r &= (glDeleteProgram = (PFNGLDELETEPROGRAMPROC)platGetProcAddress("glDeleteProgram")) != nullptr;
	r &= (glDeleteShader = (PFNGLDELETESHADERPROC)platGetProcAddress("glDeleteShader")) != nullptr;
	r &= (glDetachShader = (PFNGLDETACHSHADERPROC)platGetProcAddress("glDetachShader")) != nullptr;
	r &= (glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)platGetProcAddress("glDisableVertexAttribArray")) != nullptr;
	r &= (glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)platGetProcAddress("glEnableVertexAttribArray")) != nullptr;
	r &= (glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)platGetProcAddress("glGetActiveAttrib")) != nullptr;
	r &= (glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)platGetProcAddress("glGetActiveUniform")) != nullptr;
	r &= (glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)platGetProcAddress("glGetAttachedShaders")) != nullptr;
	r &= (glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)platGetProcAddress("glGetAttribLocation")) != nullptr;
	r &= (glGetProgramiv = (PFNGLGETPROGRAMIVPROC)platGetProcAddress("glGetProgramiv")) != nullptr;
	r &= (glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)platGetProcAddress("glGetProgramInfoLog")) != nullptr;
	r &= (glGetShaderiv = (PFNGLGETSHADERIVPROC)platGetProcAddress("glGetShaderiv")) != nullptr;
	r &= (glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)platGetProcAddress("glGetShaderInfoLog")) != nullptr;
	r &= (glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)platGetProcAddress("glGetShaderSource")) != nullptr;
	r &= (glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)platGetProcAddress("glGetUniformLocation")) != nullptr;
	r &= (glGetUniformfv = (PFNGLGETUNIFORMFVPROC)platGetProcAddress("glGetUniformfv")) != nullptr;
	r &= (glGetUniformiv = (PFNGLGETUNIFORMIVPROC)platGetProcAddress("glGetUniformiv")) != nullptr;
	r &= (glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)platGetProcAddress("glGetVertexAttribdv")) != nullptr;
	r &= (glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)platGetProcAddress("glGetVertexAttribfv")) != nullptr;
	r &= (glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)platGetProcAddress("glGetVertexAttribiv")) != nullptr;
	r &= (glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)platGetProcAddress("glGetVertexAttribPointerv")) != nullptr;
	r &= (glIsProgram = (PFNGLISPROGRAMPROC)platGetProcAddress("glIsProgram")) != nullptr;
	r &= (glIsShader = (PFNGLISSHADERPROC)platGetProcAddress("glIsShader")) != nullptr;
	r &= (glLinkProgram = (PFNGLLINKPROGRAMPROC)platGetProcAddress("glLinkProgram")) != nullptr;
	r &= (glShaderSource = (PFNGLSHADERSOURCEPROC)platGetProcAddress("glShaderSource")) != nullptr;
	r &= (glUseProgram = (PFNGLUSEPROGRAMPROC)platGetProcAddress("glUseProgram")) != nullptr;
	r &= (glUniform1f = (PFNGLUNIFORM1FPROC)platGetProcAddress("glUniform1f")) != nullptr;
	r &= (glUniform2f = (PFNGLUNIFORM2FPROC)platGetProcAddress("glUniform2f")) != nullptr;
	r &= (glUniform3f = (PFNGLUNIFORM3FPROC)platGetProcAddress("glUniform3f")) != nullptr;
	r &= (glUniform4f = (PFNGLUNIFORM4FPROC)platGetProcAddress("glUniform4f")) != nullptr;
	r &= (glUniform1i = (PFNGLUNIFORM1IPROC)platGetProcAddress("glUniform1i")) != nullptr;
	r &= (glUniform2i = (PFNGLUNIFORM2IPROC)platGetProcAddress("glUniform2i")) != nullptr;
	r &= (glUniform3i = (PFNGLUNIFORM3IPROC)platGetProcAddress("glUniform3i")) != nullptr;
	r &= (glUniform4i = (PFNGLUNIFORM4IPROC)platGetProcAddress("glUniform4i")) != nullptr;
	r &= (glUniform1fv = (PFNGLUNIFORM1FVPROC)platGetProcAddress("glUniform1fv")) != nullptr;
	r &= (glUniform2fv = (PFNGLUNIFORM2FVPROC)platGetProcAddress("glUniform2fv")) != nullptr;
	r &= (glUniform3fv = (PFNGLUNIFORM3FVPROC)platGetProcAddress("glUniform3fv")) != nullptr;
	r &= (glUniform4fv = (PFNGLUNIFORM4FVPROC)platGetProcAddress("glUniform4fv")) != nullptr;
	r &= (glUniform1iv = (PFNGLUNIFORM1IVPROC)platGetProcAddress("glUniform1iv")) != nullptr;
	r &= (glUniform2iv = (PFNGLUNIFORM2IVPROC)platGetProcAddress("glUniform2iv")) != nullptr;
	r &= (glUniform3iv = (PFNGLUNIFORM3IVPROC)platGetProcAddress("glUniform3iv")) != nullptr;
	r &= (glUniform4iv = (PFNGLUNIFORM4IVPROC)platGetProcAddress("glUniform4iv")) != nullptr;
	r &= (glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)platGetProcAddress("glUniformMatrix2fv")) != nullptr;
	r &= (glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)platGetProcAddress("glUniformMatrix3fv")) != nullptr;
	r &= (glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)platGetProcAddress("glUniformMatrix4fv")) != nullptr;
	r &= (glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)platGetProcAddress("glValidateProgram")) != nullptr;
	r &= (glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)platGetProcAddress("glVertexAttribPointer")) != nullptr;

	// GL 2.1
	r &= (glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)platGetProcAddress("glUniformMatrix2x3fv")) != nullptr;
	r &= (glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)platGetProcAddress("glUniformMatrix3x2fv")) != nullptr;
	r &= (glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)platGetProcAddress("glUniformMatrix2x4fv")) != nullptr;
	r &= (glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)platGetProcAddress("glUniformMatrix4x2fv")) != nullptr;
	r &= (glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)platGetProcAddress("glUniformMatrix3x4fv")) != nullptr;
	r &= (glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)platGetProcAddress("glUniformMatrix4x3fv")) != nullptr;

	// GL 3.0 - GL 3.3 
	{
		// GL 3.0
		r &= (glGetStringi = (PFNGLGETSTRINGIPROC)platGetProcAddress("glGetStringi")) != nullptr;
		r &= (glColorMaski = (PFNGLCOLORMASKIPROC)platGetProcAddress("glColorMaski")) != nullptr;
		r &= (glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)platGetProcAddress("glGetBooleani_v")) != nullptr;
		r &= (glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)platGetProcAddress("glGetIntegeri_v")) != nullptr;
		r &= (glEnablei = (PFNGLENABLEIPROC)platGetProcAddress("glEnablei")) != nullptr;
		r &= (glDisablei = (PFNGLDISABLEIPROC)platGetProcAddress("glDisablei")) != nullptr;
		r &= (glIsEnabledi = (PFNGLISENABLEDIPROC)platGetProcAddress("glIsEnabledi")) != nullptr;
		r &= (glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)platGetProcAddress("glBeginTransformFeedback")) != nullptr;
		r &= (glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)platGetProcAddress("glEndTransformFeedback")) != nullptr;
		r &= (glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)platGetProcAddress("glBindBufferRange")) != nullptr;
		r &= (glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)platGetProcAddress("glBindBufferBase")) != nullptr;
		r &= (glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)platGetProcAddress("glTransformFeedbackVaryings")) != nullptr;
		r &= (glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)platGetProcAddress("glGetTransformFeedbackVarying")) != nullptr;
		r &= (glClampColor = (PFNGLCLAMPCOLORPROC)platGetProcAddress("glClampColor")) != nullptr;
		r &= (glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)platGetProcAddress("glBeginConditionalRender")) != nullptr;
		r &= (glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)platGetProcAddress("glEndConditionalRender")) != nullptr;
		r &= (glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)platGetProcAddress("glVertexAttribIPointer")) != nullptr;
		r &= (glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)platGetProcAddress("glGetVertexAttribIiv")) != nullptr;
		r &= (glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)platGetProcAddress("glGetVertexAttribIuiv")) != nullptr;
		r &= (glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)platGetProcAddress("glVertexAttribI1i")) != nullptr;
		r &= (glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)platGetProcAddress("glVertexAttribI2i")) != nullptr;
		r &= (glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)platGetProcAddress("glVertexAttribI3i")) != nullptr;
		r &= (glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)platGetProcAddress("glVertexAttribI4i")) != nullptr;
		r &= (glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)platGetProcAddress("glVertexAttribI1ui")) != nullptr;
		r &= (glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)platGetProcAddress("glVertexAttribI2ui")) != nullptr;
		r &= (glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)platGetProcAddress("glVertexAttribI3ui")) != nullptr;
		r &= (glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)platGetProcAddress("glVertexAttribI4ui")) != nullptr;
		r &= (glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)platGetProcAddress("glVertexAttribI1iv")) != nullptr;
		r &= (glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)platGetProcAddress("glVertexAttribI2iv")) != nullptr;
		r &= (glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)platGetProcAddress("glVertexAttribI3iv")) != nullptr;
		r &= (glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)platGetProcAddress("glVertexAttribI4iv")) != nullptr;
		r &= (glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)platGetProcAddress("glVertexAttribI1uiv")) != nullptr;
		r &= (glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)platGetProcAddress("glVertexAttribI2uiv")) != nullptr;
		r &= (glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)platGetProcAddress("glVertexAttribI3uiv")) != nullptr;
		r &= (glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)platGetProcAddress("glVertexAttribI4uiv")) != nullptr;
		r &= (glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)platGetProcAddress("glVertexAttribI4bv")) != nullptr;
		r &= (glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)platGetProcAddress("glVertexAttribI4sv")) != nullptr;
		r &= (glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)platGetProcAddress("glVertexAttribI4ubv")) != nullptr;
		r &= (glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)platGetProcAddress("glVertexAttribI4usv")) != nullptr;
		r &= (glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)platGetProcAddress("glGetUniformuiv")) != nullptr;
		r &= (glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)platGetProcAddress("glBindFragDataLocation")) != nullptr;
		r &= (glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)platGetProcAddress("glGetFragDataLocation")) != nullptr;
		r &= (glUniform1ui = (PFNGLUNIFORM1UIPROC)platGetProcAddress("glUniform1ui")) != nullptr;
		r &= (glUniform2ui = (PFNGLUNIFORM2UIPROC)platGetProcAddress("glUniform2ui")) != nullptr;
		r &= (glUniform3ui = (PFNGLUNIFORM3UIPROC)platGetProcAddress("glUniform3ui")) != nullptr;
		r &= (glUniform4ui = (PFNGLUNIFORM4UIPROC)platGetProcAddress("glUniform4ui")) != nullptr;
		r &= (glUniform1uiv = (PFNGLUNIFORM1UIVPROC)platGetProcAddress("glUniform1uiv")) != nullptr;
		r &= (glUniform2uiv = (PFNGLUNIFORM2UIVPROC)platGetProcAddress("glUniform2uiv")) != nullptr;
		r &= (glUniform3uiv = (PFNGLUNIFORM3UIVPROC)platGetProcAddress("glUniform3uiv")) != nullptr;
		r &= (glUniform4uiv = (PFNGLUNIFORM4UIVPROC)platGetProcAddress("glUniform4uiv")) != nullptr;
		r &= (glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)platGetProcAddress("glTexParameterIiv")) != nullptr;
		r &= (glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)platGetProcAddress("glTexParameterIuiv")) != nullptr;
		r &= (glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)platGetProcAddress("glGetTexParameterIiv")) != nullptr;
		r &= (glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)platGetProcAddress("glGetTexParameterIuiv")) != nullptr;
		r &= (glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)platGetProcAddress("glClearBufferiv")) != nullptr;
		r &= (glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)platGetProcAddress("glClearBufferuiv")) != nullptr;
		r &= (glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)platGetProcAddress("glClearBufferfv")) != nullptr;
		r &= (glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)platGetProcAddress("glClearBufferfi")) != nullptr;
		r &= (glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)platGetProcAddress("glIsRenderbuffer")) != nullptr;
		r &= (glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)platGetProcAddress("glBindRenderbuffer")) != nullptr;
		r &= (glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)platGetProcAddress("glDeleteRenderbuffers")) != nullptr;
		r &= (glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)platGetProcAddress("glGenRenderbuffers")) != nullptr;
		r &= (glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)platGetProcAddress("glRenderbufferStorage")) != nullptr;
		r &= (glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)platGetProcAddress("glGetRenderbufferParameteriv")) != nullptr;
		r &= (glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)platGetProcAddress("glIsFramebuffer")) != nullptr;
		r &= (glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)platGetProcAddress("glBindFramebuffer")) != nullptr;
		r &= (glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)platGetProcAddress("glDeleteFramebuffers")) != nullptr;
		r &= (glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)platGetProcAddress("glGenFramebuffers")) != nullptr;
		r &= (glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)platGetProcAddress("glCheckFramebufferStatus")) != nullptr;
		r &= (glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)platGetProcAddress("glFramebufferTexture1D")) != nullptr;
		r &= (glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)platGetProcAddress("glFramebufferTexture2D")) != nullptr;
		r &= (glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)platGetProcAddress("glFramebufferTexture3D")) != nullptr;
		r &= (glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)platGetProcAddress("glFramebufferRenderbuffer")) != nullptr;
		r &= (glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)platGetProcAddress("glGetFramebufferAttachmentParameteriv")) != nullptr;
		r &= (glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)platGetProcAddress("glGenerateMipmap")) != nullptr;
		r &= (glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)platGetProcAddress("glBlitFramebuffer")) != nullptr;
		r &= (glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)platGetProcAddress("glRenderbufferStorageMultisample")) != nullptr;
		r &= (glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)platGetProcAddress("glFramebufferTextureLayer")) != nullptr;
		r &= (glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)platGetProcAddress("glMapBufferRange")) != nullptr;
		r &= (glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)platGetProcAddress("glFlushMappedBufferRange")) != nullptr;
		r &= (glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)platGetProcAddress("glBindVertexArray")) != nullptr;
		r &= (glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)platGetProcAddress("glDeleteVertexArrays")) != nullptr;
		r &= (glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)platGetProcAddress("glGenVertexArrays")) != nullptr;
		r &= (glIsVertexArray = (PFNGLISVERTEXARRAYPROC)platGetProcAddress("glIsVertexArray")) != nullptr;

		// GL 3.1
		if (_Config.OpenGL.majorVersion * 10 + _Config.OpenGL.minorVersion >= 31)
		{
			r &= (glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)platGetProcAddress("glDrawArraysInstanced")) != nullptr;
			r &= (glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)platGetProcAddress("glDrawElementsInstanced")) != nullptr;
			r &= (glTexBuffer = (PFNGLTEXBUFFERPROC)platGetProcAddress("glTexBuffer")) != nullptr;
			r &= (glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)platGetProcAddress("glPrimitiveRestartIndex")) != nullptr;
			r &= (glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)platGetProcAddress("glCopyBufferSubData")) != nullptr;
			r &= (glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)platGetProcAddress("glGetUniformIndices")) != nullptr;
			r &= (glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)platGetProcAddress("glGetActiveUniformsiv")) != nullptr;
			r &= (glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)platGetProcAddress("glGetActiveUniformName")) != nullptr;
			r &= (glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)platGetProcAddress("glGetUniformBlockIndex")) != nullptr;
			r &= (glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)platGetProcAddress("glGetActiveUniformBlockiv")) != nullptr;
			r &= (glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)platGetProcAddress("glGetActiveUniformBlockName")) != nullptr;
			r &= (glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)platGetProcAddress("glUniformBlockBinding")) != nullptr;
		}

		// GL 3.2
		if (_Config.OpenGL.majorVersion * 10 + _Config.OpenGL.minorVersion >= 32)
		{
			r &= (glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)platGetProcAddress("glDrawElementsBaseVertex")) != nullptr;
			r &= (glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)platGetProcAddress("glDrawRangeElementsBaseVertex")) != nullptr;
			r &= (glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)platGetProcAddress("glDrawElementsInstancedBaseVertex")) != nullptr;
			r &= (glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)platGetProcAddress("glMultiDrawElementsBaseVertex")) != nullptr;
			r &= (glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)platGetProcAddress("glProvokingVertex")) != nullptr;
			r &= (glFenceSync = (PFNGLFENCESYNCPROC)platGetProcAddress("glFenceSync")) != nullptr;
			r &= (glIsSync = (PFNGLISSYNCPROC)platGetProcAddress("glIsSync")) != nullptr;
			r &= (glDeleteSync = (PFNGLDELETESYNCPROC)platGetProcAddress("glDeleteSync")) != nullptr;
			r &= (glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)platGetProcAddress("glClientWaitSync")) != nullptr;
			r &= (glWaitSync = (PFNGLWAITSYNCPROC)platGetProcAddress("glWaitSync")) != nullptr;
			r &= (glGetInteger64v = (PFNGLGETINTEGER64VPROC)platGetProcAddress("glGetInteger64v")) != nullptr;
			r &= (glGetSynciv = (PFNGLGETSYNCIVPROC)platGetProcAddress("glGetSynciv")) != nullptr;
			r &= (glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)platGetProcAddress("glGetInteger64i_v")) != nullptr;
			r &= (glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)platGetProcAddress("glGetBufferParameteri64v")) != nullptr;
			r &= (glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)platGetProcAddress("glFramebufferTexture")) != nullptr;
			r &= (glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)platGetProcAddress("glTexImage2DMultisample")) != nullptr;
			r &= (glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)platGetProcAddress("glTexImage3DMultisample")) != nullptr;
			r &= (glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)platGetProcAddress("glGetMultisamplefv")) != nullptr;
			r &= (glSampleMaski = (PFNGLSAMPLEMASKIPROC)platGetProcAddress("glSampleMaski")) != nullptr;
		}

		// GL 3.3
		if (_Config.OpenGL.majorVersion * 10 + _Config.OpenGL.minorVersion >= 33)
		{
			r &= (glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)platGetProcAddress("glBindFragDataLocationIndexed")) != nullptr;
			r &= (glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)platGetProcAddress("glGetFragDataIndex")) != nullptr;
			r &= (glGenSamplers = (PFNGLGENSAMPLERSPROC)platGetProcAddress("glGenSamplers")) != nullptr;
			r &= (glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)platGetProcAddress("glDeleteSamplers")) != nullptr;
			r &= (glIsSampler = (PFNGLISSAMPLERPROC)platGetProcAddress("glIsSampler")) != nullptr;
			r &= (glBindSampler = (PFNGLBINDSAMPLERPROC)platGetProcAddress("glBindSampler")) != nullptr;
			r &= (glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)platGetProcAddress("glSamplerParameteri")) != nullptr;
			r &= (glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)platGetProcAddress("glSamplerParameteriv")) != nullptr;
			r &= (glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)platGetProcAddress("glSamplerParameterf")) != nullptr;
			r &= (glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)platGetProcAddress("glSamplerParameterfv")) != nullptr;
			r &= (glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)platGetProcAddress("glSamplerParameterIiv")) != nullptr;
			r &= (glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)platGetProcAddress("glSamplerParameterIuiv")) != nullptr;
			r &= (glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)platGetProcAddress("glGetSamplerParameteriv")) != nullptr;
			r &= (glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)platGetProcAddress("glGetSamplerParameterIiv")) != nullptr;
			r &= (glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)platGetProcAddress("glGetSamplerParameterfv")) != nullptr;
			r &= (glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)platGetProcAddress("glGetSamplerParameterIuiv")) != nullptr;
			r &= (glQueryCounter = (PFNGLQUERYCOUNTERPROC)platGetProcAddress("glQueryCounter")) != nullptr;
			r &= (glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)platGetProcAddress("glGetQueryObjecti64v")) != nullptr;
			r &= (glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)platGetProcAddress("glGetQueryObjectui64v")) != nullptr;
			r &= (glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)platGetProcAddress("glVertexAttribDivisor")) != nullptr;
			r &= (glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)platGetProcAddress("glVertexAttribP1ui")) != nullptr;
			r &= (glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)platGetProcAddress("glVertexAttribP1uiv")) != nullptr;
			r &= (glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)platGetProcAddress("glVertexAttribP2ui")) != nullptr;
			r &= (glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)platGetProcAddress("glVertexAttribP2uiv")) != nullptr;
			r &= (glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)platGetProcAddress("glVertexAttribP3ui")) != nullptr;
			r &= (glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)platGetProcAddress("glVertexAttribP3uiv")) != nullptr;
			r &= (glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)platGetProcAddress("glVertexAttribP4ui")) != nullptr;
			r &= (glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)platGetProcAddress("glVertexAttribP4uiv")) != nullptr;
		}
	}

	// GL 4.0 - GL 4.4
	if (_Config.OpenGL.majorVersion >= 4)
	{
		// GL 4.0
		r &= (glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC)platGetProcAddress("glMinSampleShading")) != nullptr;
		r &= (glBlendEquationi = (PFNGLBLENDEQUATIONIPROC)platGetProcAddress("glBlendEquationi")) != nullptr;
		r &= (glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)platGetProcAddress("glBlendEquationSeparatei")) != nullptr;
		r &= (glBlendFunci = (PFNGLBLENDFUNCIPROC)platGetProcAddress("glBlendFunci")) != nullptr;
		r &= (glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)platGetProcAddress("glBlendFuncSeparatei")) != nullptr;
		r &= (glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)platGetProcAddress("glDrawArraysIndirect")) != nullptr;
		r &= (glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)platGetProcAddress("glDrawElementsIndirect")) != nullptr;
		r &= (glUniform1d = (PFNGLUNIFORM1DPROC)platGetProcAddress("glUniform1d")) != nullptr;
		r &= (glUniform2d = (PFNGLUNIFORM2DPROC)platGetProcAddress("glUniform2d")) != nullptr;
		r &= (glUniform3d = (PFNGLUNIFORM3DPROC)platGetProcAddress("glUniform3d")) != nullptr;
		r &= (glUniform4d = (PFNGLUNIFORM4DPROC)platGetProcAddress("glUniform4d")) != nullptr;
		r &= (glUniform1dv = (PFNGLUNIFORM1DVPROC)platGetProcAddress("glUniform1dv")) != nullptr;
		r &= (glUniform2dv = (PFNGLUNIFORM2DVPROC)platGetProcAddress("glUniform2dv")) != nullptr;
		r &= (glUniform3dv = (PFNGLUNIFORM3DVPROC)platGetProcAddress("glUniform3dv")) != nullptr;
		r &= (glUniform4dv = (PFNGLUNIFORM4DVPROC)platGetProcAddress("glUniform4dv")) != nullptr;
		r &= (glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC)platGetProcAddress("glUniformMatrix2dv")) != nullptr;
		r &= (glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC)platGetProcAddress("glUniformMatrix3dv")) != nullptr;
		r &= (glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)platGetProcAddress("glUniformMatrix4dv")) != nullptr;
		r &= (glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC)platGetProcAddress("glUniformMatrix2x3dv")) != nullptr;
		r &= (glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC)platGetProcAddress("glUniformMatrix2x4dv")) != nullptr;
		r &= (glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC)platGetProcAddress("glUniformMatrix3x2dv")) != nullptr;
		r &= (glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC)platGetProcAddress("glUniformMatrix3x4dv")) != nullptr;
		r &= (glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC)platGetProcAddress("glUniformMatrix4x2dv")) != nullptr;
		r &= (glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC)platGetProcAddress("glUniformMatrix4x3dv")) != nullptr;
		r &= (glGetUniformdv = (PFNGLGETUNIFORMDVPROC)platGetProcAddress("glGetUniformdv")) != nullptr;
		r &= (glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)platGetProcAddress("glGetSubroutineUniformLocation")) != nullptr;
		r &= (glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)platGetProcAddress("glGetSubroutineIndex")) != nullptr;
		r &= (glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)platGetProcAddress("glGetActiveSubroutineUniformiv")) != nullptr;
		r &= (glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)platGetProcAddress("glGetActiveSubroutineUniformName")) != nullptr;
		r &= (glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC)platGetProcAddress("glGetActiveSubroutineName")) != nullptr;
		r &= (glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)platGetProcAddress("glUniformSubroutinesuiv")) != nullptr;
		r &= (glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC)platGetProcAddress("glGetUniformSubroutineuiv")) != nullptr;
		r &= (glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC)platGetProcAddress("glGetProgramStageiv")) != nullptr;
		r &= (glPatchParameteri = (PFNGLPATCHPARAMETERIPROC)platGetProcAddress("glPatchParameteri")) != nullptr;
		r &= (glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC)platGetProcAddress("glPatchParameterfv")) != nullptr;
		r &= (glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)platGetProcAddress("glBindTransformFeedback")) != nullptr;
		r &= (glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)platGetProcAddress("glDeleteTransformFeedbacks")) != nullptr;
		r &= (glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)platGetProcAddress("glGenTransformFeedbacks")) != nullptr;
		r &= (glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)platGetProcAddress("glIsTransformFeedback")) != nullptr;
		r &= (glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)platGetProcAddress("glPauseTransformFeedback")) != nullptr;
		r &= (glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)platGetProcAddress("glResumeTransformFeedback")) != nullptr;
		r &= (glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)platGetProcAddress("glDrawTransformFeedback")) != nullptr;
		r &= (glDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)platGetProcAddress("glDrawTransformFeedbackStream")) != nullptr;
		r &= (glBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC)platGetProcAddress("glBeginQueryIndexed")) != nullptr;
		r &= (glEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC)platGetProcAddress("glEndQueryIndexed")) != nullptr;
		r &= (glGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC)platGetProcAddress("glGetQueryIndexediv")) != nullptr;

		if (_Config.OpenGL.minorVersion >= 1)
		{
			// GL 4.1
			r &= (glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC)platGetProcAddress("glReleaseShaderCompiler")) != nullptr;
			r &= (glShaderBinary = (PFNGLSHADERBINARYPROC)platGetProcAddress("glShaderBinary")) != nullptr;
			r &= (glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC)platGetProcAddress("glGetShaderPrecisionFormat")) != nullptr;
			r &= (glDepthRangef = (PFNGLDEPTHRANGEFPROC)platGetProcAddress("glDepthRangef")) != nullptr;
			r &= (glClearDepthf = (PFNGLCLEARDEPTHFPROC)platGetProcAddress("glClearDepthf")) != nullptr;
			r &= (glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)platGetProcAddress("glGetProgramBinary")) != nullptr;
			r &= (glProgramBinary = (PFNGLPROGRAMBINARYPROC)platGetProcAddress("glProgramBinary")) != nullptr;
			r &= (glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)platGetProcAddress("glProgramParameteri")) != nullptr;
			r &= (glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)platGetProcAddress("glUseProgramStages")) != nullptr;
			r &= (glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)platGetProcAddress("glActiveShaderProgram")) != nullptr;
			r &= (glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)platGetProcAddress("glCreateShaderProgramv")) != nullptr;
			r &= (glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)platGetProcAddress("glBindProgramPipeline")) != nullptr;
			r &= (glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)platGetProcAddress("glDeleteProgramPipelines")) != nullptr;
			r &= (glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)platGetProcAddress("glGenProgramPipelines")) != nullptr;
			r &= (glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC)platGetProcAddress("glIsProgramPipeline")) != nullptr;
			r &= (glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC)platGetProcAddress("glGetProgramPipelineiv")) != nullptr;
			r &= (glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)platGetProcAddress("glProgramUniform1i")) != nullptr;
			r &= (glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)platGetProcAddress("glProgramUniform1iv")) != nullptr;
			r &= (glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)platGetProcAddress("glProgramUniform1f")) != nullptr;
			r &= (glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)platGetProcAddress("glProgramUniform1fv")) != nullptr;
			r &= (glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)platGetProcAddress("glProgramUniform1d")) != nullptr;
			r &= (glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)platGetProcAddress("glProgramUniform1dv")) != nullptr;
			r &= (glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)platGetProcAddress("glProgramUniform1ui")) != nullptr;
			r &= (glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)platGetProcAddress("glProgramUniform1uiv")) != nullptr;
			r &= (glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)platGetProcAddress("glProgramUniform2i")) != nullptr;
			r &= (glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)platGetProcAddress("glProgramUniform2iv")) != nullptr;
			r &= (glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)platGetProcAddress("glProgramUniform2f")) != nullptr;
			r &= (glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)platGetProcAddress("glProgramUniform2fv")) != nullptr;
			r &= (glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)platGetProcAddress("glProgramUniform2d")) != nullptr;
			r &= (glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)platGetProcAddress("glProgramUniform2dv")) != nullptr;
			r &= (glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)platGetProcAddress("glProgramUniform2ui")) != nullptr;
			r &= (glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)platGetProcAddress("glProgramUniform2uiv")) != nullptr;
			r &= (glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)platGetProcAddress("glProgramUniform3i")) != nullptr;
			r &= (glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)platGetProcAddress("glProgramUniform3iv")) != nullptr;
			r &= (glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)platGetProcAddress("glProgramUniform3f")) != nullptr;
			r &= (glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)platGetProcAddress("glProgramUniform3fv")) != nullptr;
			r &= (glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)platGetProcAddress("glProgramUniform3d")) != nullptr;
			r &= (glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)platGetProcAddress("glProgramUniform3dv")) != nullptr;
			r &= (glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)platGetProcAddress("glProgramUniform3ui")) != nullptr;
			r &= (glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)platGetProcAddress("glProgramUniform3uiv")) != nullptr;
			r &= (glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)platGetProcAddress("glProgramUniform4i")) != nullptr;
			r &= (glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)platGetProcAddress("glProgramUniform4iv")) != nullptr;
			r &= (glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)platGetProcAddress("glProgramUniform4f")) != nullptr;
			r &= (glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)platGetProcAddress("glProgramUniform4fv")) != nullptr;
			r &= (glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)platGetProcAddress("glProgramUniform4d")) != nullptr;
			r &= (glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)platGetProcAddress("glProgramUniform4dv")) != nullptr;
			r &= (glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)platGetProcAddress("glProgramUniform4ui")) != nullptr;
			r &= (glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)platGetProcAddress("glProgramUniform4uiv")) != nullptr;
			r &= (glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)platGetProcAddress("glProgramUniformMatrix2fv")) != nullptr;
			r &= (glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)platGetProcAddress("glProgramUniformMatrix3fv")) != nullptr;
			r &= (glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)platGetProcAddress("glProgramUniformMatrix4fv")) != nullptr;
			r &= (glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)platGetProcAddress("glProgramUniformMatrix2dv")) != nullptr;
			r &= (glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)platGetProcAddress("glProgramUniformMatrix3dv")) != nullptr;
			r &= (glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)platGetProcAddress("glProgramUniformMatrix4dv")) != nullptr;
			r &= (glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)platGetProcAddress("glProgramUniformMatrix2x3fv")) != nullptr;
			r &= (glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)platGetProcAddress("glProgramUniformMatrix3x2fv")) != nullptr;
			r &= (glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)platGetProcAddress("glProgramUniformMatrix2x4fv")) != nullptr;
			r &= (glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)platGetProcAddress("glProgramUniformMatrix4x2fv")) != nullptr;
			r &= (glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)platGetProcAddress("glProgramUniformMatrix3x4fv")) != nullptr;
			r &= (glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)platGetProcAddress("glProgramUniformMatrix4x3fv")) != nullptr;
			r &= (glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)platGetProcAddress("glProgramUniformMatrix2x3dv")) != nullptr;
			r &= (glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)platGetProcAddress("glProgramUniformMatrix3x2dv")) != nullptr;
			r &= (glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)platGetProcAddress("glProgramUniformMatrix2x4dv")) != nullptr;
			r &= (glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)platGetProcAddress("glProgramUniformMatrix4x2dv")) != nullptr;
			r &= (glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)platGetProcAddress("glProgramUniformMatrix3x4dv")) != nullptr;
			r &= (glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)platGetProcAddress("glProgramUniformMatrix4x3dv")) != nullptr;
			r &= (glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)platGetProcAddress("glValidateProgramPipeline")) != nullptr;
			r &= (glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)platGetProcAddress("glGetProgramPipelineInfoLog")) != nullptr;
			r &= (glVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC)platGetProcAddress("glVertexAttribL1d")) != nullptr;
			r &= (glVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC)platGetProcAddress("glVertexAttribL2d")) != nullptr;
			r &= (glVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC)platGetProcAddress("glVertexAttribL3d")) != nullptr;
			r &= (glVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC)platGetProcAddress("glVertexAttribL4d")) != nullptr;
			r &= (glVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC)platGetProcAddress("glVertexAttribL1dv")) != nullptr;
			r &= (glVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC)platGetProcAddress("glVertexAttribL2dv")) != nullptr;
			r &= (glVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC)platGetProcAddress("glVertexAttribL3dv")) != nullptr;
			r &= (glVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC)platGetProcAddress("glVertexAttribL4dv")) != nullptr;
			r &= (glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)platGetProcAddress("glVertexAttribLPointer")) != nullptr;
			r &= (glGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC)platGetProcAddress("glGetVertexAttribLdv")) != nullptr;
			r &= (glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC)platGetProcAddress("glViewportArrayv")) != nullptr;
			r &= (glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC)platGetProcAddress("glViewportIndexedf")) != nullptr;
			r &= (glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC)platGetProcAddress("glViewportIndexedfv")) != nullptr;
			r &= (glScissorArrayv = (PFNGLSCISSORARRAYVPROC)platGetProcAddress("glScissorArrayv")) != nullptr;
			r &= (glScissorIndexed = (PFNGLSCISSORINDEXEDPROC)platGetProcAddress("glScissorIndexed")) != nullptr;
			r &= (glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC)platGetProcAddress("glScissorIndexedv")) != nullptr;
			r &= (glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC)platGetProcAddress("glDepthRangeArrayv")) != nullptr;
			r &= (glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC)platGetProcAddress("glDepthRangeIndexed")) != nullptr;
			r &= (glGetFloati_v = (PFNGLGETFLOATI_VPROC)platGetProcAddress("glGetFloati_v")) != nullptr;
			r &= (glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC)platGetProcAddress("glGetDoublei_v")) != nullptr;
		}

		if (_Config.OpenGL.minorVersion >= 2)
		{
			// GL 4.2
			r &= (glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)platGetProcAddress("glDrawArraysInstancedBaseInstance")) != nullptr;
			r &= (glDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)platGetProcAddress("glDrawElementsInstancedBaseInstance")) != nullptr;
			r &= (glDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)platGetProcAddress("glDrawElementsInstancedBaseVertexBaseInstance")) != nullptr;
			r &= (glGetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC)platGetProcAddress("glGetInternalformativ")) != nullptr;
			r &= (glGetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC)platGetProcAddress("glGetActiveAtomicCounterBufferiv")) != nullptr;
			r &= (glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)platGetProcAddress("glBindImageTexture")) != nullptr;
			r &= (glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)platGetProcAddress("glMemoryBarrier")) != nullptr;
			r &= (glTexStorage1D = (PFNGLTEXSTORAGE1DPROC)platGetProcAddress("glTexStorage1D")) != nullptr;
			r &= (glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)platGetProcAddress("glTexStorage2D")) != nullptr;
			r &= (glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)platGetProcAddress("glTexStorage3D")) != nullptr;
			r &= (glDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC)platGetProcAddress("glDrawTransformFeedbackInstanced")) != nullptr;
			r &= (glDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC)platGetProcAddress("glDrawTransformFeedbackStreamInstanced")) != nullptr;
		}

		if (_Config.OpenGL.minorVersion >= 3)
		{
			// GL 4.3
			r &= (glClearBufferData = (PFNGLCLEARBUFFERDATAPROC)platGetProcAddress("glClearBufferData")) != nullptr;
			r &= (glClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC)platGetProcAddress("glClearBufferSubData")) != nullptr;
			r &= (glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)platGetProcAddress("glDispatchCompute")) != nullptr;
			r &= (glDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC)platGetProcAddress("glDispatchComputeIndirect")) != nullptr;
			r &= (glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)platGetProcAddress("glCopyImageSubData")) != nullptr;
			r &= (glFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERIPROC)platGetProcAddress("glFramebufferParameteri")) != nullptr;
			r &= (glGetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIVPROC)platGetProcAddress("glGetFramebufferParameteriv")) != nullptr;
			r &= (glGetInternalformati64v = (PFNGLGETINTERNALFORMATI64VPROC)platGetProcAddress("glGetInternalformati64v")) != nullptr;
			r &= (glInvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGEPROC)platGetProcAddress("glInvalidateTexSubImage")) != nullptr;
			r &= (glInvalidateTexImage = (PFNGLINVALIDATETEXIMAGEPROC)platGetProcAddress("glInvalidateTexImage")) != nullptr;
			r &= (glInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC)platGetProcAddress("glInvalidateBufferSubData")) != nullptr;
			r &= (glInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC)platGetProcAddress("glInvalidateBufferData")) != nullptr;
			r &= (glInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFERPROC)platGetProcAddress("glInvalidateFramebuffer")) != nullptr;
			r &= (glInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFERPROC)platGetProcAddress("glInvalidateSubFramebuffer")) != nullptr;
			r &= (glMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECTPROC)platGetProcAddress("glMultiDrawArraysIndirect")) != nullptr;
			r &= (glMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECTPROC)platGetProcAddress("glMultiDrawElementsIndirect")) != nullptr;
			r &= (glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)platGetProcAddress("glGetProgramInterfaceiv")) != nullptr;
			r &= (glGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC)platGetProcAddress("glGetProgramResourceIndex")) != nullptr;
			r &= (glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)platGetProcAddress("glGetProgramResourceName")) != nullptr;
			r &= (glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)platGetProcAddress("glGetProgramResourceiv")) != nullptr;
			r &= (glGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATIONPROC)platGetProcAddress("glGetProgramResourceLocation")) != nullptr;
			r &= (glGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC)platGetProcAddress("glGetProgramResourceLocationIndex")) != nullptr;
			r &= (glShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDINGPROC)platGetProcAddress("glShaderStorageBlockBinding")) != nullptr;
			r &= (glTexBufferRange = (PFNGLTEXBUFFERRANGEPROC)platGetProcAddress("glTexBufferRange")) != nullptr;
			r &= (glTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC)platGetProcAddress("glTexStorage2DMultisample")) != nullptr;
			r &= (glTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLEPROC)platGetProcAddress("glTexStorage3DMultisample")) != nullptr;
			r &= (glTextureView = (PFNGLTEXTUREVIEWPROC)platGetProcAddress("glTextureView")) != nullptr;
			r &= (glBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)platGetProcAddress("glBindVertexBuffer")) != nullptr;
			r &= (glVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)platGetProcAddress("glVertexAttribFormat")) != nullptr;
			r &= (glVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMATPROC)platGetProcAddress("glVertexAttribIFormat")) != nullptr;
			r &= (glVertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMATPROC)platGetProcAddress("glVertexAttribLFormat")) != nullptr;
			r &= (glVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)platGetProcAddress("glVertexAttribBinding")) != nullptr;
			r &= (glVertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISORPROC)platGetProcAddress("glVertexBindingDivisor")) != nullptr;
			r &= (glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)platGetProcAddress("glDebugMessageControl")) != nullptr;
			r &= (glDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC)platGetProcAddress("glDebugMessageInsert")) != nullptr;
			r &= (glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)platGetProcAddress("glDebugMessageCallback")) != nullptr;
			r &= (glGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC)platGetProcAddress("glGetDebugMessageLog")) != nullptr;
			r &= (glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)platGetProcAddress("glPushDebugGroup")) != nullptr;
			r &= (glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)platGetProcAddress("glPopDebugGroup")) != nullptr;
			r &= (glObjectLabel = (PFNGLOBJECTLABELPROC)platGetProcAddress("glObjectLabel")) != nullptr;
			r &= (glGetObjectLabel = (PFNGLGETOBJECTLABELPROC)platGetProcAddress("glGetObjectLabel")) != nullptr;
			r &= (glObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC)platGetProcAddress("glObjectPtrLabel")) != nullptr;
			r &= (glGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC)platGetProcAddress("glGetObjectPtrLabel")) != nullptr;
		}

		if (_Config.OpenGL.minorVersion >= 4)
		{
			// GL 4.4
			r &= (glBufferStorage = (PFNGLBUFFERSTORAGEPROC)platGetProcAddress("glBufferStorage")) != nullptr;
			r &= (glClearTexImage = (PFNGLCLEARTEXIMAGEPROC)platGetProcAddress("glClearTexImage")) != nullptr;
			r &= (glClearTexSubImage = (PFNGLCLEARTEXSUBIMAGEPROC)platGetProcAddress("glClearTexSubImage")) != nullptr;
			r &= (glBindBuffersBase = (PFNGLBINDBUFFERSBASEPROC)platGetProcAddress("glBindBuffersBase")) != nullptr;
			r &= (glBindBuffersRange = (PFNGLBINDBUFFERSRANGEPROC)platGetProcAddress("glBindBuffersRange")) != nullptr;
			r &= (glBindTextures = (PFNGLBINDTEXTURESPROC)platGetProcAddress("glBindTextures")) != nullptr;
			r &= (glBindSamplers = (PFNGLBINDSAMPLERSPROC)platGetProcAddress("glBindSamplers")) != nullptr;
			r &= (glBindImageTextures = (PFNGLBINDIMAGETEXTURESPROC)platGetProcAddress("glBindImageTextures")) != nullptr;
			r &= (glBindVertexBuffers = (PFNGLBINDVERTEXBUFFERSPROC)platGetProcAddress("glBindVertexBuffers")) != nullptr;
		}
	}

	// OES image
	_Config.OpenGL.OES_EGL_image = isExtensionSupported("GL_OES_EGL_image");
	if (_Config.OpenGL.OES_EGL_image)
	{
		r &= (glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)platGetProcAddress("glEGLImageTargetTexture2DOES")) != nullptr;
	}

	// Default extensions, suitable for any OpenGL version
	_Config.OpenGL.EXT_texture_filter_anisotropic = isExtensionSupported("GL_EXT_texture_filter_anisotropic");
	_Config.OpenGL.EXT_texture_compression_s3tc = isExtensionSupported("GL_EXT_texture_compression_s3tc") || isExtensionSupported("GL_S3_s3tc");
	_Config.OpenGL.EXT_texture_sRGB = isExtensionSupported("GL_EXT_texture_sRGB");

	return r;
}

