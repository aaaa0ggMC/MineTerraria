#ifndef DATASTRS_H_INCLUDED
#define DATASTRS_H_INCLUDED

#define GLOBAL_GAME_CONFIG_PATH "config.json"

#define CURSOR_COUNT 2

//For better vision ^-^
#ifdef BUILD_ON_WINDOWS
    #define SEPL "\\"
#else
    #define SEPL "/"
#endif // BUILD_ON_WINDOWS

#define IMG_BASE "res" SEPL "imgs" SEPL
#define CURSOR_BASE "res" SEPL "cursors" SEPL
#define CURSOR_BEG 1
#define PLAYER_BASE "res" SEPL "imgs" SEPL "Players" SEPL

vector<string> texturePaths = {
    IMG_BASE "bg.png",
    CURSOR_BASE "curNorm.png",
    CURSOR_BASE "curCAct.png"
};


vector<string> nameTexs={
    "bg","norm","act"
};

#define AUDIO_BASE "res" SEPL "audios" SEPL
vector<string> musics = {
    AUDIO_BASE "peaceful.ogg"
};

#define SHADER_BASE "res/shaders/"
vector<string> shaders = {
    SHADER_BASE "glass_like.frag",
    SHADER_BASE "clouds.frag",
    SHADER_BASE "lifeRecord.frag"
};

#define SHADER_GLASS_LIKE_F 0
#define SHADER_CLOUDS_F 1
#define SHADER_REC 2

vector<string> showingStrings = {
    "Try playing Minecraft!!!!",
    "Try playing Terraria!!!!",
    "sqrt(-1) love you!!!!",
    "made by C++ SFML",
    "Wow!!!",
    "You look so happy!",
    "Consider to play Minecraft",
    "You can play Terraria",
    "This is " CURVERSION "!",
};

vector<string> folders = {
    SEPL "StudyAll",
    SEPL "StudyAll" SEPL "UnlimitedLife",
    SEPL "StudyAll" SEPL "UnlimitedLife" SEPL "Worlds",
    SEPL "StudyAll" SEPL "UnlimitedLife" SEPL "Cache"
};

#define FOLDER_CACHE 3

#define MODS_PATH "res" SEPL "mods"

#endif // DATASTRS_H_INCLUDED
