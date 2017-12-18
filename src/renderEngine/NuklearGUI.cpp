/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:26:16 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/18 14:37:21 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define NK_INCLUDE_FIXED_TYPES
# define NK_INCLUDE_STANDARD_IO
# define NK_INCLUDE_STANDARD_VARARGS
# define NK_INCLUDE_DEFAULT_ALLOCATOR
# define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
# define NK_INCLUDE_FONT_BAKING
# define NK_INCLUDE_DEFAULT_FONT
# define NK_IMPLEMENTATION
# define NK_SDL_GL3_IMPLEMENTATION
#include "NuklearGUI.hpp"

# include "../../libs/style.c"

# define MAX_VERTEX_MEMORY 512 * 1024
# define MAX_ELEMENT_MEMORY 128 * 1024

NuklearGUI::NuklearGUI(Sdl_gl_win & sgw, Camera & camera) :
    win(sgw), camera(camera), event(SEventManager::getInstance()),
    _masterVolume(0.f), _effectsVolume(0.f), _musicVolume(0.f),
    _active_menu(){
    ctx = nk_sdl_init(win.getWin());
    nk_sdl_font_stash_begin(&atlas);
    struct nk_font *future = nk_font_atlas_add_from_file(atlas, "assets/fonts/kenvector_future_thin.ttf", 13, 0);
    nk_style_set_font(ctx, &future->handle);
    nk_sdl_font_stash_end();
    set_style(ctx, THEME_WHITE);

    event.registerEvent(Event::KEYDOWN, MEMBER_CALLBACK(NuklearGUI::handleKey));
    event.registerEvent(Event::GUI_TOGGLE, MEMBER_CALLBACK(NuklearGUI::toggle));
    event.registerEvent(Event::GUI_BASE_MENU, std::pair<CallbackType, void*>(std::bind(&NuklearGUI::toggle, this, new Menu::Enum(Menu::BASE)), this));

    event.registerEvent(Event::MASTER_VOLUME_UPDATE, MEMBER_CALLBACK(NuklearGUI::setMasterVolume));
    event.registerEvent(Event::MUSIC_VOLUME_UPDATE, MEMBER_CALLBACK(NuklearGUI::setMusicVolume));
    event.registerEvent(Event::EFFECTS_VOLUME_UPDATE, MEMBER_CALLBACK(NuklearGUI::setEffectsVolume));

    event.registerEvent(Event::SCREEN_FORMAT_UPDATE, MEMBER_CALLBACK(NuklearGUI::updateScreenFormat));

    start_time = std::chrono::steady_clock::now();
    frames = 0;
    fps = 0.f;
    
    _keyToChange = nullptr;
}

NuklearGUI::~NuklearGUI() {
    nk_sdl_shutdown();    
}

struct nk_context * NuklearGUI::getContext () const { return ctx; }

void    NuklearGUI::toggle(void *p) {
    Menu::Enum *m = static_cast<Menu::Enum *>(p);
    if (*m == Menu::NONE){
        while (!_active_menu.empty()){
            _active_menu.pop();
        }
    }
    else if (!_active_menu.empty() && *m == _active_menu.top()){
        _active_menu.pop();
    }
    else {
        _active_menu.push(*m);
    }
}

void    NuklearGUI::handleKey(void * p) {
    SDL_Keycode key = *static_cast<int*>(p);
    if (_keyToChange != nullptr) {
        *_keyToChange = key;
        _keyToChange = nullptr;
    }
}

void    NuklearGUI::render() {
    SDL_GetWindowSize(win.getWin(), &windowWidth, &windowHeight);

   float spacingY =  ctx->style.window.spacing.y; // between items
   float paddingY =  ctx->style.window.padding.y; // = nk_vec2(0,0); // above / under items

    menuWidth = windowWidth / 4;
    optionHeight = windowHeight / 25;

    menuHeight = optionHeight * 7 + spacingY * 7 + paddingY * 2;

    if (!_active_menu.empty()) {
        switch (_active_menu.top()){
            case Menu::NONE:                break;
            case Menu::DEBUG:               renderDebug(); break;
            case Menu::KEY_BINDINGS:        renderKeyBindings(); break;
            case Menu::BASE:                renderMenu(); break;
            case Menu::OPTIONS:             renderOptions(); break;
            case Menu::START:               renderStartMenu(); break;
            case Menu::LEVEL_SELECTION:     renderLevelSelection(); break;
            case Menu::SELECT_SLOT:         renderSelectSlot(); break;
        }  
    }
    renderHUD();
    nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}

void    NuklearGUI::bindKeyToEvent(Event::Enum ev, std::map<Event::Enum, SDL_Keycode> & displayedKeysMap) {
    // Reset previously bound _keyToChange if any
    if (_keyToChange != nullptr) {
        Event::Enum toReset;
        for (auto it = displayedKeysMap.begin(); it != displayedKeysMap.end(); it++) {
            if (it->second == *_keyToChange) {
                toReset = it->first;
                displayedKeysMap[toReset] = win.getKeyMap()[toReset];
                break;
            }
        }
    }
    // Set key to be changed
    _keyToChange = &displayedKeysMap[ev];
    displayedKeysMap[ev] = 0;
    event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));
}

void    NuklearGUI::renderKeyBindings() {
    SEventManager & event = SEventManager::getInstance();
    
    static std::map<Event::Enum, SDL_Keycode>  displayedKeysMap = win.getKeyMap();
    
    std::string left =  SDL_GetKeyName(displayedKeysMap[Event::HUMAN_PLAYER_LEFT]);
    std::string right = SDL_GetKeyName(displayedKeysMap[Event::HUMAN_PLAYER_RIGHT]);
    std::string up =    SDL_GetKeyName(displayedKeysMap[Event::HUMAN_PLAYER_UP]);
    std::string down =  SDL_GetKeyName(displayedKeysMap[Event::HUMAN_PLAYER_DOWN]);
    std::string drop =  SDL_GetKeyName(displayedKeysMap[Event::HUMAN_SPAWN_BOMB]);

    if (nk_begin(ctx, "KEY BINDINGS", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - menuHeight / 2, menuWidth, menuHeight),
    NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move up", NK_TEXT_LEFT);
        hover(1);
        if (nk_button_label(ctx, up.c_str())) {
            bindKeyToEvent(Event::HUMAN_PLAYER_UP, displayedKeysMap);            
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move down", NK_TEXT_LEFT);
        hover(2);
        if (nk_button_label(ctx, down.c_str())) {
            bindKeyToEvent(Event::HUMAN_PLAYER_DOWN, displayedKeysMap);            
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move left", NK_TEXT_LEFT);
        hover(3);
        if (nk_button_label(ctx, left.c_str())) {
            bindKeyToEvent(Event::HUMAN_PLAYER_LEFT, displayedKeysMap);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move right", NK_TEXT_LEFT);
        hover(4);
        if (nk_button_label(ctx, right.c_str())) {
            bindKeyToEvent(Event::HUMAN_PLAYER_RIGHT, displayedKeysMap);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Drop bomb", NK_TEXT_LEFT);
        hover(5);
        if (nk_button_label(ctx, drop.c_str())) {
            bindKeyToEvent(Event::HUMAN_SPAWN_BOMB, displayedKeysMap);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        hover(6);
        nk_spacing(ctx, 1);       
        if (nk_button_label(ctx, "Reset to default"))
        {
            /* reset key bindings */
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        hover(6);
        if (nk_button_label(ctx, "Apply"))
        {
            // win.setKeyMap(displayedKeysMap);
            event.raise(Event::KEY_MAP_UPDATE, &displayedKeysMap);
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::KEY_BINDINGS));
        }
        hover(7);
        if (nk_button_label(ctx, "Back"))
        {
            displayedKeysMap = win.getKeyMap();
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::KEY_BINDINGS));
        }
    }
    nk_end(ctx);
}

void    NuklearGUI::renderOptions() {
    SEventManager & event = SEventManager::getInstance();

    static Screen::Format                       displayedFormat = screenFormat;  
    static std::vector<SDL_DisplayMode> const & modes = win.getDisplayModes();  
    
    std::string screenResString = toString(displayedFormat.displayMode);
    std::string screenModeString = toString(displayedFormat.windowMode);

    if (nk_begin(ctx, "OPTIONS", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
    {
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Screen resolution", NK_TEXT_LEFT);
        if (nk_menu_begin_label(ctx, screenResString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth / 2, menuHeight))) {
            nk_layout_row_dynamic(ctx, optionHeight, 1);

            for (size_t i = 0; i < modes.size(); ++i) {
                if (nk_menu_item_label(ctx, toString(modes[i]).c_str(), NK_TEXT_CENTERED)) {
                    event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));
                    displayedFormat.displayMode = modes[i];
                }
            }
            nk_menu_end(ctx);
        }
        
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Screen mode", NK_TEXT_LEFT);     
        if (nk_menu_begin_label(ctx, screenModeString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth / 2, menuHeight))) {

            nk_layout_row_dynamic(ctx, optionHeight, 1);
            if (nk_menu_item_label(ctx, "WINDOWED", NK_TEXT_CENTERED)) {
                event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));
                displayedFormat.windowMode = Screen::WindowMode::WINDOWED;
            }

            if (nk_menu_item_label(ctx, "FULLSCREEN", NK_TEXT_CENTERED)) {
                event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));
                displayedFormat.windowMode = Screen::WindowMode::FULLSCREEN;
            }
            nk_menu_end(ctx);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Master volume", NK_TEXT_LEFT);             
        if (nk_slider_float(ctx, 0, &_masterVolume, 1.f, 0.01f)) {
            event.raise(Event::MASTER_VOLUME_UPDATE, &_masterVolume);
            /* Master */
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Music volume", NK_TEXT_LEFT);             
        if (nk_slider_float(ctx, 0, &_musicVolume, MIX_MAX_VOLUME, 1)) {
            event.raise(Event::MUSIC_VOLUME_UPDATE, &_musicVolume);
            /* Music */
        }
        
        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Effects volume", NK_TEXT_LEFT);             
        if (nk_slider_float(ctx, 0, &_effectsVolume, MIX_MAX_VOLUME, 1)) {
            event.raise(Event::EFFECTS_VOLUME_UPDATE, &_effectsVolume);
            /* Effects */
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Key bindings", NK_TEXT_LEFT);  
        hover(1);   
        if (nk_button_label(ctx, "Configure"))
        {
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::KEY_BINDINGS));  
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2); 
        hover(2);
        if (nk_button_label(ctx, "Apply"))
        {
            if (screenFormat.displayMode.w != displayedFormat.displayMode.w
                || screenFormat.displayMode.h != displayedFormat.displayMode.h
                || screenFormat.windowMode != displayedFormat.windowMode) {
                    screenFormat = displayedFormat;
                    event.raise(Event::SCREEN_FORMAT_UPDATE, &displayedFormat);  
            }
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::OPTIONS));  
        }
        hover(3); 
        if (nk_button_label(ctx, "Back"))
        {
            displayedFormat = screenFormat;
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::OPTIONS));  
        }
        
    }
    nk_end(ctx);
}

void    NuklearGUI::renderMenu() {
    SEventManager & event = SEventManager::getInstance();
    if (nk_begin(ctx, "MENU", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
    {
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(1);
        if (nk_button_label(ctx, "Resume"))
        {
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));            
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::NONE));
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(2);
        if (nk_button_label(ctx, "Restart level"))
        {
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));            
            /* restart */
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(3);
        if (nk_button_label(ctx, "How to play"))
        {
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));            
            /* how to play */
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(4);
        if (nk_button_label(ctx, "Options"))
        {
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));            
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::OPTIONS));
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(5);
        if (nk_button_label(ctx, "Save"))
        {
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));            
            /* save */
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(6);
        if (nk_button_label(ctx, "Back to main menu"))
        {
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));            
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::NONE)); // Must be changed
        }
   
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(7); 
        if (nk_button_label(ctx, "Quit game"))
        {
            event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::CLICK));            
            event.raise(Event::QUIT_GAME, nullptr);
        }
    }
    nk_end(ctx);
}

void    NuklearGUI::renderLevelSelection() {
//    SEventManager & event = SEventManager::getInstance();

    static struct nk_image  levelImage = loadImage("assets/textures/level1.png", GL_RGBA);
    static Level::Enum      level = Level::ONE;

    struct nk_vec2 spacing = ctx->style.window.spacing;
    struct nk_vec2 padding = ctx->style.window.padding;
    float   imageSize = (menuHeight - optionHeight) - padding.y * 2 - spacing.y * 2;

    if (nk_begin(ctx, "", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - menuHeight / 2, menuWidth, menuHeight),
    NK_WINDOW_BORDER |NK_WINDOW_NO_SCROLLBAR ))
    {
        nk_layout_row_dynamic(ctx, imageSize, 1);
        nk_button_image(ctx, levelImage);

        nk_layout_row_dynamic(ctx, optionHeight, 6);
        if (nk_button_label(ctx, "Back")) 
        {
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::LEVEL_SELECTION));            
        }
        nk_spacing(ctx, 1);
        if (nk_button_label(ctx, "Previous"))
        {
            if (level > Level::ONE)
            {
                level = static_cast<Level::Enum>(level - 1);
                levelImage = loadImage("assets/textures/level" + std::to_string(level + 1) + ".png", GL_RGBA);
            }
        }
        if (nk_button_label(ctx, "Next"))
        {
            if (level < Level::THREE)
            {
                level = static_cast<Level::Enum>(level + 1);
                levelImage = loadImage("assets/textures/level" + std::to_string(level + 1) + ".png", GL_RGBA);
            }
        }
        nk_spacing(ctx, 1);
        nk_label(ctx, std::string("Level " + std::to_string(level + 1)).c_str(), NK_TEXT_CENTERED);

    }
    nk_end(ctx);       
}

void    NuklearGUI::renderStartMenu() {
    SEventManager & event = SEventManager::getInstance();
    if (nk_begin(ctx, "", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
    {
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        if (nk_button_label(ctx, "Campaign"))
        {
            Menu::Enum  me = Menu::LEVEL_SELECTION;
            event.raise(Event::GUI_TOGGLE, &me);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Brawl"))
        {
            GameMode::Enum  gm = GameMode::BRAWL;
            event.raise(Event::NEW_GAME, &gm);

            Menu::Enum  me = Menu::NONE;
            event.raise(Event::GUI_TOGGLE, &me);
        }
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Options"))
        {
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::OPTIONS));  
        }
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Slot selection"))
        {
            event.raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::SELECT_SLOT));  
        }
   
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        if (nk_button_label(ctx, "Quit Game"))
        {
            event.raise(Event::QUIT_GAME, nullptr);  
        }
    }
    nk_end(ctx);
}

void    NuklearGUI::update_fps(void){
    std::chrono::milliseconds   dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time);
    frames++;
    if (dur.count() > 250){
        fps = static_cast<float>(frames) / static_cast<float>(dur.count()) * 1000.f;
        start_time = std::chrono::steady_clock::now();
        frames = 0;
    }
}

void    NuklearGUI::renderHUD() {

    static struct nk_image portrait = loadImage("assets/textures/BlackBM-avatar.png", GL_RGBA);
    static struct nk_image bomb = loadImage("assets/textures/BombupspriteHUD.png", GL_RGBA);
    static struct nk_image skate = loadImage("assets/textures/SkatespriteHUD.png", GL_RGBA);
    static struct nk_image fire = loadImage("assets/textures/FireupspriteHUD.png", GL_RGBA);


    float spacingY =  ctx->style.window.spacing.y; // between items
    float paddingY =  ctx->style.window.padding.y; // = nk_vec2(0,0); // above / under items 

    float   avatar_w = windowWidth * 0.075f;
    float   avatar_h = avatar_w;
    float   avatar_x = 50;
    float   avatar_y = 50;
    float   bon_w = avatar_w * 0.75f;
    float   line_height = (avatar_h * 0.6f) / 3;
    float   bon_h = line_height * 3 + spacingY * 3 + paddingY * 2;
    
    if (nk_begin(ctx, "BONUSES", nk_rect(avatar_x + avatar_w * 0.7f, avatar_y + (avatar_h - bon_h) / 2, bon_w, bon_h),
    NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {   
        nk_layout_row_dynamic(ctx, line_height, 3);
        nk_spacing(ctx, 1);
        nk_image(ctx, bomb);
        nk_label(ctx, "1", NK_TEXT_CENTERED);
        
        nk_layout_row_dynamic(ctx, line_height, 3); 
        nk_spacing(ctx, 1);
        nk_image(ctx, skate);
        nk_label(ctx, "1", NK_TEXT_CENTERED);
             
        nk_layout_row_dynamic(ctx, line_height, 3);  
        nk_spacing(ctx, 1);
        nk_image(ctx, fire);
        nk_label(ctx, "1", NK_TEXT_CENTERED);
    }
    nk_end(ctx); 
    
    struct nk_style_item tmp = ctx->style.window.fixed_background;
    ctx->style.window.fixed_background = nk_style_item_hide();
    
    if (nk_begin(ctx, "AVATAR", nk_rect(avatar_x, avatar_y, avatar_w, avatar_h),
    NK_WINDOW_NO_SCROLLBAR)) {   
        nk_layout_row_dynamic(ctx, avatar_h, 1);          
        nk_image(ctx, portrait);
    }    
    nk_end(ctx); 
    
    ctx->style.window.fixed_background = tmp;
    
}

void    NuklearGUI::renderDebug() {
    glm::vec3   camPos = camera.getPosition();
    glm::vec3   camFront = camera.getFront();
    std::string camPosString = std::to_string(camPos.x) + " : " + std::to_string(camPos.y) + " : " + std::to_string(camPos.z);
    std::string camFrontString = std::to_string(camFront.x) + " : " + std::to_string(camFront.y) + " : " + std::to_string(camFront.z);
    std::string camModeString = toString(camera.getMode());

    update_fps();
    std::string FPSString = std::to_string(fps);
    
    SEventManager & event = SEventManager::getInstance();
    
     if (nk_begin(ctx, "DEBUG MODE", nk_rect(50, 50, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE))
    {
        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Camera position", NK_TEXT_LEFT);
        nk_label(ctx, camPosString.c_str(), NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Camera front", NK_TEXT_LEFT);
        nk_label(ctx, camFrontString.c_str(), NK_TEXT_CENTERED);

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "FPS : ", NK_TEXT_LEFT);
        nk_label(ctx, FPSString.c_str(), NK_TEXT_CENTERED);
        
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Camera mode", NK_TEXT_LEFT);
        if (nk_menu_begin_label(ctx, camModeString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth / 2, menuHeight))) {
            nk_layout_row_dynamic(ctx, optionHeight, 1);
            
            if (nk_menu_item_label(ctx, "FIXED", NK_TEXT_CENTERED)) {
                event.raise(Event::CAMERA_MODE_UPDATE, new Camera::Mode(Camera::Mode::FIXED));
            }
            if (nk_menu_item_label(ctx, "FREE", NK_TEXT_CENTERED)) {
                event.raise(Event::CAMERA_MODE_UPDATE, new Camera::Mode(Camera::Mode::FREE));
            }
            if (nk_menu_item_label(ctx, "FOLLOW PLAYER", NK_TEXT_CENTERED)) {
                event.raise(Event::CAMERA_MODE_UPDATE, new Camera::Mode(Camera::Mode::FOLLOW_PLAYER));
            }
            nk_menu_end(ctx);
        }
    }
    nk_end(ctx);
}

void            NuklearGUI::renderSelectSlot(void){
    SEventManager & event = SEventManager::getInstance();

    static struct nk_vec2 spacing =  ctx->style.window.spacing;
    static struct nk_vec2 padding =  ctx->style.window.padding; 

    static float groupHeight = menuHeight - 2 * padding.y - spacing.y; 

    if (nk_begin(ctx, "", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
    {
        nk_layout_row_dynamic(ctx, groupHeight, 3);  
        nk_group_begin(ctx, "SLOT 1", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR);
        {
            nk_layout_row_dynamic(ctx, optionHeight, 1);  
            if (nk_button_label(ctx, "SLOT 1"))
            {
                Save::Enum  slot = Save::SLOT1;
                event.raise(Event::LOAD_SLOT, &slot);
    
                Menu::Enum  me = Menu::START;
                event.raise(Event::GUI_TOGGLE, &me);
            }
            nk_label(ctx, "Stuff", NK_TEXT_CENTERED);
            nk_label(ctx, "describing", NK_TEXT_CENTERED);
            nk_label(ctx, "the save", NK_TEXT_CENTERED);
            nk_label(ctx, "...", NK_TEXT_CENTERED);            
            nk_button_color(ctx, nk_rgb(0,0,255));            
            
            nk_group_end(ctx);            
        }
        nk_group_begin(ctx, "SLOT 2", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR);
        {
            nk_layout_row_dynamic(ctx, optionHeight, 1);  
            if (nk_button_label(ctx, "SLOT 2"))
            {
                Save::Enum  slot = Save::SLOT2;
                event.raise(Event::LOAD_SLOT, &slot);

                Menu::Enum  me = Menu::START;
                event.raise(Event::GUI_TOGGLE, &me);
            }
            nk_label(ctx, "Stuff", NK_TEXT_CENTERED);
            nk_label(ctx, "describing", NK_TEXT_CENTERED);
            nk_label(ctx, "the save", NK_TEXT_CENTERED);
            nk_label(ctx, "...", NK_TEXT_CENTERED);
            nk_button_color(ctx, nk_rgb(255,0,0));            
            nk_group_end(ctx);            
        }
        nk_group_begin(ctx, "SLOT 3", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR);
        {
            nk_layout_row_dynamic(ctx, optionHeight, 1);  
            if (nk_button_label(ctx, "SLOT 3"))
            {
                Save::Enum  slot = Save::SLOT3;
                event.raise(Event::LOAD_SLOT, &slot);

                Menu::Enum  me = Menu::START;
                event.raise(Event::GUI_TOGGLE, &me);
            }
            nk_label(ctx, "Stuff", NK_TEXT_CENTERED);
            nk_label(ctx, "describing", NK_TEXT_CENTERED);
            nk_label(ctx, "the save", NK_TEXT_CENTERED);
            nk_label(ctx, "...", NK_TEXT_CENTERED);
            nk_button_color(ctx, nk_rgb(0,255,0));            
            nk_group_end(ctx);
        }    
    }
    nk_end(ctx);
}

void            NuklearGUI::hover(int id) const {
    static int hovered = 0;
    if (nk_widget_is_hovered(ctx) && hovered != id) {
        event.raise(Event::UI_AUDIO, new UIAudio::Enum(UIAudio::HOVER));
        hovered = id;
    } 
}

struct nk_image  NuklearGUI::loadImage(std::string const filename, GLint format)
{
    int x,y,n;
    GLuint tex;
    unsigned char *data = stbi_load(filename.c_str(), &x, &y, &n, 0);
    if (!data) throw std::runtime_error("NuklearGUI::loadImage() - Failed to load image: " + filename);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return nk_image_id((int)tex);
}

std::string     NuklearGUI::toString(SDL_DisplayMode const & m) const {
    return std::to_string(m.w) + " x " + std::to_string(m.h);
}

std::string     NuklearGUI::toString(Screen::WindowMode m) const {
    std::string mode;
    switch (m) {
        case Screen::WindowMode::WINDOWED:  mode = "WINDOWED"; break;
        case Screen::WindowMode::FULLSCREEN:mode = "FULLSCREEN"; break;
    }
    return mode;
}

std::string     NuklearGUI::toString(Camera::Mode m) const {
    std::string mode;
    switch (m) {
        case Camera::Mode::FIXED:           mode = "FIXED"; break;
        case Camera::Mode::FREE:            mode = "FREE"; break;
        case Camera::Mode::FOLLOW_PLAYER:   mode = "FOLLOW PLAYER"; break;
    }
    return mode;
}

void    NuklearGUI::setMasterVolume(void * v) {
    _masterVolume = *static_cast<float*>(v);
}

void    NuklearGUI::setEffectsVolume(void * v) {
    _effectsVolume = *static_cast<float*>(v);
}

void    NuklearGUI::setMusicVolume(void * v) {
    _musicVolume = *static_cast<float*>(v);
}

void    NuklearGUI::updateScreenFormat(void *f) {
    Screen::Format  *format = static_cast<Screen::Format*>(f);
    screenFormat.displayMode = format->displayMode;
    screenFormat.windowMode = format->windowMode;
}