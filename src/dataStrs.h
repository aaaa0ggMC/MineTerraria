#ifndef DATASTRS_H_INCLUDED
#define DATASTRS_H_INCLUDED

#define IMG_BASE "res/imgs/"
#define TILE_BASE "res/imgs/tiles/"
#define PLAYER_BASE "res/imgs/Players/"
vector<string> texturePaths = {
    IMG_BASE "bg0.png",
    IMG_BASE "bg1.png",
    IMG_BASE "sun.png",
    IMG_BASE "star.png",
    IMG_BASE "moon.png",
    TILE_BASE "tile_grass.png",
    IMG_BASE "cloud0.png",
    IMG_BASE "cloud1.png",
    IMG_BASE "cloud2.png",
    IMG_BASE "cloud3.png",
    IMG_BASE "cloud4.png",
    IMG_BASE "cloud5.png",
    IMG_BASE "cloud6.png",
    IMG_BASE "cloud7.png"
};


vector<string> nameTexs={
    "background0",
    "background1",
    "sun",
    "star",
    "moon",
    "tgrass",
    "cloud0",
    "cloud1",
    "cloud2",
    "cloud3",
    "cloud4",
    "cloud5",
    "cloud6",
    "cloud7",
};

#define AUDIO_BASE "res/audios/"
vector<string> musics = {
    AUDIO_BASE "Tale.ogg"
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
