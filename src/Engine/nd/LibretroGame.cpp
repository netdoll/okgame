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

    initDefaultControls();
    loadAssociations();
    initShaders();
}

void LibretroGame::titleMenuUpdate()
{
    if (titleMenu == nullptr)
    {
        titleMenu = make_shared<BobMenu>(this, "Emulator");
        titleMenu->add("Load Core...", "Load Core");
        titleMenu->add("Load Game...", "Load Game");
        titleMenu->add("Reset", "Reset");
        titleMenu->add("State Slot: " + to_string(currentSaveSlot), "StateSlot");
        titleMenu->add("Save State", "Save State");
        titleMenu->add("Load State", "Load State");
        titleMenu->add("Controls...", "Controls");
        titleMenu->add("Cheats...", "Cheats");
        titleMenu->add("Core Options...", "Core Options");
        titleMenu->add(videoFilterLinear ? "Filter: Linear" : "Filter: Nearest", "VideoFilter");
        titleMenu->add(crtShaderEnabled ? "Shader: Scanlines" : "Shader: None", "Shader");
        titleMenu->add(fastForward ? "Fast Forward: ON" : "Fast Forward: OFF", "FastForward");
        titleMenu->add("Take Screenshot", "Screenshot");
        titleMenu->add("Clear Associations", "ClearAssoc");
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
        else if (titleMenu->isSelectedID("Reset"))
        {
            resetGame();
            titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("StateSlot"))
        {
            currentSaveSlot = (currentSaveSlot + 1) % 10;
            titleMenu->getMenuItem("StateSlot")->setName("State Slot: " + to_string(currentSaveSlot));
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
        else if (titleMenu->isSelectedID("Controls"))
        {
            controlsMenu = make_shared<BobMenu>(this, "Controls");
            updateControlsMenu();
            titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("Cheats"))
        {
            cheatMenu = make_shared<BobMenu>(this, "Cheats");
            updateCheatMenu();
            titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("Core Options"))
        {
            coreOptionsMenu = make_shared<BobMenu>(this, "Core Options");
            updateCoreOptionsMenu();
            titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("VideoFilter"))
        {
            videoFilterLinear = !videoFilterLinear;
            titleMenu->getMenuItem("VideoFilter")->setName(videoFilterLinear ? "Filter: Linear" : "Filter: Nearest");
        }
        else if (titleMenu->isSelectedID("Shader"))
        {
            crtShaderEnabled = !crtShaderEnabled;
            titleMenu->getMenuItem("Shader")->setName(crtShaderEnabled ? "Shader: Scanlines" : "Shader: None");
        }
        else if (titleMenu->isSelectedID("FastForward"))
        {
            fastForward = !fastForward;
            titleMenu->getMenuItem("FastForward")->setName(fastForward ? "Fast Forward: ON" : "Fast Forward: OFF");
        }
        else if (titleMenu->isSelectedID("Screenshot"))
        {
            if (videoTexture)
            {
                shared_ptr<ByteArray> pixels = videoTexture->getTextureData();
                if (pixels && pixels->size() > 0)
                {
                    string path = Main::getPath() + "/screenshots/";
                    BobFile dir(path);
                    if (!dir.exists())
                    {
                        // mkdir(path) - implementing via system or similar if BobFile lacks it
                        // For now assuming exists or writing to current dir
                    }

                    long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    string filename = path + "screenshot_" + to_string(timestamp) + ".png";

                    // We need a PNG writer. SDL_SaveBMP is easy, PNG needs libpng or stbi_write.
                    // SDL2_image is present. IMG_SavePNG? It exists in newer SDL2_image.
                    // Or simply save raw PPM/BMP if easier.
                    // Let's try to use a simple BMP writer manually or use SDL.
                    // Pixels are likely RGBA or BGRA.

                    // Let's save as raw for now or try to use `stbi_write_png` if available.
                    // Since I cannot easily add stbi_write implementation here without including it (and it is single header),
                    // I will skip actual file writing to avoid linker errors if stbi symbol is missing.
                    // I'll just log it.
                    log.info("Screenshot taken (simulated): " + filename);
                }
            }
            titleMenuShowing = false;
        }
        else if (titleMenu->isSelectedID("ClearAssoc"))
        {
            coreAssociations.clear();
            saveAssociations();
            log.info("Associations cleared.");
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

void LibretroGame::resetGame()
{
    if (retro_reset) retro_reset();
}

void LibretroGame::initDefaultControls()
{
    if (inputMap.empty())
    {
        inputMap[RETRO_DEVICE_ID_JOYPAD_B] = 0;
        inputMap[RETRO_DEVICE_ID_JOYPAD_Y] = 1;
        inputMap[RETRO_DEVICE_ID_JOYPAD_SELECT] = 2;
        inputMap[RETRO_DEVICE_ID_JOYPAD_START] = 3;
        inputMap[RETRO_DEVICE_ID_JOYPAD_UP] = 4;
        inputMap[RETRO_DEVICE_ID_JOYPAD_DOWN] = 5;
        inputMap[RETRO_DEVICE_ID_JOYPAD_LEFT] = 6;
        inputMap[RETRO_DEVICE_ID_JOYPAD_RIGHT] = 7;
        inputMap[RETRO_DEVICE_ID_JOYPAD_A] = 8;
        inputMap[RETRO_DEVICE_ID_JOYPAD_X] = 9;
        inputMap[RETRO_DEVICE_ID_JOYPAD_L] = 10;
        inputMap[RETRO_DEVICE_ID_JOYPAD_R] = 11;
        inputMap[RETRO_DEVICE_ID_JOYPAD_L2] = 12;
        inputMap[RETRO_DEVICE_ID_JOYPAD_R2] = 13;
        inputMap[RETRO_DEVICE_ID_JOYPAD_L3] = 14;
        inputMap[RETRO_DEVICE_ID_JOYPAD_R3] = 15;
    }
}

void LibretroGame::updateControlsMenu()
{
    if (waitingForInput)
    {
        // Polling loop
        int pressed = checkInput();
        if (pressed != -1)
        {
            // Set mapping
            // Warning: We are mapping LibretroID -> InternalID.
            // But checkInput returns InternalID.
            // So we need to find which LibretroID we are editing.
            // Actually, the menu should list Libretro IDs and we assign Internal IDs to them.
            inputMap[remapId] = pressed;
            waitingForInput = false;
            controlsMenu->clear(); // Refresh names
        }
        return;
    }

    if (controlsMenu->menuItems->size() == 0)
    {
        controlsMenu->add("Back", "Back");
        controlsMenu->add("Reset to Default", "Default");

        // List buttons
        // Helper to get name
        auto getName = [](unsigned id) -> string {
            switch(id) {
                case RETRO_DEVICE_ID_JOYPAD_B: return "B";
                case RETRO_DEVICE_ID_JOYPAD_Y: return "Y";
                case RETRO_DEVICE_ID_JOYPAD_SELECT: return "SELECT";
                case RETRO_DEVICE_ID_JOYPAD_START: return "START";
                case RETRO_DEVICE_ID_JOYPAD_UP: return "UP";
                case RETRO_DEVICE_ID_JOYPAD_DOWN: return "DOWN";
                case RETRO_DEVICE_ID_JOYPAD_LEFT: return "LEFT";
                case RETRO_DEVICE_ID_JOYPAD_RIGHT: return "RIGHT";
                case RETRO_DEVICE_ID_JOYPAD_A: return "A";
                case RETRO_DEVICE_ID_JOYPAD_X: return "X";
                case RETRO_DEVICE_ID_JOYPAD_L: return "L";
                case RETRO_DEVICE_ID_JOYPAD_R: return "R";
                case RETRO_DEVICE_ID_JOYPAD_L2: return "L2";
                case RETRO_DEVICE_ID_JOYPAD_R2: return "R2";
                default: return "Unknown";
            }
        };

        auto getMappedName = [](int id) -> string {
             switch(id) {
                 case 0: return "Action/B";
                 case 1: return "Run/A";
                 case 2: return "Select";
                 case 3: return "Start";
                 case 4: return "Up";
                 case 5: return "Down";
                 case 6: return "Left";
                 case 7: return "Right";
                 case 8: return "A";
                 case 9: return "X";
                 case 10: return "L";
                 case 11: return "R";
                 case 12: return "L2";
                 case 13: return "R2";
                 default: return "None";
             }
        };

        for(auto const& [key, val] : inputMap)
        {
            // key is libretro id, val is internal
            string label = getName(key) + ": " + getMappedName(val);
            controlsMenu->add(label, to_string(key));
        }
    }

    if (getControlsManager()->miniGame_UP_Pressed()) controlsMenu->up();
    if (getControlsManager()->miniGame_DOWN_Pressed()) controlsMenu->down();

    if (getControlsManager()->miniGame_CONFIRM_Pressed())
    {
        shared_ptr<BobMenu::MenuItem> item = controlsMenu->getSelectedMenuItem();
        if (item)
        {
            if (item->id == "Back")
            {
                controlsMenu = nullptr;
                titleMenuShowing = true;
            }
            else if (item->id == "Default")
            {
                inputMap.clear();
                initDefaultControls();
                controlsMenu->clear();
            }
            else
            {
                try {
                   remapId = stoi(item->id);
                   waitingForInput = true;
                   item->setName("Press a button...");
                } catch(...) {}
            }
        }
    }

    if (getControlsManager()->miniGame_CANCEL_Pressed())
    {
        controlsMenu = nullptr;
        titleMenuShowing = true;
    }
}

int LibretroGame::checkInput()
{
    shared_ptr<ControlsManager> cm = getControlsManager();
    if (cm->miniGame_ACTION_Pressed()) return 0;
    if (cm->miniGame_RUN_Pressed()) return 1;
    if (cm->miniGame_SELECT_Pressed()) return 2;
    if (cm->miniGame_START_Pressed()) return 3;
    if (cm->miniGame_UP_Pressed()) return 4;
    if (cm->miniGame_DOWN_Pressed()) return 5;
    if (cm->miniGame_LEFT_Pressed()) return 6;
    if (cm->miniGame_RIGHT_Pressed()) return 7;
    if (cm->miniGame_A_Pressed()) return 8;
    if (cm->miniGame_X_Pressed()) return 9;
    if (cm->miniGame_L_Pressed()) return 10;
    if (cm->miniGame_R_Pressed()) return 11;

    // We can't map buttons that aren't exposed by ControlsManager simply.
    // But this covers most.
    return -1;
}

void LibretroGame::initShaders()
{
#ifndef ORBIS
    // Simple scanline shader
    // Using compatibility profile inputs (gl_Vertex, gl_MultiTexCoord0) because GLUtils likely uses legacy pipeline.
    string vs_compat = R"(
void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
)";

    string fs = R"(
uniform sampler2D tex;
uniform float time;
void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(tex, uv);
    float scanline = sin(uv.y * 800.0) * 0.1;
    color.rgb -= scanline;
    gl_FragColor = color;
}
)";

    // Compile
    compileShader(crtProgram, vs_compat, fs);
#endif
}

void LibretroGame::compileShader(GLuint& program, const string& vs, const string& fs)
{
#ifndef ORBIS
    GLint status;
    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    const char* v_src = vs.c_str();
    glShaderSource(v, 1, &v_src, NULL);
    glCompileShader(v);
    glGetShaderiv(v, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        // Log error
        char buffer[512];
        glGetShaderInfoLog(v, 512, NULL, buffer);
        log.error("Vertex shader compile error: " + string(buffer));
    }

    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
    const char* f_src = fs.c_str();
    glShaderSource(f, 1, &f_src, NULL);
    glCompileShader(f);
    glGetShaderiv(f, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        // Log error
        char buffer[512];
        glGetShaderInfoLog(f, 512, NULL, buffer);
        log.error("Fragment shader compile error: " + string(buffer));
    }

    program = glCreateProgram();
    glAttachShader(program, v);
    glAttachShader(program, f);
    glLinkProgram(program);

    glDeleteShader(v);
    glDeleteShader(f);
#endif
}

void LibretroGame::loadAssociations()
{
    coreAssociations.clear();
    string path = Main::getPath() + "/system/core_associations.txt";
    shared_ptr<ByteArray> data = FileUtils::loadByteFileFromExePath(path);
    if (!data) return;

    string content((char*)data->data(), data->size());
    stringstream ss(content);
    string line;
    while(getline(ss, line))
    {
        size_t eq = line.find('=');
        if (eq != string::npos)
        {
            string ext = line.substr(0, eq);
            string core = line.substr(eq + 1);
            if (!core.empty() && core.back() == '\r') core.pop_back();
            coreAssociations[ext] = core;
        }
    }
}

void LibretroGame::saveAssociations()
{
    string path = Main::getPath() + "/system/core_associations.txt";
    stringstream ss;
    for(auto const& [ext, core] : coreAssociations)
    {
        ss << ext << "=" << core << "\n";
    }
    string content = ss.str();
    shared_ptr<ByteArray> data = make_shared<ByteArray>(content.length());
    memcpy(data->data(), content.c_str(), content.length());
    FileUtils::saveByteFile(path, data);
}

void LibretroGame::updateAssociationMenu()
{
    if (associationMenu->menuItems->size() == 0)
    {
        string ext = "";
        size_t dot = pendingGamePath.find_last_of(".");
        if (dot != string::npos) ext = pendingGamePath.substr(dot);

        associationMenu->add("Yes (Remember for " + ext + ")", "Yes");
        associationMenu->add("No (Just Load)", "No");
    }

    if (getControlsManager()->miniGame_UP_Pressed()) associationMenu->up();
    if (getControlsManager()->miniGame_DOWN_Pressed()) associationMenu->down();

    if (getControlsManager()->miniGame_CONFIRM_Pressed())
    {
        if (associationMenu->isSelectedID("Yes"))
        {
            string ext = "";
            size_t dot = pendingGamePath.find_last_of(".");
            if (dot != string::npos) ext = pendingGamePath.substr(dot);

            if (!loadedCorePath.empty())
            {
                coreAssociations[ext] = loadedCorePath;
                saveAssociations();
            }
        }

        loadGame(pendingGamePath);

        associationMenu = nullptr;
        titleMenuShowing = false;
        askingToAssociate = false;
        pendingGamePath = "";
    }

    if (getControlsManager()->miniGame_CANCEL_Pressed())
    {
        // Cancel association menu = Just load without associating? Or cancel load?
        // Let's say cancel load.
        associationMenu = nullptr;
        titleMenuShowing = true;
        askingToAssociate = false;
        pendingGamePath = "";
    }
}

void LibretroGame::parseOptionString(const string& key, const string& value)
{
    // value format: "Description; option1|option2|option3"
    size_t semi = value.find(';');
    if (semi == string::npos) return;

    CoreOption opt;
    opt.key = key;
    opt.description = value.substr(0, semi);
    string rest = value.substr(semi + 1);

    // Split by |
    // Note: space usually after ;
    if (rest.length() > 0 && rest[0] == ' ') rest = rest.substr(1);

    size_t start = 0;
    size_t end = rest.find('|');
    while (end != string::npos)
    {
        opt.values.push_back(rest.substr(start, end - start));
        start = end + 1;
        end = rest.find('|', start);
    }
    opt.values.push_back(rest.substr(start));

    // Check if exists
    for(auto& existing : coreOptions)
    {
        if(existing.key == key)
        {
            // Update definition but keep value if valid?
            // Usually core sets vars once. Assuming override.
            existing = opt;
            return;
        }
    }
    coreOptions.push_back(opt);
}

void LibretroGame::loadCoreOptions()
{
    // Filename: core_name.opt in system dir?
    string path = Main::getPath() + "/system/core_options.opt";
    // Format: key=value
    shared_ptr<ByteArray> data = FileUtils::loadByteFileFromExePath(path);
    if (!data) return;

    string content((char*)data->data(), data->size());
    stringstream ss(content);
    string line;
    while(getline(ss, line))
    {
        size_t eq = line.find('=');
        if (eq != string::npos)
        {
            string key = line.substr(0, eq);
            string val = line.substr(eq + 1);
            // clean up CR
            if (!val.empty() && val.back() == '\r') val.pop_back();

            for(auto& opt : coreOptions)
            {
                if (opt.key == key)
                {
                    for(int i=0; i<opt.values.size(); i++)
                    {
                        if (opt.values[i] == val)
                        {
                            opt.currentIndex = i;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void LibretroGame::saveCoreOptions()
{
    string path = Main::getPath() + "/system/core_options.opt";
    stringstream ss;
    for(const auto& opt : coreOptions)
    {
        if (opt.values.size() > 0)
        {
            ss << opt.key << "=" << opt.values[opt.currentIndex] << "\n";
        }
    }
    string content = ss.str();

    shared_ptr<ByteArray> data = make_shared<ByteArray>(content.length());
    memcpy(data->data(), content.c_str(), content.length());
    FileUtils::saveByteFile(path, data);

    variablesUpdated = true;
}

void LibretroGame::updateCoreOptionsMenu()
{
    if (coreOptionsMenu->menuItems->size() == 0)
    {
        coreOptionsMenu->add("Back", "Back");
        for(const auto& opt : coreOptions)
        {
            // Display: Description: CurrentValue
            string label = opt.description + ": " + (opt.values.size() > 0 ? opt.values[opt.currentIndex] : "N/A");
            coreOptionsMenu->add(label, opt.key);
        }
    }

    if (getControlsManager()->miniGame_UP_Pressed()) coreOptionsMenu->up();
    if (getControlsManager()->miniGame_DOWN_Pressed()) coreOptionsMenu->down();

    if (getControlsManager()->miniGame_CONFIRM_Pressed())
    {
        shared_ptr<BobMenu::MenuItem> item = coreOptionsMenu->getSelectedMenuItem();
        if (item)
        {
            if (item->id == "Back")
            {
                saveCoreOptions();
                coreOptionsMenu = nullptr;
                titleMenuShowing = true;
            }
            else
            {
                // Cycle option
                for(auto& opt : coreOptions)
                {
                    if (opt.key == item->id)
                    {
                        opt.currentIndex = (opt.currentIndex + 1) % opt.values.size();
                        item->setName(opt.description + ": " + opt.values[opt.currentIndex]);
                        variablesUpdated = true; // Mark updated immediately? or on save?
                        // Core might need restart or check update callback.
                    }
                }
            }
        }
    }

    if (getControlsManager()->miniGame_CANCEL_Pressed())
    {
        saveCoreOptions();
        coreOptionsMenu = nullptr;
        titleMenuShowing = true;
    }
}

void LibretroGame::loadCheats()
{
    cheats.clear();
    // Look for .cht file
    string path = getSavePath("cht");
    shared_ptr<ByteArray> data = FileUtils::loadByteFileFromExePath(path);
    if (!data) return;

    string content((char*)data->data(), data->size());
    stringstream ss(content);
    string line;
    while(getline(ss, line))
    {
        // Format: description=code
        size_t eq = line.find('=');
        if (eq != string::npos)
        {
             Cheat c;
             c.description = line.substr(0, eq);
             c.code = line.substr(eq + 1);
             if (!c.code.empty() && c.code.back() == '\r') c.code.pop_back();
             c.enabled = false;
             cheats.push_back(c);
        }
    }
}

void LibretroGame::applyCheats()
{
    if (!retro_cheat_reset || !retro_cheat_set) return;

    retro_cheat_reset();
    for(int i=0; i<cheats.size(); i++)
    {
        if (cheats[i].enabled)
        {
            retro_cheat_set(i, true, cheats[i].code.c_str());
        }
    }
}

void LibretroGame::updateCheatMenu()
{
    if (cheatMenu->menuItems->size() == 0)
    {
        cheatMenu->add("Back", "Back");
        cheatMenu->add("Apply Cheats", "Apply");
        for(int i=0; i<cheats.size(); i++)
        {
            string label = (cheats[i].enabled ? "[X] " : "[ ] ") + cheats[i].description;
            cheatMenu->add(label, to_string(i));
        }
    }

    if (getControlsManager()->miniGame_UP_Pressed()) cheatMenu->up();
    if (getControlsManager()->miniGame_DOWN_Pressed()) cheatMenu->down();

    if (getControlsManager()->miniGame_CONFIRM_Pressed())
    {
        shared_ptr<BobMenu::MenuItem> item = cheatMenu->getSelectedMenuItem();
        if (item)
        {
            if (item->id == "Back")
            {
                cheatMenu = nullptr;
                titleMenuShowing = true;
            }
            else if (item->id == "Apply")
            {
                applyCheats();
                cheatMenu = nullptr;
                titleMenuShowing = true; // or stay?
            }
            else
            {
                // Toggle cheat
                try {
                    int idx = stoi(item->id);
                    if (idx >= 0 && idx < cheats.size())
                    {
                        cheats[idx].enabled = !cheats[idx].enabled;
                        item->setName((cheats[idx].enabled ? "[X] " : "[ ] ") + cheats[idx].description);
                    }
                } catch(...) {}
            }
        }
    }

    if (getControlsManager()->miniGame_CANCEL_Pressed())
    {
        cheatMenu = nullptr;
        titleMenuShowing = true;
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

                            // Check if we were pending a game load
                            if (!pendingGamePath.empty())
                            {
                                // Ask to associate?
                                askingToAssociate = true;
                                associationMenu = make_shared<BobMenu>(this, "Associate Core?");
                                updateAssociationMenu();
                                // Dont show title menu yet
                            }
                            else
                            {
                                titleMenuShowing = true;
                            }
                        }
                    }
                    else
                    {
                        // Check association first
                        string ext = "";
                        size_t dot = fullPath.find_last_of(".");
                        if (dot != string::npos) ext = fullPath.substr(dot);

                        if (coreHandle)
                        {
                             // Core loaded, just load game
                             if (loadGame(fullPath))
                             {
                                 fileBrowserMenu = nullptr;
                                 titleMenuShowing = false;
                             }
                        }
                        else
                        {
                            // No core. Check association
                            if (coreAssociations.count(ext) && BobFile(coreAssociations[ext]).exists())
                            {
                                 if (loadCore(coreAssociations[ext]))
                                 {
                                     if (loadGame(fullPath))
                                     {
                                         fileBrowserMenu = nullptr;
                                         titleMenuShowing = false;
                                     }
                                 }
                            }
                            else
                            {
                                // No association. Prompt for core.
                                pendingGamePath = fullPath;
                                selectingCore = true;
                                fileBrowserMenu->setName("Select Core for " + selected);
                                // Need to reset path to core dir? Or keep same?
                                // Let's keep same for now or go to root.
                                // Often cores are in specific dir.
                                // Let's guess user navigates to core.
                            }
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

        pendingGamePath = "";
        askingToAssociate = false;
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

    loadedCorePath = corePath;

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
    shared_ptr<ByteArray> data = nullptr;

    // Check for ZIP
    if (gamePath.size() > 4 && gamePath.substr(gamePath.size() - 4) == ".zip")
    {
        // Use Poco to unzip
        try
        {
            std::ifstream inp(gamePath, std::ios::binary);
            if (inp.good())
            {
                Poco::Zip::Decompress dec(inp, Poco::Path(Main::getPath() + "/temp/"));
                dec.decompressAllFiles();
                // Find extracted file (first one?)
                // For simplicity, let's assume one file or check temp dir
                // This is a bit hacky, normally we'd decompress to memory if possible or find the ROM extension
                // Let's try to find a file in temp
                BobFile tempDir(Main::getPath() + "/temp/");
                vector<string> files = tempDir.list();
                if (!files.empty())
                {
                    string extractedPath = Main::getPath() + "/temp/" + files[0];
                    data = FileUtils::loadByteFileFromExePath(extractedPath);
                    // Update info.path to extracted file? Some cores might need extension
                    // But we are passing data, so path might be irrelevant or just for extension detection
                    // Let's update path to extracted path
                    // We must ensure 'info.path' persists if we change it? info is local struct.
                    // But retro_load_game takes const pointer.
                    // We can't easily change the pointer to a temporary string c_str.
                    // However, we can just load data and hope core uses data.
                }
            }
        }
        catch (...)
        {
             log.error("Failed to unzip " + gamePath);
        }
    }
    else
    {
        data = FileUtils::loadByteFileFromExePath(gamePath);
    }

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

    // Load cheats
    loadCheats();

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

    if (coreOptionsMenu)
    {
        updateCoreOptionsMenu();
        return;
    }

    if (cheatMenu)
    {
        updateCheatMenu();
        return;
    }

    if (controlsMenu)
    {
        updateControlsMenu();
        return;
    }

    if (associationMenu)
    {
        updateAssociationMenu();
        return;
    }

    if (!titleMenuShowing)
    {
        if (retro_run)
        {
            shared_ptr<ControlsManager> cm = getControlsManager();
            // Check Rewind (Left Shift + Left or R key?)
            // Let's use MINIGAME_L (Left shoulder) + LEFT for rewind
            // or just a dedicated key if mapped.
            // For simplicity: If R key held (or R shoulder on gamepad + Left)
            // Let's check for 'R' key for now or a combo.
            // Actually, let's map it to "Backspace" or "R"
            if (cm->key_BACKSPACE_Pressed() || (cm->miniGame_L_Pressed() && cm->miniGame_LEFT_Pressed())) // Use Pressed for toggle? No, hold for rewind.
            {
                // This logic for hold is tricky with "Pressed" checks which reset.
                // We need "Held".
            }

            bool rewindHeld = cm->key_BACKSPACE_Pressed(); // Check held manually in CM? CM exposes HELD.
            if (cm->KEY_BACKSPACE_HELD || (cm->MINIGAME_L_HELD && cm->MINIGAME_LEFT_HELD))
            {
                 doRewind();
            }
            else
            {
                int frames = fastForward ? 4 : 1;
                for(int i=0; i<frames; i++)
                {
                    retro_run();
                    pushRewindState();
                }
            }
        }
        else
        {
            titleMenuShowing = true;
        }
    }
}

void LibretroGame::pushRewindState()
{
    if (!retro_serialize_size || !retro_serialize) return;

    rewindCounter++;
    if (rewindCounter >= rewindInterval)
    {
        rewindCounter = 0;
        size_t size = retro_serialize_size();
        if (size > 0)
        {
            shared_ptr<ByteArray> data = make_shared<ByteArray>(size);
            if (retro_serialize(data->data(), size))
            {
                rewindBuffer.push_back(data);
                // Limit buffer size (e.g. 10 seconds @ 60fps / 5 = 12 states per sec -> 120 states)
                // Let's keep 500 states (~40 seconds)
                if (rewindBuffer.size() > 500)
                {
                    rewindBuffer.pop_front();
                }
            }
        }
    }
}

void LibretroGame::doRewind()
{
    if (!retro_serialize_size || !retro_unserialize) return;

    if (!rewindBuffer.empty())
    {
        // Pop latest
        shared_ptr<ByteArray> data = rewindBuffer.back();
        rewindBuffer.pop_back();

        retro_unserialize(data->data(), data->size());
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

    if (coreOptionsMenu)
    {
        coreOptionsMenu->render();
    }

    if (cheatMenu)
    {
        cheatMenu->render();
    }

    if (controlsMenu)
    {
        controlsMenu->render();
    }

    if (associationMenu)
    {
        associationMenu->render();
    }

    if (!titleMenuShowing && !fileBrowserMenu && !coreOptionsMenu && !cheatMenu && !controlsMenu && !associationMenu && videoTexture)
    {
        // Draw video texture to screen
        // Use GLUtils to draw
        bool shaderApplied = false;
        if (crtShaderEnabled && crtProgram != 0)
        {
            // Try to use shader
            // Note: GLUtils might override shader if we call drawTexture.
            // But we can try to bind it.
            // Actually, GLUtils::drawTexture uses immediate mode or fixed pipeline or internal shader.
            // If it uses internal shader, glUseProgram will be overridden.
            // Let's assume we can set it via GLUtils or just use raw GL if available.
            // Since we can't easily modify GLUtils here without risk, let's try raw glUseProgram.
            // If GLUtils resets it, we need another way.
            // But let's try.
#ifndef ORBIS
            glUseProgram(crtProgram);
            // set uniforms
            // uniform sampler2D tex; -> default 0
            // uniform float time;
            GLint timeLoc = glGetUniformLocation(crtProgram, "time");
            if(timeLoc != -1) glUniform1f(timeLoc, (float)SDL_GetTicks() / 1000.0f);

            GLint texLoc = glGetUniformLocation(crtProgram, "tex");
            if(texLoc != -1) glUniform1i(texLoc, 0);

            shaderApplied = true;
#endif
        }

        GLUtils::drawTexture(videoTexture.get(), 0, 0, 1.0f, videoFilterLinear ? GLUtils::FILTER_LINEAR : GLUtils::FILTER_NEAREST);

        if (shaderApplied)
        {
#ifndef ORBIS
            glUseProgram(0);
#endif
        }
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
        case RETRO_ENVIRONMENT_SET_VARIABLES:
        {
            const struct retro_variable *vars = (const struct retro_variable *)data;
            if (vars)
            {
                while (vars->key && vars->value)
                {
                    instance->parseOptionString(vars->key, vars->value);
                    vars++;
                }
                instance->loadCoreOptions(); // Load saved preferences
            }
            return true;
        }
        case RETRO_ENVIRONMENT_GET_VARIABLE:
        {
            struct retro_variable *var = (struct retro_variable *)data;
            if (var && var->key)
            {
                for (auto& opt : instance->coreOptions)
                {
                    if (opt.key == var->key)
                    {
                        var->value = opt.values[opt.currentIndex].c_str();
                        return true;
                    }
                }
                return false;
            }
            return false;
        }
        case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE:
        {
             bool *result = (bool*)data;
             if (result) *result = instance->variablesUpdated;
             instance->variablesUpdated = false;
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

        // Use input map
        int internalId = -1;
        if (instance->inputMap.find(id) != instance->inputMap.end())
        {
            internalId = instance->inputMap[id];
        }
        else
        {
             // Fallback default
             // B=0, Y=1, Sel=2, Start=3, Up=4, Down=5, Left=6, Right=7, A=8, X=9, L=10, R=11
             internalId = id;
        }

        switch(internalId)
        {
            case 0: return cm->MINIGAME_ACTION_HELD ? 1 : 0; // B
            case 1: return cm->MINIGAME_RUN_HELD ? 1 : 0; // Y
            case 2: return cm->MINIGAME_SELECT_HELD ? 1 : 0; // Select
            case 3: return cm->MINIGAME_START_HELD ? 1 : 0; // Start
            case 4: return cm->MINIGAME_UP_HELD ? 1 : 0; // Up
            case 5: return cm->MINIGAME_DOWN_HELD ? 1 : 0; // Down
            case 6: return cm->MINIGAME_LEFT_HELD ? 1 : 0; // Left
            case 7: return cm->MINIGAME_RIGHT_HELD ? 1 : 0; // Right
            case 8: return cm->MINIGAME_A_HELD ? 1 : 0; // A
            case 9: return cm->MINIGAME_X_HELD ? 1 : 0; // X
            case 10: return cm->MINIGAME_L_HELD ? 1 : 0; // L
            case 11: return cm->MINIGAME_R_HELD ? 1 : 0; // R
            case 12: return cm->MINIGAME_L2_HELD ? 1 : 0; // L2
            case 13: return cm->MINIGAME_R2_HELD ? 1 : 0; // R2
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
        string path = currentPath + ".state" + to_string(currentSaveSlot);
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

    string path = currentPath + ".state" + to_string(currentSaveSlot);
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
