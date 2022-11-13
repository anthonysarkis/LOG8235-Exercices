//#pragma once
//
//#define MICRO_PROFILE_IMPL
//#include "profiler.h"
//
//#include "SoftDesignTraining.h"
//#include "Stats/Stats2.h"
//#include "StatsData.h"
//
//#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib, "winmm.lib")
//
//static TMap<ThreadIdType, MicroProfileThreadLog*> g_MicroProfileThreadInfo;
//
//TMap<uint64_t, TStatId>& GetTokenToTStatIdMap()
//{
//    static TMap<uint64_t, TStatId> s_TokenToTStatId;
//    return s_TokenToTStatId;
//}
//
//TMap<NAME_INDEX, MicroProfileToken>& GetIndexToTokenMap()
//{
//    static TMap<NAME_INDEX, MicroProfileToken> s_IndexToToken;
//    return s_IndexToToken;
//}
//
//uint64_t ProfilerRegisterToken(TStatId statId, const char* sGroup, const char* sName, uint32_t nColor, int32 type)
//{
//    MicroProfileToken nToken = MicroProfileGetToken(sGroup, sName, nColor, (MicroProfileTokenType)type);
//    NAME_INDEX index = statId.GetName().GetComparisonIndex();
//    TMap<NAME_INDEX, MicroProfileToken>& indexToToken = GetIndexToTokenMap();
//    indexToToken.Add(index, nToken);
//    TMap<uint64_t, TStatId > &tokenToTStatId = GetTokenToTStatIdMap();
//    tokenToTStatId.Add(nToken, statId);
//    return nToken;
//}
//
//void ProfilerConvertStats()
//{
//    FStatsThreadState& Stats = FStatsThreadState::GetLocalState();
//    int64 Latest = Stats.GetLatestValidFrame();
//    if (Latest < 0)
//        return;
//    TMap<NAME_INDEX, MicroProfileToken>& indexToToken = GetIndexToTokenMap();
//    const FStatPacketArray& FrameStats = Stats.GetStatPacketArray(Latest);
//
//    int64 lastCycleGameScopeEnd = 0;
//    for (int32 PacketIndex = 0; PacketIndex < FrameStats.Packets.Num(); PacketIndex++)
//    {
//        FStatPacket const& Packet = *FrameStats.Packets[PacketIndex];
//
//        // Too confusing... skipping the worker threads, nothing interesting anyway...
//       if (Packet.ThreadType == EThreadType::Other)
//           continue;
//
//        const char* threadType = Packet.ThreadType == EThreadType::Game ? "Game" : (Packet.ThreadType == EThreadType::Renderer ? "Renderer" : "Other");
//
//        MicroProfileThreadLog* pLog;
//        MicroProfileThreadLog** ppLog = g_MicroProfileThreadInfo.Find(Packet.ThreadId);
//        if (ppLog == nullptr)
//        {
//            char threadName[64];
//            sprintf(threadName, "%s_%d", threadType, Packet.ThreadId);
//            pLog = MicroProfileCreateThreadLog(threadName);
//            pLog->nThreadId = Packet.ThreadId;
//            g_MicroProfileThreadInfo.Add(Packet.ThreadId, pLog);
//        }
//        else
//        {
//            pLog = *ppLog;
//        }
//        const FStatMessagesArray& Data = Packet.StatMessages;
//        for (int32 Index = 0; Index < Data.Num(); Index++)
//        {
//            FStatMessage const& Item = Data[Index];
//            EStatOperation::Type Op = Item.NameAndInfo.GetField<EStatOperation>();
//
//            MicroProfileToken nToken;
//            NAME_INDEX index = Item.NameAndInfo.GetRawName().GetComparisonIndex();
//            MicroProfileToken* pToken = indexToToken.Find(index);
//            if (pToken == nullptr)
//            {
//                const char* name = Item.NameAndInfo.GetShortName().GetPlainANSIString();
//                if (strncmp("STAT_", name, 5) == 0)
//                    name += 5;
//                // Too many groups for microprofiler :(
////                 const char* group = Item.NameAndInfo.GetGroupName().GetPlainANSIString(); 
////                 if (strncmp("STATGROUP_", group, 10) == 0)
////                     group += 10;
//                const char* group = Packet.ThreadType == EThreadType::Game ? "UnrealGame" : (Packet.ThreadType == EThreadType::Renderer ? "UnrealRenderer" : "UnrealOther");
//                nToken = MicroProfileGetToken(group, name, 0xff3399ff, MicroProfileTokenTypeCpu);
//                indexToToken.Add(index, nToken);
//            }
//            else
//            {
//                nToken = *pToken;
//            }
//
//
//            switch (Op)
//            {
//                case EStatOperation::CycleScopeStart:
//                {
//                    check(Item.NameAndInfo.GetFlag(EStatMetaFlags::IsCycle));
//                    if (Packet.ThreadType == EThreadType::Game)
//                        lastCycleGameScopeEnd = 0;
//                    MicroProfileLogPut(nToken, Item.GetValue_int64(), MP_LOG_ENTER, pLog);
//                    break;
//                }
//                case EStatOperation::CycleScopeEnd:
//                {
//                    check(Item.NameAndInfo.GetFlag(EStatMetaFlags::IsCycle));
//                    int64 val = Item.GetValue_int64();
//                    if (Packet.ThreadType == EThreadType::Game)
//                        lastCycleGameScopeEnd = val;
//                    MicroProfileLogPut(nToken, val, MP_LOG_LEAVE, pLog);
//                    break;
//                }
//            }
//        }
//    }
//
//    MicroProfileFlip(lastCycleGameScopeEnd);
//}
//
//
//
