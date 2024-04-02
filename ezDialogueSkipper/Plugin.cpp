// This file added in headers queue
// File: "Sources.h"
#include "resource.h"

namespace GOTHIC_ENGINE {
  HOOK Ivk_CGameManager_HandleEvent PATCH(&CGameManager::HandleEvent, &CGameManager::HandleEvent_Union);
  int CGameManager::HandleEvent_Union(int key)
  {
    if (skipHotkey == Invalid)
    {
      return THISCALL(Ivk_CGameManager_HandleEvent)(key);
    }

    auto skipHotkeyPressed = zKeyPressed(skipHotkey);

    auto result = THISCALL(Ivk_CGameManager_HandleEvent)(key);
    if (!result
#if ENGINE >= Engine_G2
      || key != MOUSE_BUTTONRIGHT
#else
      || key != 2052
#endif
      ||
      oCInformationManager::GetInformationManager().HasFinished())
    {
      return result;
    }

    if (skipHotkeyPressed)
    {
      std::vector<oCNpc*> npcs{ player, player->talkOther, player->GetFocusNpc() };
      for (auto npc : npcs)
      {
        if (!npc)
        {
          continue;
        }

        auto msgList = npc->GetEM()->messageList;
        for (int i = 0; i < msgList.GetNumInList(); i++)
        {
          auto msg = msgList[i];
          if (!msg)
          {
            continue;
          }

          auto msgConv = zDYNAMIC_CAST<oCMsgConversation>(msg);

          if (!msgConv)
          {
            continue;
          }

          if (msgConv->subType == oCMsgConversation::EV_OUTPUT || msgConv->subType == oCMsgConversation::EV_OUTPUTSVM || msgConv->subType == oCMsgConversation::EV_OUTPUTSVM_OVERLAY || msgConv->subType == oCMsgConversation::EV_PLAYANI_NOOVERLAY)
          {
            msgConv->Delete();
            result++;
          }
        }
      }
    }

    return result;
  }

  void Game_Entry() {
  }

  void Game_Init() {
    skipHotkey = GetKey(zoptions->ReadString("ezDialogueSkipper", "HotKey", "KEY_RSHIFT"));
  }

  void Game_Exit() {
  }

  void Game_PreLoop() {
  }

  void Game_Loop() {
  }

  void Game_PostLoop() {
  }

  void Game_MenuLoop() {
  }

  // Information about current saving or loading world
  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

  void Game_SaveBegin() {
  }

  void Game_SaveEnd() {
  }

  void LoadBegin() {
  }

  void LoadEnd() {
  }

  void Game_LoadBegin_NewGame() {
    LoadBegin();
  }

  void Game_LoadEnd_NewGame() {
    LoadEnd();
  }

  void Game_LoadBegin_SaveGame() {
    LoadBegin();
  }

  void Game_LoadEnd_SaveGame() {
    LoadEnd();
  }

  void Game_LoadBegin_ChangeLevel() {
    LoadBegin();
  }

  void Game_LoadEnd_ChangeLevel() {
    LoadEnd();
  }

  void Game_LoadBegin_Trigger() {
  }

  void Game_LoadEnd_Trigger() {
  }

  void Game_Pause() {
  }

  void Game_Unpause() {
  }

  void Game_DefineExternals() {
  }

  void Game_ApplyOptions() {
  }

  /*
  Functions call order on Game initialization:
    - Game_Entry           * Gothic entry point
    - Game_DefineExternals * Define external script functions
    - Game_Init            * After DAT files init

  Functions call order on Change level:
    - Game_LoadBegin_Trigger     * Entry in trigger
    - Game_LoadEnd_Trigger       *
    - Game_Loop                  * Frame call window
    - Game_LoadBegin_ChangeLevel * Load begin
    - Game_SaveBegin             * Save previous level information
    - Game_SaveEnd               *
    - Game_LoadEnd_ChangeLevel   *

  Functions call order on Save game:
    - Game_Pause     * Open menu
    - Game_Unpause   * Click on save
    - Game_Loop      * Frame call window
    - Game_SaveBegin * Save begin
    - Game_SaveEnd   *

  Functions call order on Load game:
    - Game_Pause              * Open menu
    - Game_Unpause            * Click on load
    - Game_LoadBegin_SaveGame * Load begin
    - Game_LoadEnd_SaveGame   *
  */

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled(AppDefault) Game_Entry,
    Enabled(AppDefault) Game_Init,
    Enabled(AppDefault) Game_Exit,
    Enabled(AppDefault) Game_PreLoop,
    Enabled(AppDefault) Game_Loop,
    Enabled(AppDefault) Game_PostLoop,
    Enabled(AppDefault) Game_MenuLoop,
    Enabled(AppDefault) Game_SaveBegin,
    Enabled(AppDefault) Game_SaveEnd,
    Enabled(AppDefault) Game_LoadBegin_NewGame,
    Enabled(AppDefault) Game_LoadEnd_NewGame,
    Enabled(AppDefault) Game_LoadBegin_SaveGame,
    Enabled(AppDefault) Game_LoadEnd_SaveGame,
    Enabled(AppDefault) Game_LoadBegin_ChangeLevel,
    Enabled(AppDefault) Game_LoadEnd_ChangeLevel,
    Enabled(AppDefault) Game_LoadBegin_Trigger,
    Enabled(AppDefault) Game_LoadEnd_Trigger,
    Enabled(AppDefault) Game_Pause,
    Enabled(AppDefault) Game_Unpause,
    Enabled(AppDefault) Game_DefineExternals,
    Enabled(AppDefault) Game_ApplyOptions
  );
}