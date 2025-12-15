#include "stdafx.h"
#include "LibretroGame.h"

LibretroGame* LibretroGame::instance = nullptr;

LibretroGame::LibretroGame(ND* nD) : NDGameEngine(nD)
{
    instance = this;
}

LibretroGame::~LibretroGame()
{
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
    // videoTexture = GLUtils::getTextureFromData("Libretro", av_info.geometry.base_width, av_info.geometry.base_height, ...);

    return true;
}

void LibretroGame::update()
{
    if (retro_run)
    {
        retro_run();
    }
}

void LibretroGame::render()
{
    if (videoTexture)
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
            return true;
        }
        // Handle other environment calls
    }
    return false;
}

void LibretroGame::retroVideoRefresh(const void* data, unsigned width, unsigned height, size_t pitch)
{
    if (instance && data)
    {
        // Update texture
        // Convert data to texture format if needed
        // Assuming XRGB8888 or RGB565 based on pixel format set

        // For prototype, create/update texture
        // instance->videoTexture = ...

        // This runs in the core loop, so be careful with OpenGL calls if threaded
    }
}

void LibretroGame::retroAudioSample(int16_t left, int16_t right)
{
    // Feed audio
}

size_t LibretroGame::retroAudioSampleBatch(const int16_t* data, size_t frames)
{
    // Feed audio batch
    return frames;
}

void LibretroGame::retroInputPoll()
{
    // Poll input
}

int16_t LibretroGame::retroInputState(unsigned port, unsigned device, unsigned index, unsigned id)
{
    // Return input state
    return 0;
}
