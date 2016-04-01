# Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# file(GLOB_RECURSE sources_Custom Custom/*.cpp Custom/*.h)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
#  ${sources_Custom}
Custom/Transmog/Transmogrification.h
Custom/Transmog/Transmogrification.cpp
Custom/Transmog/Transmogrifier.cpp
Custom/world_chat.cpp
Custom/vip_buff.cpp
Custom/factionchange.cpp
Custom/vipcomm.cpp
Custom/hearthstone_summon.cpp
Custom/Trainer.cpp
Custom/npc_beastmaster.cpp
Custom/pvp_title.cpp
Custom/proffes.cpp
Custom/eventcommandscript.cpp
Custom/chat_censure.cpp
Custom/npc_token.cpp
Custom/gamble.cpp
Custom/announce_login.cpp
Custom/Boss_Announcer.cpp
custom/Duel_Reset.cpp
custom/npc_title.cpp

)

message("  -> Prepared: Custom")
