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

    // Audio Ring Buffer
    const size_t AUDIO_CAPACITY = 1024 * 64;
    std::vector<int16_t> audioRingBuffer;
    size_t audioWriteIndex = 0;
    size_t audioReadIndex = 0;
    size_t audioBufferSize = 0;
    std::mutex audioMutex;

    void checkSaveRAM();
    int sramSaveInterval = 600;
    int sramSaveCounter = 0;
    shared_ptr<ByteArray> lastSramData = nullptr;
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

    struct Cheat {
        string description;
        string code;
        bool enabled = false;
    };
    vector<Cheat> cheats;
    void loadCheats();
    void applyCheats();

    void updateCheatMenu();
    shared_ptr<BobMenu> cheatMenu = nullptr;

    int currentSaveSlot = 0;

    bool videoFilterLinear = false;
    void resetGame();

    // Input mapping
    // We map Libretro ID (e.g. RETRO_DEVICE_ID_JOYPAD_B) to our internal ID
    // 0=B, 1=Y, 2=Select, 3=Start, 4=Up, 5=Down, 6=Left, 7=Right, 8=A, 9=X, 10=L, 11=R
    std::map<unsigned, int> inputMap;
    void initDefaultControls();
    void updateControlsMenu();
    shared_ptr<BobMenu> controlsMenu = nullptr;
    bool waitingForInput = false;
    unsigned remapId = 0;

    // Check input for remapping
    int checkInput();

    string getCoreName();
    void loadInputMap();
    void saveInputMap();

    int aspectRatioMode = 0; // 0=Core, 1=4:3, 2=16:9, 3=Stretch
    float coreAspectRatio = 0.0f;

    // Rewind support
    std::deque<shared_ptr<ByteArray>> rewindBuffer;
    int rewindInterval = 5; // Frames between saves
    int rewindCounter = 0;
    bool rewinding = false;
    void doRewind();
    void pushRewindState();

    // Core Associations
    std::map<string, string> coreAssociations;
    void loadAssociations();
    void saveAssociations();
    string pendingGamePath = "";
    bool askingToAssociate = false;
    shared_ptr<BobMenu> associationMenu = nullptr;
    void updateAssociationMenu();
    string loadedCorePath = "";

    // Recent Games
    vector<string> recentGames;
    void loadRecentGames();
    void saveRecentGames();
    void addRecentGame(const string& path);
    shared_ptr<BobMenu> recentMenu = nullptr;
    void updateRecentMenu();

    // CRT Shader
    bool crtShaderEnabled = false;
    GLuint crtProgram = 0;
    void initShaders();
    void compileShader(GLuint& program, const string& vs, const string& fs);

    // HQ2X
    bool hq2xEnabled = false;
    HQ2X hq2xScaler;
    shared_ptr<ByteArray> hq2xBuffer = nullptr;
    shared_ptr<BobTexture> hq2xTexture = nullptr;

    // Save State Thumbnails
    shared_ptr<BobTexture> stateThumbnail = nullptr;
    int lastThumbnailSlot = -1;
    void updateStateThumbnail();

    shared_ptr<ByteArray> lastFrameData = nullptr;
    int lastFrameWidth = 0;
    int lastFrameHeight = 0;
};
