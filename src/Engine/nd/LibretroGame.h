//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------

#pragma once
#include "NDGameEngine.h"
#include "libretro.h"
#include <mutex>

#ifdef __WINDOWS__
#include <windows.h>
#else
#include <dlfcn.h>
#endif

class LibretroGame : public NDGameEngine
{
private:
    typedef NDGameEngine super;
public:
    static LibretroGame* instance;

    LibretroGame(ND* nD);
    virtual ~LibretroGame();

    virtual void init() override;
    virtual void update() override;
    virtual void render() override;

    bool loadCore(const string& corePath);
    bool loadGame(const string& gamePath);

    virtual void titleMenuUpdate() override;

    shared_ptr<BobMenu> fileBrowserMenu = nullptr;
    string currentPath = "";
    bool selectingCore = false;

    void updateFileBrowser();

    void saveState();
    void loadState();

private:
    void* coreHandle = nullptr;

    // Core callbacks
    static bool retroEnvironment(unsigned cmd, void* data);
    static void retroVideoRefresh(const void* data, unsigned width, unsigned height, size_t pitch);
    static void retroAudioSample(int16_t left, int16_t right);
    static size_t retroAudioSampleBatch(const int16_t* data, size_t frames);
    static void retroInputPoll();
    static int16_t retroInputState(unsigned port, unsigned device, unsigned index, unsigned id);

    // Audio callback for SDL mixer
    static void audioCallback(void *udata, Uint8 *stream, int len);

    // Core API function pointers
    void (*retro_init)(void) = nullptr;
    void (*retro_deinit)(void) = nullptr;
    unsigned (*retro_api_version)(void) = nullptr;
    void (*retro_get_system_info)(struct retro_system_info* info) = nullptr;
    void (*retro_get_system_av_info)(struct retro_system_av_info* info) = nullptr;
    void (*retro_set_environment)(retro_environment_t) = nullptr;
    void (*retro_set_video_refresh)(retro_video_refresh_t) = nullptr;
    void (*retro_set_audio_sample)(retro_audio_sample_t) = nullptr;
    void (*retro_set_audio_sample_batch)(retro_audio_sample_batch_t) = nullptr;
    void (*retro_set_input_poll)(retro_input_poll_t) = nullptr;
    void (*retro_set_input_state)(retro_input_state_t) = nullptr;
    void (*retro_set_controller_port_device)(unsigned port, unsigned device) = nullptr;
    void (*retro_reset)(void) = nullptr;
    void (*retro_run)(void) = nullptr;
    size_t (*retro_serialize_size)(void) = nullptr;
    bool (*retro_serialize)(void* data, size_t size) = nullptr;
    bool (*retro_unserialize)(const void* data, size_t size) = nullptr;
    void (*retro_cheat_reset)(void) = nullptr;
    void (*retro_cheat_set)(unsigned index, bool enabled, const char* code) = nullptr;
    bool (*retro_load_game)(const struct retro_game_info* game) = nullptr;
    bool (*retro_load_game_special)(unsigned game_type, const struct retro_game_info* info, size_t num_info) = nullptr;
    void (*retro_unload_game)(void) = nullptr;
    unsigned (*retro_get_region)(void) = nullptr;
    void* (*retro_get_memory_data)(unsigned id) = nullptr;
    size_t (*retro_get_memory_size)(unsigned id) = nullptr;

    shared_ptr<BobTexture> videoTexture = nullptr;

    std::vector<int16_t> audioBuffer;
    std::mutex audioMutex;

    void checkSaveRAM();
    void loadSRAM();
    void saveSRAM();

    // Helper to get save path
    string getSavePath(const string& ext);

    struct CoreOption {
        string key;
        string description;
        vector<string> values;
        int currentIndex = 0;
    };
    vector<CoreOption> coreOptions;
    bool variablesUpdated = false;
    bool fastForward = false;

    void parseOptionString(const string& key, const string& value);
    void loadCoreOptions();
    void saveCoreOptions();

    void updateCoreOptionsMenu();
    shared_ptr<BobMenu> coreOptionsMenu = nullptr;
};
