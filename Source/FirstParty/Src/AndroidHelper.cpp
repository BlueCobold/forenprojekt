
#ifdef ANDROID

#include "AndroidHelper.hpp"
#include "Utility.hpp"

#include <android/asset_manager.h>
#include <android/native_activity.h>
#include <sys/stat.h>
#include <android/log.h>
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_INFO, "de.gamecoding.ricketyracquet", __VA_ARGS__))

#include <SFML/System/SystemHandle.hpp>
#include <SFML/System.hpp>

ANativeActivity* getActivity()
{
    static ANativeActivity* activity = nullptr;
    if(activity == nullptr)
    {
        activity = (ANativeActivity*)sf::getSystemHandle();
    }
    return activity;
}

std::string resourcePathAndroid()
{
    static std::string dataPath(getActivity()->internalDataPath);
    return dataPath + "/";
}

bool dirExists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

void copyAssetFolder(std::string folder)
{
    auto activity = getActivity();
    std::string dirSlash = folder.length() > 0 ? folder + "/" : "";
    std::string dataPath = resourcePathAndroid();

    if(folder != "" && dirExists((dataPath + folder).c_str()))
        return;

    int result_code = mkdir((dataPath + folder).c_str(), 0770);
    auto mgr = activity->assetManager;
    AAssetDir* assetDir = AAssetManager_openDir(mgr, folder.c_str());
    const char* filename = nullptr;
    while(filename = AAssetDir_getNextFileName(assetDir))
    {
        if(auto test = fopen((dataPath + dirSlash + filename).c_str(), "r"))
        {
            fclose(test);
            continue;
        }

        AAsset* asset = AAssetManager_open(mgr, (dirSlash + filename).c_str(), AASSET_MODE_STREAMING);
        char buf[BUFSIZ];
        int nb_read = 0;
        FILE* out = fopen((dataPath + dirSlash + filename).c_str(), "w");
        while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
            fwrite(buf, nb_read, 1, out);
        fclose(out);
        AAsset_close(asset);
    }
    AAssetDir_close(assetDir);
}


void showErrorAndroid(const std::string& msg)
{
    auto cmd = std::string("am startservice ")
                         + "--user 0 "
                         + "-n de.gamecoding.ricketyracquet/.AlertService "      // component
                         + "-c de.gamecoding.ricketyracquet.AlertService.ERROR " // category
                         + "-a de.gamecoding.ricketyracquet.AlertService.ERROR " // action
                         + "-e msg \"" + msg + "\"";                          // string extra
    system(cmd.c_str());
}

void init()
{
    copyAssetFolder("");
    copyAssetFolder("res");
    copyAssetFolder("res/audio");
    copyAssetFolder("res/bitmapfont");
    copyAssetFolder("res/key");
    copyAssetFolder("res/level");
    copyAssetFolder("res/level/assets");
    copyAssetFolder("res/level/assets/ball");
    copyAssetFolder("res/level/assets/spikes");
    copyAssetFolder("res/level/assets/target");
    copyAssetFolder("res/level/assets/teeter");
    copyAssetFolder("res/level/pastureland");
    copyAssetFolder("res/level/sandy");
    copyAssetFolder("res/menus");
    copyAssetFolder("res/spritesheet");
    copyAssetFolder("res/spritesheet/assets");
    copyAssetFolder("res/spritesheet/backgrounds");
    copyAssetFolder("res/spritesheet/gui");
}

int getMemoryClass()
{
    static int32_t memsize = 0;
    if(memsize != 0)
        return memsize;

    if(auto meminfo = fopen((resourcePathAndroid() + "/meminfo").c_str(), "r"))
    {
        fread(&memsize, sizeof(memsize), 1, meminfo);
        fclose(meminfo);
        return static_cast<int>(memsize);
    }
    else
        LOGE("Couldn't find memory info file");

    return 0;
}

#endif
