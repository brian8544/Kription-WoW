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
#include "World.h"

class hva_commandscript : public CommandScript
{
public:
    hva_commandscript() : CommandScript("hva_commandscript") { }

	ChatCommand* GetCommands() const
    {
        static ChatCommand IngameCommandTable[] =
        {
			{ "pvp",                SEC_PLAYER,  false, &HandlePvpCommand,                 "",  NULL },
			{ "stoppvp",            SEC_PLAYER,  false, &HandleStoppvpCommand,             "",  NULL },
			{ "pvm",                SEC_PLAYER,  false, &HandlePvmCommand,                 "",  NULL },
			{ "stoppvm",            SEC_PLAYER,  false, &HandleStoppvmCommand,             "",  NULL },
            {  NULL,                         0,  false, NULL,                              "",  NULL }
        };
		return IngameCommandTable;
    }
	
	    static bool HandlePvpCommand(ChatHandler* handler, const char* args)
		{
		            Player* me = handler->GetSession()->GetPlayer();
					{
					me->SummonCreature(189999, -1720.53f , 3186.02f, 4.89052f, 1, TEMPSUMMON_TIMED_DESPAWN, 600000);
					char msg[250];
		            snprintf(msg, 250, "|cffff0000[Event System]|r The Event|cffff0000 PvP|r has now started! ", NULL);  // The message
		            sWorld->SendGlobalText(msg, NULL);
					return true;
					}
		}
		
		static bool HandleStoppvpCommand(ChatHandler* handler, const char* args)
		{
		           Player* me = handler->GetSession()->GetPlayer();
					{
			        char msg[250];
		            snprintf(msg, 250, "|cffff0000[Event System]|r The Event|cffff0000 PvP|r has now ended! ", NULL);  // The message
		            sWorld->SendGlobalText(msg, NULL);
					return true;
					}
		}
		static bool HandlePvmCommand(ChatHandler* handler, const char* args)
		{
		            Player* me = handler->GetSession()->GetPlayer();
					{
					me->SummonCreature(189998, -1720.53f , 3186.02f, 4.89052f, 1, TEMPSUMMON_TIMED_DESPAWN, 600000);
					char msg[250];
		            snprintf(msg, 250, "|cffff0000[Event System]|r The Event|cffff0000 PvM|r has now started! ", NULL);  // The message
		            sWorld->SendGlobalText(msg, NULL);
					return true;
					}
		}
		
		static bool HandleStoppvmCommand(ChatHandler* handler, const char* args)
		{
		           Player* me = handler->GetSession()->GetPlayer();
					{
			        char msg[250];
		            snprintf(msg, 250, "|cffff0000[Event System]|r The Event|cffff0000 PvM|r has now ended! ", NULL);  // The message
		            sWorld->SendGlobalText(msg, NULL);
					return true;
					}
		}
		private:
};

void AddSC_hva_commandscript()
{
    new hva_commandscript();
}	