#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
 
class Duel_Reset : public PlayerScript
{
        public:
                Duel_Reset() : PlayerScript("Duel_Reset"){}
 
                void OnDuelEnd(Player* Plr, Player* Plr2, DuelCompleteType /*type*/)
                {
                        Plr->SetHealth(Plr->GetMaxHealth());
                        Plr->RemoveArenaSpellCooldowns();
                        Plr->ResetAllPowers();
                        Plr2->SetHealth(Plr2->GetMaxHealth());
                        Plr2->RemoveArenaSpellCooldowns();
                        Plr2->ResetAllPowers();
                }
};
 
void AddSC_Duel_Reset()
{
        new Duel_Reset();
}