#ifndef DATASTRS_H_INCLUDED
#define DATASTRS_H_INCLUDED


///游戏存储资源路径
#define GLOBAL_GAME_CONFIG_PATH "config.json"

#define IMG_BASE "res/imgs/"
#define PLAYER_BASE "res/imgs/Players/"
vector<string> texturePaths = {
    IMG_BASE "bg.png"
};


vector<string> nameTexs={
    "bg"
};

#define AUDIO_BASE "res/audios/"
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
    "\\StudyAll",
    "\\StudyAll\\UnlimitedLife",
    "\\StudyAll\\UnlimitedLife\\Mods",
    "\\StudyAll\\UnlimitedLife\\Worlds",
    "\\StudyAll\\UnlimitedLife\\Players",
    "\\StudyAll\\UnlimitedLife\\Cache"
};

#define FOLDER_CACHE 5

#endif // DATASTRS_H_INCLUDED
