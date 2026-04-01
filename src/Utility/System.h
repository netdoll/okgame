//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


#pragma once
#include "bobtypes.h"
class Logger;

class ConsoleText;

class System
{
public:
	static Logger log;

	static string glVendor;
	static string glVersion;
	static string glRenderer;
	static string shaderVersion;
	static string glExtensions;

	static string bobVersion;

	static void initSystemInfo();
	static void initClockAndTimeZone();


	static ConsoleText* totalRendersText;
	static ConsoleText* totalUpdatesText;
	static ConsoleText* rendersPerSecondText;
	static ConsoleText* averageRendersPerSecondText;
	static ConsoleText* updatesPerSecondText;
	static ConsoleText* rendersSkippedText;
	static ConsoleText* updatesSkippedText;
	static ConsoleText* averageTicksPerFrameText;
	static ConsoleText* averageTicksPerUpdateText;
	static ConsoleText* onlineStatusText;

	static ConsoleText* upTimeText;

	const static int amtTicksTexts = 10;
	static ConsoleText* ticksText[amtTicksTexts];


private:
	static long long lastRenderTime;
	static long long lastUpdateTime;
	static int ticksTextCount;
	static int averageTicksPerRenderLastSecond;
	static int averageTicksPerUpdateLastSecond;


public:

	static ConsoleText* mxThreadCountText;

	static ConsoleText* memoryText;
	static ConsoleText* texturesLoadedText;
	static ConsoleText* textureBytesLoadedText;


	static int mb;

	static long long usedMemory;
	static long long maxUsedMemory;
	static long long totalMemory;
	static long long freeMemory;
	static long long maxMemory;

	ConsoleText* vramText = nullptr;

	static void initStats();
	static void updateStats();


private:
	static long long highResTimer;
	static long long highResolutionTicksPerSecond;
	static long long timerOffset;


	static int totalSecondsPassed;
	static int averageFPSTestSecondsPassed;
	static int averageRendersPerSecond;
	static int totalUpdates;
	static int totalRenders;
	static int averageFPSTestTotalRenders;
	static long long lastUpdateHighResTime; 
	static long long lastRenderHighResTime; 
	static long long currentHighResTime;

	
	static int updatesThisSecond;
	static int rendersThisSecond;
	static long long lastSecondTime;

	static long long totalUpdateTicks;
	static long long totalRenderTicks;
	static int rendersSkipped;
	static int updatesSkipped;
	


public:
	static int ticksPassedThisRender;
	static int ticksPassedThisUpdate;
	static double cpuFreq;
	static void initTimers();


	static void updateRenderTimers();
	static void updateUpdateTimers();


	static double getTicksBetweenTimes(long long start, long long finish);
	static long long currentHighResTimer();
	
	static int getTotalRenderTicksPassed();
	static void resetTotalRenderTicksPassed();



	static uint64_t GetPerformanceFrequency();

	static long long getPerformanceCounter();
};
