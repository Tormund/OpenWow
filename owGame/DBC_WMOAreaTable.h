#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_WMOAreaTable)

__DBC_TVALUE(uint32, WMOID, 2);
__DBC_TVALUE(uint32, NameSetID, 3);
__DBC_TVALUE(int32, WMOGroupID, 4);

//foreign_key<uint32_t, &SoundProviderPreferencesRec::m_ID> m_SoundProviderPref;
//foreign_key<uint32_t, &SoundProviderPreferencesRec::m_ID> m_SoundProviderPrefUnderwater;
//foreign_key<uint32_t, &SoundAmbienceRec::m_ID> m_AmbienceID;
//foreign_key<uint32_t, &ZoneMusicRec::m_ID> m_ZoneMusic;
//foreign_key<uint32_t, &ZoneIntroMusicTableRec::m_ID> m_IntroSound;

__DBC_TVALUE(uint32, Flags, 10);
__DBC_REF_ID(DBÑ_AreaTable, AreaTableID, 11);
__DBC_LOCSTR(AreaNamelang, 12);

//foreign_key<uint32_t, &ZoneIntroMusicTableRec::m_ID> m_uwIntroSound;
//foreign_key<uint32_t, &ZoneMusicRec::m_ID> m_uwZoneMusic;
//foreign_key<uint32_t, &SoundAmbienceRec::m_ID> m_uwAmbience;

DBC_DEF_END
