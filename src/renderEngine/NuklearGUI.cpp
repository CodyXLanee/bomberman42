/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:26:16 by lfourque          #+#    #+#             */
/*   Updated: 2018/01/29 16:18:37 by lfourque         ###   ########.fr       */
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
#include "SGameManager.hpp"

#include "../../libs/style.c"

# define MAX_VERTEX_MEMORY 512 * 1024
# define MAX_ELEMENT_MEMORY 128 * 1024

/* -- Tools -- */

static long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* ----------- */

NuklearGUI::NuklearGUI(Sdl_gl_win & sgw, Camera & camera) :
    win(sgw), camera(camera), event(SEventManager::getInstance()),
    atlas(NULL), screenFormatUpdate(false),
    _masterVolume(0), _effectsVolume(0), _musicVolume(0),
    _reset_options_display(false), _reset_key_bindings_display(false),
    _active_menu(){
    ctx = nk_sdl_init(win.getWin());

    set_style(ctx, THEME_WHITE);
    setupProportions();

    event.registerEvent(Event::KEYDOWN, MEMBER_CALLBACK(NuklearGUI::handleKey));
    event.registerEvent(Event::GUI_TOGGLE, MEMBER_CALLBACK(NuklearGUI::toggle));
    Menu::Enum menuEnum = Menu::Enum(Menu::BASE);
    event.registerEvent(Event::GUI_BASE_MENU, std::pair<CallbackType, void*>(std::bind(&NuklearGUI::toggle, this, &menuEnum), this));

    event.registerEvent(Event::MASTER_VOLUME_UPDATE, MEMBER_CALLBACK(NuklearGUI::setMasterVolume));
    event.registerEvent(Event::MUSIC_VOLUME_UPDATE, MEMBER_CALLBACK(NuklearGUI::setMusicVolume));
    event.registerEvent(Event::EFFECTS_VOLUME_UPDATE, MEMBER_CALLBACK(NuklearGUI::setEffectsVolume));
    event.registerEvent(Event::UPDATE_ALL_CAMPAIGN_STARS, MEMBER_CALLBACK(NuklearGUI::setStarsCampaign));

    event.registerEvent(Event::SCREEN_FORMAT_UPDATE, MEMBER_CALLBACK(NuklearGUI::updateScreenFormat));
    event.registerEvent(Event::BONUS_ACTIVATE, MEMBER_CALLBACK(NuklearGUI::updateHumanPlayerBonus));

    event.registerEvent(Event::START_ANIMATION, MEMBER_CALLBACK(NuklearGUI::startAnimation));

    event.registerEvent(Event::NEW_GAME, MEMBER_CALLBACK(NuklearGUI::resetHumanPlayerBonus));
    event.registerEvent(Event::RESTART_GAME, MEMBER_CALLBACK(NuklearGUI::resetHumanPlayerBonus));
    
    start_time = std::chrono::steady_clock::now();
    frames = 0;
    fps = 0.f;
    
    _keyToChange = nullptr;

   // struct nk_color white =  nk_rgba(232, 233, 217, 255);
    struct nk_color black =  nk_rgba(35, 33, 44, 255);

    ctx->style.button.rounding = 0;
    ctx->style.button.border = 0;
    ctx->style.button.text_hover = black;
    ctx->style.contextual_button.text_hover = black;

}

NuklearGUI::~NuklearGUI() {
    nk_sdl_shutdown();    
}

NuklearGUI::NuklearGUI(NuklearGUI const & src) : win(src.getSGW()), camera(src.getCamera()), event(SEventManager::getInstance()) {
    *this = src;
}

NuklearGUI&     NuklearGUI::operator=(NuklearGUI const &) {
    return *this;
}

Sdl_gl_win&     NuklearGUI::getSGW() const {
    return win;
}

Camera&         NuklearGUI::getCamera() const {
    return camera;
}

struct nk_context * NuklearGUI::getContext () const { return ctx; }

void    NuklearGUI::toggle(void *p) {  
    Menu::Enum *m = static_cast<Menu::Enum *>(p);
    if (*m == Menu::BASE && SGameManager::getInstance().is_game_active() == false)
        return;
    if (*m == Menu::OPTIONS)
        _reset_options_display = true;
    if (*m == Menu::KEY_BINDINGS)
        _reset_key_bindings_display = true; 
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
    UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
    event.raise(Event::UI_AUDIO, &audio);
}

void    NuklearGUI::setupProportions() {
    float spacingY =  ctx->style.window.spacing.y;
    float paddingY =  ctx->style.window.padding.y;

    SDL_GetWindowSize(win.getWin(), &windowWidth, &windowHeight);   
    menuWidth = windowWidth / 3;
    optionHeight = static_cast<float>(windowHeight) / 10.f;
    menuHeight = optionHeight * 7.f + spacingY * 7.f + paddingY * 2.f;
    setupFont();
    screenFormatUpdate = false;  
}

void    NuklearGUI::setupFont() {
    if (atlas) nk_font_atlas_cleanup(atlas);   
    
    nk_sdl_font_stash_begin(&atlas);
    bigFont = nk_font_atlas_add_from_file(atlas, "assets/fonts/kenvector_future_thin.ttf", optionHeight, 0);
    nk_sdl_font_stash_end();
    
    nk_sdl_font_stash_begin(&atlas);
    mediumFont = nk_font_atlas_add_from_file(atlas, "assets/fonts/kenvector_future_thin.ttf", optionHeight / 2.f, 0);
    nk_sdl_font_stash_end();
    
    nk_sdl_font_stash_begin(&atlas);
    smallFont = nk_font_atlas_add_from_file(atlas, "assets/fonts/kenvector_future_thin.ttf", optionHeight / 4.f, 0);
    nk_sdl_font_stash_end();
    
    nk_style_set_font(ctx, &smallFont->handle);    
}

void    NuklearGUI::handleKey(void * p) {
    SDL_Keycode key = *static_cast<int*>(p);
    if (_keyToChange != nullptr) {
        *_keyToChange = key;
        _keyToChange = nullptr;
    }
}

void    NuklearGUI::render(bool game_is_active) {
    
    if (screenFormatUpdate) {
        setupProportions();
    }

    if (game_is_active) {
        renderCountDown();
        renderHUD();
    }
    else {
        renderBackgroundImage();
    }
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
            case Menu::NEW_BRAWL:           renderNewBrawlMenu(); break;
            case Menu::HOW_TO_PLAY:         renderHowToPlayMenu(); break;
            case Menu::GAME_OVER:           renderGameOverMenu(); break;
            case Menu::BRAWL_WIN:           renderBrawlWinMenu(); break;
        }
    }
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

    UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
    event.raise(Event::UI_AUDIO, &audio);
}

void    NuklearGUI::renderKeyBindings() {
    static std::map<Event::Enum, SDL_Keycode>  displayedKeysMap = win.getKeyMap();
    
    if (_reset_key_bindings_display){
        displayedKeysMap = win.getKeyMap();
        _reset_key_bindings_display = false;
    }

    std::string left =  SDL_GetKeyName(displayedKeysMap[Event::HUMAN_PLAYER_LEFT]);
    std::string right = SDL_GetKeyName(displayedKeysMap[Event::HUMAN_PLAYER_RIGHT]);
    std::string up =    SDL_GetKeyName(displayedKeysMap[Event::HUMAN_PLAYER_UP]);
    std::string down =  SDL_GetKeyName(displayedKeysMap[Event::HUMAN_PLAYER_DOWN]);
    std::string drop =  SDL_GetKeyName(displayedKeysMap[Event::HUMAN_SPAWN_BOMB]);

    if (nk_begin(ctx, "KEY BINDINGS", nk_rect(windowWidth / 2 - menuWidth, windowHeight / 2 - menuHeight / 2, menuWidth * 2, menuHeight),
    NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move up", NK_TEXT_CENTERED);
        hover(1);
        if (nk_button_label(ctx, up.c_str())) {
            bindKeyToEvent(Event::HUMAN_PLAYER_UP, displayedKeysMap);            
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move down", NK_TEXT_CENTERED);
        hover(2);
        if (nk_button_label(ctx, down.c_str())) {
            bindKeyToEvent(Event::HUMAN_PLAYER_DOWN, displayedKeysMap);            
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move left", NK_TEXT_CENTERED);
        hover(3);
        if (nk_button_label(ctx, left.c_str())) {
            bindKeyToEvent(Event::HUMAN_PLAYER_LEFT, displayedKeysMap);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Move right", NK_TEXT_CENTERED);
        hover(4);
        if (nk_button_label(ctx, right.c_str())) {
            bindKeyToEvent(Event::HUMAN_PLAYER_RIGHT, displayedKeysMap);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Drop bomb", NK_TEXT_CENTERED);
        hover(5);
        if (nk_button_label(ctx, drop.c_str())) {
            bindKeyToEvent(Event::HUMAN_SPAWN_BOMB, displayedKeysMap);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_spacing(ctx, 1);       
        hover(6);
        if (nk_button_label(ctx, "Reset to default"))
        {
            displayedKeysMap[Event::HUMAN_PLAYER_UP] = SDLK_w;
            displayedKeysMap[Event::HUMAN_PLAYER_DOWN] = SDLK_s;
            displayedKeysMap[Event::HUMAN_PLAYER_LEFT] = SDLK_a;
            displayedKeysMap[Event::HUMAN_PLAYER_RIGHT] = SDLK_d;
            displayedKeysMap[Event::HUMAN_SPAWN_BOMB] = SDLK_SPACE;
            UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
            event.raise(Event::UI_AUDIO, &audio);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        hover(6);
        if (nk_button_label(ctx, "Back"))
        {
            displayedKeysMap = win.getKeyMap();
            Menu::Enum menuEnum = Menu::Enum(Menu::KEY_BINDINGS);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }
        hover(7);
        if (nk_button_label(ctx, "Apply"))
        {
            event.raise(Event::KEY_MAP_UPDATE, &displayedKeysMap);
            Menu::Enum menuEnum = Menu::Enum(Menu::KEY_BINDINGS);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }
        
    }
    nk_end(ctx);
}

void    NuklearGUI::renderBackgroundImage() {
    static struct nk_image image = loadImage("assets/textures/minimal_bomberman_BG.png", GL_RGBA);
    struct nk_style_item tmp = ctx->style.window.fixed_background;
    ctx->style.window.fixed_background = nk_style_item_image(image);

    if (nk_begin(ctx, "BACKGROUND", nk_rect(1, 1, windowWidth - 1, windowHeight - 1),
    NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_NOT_INTERACTIVE)) {
        nk_end(ctx); 
    }    
    ctx->style.window.fixed_background = tmp;
}


void    NuklearGUI::renderOptions() {

    static Screen::Format                       displayedFormat = screenFormat;  
    static std::vector<SDL_DisplayMode>         modes(win.getDisplayModes()); 
    
    static unsigned long master = static_cast<unsigned long>(_masterVolume * 100.f);
    static unsigned long music = static_cast<unsigned long>(_musicVolume);
    static unsigned long effects = static_cast<unsigned long>(_effectsVolume);

    if (_reset_options_display == true){
        displayedFormat = screenFormat; 
        modes = std::vector<SDL_DisplayMode>(win.getDisplayModes()); 
        master = static_cast<unsigned long>(_masterVolume * 100.f);
        music = static_cast<unsigned long>(_musicVolume);
        effects = static_cast<unsigned long>(_effectsVolume);
        _reset_options_display = false;
    }
    
    std::string screenResString = toString(displayedFormat.displayMode);
    std::string screenModeString = toString(displayedFormat.windowMode);

    if (nk_begin(ctx, "OPTIONS", nk_rect(windowWidth / 2 - menuWidth, windowHeight / 2 - menuHeight / 2, menuWidth * 2, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
    {
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Screen resolution", NK_TEXT_CENTERED);
        hover(4);
        if (nk_menu_begin_label(ctx, screenResString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth, menuHeight))) {
            nk_layout_row_dynamic(ctx, optionHeight, 1);
            
            for (size_t i = 0; i < modes.size(); ++i) {
                hover(11 + i);
                if (nk_menu_item_label(ctx, toString(modes[i]).c_str(), NK_TEXT_CENTERED)) {
                    UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
                    event.raise(Event::UI_AUDIO, &audio);
                    displayedFormat.displayMode = modes[i];
                }
            }
            nk_menu_end(ctx);
        }
        
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Screen mode", NK_TEXT_CENTERED);  
        hover(5);   
        if (nk_menu_begin_label(ctx, screenModeString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth, menuHeight))) {

            nk_layout_row_dynamic(ctx, optionHeight, 1);
            hover(6);
            if (nk_menu_item_label(ctx, "WINDOWED", NK_TEXT_CENTERED)) {
                UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
                event.raise(Event::UI_AUDIO, &audio);
                displayedFormat.windowMode = Screen::WindowMode::WINDOWED;
            }

            hover(7);
            if (nk_menu_item_label(ctx, "FULLSCREEN", NK_TEXT_CENTERED)) {
                UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
                event.raise(Event::UI_AUDIO, &audio);
                displayedFormat.windowMode = Screen::WindowMode::FULLSCREEN;
            }
            nk_menu_end(ctx);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Master volume", NK_TEXT_CENTERED);    
        hover(1);
        nk_progress(ctx, &master, 100, NK_MODIFIABLE);

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Music volume", NK_TEXT_CENTERED);  
        hover(2);           
        nk_progress(ctx, &music, MIX_MAX_VOLUME, NK_MODIFIABLE);
        
        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Effects volume", NK_TEXT_CENTERED); 
        hover(3);            
        nk_progress(ctx, &effects, MIX_MAX_VOLUME, NK_MODIFIABLE);

        nk_layout_row_dynamic(ctx, optionHeight, 2);  
        nk_label(ctx, "Key bindings", NK_TEXT_CENTERED);  
        hover(8);   
        if (nk_button_label(ctx, "Configure"))
        {
            Menu::Enum menuEnum = Menu::Enum(Menu::KEY_BINDINGS);
            event.raise(Event::GUI_TOGGLE, &menuEnum);  
        }

        nk_layout_row_dynamic(ctx, optionHeight, 2); 
        hover(9);
        if (nk_button_label(ctx, "Back"))
        {
            displayedFormat = screenFormat;
            Menu::Enum menuEnum = Menu::Enum(Menu::OPTIONS);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
            master = static_cast<unsigned long>(_masterVolume * 100);
            music = static_cast<unsigned long>(_musicVolume);
            effects = static_cast<unsigned long>(_effectsVolume);
        }
        hover(10); 
        if (nk_button_label(ctx, "Apply"))
        {
            if (screenFormat.displayMode.w != displayedFormat.displayMode.w
                || screenFormat.displayMode.h != displayedFormat.displayMode.h
                || screenFormat.windowMode != displayedFormat.windowMode) {
                    screenFormat = displayedFormat;
                    event.raise(Event::SCREEN_FORMAT_UPDATE, &displayedFormat);  
            }
            
            if (static_cast<float>(master) != _masterVolume) {
                float   v = static_cast<float>(master) / 100;
                event.raise(Event::MASTER_VOLUME_UPDATE, &v);
            }

            if (static_cast<float>(music) != _musicVolume) {
                float   v = static_cast<float>(music);
                event.raise(Event::MUSIC_VOLUME_UPDATE, &v);
            }

            if (static_cast<float>(effects) != _effectsVolume) {
                float   v = static_cast<float>(effects);
                event.raise(Event::EFFECTS_VOLUME_UPDATE, &v);
            }

            Menu::Enum menuEnum = Menu::Enum(Menu::OPTIONS);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }
    }
    nk_end(ctx);
}

void    NuklearGUI::renderMenu() {
    if (nk_begin(ctx, "MENU", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
    {
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(1);
        if (nk_button_label(ctx, "Resume"))
        {
            Menu::Enum menuEnum = Menu::Enum(Menu::NONE);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(2);
        if (nk_button_label(ctx, "Restart level"))
        {
            Menu::Enum menuEnum = Menu::Enum(Menu::BASE);
            event.raise(Event::GUI_TOGGLE, &menuEnum);                
            event.raise(Event::RESTART_GAME, nullptr);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(3);
        if (nk_button_label(ctx, "How to play"))
        {
            Menu::Enum menuEnum = Menu::Enum(Menu::HOW_TO_PLAY);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(4);
        if (nk_button_label(ctx, "Options"))
        {
            Menu::Enum menuEnum = Menu::Enum(Menu::OPTIONS);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(5);
        if (nk_button_label(ctx, "Save"))
        {
            /* save */
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(6);
        if (nk_button_label(ctx, "Back to main menu"))
        {
            event.raise(Event::GAME_FINISH, nullptr);
            event.raise(Event::END_END_ANIMATION, nullptr);
            Menu::Enum menuEnum = Menu::Enum(Menu::START);
            event.raise(Event::GUI_TOGGLE, &menuEnum);  
        }
   
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(7); 
        if (nk_button_label(ctx, "Quit game"))
        {
            UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
            event.raise(Event::UI_AUDIO, &audio);
            event.raise(Event::QUIT_GAME, nullptr);
        }
    }
    nk_end(ctx);
}

void    NuklearGUI::renderLevelSelection() {

    static GameParams game_params(GameMode::CAMPAIGN, Level::ONE, PlayerColor::WHITE, 1, Difficulty::EASY);
    static struct nk_image  levelImage = loadImage("assets/textures/level1.png", GL_RGBA);
    static Level::Enum      level = Level::ONE;

    struct nk_vec2 spacing = ctx->style.window.spacing;
    struct nk_vec2 padding = ctx->style.window.padding;
    float   imageSize = (menuHeight - optionHeight) - padding.y * 2 - spacing.y * 2;
    float   buttonsXOffset = (windowWidth / 2 - menuWidth) / 2;

    static struct nk_image  fullStar = loadImage("assets/textures/star_full.png", GL_RGBA);
    static struct nk_image  emptyStar = loadImage("assets/textures/star_empty.png", GL_RGBA);

    if (nk_begin(ctx, "LEFT PREVIOUS", nk_rect(buttonsXOffset, windowHeight / 2 - optionHeight / 2, optionHeight, optionHeight),
    NK_WINDOW_BORDER |NK_WINDOW_NO_SCROLLBAR ))
    {
        nk_layout_row_dynamic(ctx, optionHeight - padding.y * 2 - spacing.y * 2, 1);
        hover(1);
        if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT)) {
            if (level > Level::ONE)
            {
                level = static_cast<Level::Enum>(level - 1);
                game_params.set_level(level);
                levelImage = loadImage("assets/textures/level" + std::to_string(level + 1) + ".png", GL_RGBA);
                UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
                event.raise(Event::UI_AUDIO, &audio);
            }
        }        
    }    
    nk_end(ctx);

    if (nk_begin(ctx, "RIGHT NEXT", nk_rect(windowWidth - buttonsXOffset - optionHeight, windowHeight / 2 - optionHeight / 2, optionHeight, optionHeight),
    NK_WINDOW_BORDER |NK_WINDOW_NO_SCROLLBAR ))
    {
        nk_layout_row_dynamic(ctx, optionHeight - padding.y * 2 - spacing.y * 2, 1);   
        hover(2);
        if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT)) {
            if (level < Level::SIX)
            {
                level = static_cast<Level::Enum>(level + 1);
                game_params.set_level(level);
                levelImage = loadImage("assets/textures/level" + std::to_string(level + 1) + ".png", GL_RGBA);
                UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
                event.raise(Event::UI_AUDIO, &audio);
            }
        }        
    }    
    nk_end(ctx);

    if (nk_begin(ctx, "LEVEL SELECTION", nk_rect(windowWidth / 2 - menuWidth, windowHeight / 2 - menuHeight / 2, menuWidth * 2, menuHeight),
    NK_WINDOW_BORDER |NK_WINDOW_NO_SCROLLBAR ))
    {
        nk_layout_row_dynamic(ctx, imageSize, 1);
        hover(3);
        if (nk_button_image(ctx, levelImage))
        {
            Menu::Enum  menu = Menu::NONE;
            event.raise(Event::GUI_TOGGLE, &menu);
            _human_player_color = PlayerColor::WHITE;
            _human_player_bonus = glm::ivec3(1, 1, 1);
            event.raise(Event::NEW_GAME, &game_params);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 8);
        hover(4);
        if (nk_button_label(ctx, "Back")) {
            event.raise(Event::END_END_ANIMATION, nullptr);
            Menu::Enum menuEnum = Menu::Enum(Menu::LEVEL_SELECTION);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }
        nk_spacing(ctx, 1);
        nk_label(ctx, "Level ", NK_TEXT_CENTERED);
        nk_label(ctx, std::string(std::to_string(level + 1)).c_str(), NK_TEXT_CENTERED);
        (_starsCampaign[level] >= 1) ? nk_image(ctx, fullStar) : nk_image(ctx, emptyStar);
        (_starsCampaign[level] >= 2) ? nk_image(ctx, fullStar) : nk_image(ctx, emptyStar);
        (_starsCampaign[level] >= 3) ? nk_image(ctx, fullStar) : nk_image(ctx, emptyStar);

        hover(5);
        if (nk_button_label(ctx, "Go !"))
        {
            Menu::Enum  menu = Menu::NONE;
            event.raise(Event::GUI_TOGGLE, &menu);
            _human_player_color = PlayerColor::WHITE;
            _human_player_bonus = glm::ivec3(1, 1, 1);
            event.raise(Event::NEW_GAME, &game_params);
        }
    }
    nk_end(ctx);
}

static std::string     color_to_image(PlayerColor::Enum color){
    switch (color) {
        case PlayerColor::WHITE :   return ("assets/textures/white.png");
        case PlayerColor::BLACK :   return ("assets/textures/black.png");
        case PlayerColor::RED :   return ("assets/textures/red.png");
        case PlayerColor::YELLOW :   return ("assets/textures/yellow.png");
    }
}
void    NuklearGUI::renderNewBrawlMenu() {

    static GameParams game_params(GameMode::BRAWL, Level::ONE, PlayerColor::WHITE, 1, Difficulty::EASY);
    static struct nk_image  levelImage = loadImage("assets/textures/Brawl_bombermans.png", GL_RGBA);
    static struct nk_image  playerImage = loadImage(color_to_image(game_params.get_color()).c_str(), GL_RGBA);    

    if (nk_begin(ctx, "NEW BRAWL", nk_rect(windowWidth / 2 - menuWidth, windowHeight / 2 - menuHeight / 2, menuWidth * 2, menuHeight),
    NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {

        nk_layout_row_dynamic(ctx, optionHeight * 4, 1);
        nk_image(ctx, levelImage);

        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Player", NK_TEXT_CENTERED);
        hover(1);
        if (nk_button_image_label(ctx, playerImage, toString(game_params.get_color()).c_str(), NK_TEXT_CENTERED)) {
            game_params.set_color(game_params.get_color() == PlayerColor::YELLOW ? PlayerColor::WHITE : static_cast<PlayerColor::Enum>(game_params.get_color() + 1));
            playerImage = loadImage(color_to_image(game_params.get_color()).c_str(), GL_RGBA);
            UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
            event.raise(Event::UI_AUDIO, &audio);
        }        
        
        nk_layout_row_dynamic(ctx, optionHeight, 2);
        nk_label(ctx, "Enemies", NK_TEXT_CENTERED);
        hover(2);
        if (nk_button_label(ctx, std::to_string(game_params.get_brawl_enemy_nb()).c_str())) {
            game_params.set_brawl_enemy_nb(game_params.get_brawl_enemy_nb() + 1);
            UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
            event.raise(Event::UI_AUDIO, &audio);
        }
            
        nk_layout_row_dynamic(ctx, optionHeight, 2);   
        hover(3);
        if (nk_button_label(ctx, "Back")) {
            Menu::Enum menuEnum = Menu::Enum(Menu::NEW_BRAWL);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }    
        hover(4);
        if (nk_button_label(ctx, "Go !")) {
            Menu::Enum  menu = Menu::NONE;
            event.raise(Event::GUI_TOGGLE, &menu);
            _human_player_color = game_params.get_color();
            _human_player_bonus = glm::ivec3(1, 1, 1);
            event.raise(Event::NEW_GAME, &game_params);
        }   
        nk_end(ctx);
    }
}

void    NuklearGUI::renderGameOverMenu() {
    static float    tmp_h = menuHeight / 7.0f * 5.0f;

    if (nk_begin(ctx, "GAME OVER", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - tmp_h / 2, menuWidth, tmp_h),
    NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {

        nk_style_set_font(ctx, &bigFont->handle);
        nk_layout_row_dynamic(ctx, optionHeight * 2, 1);
        nk_label(ctx, "Game Over", NK_TEXT_CENTERED);  

        nk_style_set_font(ctx, &mediumFont->handle);               
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(1);
        if (nk_button_label(ctx, "Retry")) {
            Menu::Enum menuEnum = Menu::Enum(Menu::GAME_OVER);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
            event.raise(Event::RESTART_GAME, nullptr);
        }       

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(1);
        if (nk_button_label(ctx, "Back to main menu")) {
            event.raise(Event::GAME_FINISH, nullptr);
            Menu::Enum menuEnum = Menu::Enum(Menu::START);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
            event.raise(Event::END_END_ANIMATION, nullptr);
        }
        
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(1);
        if (nk_button_label(ctx, "Quit game")) {
            event.raise(Event::QUIT_GAME, nullptr);  
        }  
    }
    nk_end(ctx);
}

void    NuklearGUI::renderBrawlWinMenu() {
    static float    tmp_h = menuHeight / 7.0f * 5.0f;

    if (nk_begin(ctx, "GAME OVER", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - tmp_h / 2, menuWidth, tmp_h),
    NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {

        nk_style_set_font(ctx, &bigFont->handle);
        nk_layout_row_dynamic(ctx, optionHeight * 2, 1);
        nk_label(ctx, "You Win !", NK_TEXT_CENTERED);  

        nk_style_set_font(ctx, &mediumFont->handle);               
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        if (nk_button_label(ctx, "Play Again")) {
            Menu::Enum menuEnum = Menu::Enum(Menu::GAME_OVER);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
            event.raise(Event::RESTART_GAME, nullptr);
            event.raise(Event::END_END_ANIMATION, nullptr);        
        }       

        nk_layout_row_dynamic(ctx, optionHeight, 1);
        if (nk_button_label(ctx, "Back to main menu")) {
            Menu::Enum menuEnum = Menu::Enum(Menu::START);
            event.raise(Event::GUI_TOGGLE, &menuEnum);

            event.raise(Event::END_END_ANIMATION, nullptr);               
        }
        
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        if (nk_button_label(ctx, "Quit game")) {
            event.raise(Event::QUIT_GAME, nullptr);
        }  
    }
    nk_end(ctx);
}

void    NuklearGUI::renderHowToPlayMenu() {

    static  struct nk_image bomb = loadImage("./assets/textures/Bombupsprite.png", GL_RGB);
    static  struct nk_image flame = loadImage("./assets/textures/Fireupsprite.png", GL_RGB);
    static  struct nk_image speed = loadImage("./assets/textures/Skatesprite.png", GL_RGB);

    if (nk_begin(ctx, "HOW TO PLAY", nk_rect(windowWidth / 2 - menuWidth, windowHeight / 2 - menuHeight / 2, menuWidth * 2, menuHeight),
    NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {

        nk_style_set_font(ctx, &bigFont->handle);
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        nk_label(ctx, "How to play", NK_TEXT_CENTERED);

        nk_style_set_font(ctx, &mediumFont->handle);        
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        nk_label(ctx, "Campaign", NK_TEXT_CENTERED); 
        
        nk_style_set_font(ctx, &smallFont->handle);
        nk_layout_row_dynamic(ctx, optionHeight / 2, 1);
        nk_label(ctx, "Destroy all breakable blocks using your bombs as little as possible !", NK_TEXT_CENTERED);
        
        nk_style_set_font(ctx, &mediumFont->handle);
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        nk_label(ctx, "Brawl", NK_TEXT_CENTERED);
            
        nk_style_set_font(ctx, &smallFont->handle);
        nk_layout_row_dynamic(ctx, optionHeight / 2, 1);
        nk_label(ctx, "Destroy all your enemies, the last player to be alive wins !", NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, optionHeight / 2, 1);
        nk_label(ctx, "Use bonuses at your advantage...", NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, optionHeight / 2, 1);

        nk_layout_row_dynamic(ctx, optionHeight, 9);
        nk_spacing(ctx, 2);

        if (nk_widget_is_hovered(ctx))
            nk_tooltip(ctx, "Drop more bombs !");
        nk_image(ctx, bomb);

        nk_spacing(ctx, 1);

        if (nk_widget_is_hovered(ctx))
            nk_tooltip(ctx, "Your bombs have more range !");
        nk_image(ctx, flame);

        nk_spacing(ctx, 1);

        if (nk_widget_is_hovered(ctx))
            nk_tooltip(ctx, "Run faster !");
        nk_image(ctx, speed);
        
        nk_style_set_font(ctx, &mediumFont->handle);
        nk_layout_row_dynamic(ctx, optionHeight, 4);
        hover(1);
        if (nk_button_label(ctx, "Back")) {
            Menu::Enum menuEnum = Menu::Enum(Menu::HOW_TO_PLAY);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }  

        nk_end(ctx);
    }
}

void    NuklearGUI::renderStartMenu() {
    
    static bool confirmationPopup = false;
    static struct nk_vec2 spacing = ctx->style.window.spacing;
    static struct nk_vec2 padding = ctx->style.window.padding;
    
    nk_style_set_font(ctx, &mediumFont->handle);
    
    if (nk_begin(ctx, "START MENU", nk_rect(windowWidth / 2 - menuWidth / 2, windowHeight / 2 - menuHeight / 2, menuWidth, menuHeight),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
    {
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(1);
        if (nk_button_label(ctx, "Campaign"))
        {
            Menu::Enum  me = Menu::LEVEL_SELECTION;
            event.raise(Event::GUI_TOGGLE, &me);
        }

        nk_layout_row_dynamic(ctx, optionHeight, 1); 
        hover(2); 
        if (nk_button_label(ctx, "Brawl"))
        {
            Menu::Enum  me = Menu::NEW_BRAWL;
            event.raise(Event::GUI_TOGGLE, &me);
        }
        nk_layout_row_dynamic(ctx, optionHeight, 1);  
        hover(3);
        if (nk_button_label(ctx, "How to play"))
        {
            Menu::Enum  me = Menu::HOW_TO_PLAY;
            event.raise(Event::GUI_TOGGLE, &me);
        }
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(4);
        if (nk_button_label(ctx, "Options"))
        {
            Menu::Enum menuEnum = Menu::Enum(Menu::OPTIONS);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(5);
        if (nk_button_label(ctx, "Reset progression"))
        {
            UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
            event.raise(Event::UI_AUDIO, &audio);
            confirmationPopup = true;
        }
        if (confirmationPopup)
        {
            if (nk_popup_begin(ctx, NK_POPUP_STATIC, "CONFIRMATION", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER,
            nk_rect(0, menuHeight / 4.f, menuWidth, optionHeight * 2.f)))
            {
                nk_layout_row_dynamic(ctx, optionHeight - spacing.y - padding.y, 1);
                nk_label(ctx, "Are you sure ?", NK_TEXT_CENTERED);
                nk_layout_row_dynamic(ctx, optionHeight - spacing.y - padding.y, 2);
                if (nk_button_label(ctx, "Yes")) {
                    SGameManager::getInstance().getCurrentSlot().reset_stars_campaign();            
                    confirmationPopup = 0;
                    UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
                    event.raise(Event::UI_AUDIO, &audio);
                    nk_popup_close(ctx);
                }
                if (nk_button_label(ctx, "No")) {
                    confirmationPopup = 0;
                    UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
                    event.raise(Event::UI_AUDIO, &audio);
                    nk_popup_close(ctx);
                }
                nk_popup_end(ctx);
            } else confirmationPopup = nk_false;
        }
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(6);
        if (nk_button_label(ctx, "Slot selection"))
        {
            Menu::Enum menuEnum = Menu::Enum(Menu::SELECT_SLOT);
            event.raise(Event::GUI_TOGGLE, &menuEnum);
        }
        nk_layout_row_dynamic(ctx, optionHeight, 1);
        hover(7);
        if (nk_button_label(ctx, "Quit Game"))
        {
            UIAudio::Enum audio = UIAudio::Enum(UIAudio::CLICK);
            event.raise(Event::UI_AUDIO, &audio);
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

static std::string     color_to_HUD_image(PlayerColor::Enum color){
    switch (color) {
        case PlayerColor::WHITE :   return ("assets/textures/white_HUD.png");
        case PlayerColor::BLACK :   return ("assets/textures/black_HUD.png");
        case PlayerColor::RED :   return ("assets/textures/red_HUD.png");
        case PlayerColor::YELLOW :   return ("assets/textures/yellow_HUD.png");
    }
}

void    NuklearGUI::updateHumanPlayerBonus(void *p){
    std::pair<Bonus *, Player *> *pair = static_cast<std::pair<Bonus *, Player *> *>(p);
    if (pair->second->getColor() == _human_player_color){
        switch (pair->first->getBonusType()){
            case BonusType::BOMB_UP:   _human_player_bonus.x += 1; break;
            case BonusType::FLAME_UP:  _human_player_bonus.y += 1; break;
            case BonusType::SPEED_UP:  _human_player_bonus.z += 1; break;
        }
    }
}

void    NuklearGUI::initRenderCountDown(){
    count_down_start_time = std::chrono::steady_clock::now();
}

void    NuklearGUI::renderCountDown(){
    static struct nk_image count_3 = loadImage("assets/textures/3.png", GL_RGBA);
    static struct nk_image count_2 = loadImage("assets/textures/2.png", GL_RGBA);
    static struct nk_image count_1 = loadImage("assets/textures/1.png", GL_RGBA);

    static struct nk_image count_go = loadImage("assets/textures/GO!.png", GL_RGBA);

    struct nk_image *ptr = nullptr;



    std::chrono::milliseconds   dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - count_down_start_time);

    if (dur.count() < 1000){
        ptr = &count_3;        
    }
    else if (dur.count() < 2000){
        ptr = &count_2;        
    }
    else if (dur.count() < 3000){
        ptr = &count_1;        
    }
    else if (dur.count() < 4000){
        ptr = &count_go;        
    }

    if (ptr){
        struct nk_style_item tmp = ctx->style.window.fixed_background;
        ctx->style.window.fixed_background = nk_style_item_image(*ptr);
        nk_style_set_font(ctx, &smallFont->handle);

        if (nk_begin(ctx, "COUNTDOWN", nk_rect(1, 1, windowWidth - 1, windowHeight - 1),
        NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_NOT_INTERACTIVE)) {
            nk_end(ctx); 
        }    
        
        ctx->style.window.fixed_background = tmp;
        nk_style_set_font(ctx, &mediumFont->handle);
        
        while(!_active_menu.empty())
            _active_menu.pop();
    }

}


void    NuklearGUI::renderHUD() {

    static struct nk_image portrait = loadImage(color_to_HUD_image(_human_player_color), GL_RGBA);

    static struct nk_vec2 spacing =  ctx->style.window.spacing; // between items

    float   avatar_w = windowWidth * 0.1f;
    float   avatar_h = avatar_w;
    float   avatar_x = 50;
    float   avatar_y = 50;
    float w = avatar_w + 8 * spacing.x;
    float h = avatar_h + 2 * spacing.y;
    
    float barHeight = avatar_h * 0.22f;

    struct nk_style_item tmp = ctx->style.window.fixed_background;
    ctx->style.window.fixed_background = nk_style_item_image(portrait);
    nk_style_set_font(ctx, &smallFont->handle);

    if (nk_begin(ctx, "TOP_LEFT_HUD", nk_rect(avatar_x, avatar_y, w, h),
    NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, avatar_h - barHeight, 1);
        nk_layout_row_static(ctx, barHeight, avatar_w / 8, 8);
        nk_spacing(ctx, 1);
        nk_spacing(ctx, 1);
        nk_label(ctx, std::to_string(_human_player_bonus.x).c_str(), NK_TEXT_LEFT);
        nk_spacing(ctx, 1);
        nk_label(ctx, std::to_string(_human_player_bonus.y).c_str(), NK_TEXT_LEFT);
        nk_spacing(ctx, 1);
        nk_label(ctx, std::to_string(_human_player_bonus.z).c_str(), NK_TEXT_LEFT);
        nk_spacing(ctx, 1);
    }
    nk_end(ctx);
    
    ctx->style.window.fixed_background = tmp;
    nk_style_set_font(ctx, &mediumFont->handle);    
}

void    NuklearGUI::renderDebug() {
    glm::vec3   camPos = camera.getPosition();
    glm::vec3   camFront = camera.getFront();
    std::string camPosString = std::to_string(camPos.x) + " : " + std::to_string(camPos.y) + " : " + std::to_string(camPos.z);
    std::string camFrontString = std::to_string(camFront.x) + " : " + std::to_string(camFront.y) + " : " + std::to_string(camFront.z);
    std::string camModeString = toString(camera.getMode());

    update_fps();
    std::string FPSString = std::to_string(static_cast<int>(fps));
    
    nk_style_set_font(ctx, &smallFont->handle);
     if (nk_begin(ctx, "DEBUG MODE", nk_rect(50, 50, menuWidth, (optionHeight / 3.f) * 7.f),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
    {
        nk_layout_row_dynamic(ctx, optionHeight / 3, 1);  
        nk_label(ctx, "Camera position", NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, optionHeight / 3, 1);  
        nk_label(ctx, camPosString.c_str(), NK_TEXT_CENTERED);
        
        nk_layout_row_dynamic(ctx, optionHeight / 3, 1);  
        nk_label(ctx, "Camera front", NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, optionHeight / 3, 1);          
        nk_label(ctx, camFrontString.c_str(), NK_TEXT_CENTERED);

        nk_layout_row_dynamic(ctx, optionHeight / 3, 2);  
        nk_label(ctx, "FPS : ", NK_TEXT_LEFT);
        nk_label(ctx, FPSString.c_str(), NK_TEXT_CENTERED);
        
        nk_layout_row_dynamic(ctx, optionHeight / 3, 2);
        nk_label(ctx, "Camera mode", NK_TEXT_LEFT);
        if (nk_menu_begin_label(ctx, camModeString.c_str(), NK_TEXT_CENTERED, nk_vec2(menuWidth / 2, menuHeight))) {
            nk_layout_row_dynamic(ctx, optionHeight / 3, 1);
            
            if (nk_menu_item_label(ctx, "FIXED", NK_TEXT_CENTERED)) {
                Camera::Mode camMode = Camera::Mode(Camera::Mode::FIXED);
                event.raise(Event::CAMERA_MODE_UPDATE, &camMode);
            }
            if (nk_menu_item_label(ctx, "FREE", NK_TEXT_CENTERED)) {
                Camera::Mode camMode = Camera::Mode(Camera::Mode::FREE);
                event.raise(Event::CAMERA_MODE_UPDATE, &camMode);
            }
            if (nk_menu_item_label(ctx, "FOLLOW PLAYER", NK_TEXT_CENTERED)) {
                Camera::Mode camMode = Camera::Mode(Camera::Mode::FOLLOW_PLAYER);
                event.raise(Event::CAMERA_MODE_UPDATE, &camMode);
            }
            nk_menu_end(ctx);
        }
    }
    nk_end(ctx);
}

void            NuklearGUI::renderSelectSlot(void){
    SGameManager & gm = SGameManager::getInstance();

    std::string dates[3];
    std::string times[3];
    int         progress[3];

    for (int i = 0; i < 3; ++i)
    {
        std::string lss = gm.getSlot(i).get_last_save_string();
        dates[i] = lss.substr(0, 10);
        times[i] = lss.substr(11, 8);

        int                 total = 0;
        std::vector<int>    v = gm.getSlot(i).get_all_stars_campaign();
        std::for_each(v.begin(), v.end(), [&] (int n) {
            total += n;
        });
        progress[i] = map(total, 0, 18, 0, 100); // This 18 is hardcoded: 3 * number of levels

        renderSlot(i, progress[i], dates[i], times[i]);
    }
}

void    NuklearGUI::renderSlot(int n, int progress, std::string date, std::string time) {
    float slotWidth = windowWidth * 0.25f;

    struct nk_style_item tmp = ctx->style.window.fixed_background;
    ctx->style.window.fixed_background = nk_style_item_hide();

    std::string s = "SLOT " + std::to_string(n + 1);
    float       xOffset[3] = { 0.2f, 0.5f, 0.8f };

    if (nk_begin(ctx, s.c_str(), nk_rect(windowWidth * xOffset[n] - slotWidth / 2, windowHeight / 2 - slotWidth / 2, slotWidth, slotWidth * 3),
    NK_WINDOW_NO_SCROLLBAR)) {
        nk_style_set_font(ctx, &bigFont->handle);
        nk_layout_row_dynamic(ctx, optionHeight, 1); 
        hover(n); 
        if (nk_button_label(ctx, s.c_str())) {
            Save::Enum  slot = static_cast<Save::Enum>(n);
            event.raise(Event::LOAD_SLOT, &slot);

            Menu::Enum  me = Menu::START;
            event.raise(Event::GUI_TOGGLE, &me);
        }
        nk_layout_row_dynamic(ctx, optionHeight / 4, 1);                  
        
        nk_style_set_font(ctx, &smallFont->handle);        
        nk_layout_row_dynamic(ctx, optionHeight / 4, 1);  
        nk_label(ctx, "Progression", NK_TEXT_CENTERED);

        nk_style_set_font(ctx, &mediumFont->handle);        
        nk_layout_row_dynamic(ctx, optionHeight / 2, 1);  
        nk_label(ctx, std::string(std::to_string(progress) + " %").c_str(), NK_TEXT_CENTERED);

        nk_layout_row_dynamic(ctx, optionHeight / 4, 1);          
        
        nk_style_set_font(ctx, &smallFont->handle);        
        nk_layout_row_dynamic(ctx, optionHeight / 4, 1);  
        nk_label(ctx, "Last save", NK_TEXT_CENTERED);

        nk_style_set_font(ctx, &mediumFont->handle);        
        nk_layout_row_dynamic(ctx, optionHeight / 2, 1);  
        nk_label(ctx, date.c_str(), NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, optionHeight / 2, 1);  
        nk_label(ctx, time.c_str(), NK_TEXT_CENTERED);        

        nk_end(ctx);      
    }
    ctx->style.window.fixed_background = tmp;    
}

void            NuklearGUI::hover(int id) const {
    static int hovered = 0;
    if (nk_widget_is_hovered(ctx) && hovered != id) {
        UIAudio::Enum audio = UIAudio::Enum(UIAudio::HOVER);
        event.raise(Event::UI_AUDIO, &audio);
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

std::string     NuklearGUI::toString(PlayerColor::Enum p) const {
    std::string player;
    switch (p) {
        case PlayerColor::WHITE:           player = "WHITE"; break;
        case PlayerColor::BLACK:           player = "BLACK"; break;
        case PlayerColor::RED:             player = "RED"; break;
        case PlayerColor::YELLOW:          player = "YELLOW"; break;
    }
    return player;
}

std::string     NuklearGUI::toString(Difficulty::Enum d) const {
    std::string difficulty;
    switch (d) {
        case Difficulty::EASY:             difficulty = "EASY"; break;
        case Difficulty::MEDIUM:           difficulty = "MEDIUM"; break;
        case Difficulty::HARD:             difficulty = "HARD"; break;
    }
    return difficulty;
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

void    NuklearGUI::setStarsCampaign(void *starsCampaign)
{
    _starsCampaign = std::vector<int>(*static_cast<std::vector<int> *>(starsCampaign));
}

void    NuklearGUI::updateScreenFormat(void *f) {
    Screen::Format  *format = static_cast<Screen::Format*>(f);
    screenFormat.displayMode = format->displayMode;
    screenFormat.windowMode = format->windowMode;
    screenFormatUpdate = true;
}

void    NuklearGUI::startAnimation(void *a){
    Animation::Enum *anim = static_cast<Animation::Enum *>(a);
    if (*anim == Animation::START){
        initRenderCountDown();
    }
}

void    NuklearGUI::resetHumanPlayerBonus(void *){
    _human_player_bonus = glm::ivec3(1, 1, 1);
}