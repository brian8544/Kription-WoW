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

class announce_login : public PlayerScript
{
public:
	announce_login() : PlayerScript("announce_login") { }
 
	void OnLogin(Player* player, bool /*loginFirst*/)
	{
		if (player->GetTeam() == ALLIANCE)
				{
			std::ostringstream ss;
						ss << "|cff3DAEFF[ Kription WoW ]|cffFFD800 : Player|cff4CFF00 " << player->GetName() << " |cffFFD800Has Online. This Player is|cff0026FF Alliance";
			sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
				}
				else
				{
			std::ostringstream ss;
						ss << "|cff3DAEFF[ Kription WoW ]|cffFFD800 : Player|cff4CFF00 " << player->GetName() << " |cffFFD800Has Online. This Player is|cffFF0000 Horde" ;
			sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
				}
		}
};
 
void AddSC_announce_login()
{
	new announce_login;
}