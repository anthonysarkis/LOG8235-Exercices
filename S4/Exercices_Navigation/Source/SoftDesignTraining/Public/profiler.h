//#pragma once
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#include "Windows/AllowWindowsPlatformTypes.h"
//#include <stdio.h>
//#include <stdarg.h>
//#include <string>
//#include <thread>
//#include <atomic>
//#include <winsock.h>
//
//#define MICROPROFILE_ENABLED 1
//#define MICROPROFILE_WEBSERVER  1
//#define MICROPROFILE_UI         0
//#define MICROPROFILE_GPU_TIMERS 0
//#define MICROPROFILE_CONTEXT_SWITCH_TRACE 0
//#define MICROPROFILE_CUSTOM_MACRO
//
//#define MICROPROFILE_DECLARE_GROUP(group) \
//    DECLARE_STATS_GROUP(TEXT(#group), group, STATCAT_Advanced);
//
//#define MICROPROFILE_DECLARE(var) extern MicroProfileToken g_mp_##var;
//#define MICROPROFILE_DEFINE(var, group, name, color) \
//    DECLARE_STAT(TEXT(#name), name, group, EStatDataType::ST_int64, true, true, FPlatformMemory::MCR_Invalid); \
//    DEFINE_STAT(name); \
//    MicroProfileToken g_mp_##var = ProfilerRegisterToken(GET_STATID(name), #group, #name, color, MicroProfileTokenTypeCpu)
//
//#define MICROPROFILE_DECLARE_GPU(var) extern MicroProfileToken g_mp_##var
//#define MICROPROFILE_DEFINE_GPU(var, group, name, color) MicroProfileToken g_mp_##var = MicroProfileGetToken(group, name, color, MicroProfileTokenTypeGpu)
//#define MICROPROFILE_TOKEN_PASTE0(a, b) a ## b
//#define MICROPROFILE_TOKEN_PASTE(a, b)  MICROPROFILE_TOKEN_PASTE0(a,b)
//#define MICROPROFILE_SCOPE(var) ProfilerScopeHandler MICROPROFILE_TOKEN_PASTE(foo, __LINE__)(g_mp_##var)
//#define MICROPROFILE_SCOPE_TOKEN(token) ProfilerScopeHandler MICROPROFILE_TOKEN_PASTE(foo, __LINE__)(token)
//#define MICROPROFILE_SCOPEI(group, name, color) \
//    DECLARE_STAT(TEXT(#name), name, group, EStatDataType::ST_int64, true, true, FPlatformMemory::MCR_Invalid); \
//    static DEFINE_STAT(name); \
//    static MicroProfileToken MICROPROFILE_TOKEN_PASTE(g_mp, __LINE__) = ProfilerRegisterToken(GET_STATID(name), #group, #name, color, MicroProfileTokenTypeCpu); \
//    ProfilerScopeHandler MICROPROFILE_TOKEN_PASTE(foo,__LINE__)( MICROPROFILE_TOKEN_PASTE(g_mp,__LINE__))
//
//#define MICROPROFILE_SCOPEGPU(var) MicroProfileScopeGpuHandler MICROPROFILE_TOKEN_PASTE(foo, __LINE__)(g_mp_##var)
//#define MICROPROFILE_SCOPEGPUI(group, name, color) static MicroProfileToken MICROPROFILE_TOKEN_PASTE(g_mp,__LINE__) = MicroProfileGetToken(group, name, color,  MicroProfileTokenTypeGpu); MicroProfileScopeGpuHandler MICROPROFILE_TOKEN_PASTE(foo,__LINE__)( MICROPROFILE_TOKEN_PASTE(g_mp,__LINE__))
//#define MICROPROFILE_META_CPU(name, count) static MicroProfileToken MICROPROFILE_TOKEN_PASTE(g_mp_meta,__LINE__) = MicroProfileGetMetaToken(name); MicroProfileMetaUpdate(MICROPROFILE_TOKEN_PASTE(g_mp_meta,__LINE__), count, MicroProfileTokenTypeCpu)
//#define MICROPROFILE_META_GPU(name, count) static MicroProfileToken MICROPROFILE_TOKEN_PASTE(g_mp_meta,__LINE__) = MicroProfileGetMetaToken(name); MicroProfileMetaUpdate(MICROPROFILE_TOKEN_PASTE(g_mp_meta,__LINE__), count, MicroProfileTokenTypeGpu)
//
//TMap<uint64_t, TStatId>& GetTokenToTStatIdMap();
//uint64_t ProfilerRegisterToken(TStatId statId, const char* sGroup, const char* sName, uint32_t nColor, int32 type);
//struct ProfilerScopeHandler : public FCycleCounter
//{
//    ProfilerScopeHandler(uint64_t token)
//    {
//        //check(GetTokenToTStatIdMap().Contains(token)); // unregistered stats, please use MICROPROFILE_DECLARE/MICROPROFILE_DEFINE
//        TStatId statId = GetTokenToTStatIdMap().FindRef(token);
//        Start(statId);
//    }
//    ~ProfilerScopeHandler()
//    {
//        Stop();
//    }
//};
//MICROPROFILE_DECLARE_GROUP(MicroProfile)
//
//#include "microprofile.h"
//
//void ProfilerConvertStats();
//
//
