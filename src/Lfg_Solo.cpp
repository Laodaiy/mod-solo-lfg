/*
** Made by Traesh https://github.com/Traesh
** AzerothCore 2019 http://www.azerothcore.org/
** Conan513 https://github.com/conan513
** Made into a module by Micrah https://github.com/milestorme/
*/

#include "ScriptMgr.h"
#include "Player.h"
#include "Configuration/Config.h"
#include "World.h"
#include "LFGMgr.h"
#include "Chat.h"
#include "Opcodes.h"

class lfg_solo_announce : public PlayerScript
{
public:
    lfg_solo_announce() : PlayerScript("lfg_solo_announce") {}

    void OnPlayerLogin(Player* player) override
    {
        // Announce Module
        if (sConfigMgr->GetOption<bool>("SoloLFG.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("服务器已启用 |cff4CFF00单人地下城查找器 |r模块，此模块受渐进模块影响。");
        }
    }

    void OnPlayerRewardKillRewarder(Player* /*player*/, KillRewarder* /*rewarder*/, bool isDungeon, float& rate) override
    {
        if (!isDungeon
            || !sConfigMgr->GetOption<bool>("SoloLFG.Enable", true)
            || !sConfigMgr->GetOption<bool>("SoloLFG.FixedXP", true))
        {
            return;
        }

        // Force the rate to FixedXPRate regardless of group size, to encourage group play
        rate = sConfigMgr->GetOption<float>("SoloLFG.FixedXPRate", 0.2);
    }
};

class lfg_solo : public WorldScript
{
public:
    lfg_solo() : WorldScript("lfg_solo") {}

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        if (sConfigMgr->GetOption<bool>("SoloLFG.Enable", true) != sLFGMgr->IsTesting())
        {
            sLFGMgr->ToggleTesting();
        }
    }
};

void AddLfgSoloScripts()
{
    new lfg_solo_announce();
    new lfg_solo();
}
