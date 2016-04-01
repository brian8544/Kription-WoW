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
 
#define FACTION_SPECIFIC 0
 
std::string GetNameLink(Player* player)
{
        std::string name = player->GetName();
        std::string color;
        switch(player->getClass())
        {
        case CLASS_DEATH_KNIGHT:
                color = "|cffC41F3B|TInterface\\icons\\spell_deathknight_classicon:15|t";
                break;
        case CLASS_DRUID:
                color = "|cffFF7D0A|TInterface\\icons\\ability_druid_maul:15|t";
                break;
        case CLASS_HUNTER:
                color = "|cffABD473|TInterface\\icons\\inv_weapon_bow_07:15|t";
                break;
        case CLASS_MAGE:
                color = "|cff69CCF0|TInterface\\icons\\inv_staff_13:15|t";
                break;
        case CLASS_PALADIN:
                color = "|cffF58CBA|TInterface\\icons\\ability_thunderbolt:15|t";
                break;
        case CLASS_PRIEST:
                color = "|cffFFFFFF|TInterface\\icons\\inv_staff_30:15|t";
                break;
        case CLASS_ROGUE:
                color = "|cffFFF569|TInterface\\icons\\inv_throwingknife_04:15|t";
                break;
        case CLASS_SHAMAN:
                color = "|cff0070DE|TInterface\\icons\\spell_nature_bloodlust:15|t";
                break;
        case CLASS_WARLOCK:
                color = "|cff9482C9|TInterface\\icons\\spell_nature_drowsy:15|t";
                break;
       case CLASS_WARRIOR:
               color = "|cffC79C6E|TInterface\\icons\\inv_sword_27:15|t";
               break;
			   
        }
        return "|Hplayer:"+name+"|h|cffFFFFFF["+color+name+"|cffFFFFFF]|h|r";
}
 
class World_Chat : public CommandScript
{
        public:
                World_Chat() : CommandScript("World_Chat"){}
 
        ChatCommand * GetCommands() const
        {
                static ChatCommand WorldChatCommandTable[] =
                {
					{ "chat", SEC_PLAYER, true, &HandleWorldChatCommand, "", NULL },
                        {NULL,          0,                              false,          NULL,                                           "", NULL}
                };
 
                return WorldChatCommandTable;
        }
 
        static bool HandleWorldChatCommand(ChatHandler * handler, const char * args)
        {
                if (!handler->GetSession()->GetPlayer()->CanSpeak())
                        return false;
                std::string temp = args;
 
                if (!args || temp.find_first_not_of(' ') == std::string::npos)
                        return false;
 
                std::string msg = "";
                Player * player = handler->GetSession()->GetPlayer();
 
                switch(player->GetSession()->GetSecurity())
                {
                        // Player
                        case SEC_PLAYER:
                                if (player->GetTeam() == ALLIANCE)
                      {
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Inv_Misc_Tournaments_banner_Human:15|t ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
                      }

                      else
                     {
                               msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Inv_Misc_Tournaments_banner_Orc:15|t ";
                               msg += GetNameLink(player);
                               msg += " |cfffaeb00"; 
                   }
                               break;
                        // VIP
                        case SEC_VIP:
						        if (player->GetTeam() == ALLIANCE)
                      {
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\INV_Misc_Token_ArgentDawn3:15|t |cffFFFF00[V.I.P]|TInterface\\icons\\Inv_Misc_Tournaments_banner_Human:15|t ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
                      }

                      else
                     {
                               msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\INV_Misc_Token_ArgentDawn3:15|t |cffFFFF00[V.I.P]|TInterface\\icons\\Inv_Misc_Tournaments_banner_Orc:15|t ";
                               msg += GetNameLink(player);
                               msg += " |cfffaeb00"; 
                   }
                                break;
                        // Trial GM
                        case SEC_MODERATOR:
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Mail_GMIcon:15|t |cffFF8C00[Trial GM] ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
                                break;
                        // GM
                        case SEC_GAMEMASTER:
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Mail_GMIcon:15|t |cffFF8C00[GM] ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
                                break;
                      // HeaD GM
                        case SEC_ADMINISTRATOR:
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Mail_GMIcon:15|t |cffFF8C00[Head GM] ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
                                break;
					 // Admin
                        case SEC_DEV:
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Mail_GMIcon:15|t |cffFFA500[DEV] ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
								break;
                        // Admin
                        case SEC_HD:
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Mail_GMIcon:15|t |cffFFA500[Admin] ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
								break;
		              // Admin
                        case SEC_HA:
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Mail_GMIcon:15|t |cffFFA500[Head Admin] ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
								break;
                        // Co-Owner
                        case SEC_CO:
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Mail_GMIcon:15|t |cffFF0000[Co-Owner] ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
								break;
                        // Owner
                        case SEC_O:
                                msg += "|cffFF0000[Kription WoW]|TInterface\\icons\\Mail_GMIcon:15|t |cffFF0000[Owner] ";
                                msg += GetNameLink(player);
                                msg += " |cfffaeb00";
								break;

                }
                       
                msg += args;
                if (FACTION_SPECIFIC)
                {
                        SessionMap sessions = sWorld->GetAllSessions();
                        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
                                if (Player* plr = itr->second->GetPlayer())
                                        if (plr->GetTeam() == player->GetTeam())
                                                sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), plr);
                }
                else
                        sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), 0);  
 
                return true;
        }
};
 
void AddSC_World_Chat()
{
        new World_Chat;
}