#include "stdafx.h"
#include "LibretroGame.h"
#include "src/Utility/ControlsManager.h"
#include "src/Utility/gl/GLUtils.h"
#include "src/Utility/audio/AudioManager.h"

LibretroGame* LibretroGame::instance = nullptr;

LibretroGame::LibretroGame(ND* nD) : NDGameEngine(nD)
{
    instance = this;
}

LibretroGame::~LibretroGame()
{
    Mix_HookMusic(NULL, NULL);
    if (coreHandle)
    {
        if (retro_deinit) retro_deinit();
#ifdef __WINDOWS__
        FreeLibrary((HMODULE)coreHandle);
#else
        dlclose(coreHandle);
#endif
    }
    if(instance == this) instance = nullptr;
}

void LibretroGame::init()
{
    super::init();
    titleMenuShowing = true;
    currentPath = Main::getPath();

    // Create system directory if it doesn't exist
    string systemDir = Main::getPath() + "/system";
    BobFile sd(systemDir);
    if (!sd.exists())
    {
        // mkdir? BobFile doesn't have mkdir, but let's assume it might be needed.
        // For now, we just point to it.
    }
}

void LibretroGame::titleMenuUpdate()
{
    if (titleMenu == nullptr)
    {
        titleMenu = make_shared<BobMenu>(this, "Emulator");
        titleMenu->add("Load Core...", "Load Core");
        titleMenu->add("Load Game...", "Load Game");
        titleMenu->add("Save State", "Save State");
        titleMenu->add("Load State", "Load State");
        titleMenu->add("Resume");
        titleMenu->add("Exit");
    }

    if (getControlsManager()->miniGame_UP_Pressed()) titleMenu->up();
    if (getControlsManager()->miniGame_DOWN_Pressed()) titleMenu->down();

    if (getControlsManager()->miniGame_CONFIRM_Pressed())
    {
        if (titleMenu->isSelectedID("Load Core"))
        {
            selectingCore = true;
            fileBrowserMenu = make_shared<BobMenu>(this, "Select Core");
            updateFileBrowser();
            titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("Load Game"))
        {
            selectingCore = false;
            fileBrowserMenu = make_shared<BobMenu>(this, "Select Game");
            updateFileBrowser();
            titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("Save State"))
        {
            saveState();
            titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("Load State"))
        {
            loadState();
            titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("Resume"))
        {
            if (retro_run) titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("Exit"))
        {
            // Exit logic (e.g. return to ND menu)
            // nD->setGame(nullptr); // ?
            titleMenuShowing = false; // Just hide for now
        }
    }
}

void LibretroGame::updateFileBrowser()
{
    if (fileBrowserMenu->menuItems->size() == 0)
    {
        fileBrowserMenu->add("..", "..");
        BobFile dir(currentPath);
        vector<string> files = dir.list();
        for (const string& f : files)
        {
            fileBrowserMenu->add(f, f);
        }
    }

    if (getControlsManager()->miniGame_UP_Pressed()) fileBrowserMenu->up();
    if (getControlsManager()->miniGame_DOWN_Pressed()) fileBrowserMenu->down();

    if (getControlsManager()->miniGame_CONFIRM_Pressed())
    {
        shared_ptr<BobMenu::MenuItem> item = fileBrowserMenu->getSelectedMenuItem();
        if (item)
        {
            string selected = item->id;
            if (selected == "..")
            {
                // Go up directory
                // naive implementation
                size_t pos = currentPath.find_last_of("/\\");
                if (pos != string::npos) currentPath = currentPath.substr(0, pos);
                fileBrowserMenu->clear();
            }
            else
            {
                string fullPath = currentPath + "/" + selected;
                BobFile f(fullPath);
                if (f.isDirectory())
                {
                    currentPath = fullPath;
                    fileBrowserMenu->clear();
                }
                else
                {
                    if (selectingCore)
                    {
                        if (loadCore(fullPath))
                        {
                            fileBrowserMenu = nullptr;
                            titleMenuShowing = true;
                        }
                    }
                    else
                    {
                        if (loadGame(fullPath))
                        {
                            fileBrowserMenu = nullptr;
                            titleMenuShowing = false; // Start game
                        }
                    }
                }
            }
        }
    }

    if (getControlsManager()->miniGame_CANCEL_Pressed())
    {
        fileBrowserMenu = nullptr;
        titleMenuShowing = true;

        // Ensure SRAM is saved when returning to menu or changing context
        if (!selectingCore) saveSRAM();
    }
}

bool LibretroGame::loadCore(const string& corePath)
{
    if (coreHandle)
    {
        if (retro_deinit) retro_deinit();
#ifdef __WINDOWS__
        FreeLibrary((HMODULE)coreHandle);
#else
        dlclose(coreHandle);
#endif
        coreHandle = nullptr;
    }

#ifdef __WINDOWS__
    coreHandle = LoadLibrary(corePath.c_str());
#else
    coreHandle = dlopen(corePath.c_str(), RTLD_LAZY);
#endif

    if (!coreHandle)
    {
        // Log error
        return false;
    }

#ifdef __WINDOWS__
    #define LOAD_SYM(x) x = (decltype(x))GetProcAddress((HMODULE)coreHandle, #x)
#else
    #define LOAD_SYM(x) x = (decltype(x))dlsym(coreHandle, #x)
#endif

    LOAD_SYM(retro_init);
    LOAD_SYM(retro_deinit);
    LOAD_SYM(retro_api_version);
    LOAD_SYM(retro_get_system_info);
    LOAD_SYM(retro_get_system_av_info);
    LOAD_SYM(retro_set_environment);
    LOAD_SYM(retro_set_video_refresh);
    LOAD_SYM(retro_set_audio_sample);
    LOAD_SYM(retro_set_audio_sample_batch);
    LOAD_SYM(retro_set_input_poll);
    LOAD_SYM(retro_set_input_state);
    LOAD_SYM(retro_set_controller_port_device);
    LOAD_SYM(retro_reset);
    LOAD_SYM(retro_run);
    LOAD_SYM(retro_serialize_size);
    LOAD_SYM(retro_serialize);
    LOAD_SYM(retro_unserialize);
    LOAD_SYM(retro_cheat_reset);
    LOAD_SYM(retro_cheat_set);
    LOAD_SYM(retro_load_game);
    LOAD_SYM(retro_load_game_special);
    LOAD_SYM(retro_unload_game);
    LOAD_SYM(retro_get_region);
    LOAD_SYM(retro_get_memory_data);
    LOAD_SYM(retro_get_memory_size);

    if (!retro_init) return false;

    retro_set_environment(retroEnvironment);
    retro_set_video_refresh(retroVideoRefresh);
    retro_set_audio_sample(retroAudioSample);
    retro_set_audio_sample_batch(retroAudioSampleBatch);
    retro_set_input_poll(retroInputPoll);
    retro_set_input_state(retroInputState);

    retro_init();

    return true;
}

bool LibretroGame::loadGame(const string& gamePath)
{
    if (!retro_load_game) return false;

    struct retro_game_info info = {0};
    info.path = gamePath.c_str();
    // Load content into memory if needed, or just pass path if core supports it
    // For now, assuming path loading or we need to load data
    // Usually we read the file into a buffer

    // Simple implementation: read file
    shared_ptr<ByteArray> data = FileUtils::loadByteFileFromExePath(gamePath);
    if(data)
    {
        info.data = data->data();
        info.size = data->size();
    }
    else
    {
        // If file not found or path relative issue, try absolute?
        // Let's rely on core loading from path if data is null, but many cores need data.
    }

    if (!retro_load_game(&info))
    {
        return false;
    }

    struct retro_system_av_info av_info;
    retro_get_system_av_info(&av_info);

    // Initialize videoTexture with av_info geometry
    shared_ptr<ByteArray> emptyData = make_shared<ByteArray>(av_info.geometry.base_width * av_info.geometry.base_height * 4);
    videoTexture = GLUtils::getTextureFromData("Libretro", av_info.geometry.base_width, av_info.geometry.base_height, emptyData.get());

    Mix_HookMusic(audioCallback, this);

    // Try to load SRAM
    loadSRAM();

    return true;
}

void LibretroGame::update()
{
    super::update(); // Handles titleMenu update

    if (fileBrowserMenu)
    {
        updateFileBrowser();
        return; // Don't run game while browsing
    }

    if (!titleMenuShowing)
    {
        if (retro_run)
        {
            retro_run();
            // Periodically check/save SRAM?
            // For now, let's just save on menu open or exit
        }
        else
        {
            titleMenuShowing = true;
        }
    }
}

void LibretroGame::render()
{
    if (titleMenuShowing && titleMenu)
    {
        titleMenu->render();
    }

    if (fileBrowserMenu)
    {
        fileBrowserMenu->render();
    }

    if (!titleMenuShowing && !fileBrowserMenu && videoTexture)
    {
        // Draw video texture to screen
        // Use GLUtils to draw
        GLUtils::drawTexture(videoTexture.get(), 0, 0, 1.0f, GLUtils::FILTER_NEAREST);
    }
}

bool LibretroGame::retroEnvironment(unsigned cmd, void* data)
{
    switch (cmd)
    {
        case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
        {
            struct retro_log_callback *cb = (struct retro_log_callback *)data;
            // cb->log = ...; // Implement log callback
            break;
        }
        case RETRO_ENVIRONMENT_GET_CAN_DUPE:
            *(bool*)data = true;
            return true;
        case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
        {
            const enum retro_pixel_format *fmt = (enum retro_pixel_format *)data;
            // Handle pixel format
            // For now assuming it is compatible with RGBA/BGRA
            return true;
        }
        case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
        {
            const char **dir = (const char **)data;
            static string sysDir = Main::getPath() + "/system";
            *dir = sysDir.c_str();
            return true;
        }
        case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
        {
            const char **dir = (const char **)data;
            // Use same directory as ROM? or a "saves" dir?
            // Let's use current ROM directory for now or just return null to indicate "same as ROM"
            *dir = NULL;
            return true;
        }
        // Handle other environment calls
    }
    return false;
}

void LibretroGame::retroVideoRefresh(const void* data, unsigned width, unsigned height, size_t pitch)
{
    if (instance && data && instance->videoTexture)
    {
       GLUtils::updateTexture(instance->videoTexture, 0, 0, width, height, (u8*)data);
    }
}

void LibretroGame::retroAudioSample(int16_t left, int16_t right)
{
    if (instance)
    {
        std::lock_guard<std::mutex> lock(instance->audioMutex);
        instance->audioBuffer.push_back(left);
        instance->audioBuffer.push_back(right);
        if (instance->audioBuffer.size() > 44100 * 2) {
             instance->audioBuffer.clear();
        }
    }
}

size_t LibretroGame::retroAudioSampleBatch(const int16_t* data, size_t frames)
{
    if (instance)
    {
        std::lock_guard<std::mutex> lock(instance->audioMutex);
        instance->audioBuffer.insert(instance->audioBuffer.end(), data, data + frames * 2);
        if (instance->audioBuffer.size() > 44100 * 2) {
             instance->audioBuffer.clear();
        }
    }
    return frames;
}

void LibretroGame::retroInputPoll()
{
    // Poll input
}

int16_t LibretroGame::retroInputState(unsigned port, unsigned device, unsigned index, unsigned id)
{
    if (port == 0 && device == RETRO_DEVICE_JOYPAD && instance)
    {
        shared_ptr<ControlsManager> cm = instance->getControlsManager();
        if(!cm) return 0;

        switch(id)
        {
            case RETRO_DEVICE_ID_JOYPAD_B: return cm->MINIGAME_ACTION_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_Y: return cm->MINIGAME_RUN_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_SELECT: return cm->MINIGAME_SELECT_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_START: return cm->MINIGAME_START_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_UP: return cm->MINIGAME_UP_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_DOWN: return cm->MINIGAME_DOWN_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_LEFT: return cm->MINIGAME_LEFT_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_RIGHT: return cm->MINIGAME_RIGHT_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_A: return cm->MINIGAME_A_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_X: return cm->MINIGAME_X_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_L: return cm->MINIGAME_L_HELD ? 1 : 0;
            case RETRO_DEVICE_ID_JOYPAD_R: return cm->MINIGAME_R_HELD ? 1 : 0;
        }
    }
    return 0;
}

void LibretroGame::audioCallback(void *udata, Uint8 *stream, int len)
{
    LibretroGame* self = (LibretroGame*)udata;
    if (!self) return;

    memset(stream, 0, len);

    std::lock_guard<std::mutex> lock(self->audioMutex);

    size_t bytesNeeded = len;
    size_t bytesAvailable = self->audioBuffer.size() * sizeof(int16_t);

    size_t bytesToCopy = (bytesAvailable < bytesNeeded) ? bytesAvailable : bytesNeeded;

    if (bytesToCopy > 0)
    {
        memcpy(stream, self->audioBuffer.data(), bytesToCopy);
        size_t samplesConsumed = bytesToCopy / sizeof(int16_t);
        self->audioBuffer.erase(self->audioBuffer.begin(), self->audioBuffer.begin() + samplesConsumed);
    }
}

void LibretroGame::saveState()
{
    if (!retro_serialize_size || !retro_serialize) return;

    size_t size = retro_serialize_size();
    if(size == 0) return;

    shared_ptr<ByteArray> data = make_shared<ByteArray>(size);
    if (retro_serialize(data->data(), size))
    {
        // Save to file
        string path = currentPath + ".state";
        FileUtils::saveByteFile(path, data);
        log.info("Saved state to " + path);
    }
    else
    {
        log.error("Failed to serialize state");
    }
}

void LibretroGame::loadState()
{
    if (!retro_serialize_size || !retro_unserialize) return;

    string path = currentPath + ".state";
    shared_ptr<ByteArray> data = FileUtils::loadByteFileFromExePath(path);

    if (data && data->size() > 0)
    {
        if (retro_unserialize(data->data(), data->size()))
        {
            log.info("Loaded state from " + path);
        }
        else
        {
            log.error("Failed to unserialize state");
        }
    }
    else
    {
        log.error("State file not found: " + path);
    }
}

string LibretroGame::getSavePath(const string& ext)
{
    // If currentPath is "/path/to/game.gb", return "/path/to/game.srm"
    size_t lastDot = currentPath.find_last_of(".");
    if (lastDot != string::npos)
    {
        return currentPath.substr(0, lastDot) + "." + ext;
    }
    return currentPath + "." + ext;
}

void LibretroGame::checkSaveRAM()
{
    // Usually called every frame or periodically.
    // However, since we are implementing manual saveSRAM called on pause/exit,
    // we might not need this unless we want auto-save.
    // Some cores expose a dirty flag if we implement RETRO_ENVIRONMENT_GET_VARIABLE
    // but standard SRAM doesn't always have dirty flags.
}

void LibretroGame::saveSRAM()
{
    if (!retro_get_memory_data || !retro_get_memory_size) return;

    size_t size = retro_get_memory_size(RETRO_MEMORY_SAVE_RAM);
    void* data = retro_get_memory_data(RETRO_MEMORY_SAVE_RAM);

    if (size > 0 && data)
    {
        string path = getSavePath("srm");
        // We need a way to save raw bytes.
        // copy to ByteArray
        shared_ptr<ByteArray> ba = make_shared<ByteArray>(size);
        memcpy(ba->data(), data, size);
        FileUtils::saveByteFile(path, ba);
        log.info("Saved SRAM to " + path);
    }
}

void LibretroGame::loadSRAM()
{
    if (!retro_get_memory_data || !retro_get_memory_size) return;

    size_t size = retro_get_memory_size(RETRO_MEMORY_SAVE_RAM);
    void* ptr = retro_get_memory_data(RETRO_MEMORY_SAVE_RAM);

    if (size > 0 && ptr)
    {
        string path = getSavePath("srm");
        shared_ptr<ByteArray> data = FileUtils::loadByteFileFromExePath(path);

        if (data && data->size() == size)
        {
            memcpy(ptr, data->data(), size);
            log.info("Loaded SRAM from " + path);
        }
        else if (data)
        {
             log.warn("SRAM file size mismatch. Expected " + to_string(size) + " got " + to_string(data->size()));
             // Try to copy partial? Or ignore. Safest to ignore to avoid corruption, or copy min.
             size_t copySize = (data->size() < size) ? data->size() : size;
             memcpy(ptr, data->data(), copySize);
        }
    }
}
