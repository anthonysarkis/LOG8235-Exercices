// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SoftDesignTraining.h"
//#include "profiler.h"
#include "Stats/Stats.h"

static FDelegateHandle WorldTickStartHandle;
static FDelegateHandle WorldCleanupHandle;
static FDelegateHandle WorldLogFrameStatsHandle;

void SoftDesignTrainingModuleImpl::StartupModule()
{
    // TODO: Re-enable Microprofile ?
	//MicroProfileSetEnableAllGroups(true);
	//MicroProfileSetForceMetaCounters(true);
    //WorldTickStartHandle = FWorldDelegates::OnWorldTickStart.AddStatic(WorldTickStart);

    WorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddStatic(WorldCleanup);
}

void SoftDesignTrainingModuleImpl::WorldTickStart(ELevelTick, float)
{

}

void SoftDesignTrainingModuleImpl::WorldBeginPlay()
{
    // TODO: Re-enable Microprofile ?
    //MicroProfileSetForceEnable(true);
    StatsMasterEnableAdd();

    // TODO: Re-enable Microprofile ?
    //FStatsThreadState& Stats = FStatsThreadState::GetLocalState();
    //WorldLogFrameStatsHandle = Stats.NewFrameDelegate.AddStatic(WorldLogFrameStats);
}

void SoftDesignTrainingModuleImpl::WorldLogFrameStats(int64 Frame)
{
    // TODO: Re-enable Microprofile ?
    //if (!MicroProfileGetForceEnable())
    //    return;

    //ProfilerConvertStats();
}

void SoftDesignTrainingModuleImpl::WorldCleanup(UWorld*, bool, bool)
{
    // TODO: Re-enable Microprofile ?
    //if (!MicroProfileGetForceEnable())
    //    return;

    //StatsMasterEnableSubtract();
    //MicroProfileSetForceEnable(false);

    //FStatsThreadState& Stats = FStatsThreadState::GetLocalState();
    //Stats.NewFrameDelegate.Remove(WorldLogFrameStatsHandle);

}

void SoftDesignTrainingModuleImpl::ShutdownModule()
{
    // TODO: Re-enable Microprofile ?
	//MicroProfileShutdown();
    //FWorldDelegates::OnWorldTickStart.Remove(WorldTickStartHandle);

    FWorldDelegates::OnWorldCleanup.Remove(WorldCleanupHandle);
}


IMPLEMENT_PRIMARY_GAME_MODULE(SoftDesignTrainingModuleImpl, SoftDesignTraining, "SoftDesignTraining");

DEFINE_LOG_CATEGORY(LogSoftDesignTraining)
 