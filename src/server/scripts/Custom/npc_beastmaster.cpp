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
#include <cstring>
#include "Pet.h"
#include "Formulas.h"

void CreatePet(Player *player, Creature * m_creature, uint32 entry) {

    if(player->getClass() != CLASS_HUNTER) {

        ChatHandler(player->GetSession()).PSendSysMessage("You are not hunter! Your order has been rejected.");
        player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    if(player->GetPet()) {
        ChatHandler(player->GetSession()).PSendSysMessage("You must abandon your current pet first!");
        player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    Creature *creatureTarget = m_creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY()+2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
    if(!creatureTarget) return;

    Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);
    if(!pet) return;

    creatureTarget->setDeathState(JUST_DIED);
    creatureTarget->RemoveCorpse();
    creatureTarget->SetHealth(0);
    pet->SetPower(POWER_HAPPINESS, 1048000);
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, 79);
    pet->GetMap()->AddToMap((Creature*)pet);
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, 80);

    if(!pet->InitStatsForLevel(80))
        //sLog->outError(LOG_FILTER_WORLDSERVER, "Pet Creation failed: no initial stats for entry %u", entry);

    pet->UpdateAllStats();
    player->SetMinion(pet, true);
    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
    pet->InitTalentForLevel();
    player->PetSpellInitialize();
    player->PlayerTalkClass->SendCloseGossip();
    ChatHandler(player->GetSession()).PSendSysMessage("Pet added. You might want to feed it and give it a name.");
}

class Npc_Beastmaster : public CreatureScript
{
public:
	Npc_Beastmaster() : CreatureScript("Npc_Beastmaster") {}

    bool OnGossipHello(Player* player, Creature* _Creature)
    {
        if (player->getClass() != CLASS_HUNTER)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("You are not a hunter! Your order has been rejected.");
            return true;
        }
        player->ADD_GOSSIP_ITEM(4, "|TInterface\\icons\\ability_hunter_pet_wolf:35|t|r Get a new pet", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
        if (player->CanTameExoticPets())
            player->ADD_GOSSIP_ITEM(4, "|TInterface\\icons\\ability_druid_primalprecision:35|t|r Get a new exotic pet", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 50);
        player->ADD_GOSSIP_ITEM(5, "|TInterface\\icons\\spell_chargenegative:35|t|r Reset pet talent points", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4015);
        player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player *player, Creature *_Creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF + 100:
            player->ADD_GOSSIP_ITEM(4, "|TInterface\\icons\\ability_hunter_pet_wolf:35|t|r Get a new pet", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
            if (player->CanTameExoticPets())
                player->ADD_GOSSIP_ITEM(4, "|TInterface\\icons\\ability_druid_primalprecision:35|t|r Get a new exotic pet", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 50);
            player->ADD_GOSSIP_ITEM(5, "|TInterface\\icons\\spell_chargenegative:35|t|r Reset pet talent points", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4015);
            player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 30:
            player->ADD_GOSSIP_ITEM(2, "|TInterface\\icons\\spell_chargenegative:35|t|r << Back to main menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
            player->ADD_GOSSIP_ITEM(4, "|TInterface\\icons\\spell_chargepositive:35|t|r Next page >>", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_bat:35|t|r Bat", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_bear:35|t|r Bear", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_boar:35|t|r Boar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_cat:35|t|r Cat", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_vulture:35|t|r Carrion Bird", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_crab:35|t|r Crab", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_crocolisk:35|t|r Crocolisk", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_dragonhawk:35|t|r Dragonhawk", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_gorilla:35|t|r Gorilla", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_wolf:35|t|r Wolf", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_hyena:35|t|r Hyena", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_moth:35|t|r Moth", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_owl:35|t|r Owl", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 31:     
            player->ADD_GOSSIP_ITEM(2, "|TInterface\\icons\\spell_chargenegative:35|t|r << Back to main menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
            player->ADD_GOSSIP_ITEM(4, "|TInterface\\icons\\spell_chargepositive:35|t|r Next page >>", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_raptor:35|t|r Raptor", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_ravager:35|t|r Ravager", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_tallstrider:35|t|r Strider", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_scorpid:35|t|r Scorpid", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_spider:35|t|r Spider", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\spell_nature_guardianward:35|t|r Serpent", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_turtle:35|t|r Turtle", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_wasp:35|t|r Wasp", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 50:     
            player->ADD_GOSSIP_ITEM(2, "|TInterface\\icons\\spell_chargenegative:35|t|r << Back to main menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_chimera:35|t|r Chimera", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 51);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_corehound:35|t|r Corehound", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 52);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_devilsaur:35|t|r Devilsaur", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 53);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_rhino:35|t|r Rhino", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 54);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_silithid:35|t|r Silithid", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 55);
			player->ADD_GOSSIP_ITEM(6, "|TInterface\\icons\\ability_hunter_pet_worm:35|t|r Worm", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 56); 
            player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 4015: // Reset Pet Talent Points
            player->CLOSE_GOSSIP_MENU();

            if (player->GetPet())
                player->ResetPetTalents();
            else
                ChatHandler(player->GetSession()).PSendSysMessage("You do not have a pet!");
            break;

        case GOSSIP_OPTION_STABLEPET:
            player->GetSession()->SendStablePet(_Creature->GetGUID());
            break;    
        case GOSSIP_OPTION_VENDOR:
            player->GetSession()->SendListInventory( _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 51: //chimera
            CreatePet(player, _Creature, 21879);
            break;
        case GOSSIP_ACTION_INFO_DEF + 52: //core hound
            CreatePet(player, _Creature, 21108);
            break;
        case GOSSIP_ACTION_INFO_DEF + 53: //Devilsaur
            CreatePet(player, _Creature, 20931);
            break;
        case GOSSIP_ACTION_INFO_DEF + 54: //rhino
            CreatePet(player, _Creature, 30445);
            break;
        case GOSSIP_ACTION_INFO_DEF + 55: //silithid
            CreatePet(player, _Creature, 5460);
            break;
        case GOSSIP_ACTION_INFO_DEF + 56: //Worm
            CreatePet(player, _Creature, 30148);
            break;
        case GOSSIP_ACTION_INFO_DEF + 16: //Spider
            CreatePet(player, _Creature, 2349);
            break;
        case GOSSIP_ACTION_INFO_DEF + 17: //Dragonhawk
            CreatePet(player, _Creature, 27946);
            break;
        case GOSSIP_ACTION_INFO_DEF + 18: //Bat
            CreatePet(player, _Creature, 28233);
            break;
        case GOSSIP_ACTION_INFO_DEF + 19: //Ravager
            CreatePet(player, _Creature, 17199);
            break;
        case GOSSIP_ACTION_INFO_DEF + 20: //Raptor
            CreatePet(player, _Creature, 14821);
            break;
        case GOSSIP_ACTION_INFO_DEF + 21: //Serpent
            CreatePet(player, _Creature, 28358);
            break;
        case GOSSIP_ACTION_INFO_DEF + 1: //bear
            CreatePet(player, _Creature, 29319);
            break;
        case GOSSIP_ACTION_INFO_DEF + 2: //Boar
            CreatePet(player, _Creature, 29996);
            break;
        case GOSSIP_ACTION_INFO_DEF + 3: //Bug
            CreatePet(player, _Creature, 28085);
            break;
        case GOSSIP_ACTION_INFO_DEF + 4: //cat
            CreatePet(player, _Creature, 28097);
            break;
        case GOSSIP_ACTION_INFO_DEF + 5: //carrion
            CreatePet(player, _Creature, 26838);
            break;
        case GOSSIP_ACTION_INFO_DEF + 6: //crab
            CreatePet(player, _Creature, 24478);
            break;      
        case GOSSIP_ACTION_INFO_DEF + 7: //crocolisk
            CreatePet(player, _Creature, 1417);
            break;     
        case GOSSIP_ACTION_INFO_DEF + 8: //gorila
            CreatePet(player, _Creature, 28213);
            break;
        case GOSSIP_ACTION_INFO_DEF + 9: //hound
            CreatePet(player, _Creature, 29452);
            break;
        case GOSSIP_ACTION_INFO_DEF + 10: //hynea
            CreatePet(player, _Creature, 13036);
            break;
        case GOSSIP_ACTION_INFO_DEF + 11: //Moth
            CreatePet(player, _Creature, 27421);
            break;
        case GOSSIP_ACTION_INFO_DEF + 12: //owl
            CreatePet(player, _Creature, 22265);
            break;
        case GOSSIP_ACTION_INFO_DEF + 13: //strider
            CreatePet(player, _Creature, 22807);
            break;
        case GOSSIP_ACTION_INFO_DEF + 14: //scorpid
            CreatePet(player, _Creature, 9698);
            break;
        case GOSSIP_ACTION_INFO_DEF + 15: //turtle
            CreatePet(player, _Creature, 25482);
            break;
        }

        return true;
    }
};

void AddSC_Npc_Beastmaster()
{
	new Npc_Beastmaster();
}
