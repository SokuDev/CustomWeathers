//
// Created by PinkySmile on 31/10/2020
//

#include <SokuLib.hpp>
#include <map>


// Contructor:
// void FUN_0047f070(HudPlayerState *param_1, char param_1_00, CDesignBase *param_3, CDesignBase *param_4, int param_5)
// FUN_0047f070(&this->p1state, 0,(CDesignBase *)&this->field_0x98,(CDesignBase *)&this->field_0xcc,&this->field_0x4);
// Init:
// void FUN_0047ede0(HudPlayerState *param_1, Player *param_1_00)
// FUN_0047ede0(this, player);
struct HudPlayerState {
	char offset_0x00[0xC];
	unsigned hp;
	unsigned lastHp;
	char offset_0x14[0x4];
	SokuLib::CharacterManager *player;
	char offset_0x1C[0xDC];
};

// Contructor:
// Init:
// void FUN_00435f10(HudPlayerStateUnknown2 *this, DeckInfo *deckInfo, int index)
// FUN_00435f10(this, &player->deckInfo, playerIndex);
struct HudPlayerState2 {
	SokuLib::DeckInfo *deck;
	char offset_0x000[0x150];
};

// Contructor:
// Init:
// void FUN_00478c50 (HudPlayerState3 *this, CDesignBase *obj, Player *player)
// FUN_00478c50(this_00, (CDesignBase *)&this->field_0x100, player);
struct HudPlayerState3 {
	void *designObject;
	SokuLib::CharacterManager *player;
	char offset_0x000[0x2C];
};

struct CInfoManager {
	void **vtable;
	char offset_0x004[0x94];
	SokuLib::CDesign upperHUD;
	SokuLib::CDesign underHUD;
	char offset_0x100[0x6C];
	HudPlayerState3 p1State3;
	HudPlayerState3 p2State3;
	HudPlayerState2 p1State2;
	HudPlayerState2 p2State2;
	SokuLib::Sprite *p1Portrait;
	SokuLib::Sprite *p2Portrait;
	char offset_0x484[0x14];
	HudPlayerState p1State;
	HudPlayerState p2State;
};

struct GameDataManager {
	// 0x00
	SokuLib::List<std::pair<int, SokuLib::PlayerInfo*>> createQueue;
	// 0x0C
	bool isThreadsRunning; // align 3
	// 0x10
	uint32_t threadHandleA; // 0x46e6d0
	uint32_t threadIdA;
	uint32_t threadHandleB; // 0x46e6f0 THREAD_PRIORITY_IDLE
	uint32_t threadIdB;
	uint32_t eventHandleA;
	uint32_t eventHandleB;
	// 0x28
	SokuLib::CharacterManager* players[4];
	// 0x38
	bool enabledPlayers[4]; // 01 01 00 00
	// 0x3C
	SokuLib::Vector<SokuLib::CharacterManager*> activePlayers;
	// 0x4C
	SokuLib::List<SokuLib::CharacterManager*> destroyQueue;
}; // 0x58

enum CustomWeathers {
	CUSTOMWEATHER_ANGEL_HALO = SokuLib::WEATHER_CLEAR + 1,
	CUSTOMWEATHER_DESERT_MIRAGE,
	CUSTOMWEATHER_SHOOTING_STAR,
	CUSTOMWEATHER_THUNDERSTORM,
	CUSTOMWEATHER_IMPASSABLE_FOG,
	CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER,
	CUSTOMWEATHER_FORGETFUL_WIND,
	CUSTOMWEATHER_BLIZZARD,
	CUSTOMWEATHER_SIZE
};

void deleteCharacter(SokuLib::CharacterManager *p)
{
	(*(void (__thiscall **)(SokuLib::CharacterManager *, char))p->objectBase.vtable)(p, 0);
	SokuLib::Delete(p);
}

static SokuLib::BattleManager *(SokuLib::BattleManager::*ogBattleMgrDestructor)(char unknown);
static int (SokuLib::BattleManager::*ogBattleMgrOnMatchProcess)();
static int (SokuLib::BattleManager::*ogBattleMgrOnProcess)();
static int (SokuLib::SelectClient::*ogSelectClientOnProcess)();
static int (SokuLib::SelectServer::*ogSelectServerOnProcess)();
static int (SokuLib::Select::*ogSelectOnProcess)();
static int (__thiscall *ogHudRender)(void *);

#define setRenderMode(mode) ((void (__thiscall *)(int, int))0x404B80)(0x896B4C, mode)

static bool needCleaning = false;
static bool init = false;
static SokuLib::SWRFont font;
static const auto sokuRand = reinterpret_cast<int (*)(int)>(0x4099F0);
static unsigned char characterAlpha[2] = {255, 255};
static SokuLib::CharacterManager *characterBackup[2] = {nullptr, nullptr};
static std::shared_ptr<SokuLib::CharacterManager> extraCharacters[2] = {nullptr, nullptr};
static std::pair<SokuLib::PlayerInfo, SokuLib::PlayerInfo> extra;
static bool loadingExtraChrs = false;
static std::thread extraCharacterLoadingThread;
static CInfoManager *&hud = *(CInfoManager **)0x8985E8;
static SokuLib::DrawUtils::Sprite viewWindow;
static const auto switchWeather = (void (__thiscall *)(unsigned, unsigned))0x4388e0;
static GameDataManager*& dataMgr = *(GameDataManager**)SokuLib::ADDR_GAME_DATA_MANAGER;
static std::mutex extraChrMutex;
static std::list<SokuLib::KeyInput> lastInputs[2];


class SavedFrame {
private:
	std::shared_ptr<SokuLib::CharacterManager> desertMirage_extraCharacters[2];
	bool desertMirage_swapped1;
	bool desertMirage_swapped2;
	bool impassableSmog_characterAlpha1;
	bool impassableSmog_characterAlpha2;
	unsigned char desertMirage_chr1;
	unsigned char desertMirage_chr2;
	unsigned char twilight_alpha;
	unsigned char twilight_red;
	std::list<SokuLib::KeyInput> blizzard_lastInputs1;
	std::list<SokuLib::KeyInput> blizzard_lastInputs2;

public:
	SavedFrame()
	{
		this->desertMirage_swapped1 = characterBackup[0] != nullptr;
		this->desertMirage_swapped2 = characterBackup[1] != nullptr;
		this->desertMirage_chr1 = extra.first.character;
		this->desertMirage_chr2 = extra.second.character;
		this->impassableSmog_characterAlpha1 = characterAlpha[0];
		this->impassableSmog_characterAlpha2 = characterAlpha[1];
		this->desertMirage_extraCharacters[0] = extraCharacters[0];
		this->desertMirage_extraCharacters[1] = extraCharacters[1];
		this->twilight_alpha = viewWindow.tint.a;
		this->twilight_red = viewWindow.tint.r;
		this->blizzard_lastInputs1 = lastInputs[0];
		this->blizzard_lastInputs2 = lastInputs[1];
	}

	void restorePre()
	{
		auto players = (SokuLib::CharacterManager**)((int)&SokuLib::getBattleMgr() + 0x0C);
		auto arr = *(SokuLib::CharacterManager ***)(*(int *)SokuLib::ADDR_GAME_DATA_MANAGER + 0x40);

		for (int i = 0; i < 2; i++) {
			if ((&this->desertMirage_swapped1)[i] == (characterBackup[i] != nullptr))
				continue;
			if (this->desertMirage_extraCharacters[i]) {
				extraChrMutex.lock();
				extraCharacters[i] = this->desertMirage_extraCharacters[i];
				extraChrMutex.unlock();
			}

			auto &elem = characterBackup[i] ? *characterBackup[i] : *extraCharacters[i];
			auto hasBackup = (characterBackup[i] != nullptr);

			if (!hasBackup)
				characterBackup[i] = dataMgr->players[i];

			dataMgr->players[i] = &elem;
			players[i] = &elem;
			arr[i] = &elem;
			if (hasBackup)
				characterBackup[i] = nullptr;
			else
				extraCharacters[i] = nullptr;

			players[i]->objectBase.owner2 = players[i];
			players[!i]->objectBase.opponent = players[i];
			players[i]->objectBase.opponent = players[!i];
			(&SokuLib::camera.offset_0x44)[i] = &players[i]->objectBase.position.x;
			(&SokuLib::camera.offset_0x4C)[i] = &players[i]->objectBase.position.y;
		}
	}

	void restorePost()
	{
		lastInputs[0] = this->blizzard_lastInputs1;
		lastInputs[1] = this->blizzard_lastInputs2;
		characterAlpha[0] = this->impassableSmog_characterAlpha1;
		characterAlpha[1] = this->impassableSmog_characterAlpha2;
		viewWindow.tint.a = this->twilight_alpha;
		viewWindow.tint.r = this->twilight_red;
		extra.first.character = (SokuLib::Character)this->desertMirage_chr1;
		extra.second.character = (SokuLib::Character)this->desertMirage_chr2;
	}
};

unsigned lastFrameId = 0;
std::map<unsigned, SavedFrame> frames;
const auto weatherTimerReturnAddr = (void (*)())0x4824FE;
const short weatherTimes[] {
	999, // WEATHER_SUNNY
	999, // WEATHER_DRIZZLE
	999, // WEATHER_CLOUDY
	999, // WEATHER_BLUE_SKY
	999, // WEATHER_HAIL
	500, // WEATHER_SPRING_HAZE
	999, // WEATHER_HEAVY_FOG
	999, // WEATHER_SNOW
	750, // WEATHER_SUN_SHOWER
	999, // WEATHER_SPRINKLE
	500, // WEATHER_TEMPEST
	999, // WEATHER_MOUNTAIN_VAPOR
	750, // WEATHER_RIVER_MIST
	750, // WEATHER_TYPHOON
	999, // WEATHER_CALM
	999, // WEATHER_DIAMOND_DUST
	999, // WEATHER_DUST_STORM
	999, // WEATHER_SCORCHING_SUN
	999, // WEATHER_MONSOON
	999, // WEATHER_AURORA
	500, // WEATHER_TWILIGHT
	000, // WEATHER_CLEAR
	999, // CUSTOMWEATHER_ANGEL_HALO
	999, // CUSTOMWEATHER_DESERT_MIRAGE
	999, // CUSTOMWEATHER_SHOOTING_STAR
	333, // CUSTOMWEATHER_THUNDERSTORM
	500, // CUSTOMWEATHER_IMPASSABLE_FOG
	999, // CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER
	500, // CUSTOMWEATHER_FORGETFUL_WIND
	999, // CUSTOMWEATHER_BLIZZARD
};

void loadExtraCharactersThread()
{
	if (extraCharacters[0] == nullptr) {
		auto old = dataMgr->players[0];

		puts("Loading character 1");
		((void (__thiscall *)(GameDataManager*, int, SokuLib::PlayerInfo &))0x46da40)(dataMgr, 0, extra.first);
		puts("Init");
		(*(void (__thiscall **)(SokuLib::CharacterManager *))(*(int *)dataMgr->players[0] + 0x44))(dataMgr->players[0]);
		extraChrMutex.lock();
		if (extraCharacters[0] == nullptr) {
			extra.first.character = SokuLib::CHARACTER_RANDOM;
			extraCharacters[0] = std::shared_ptr<SokuLib::CharacterManager>{dataMgr->players[0], deleteCharacter};
		} else
			deleteCharacter(dataMgr->players[0]);
		extraChrMutex.unlock();
		dataMgr->players[0] = old;
		puts("Done");
	}
	if (extraCharacters[1] == nullptr) {
		auto old = dataMgr->players[1];

		puts("Loading character 2");
		((void (__thiscall *)(GameDataManager*, int, SokuLib::PlayerInfo &))0x46da40)(dataMgr, 1, extra.second);
		puts("Init");
		(*(void (__thiscall **)(SokuLib::CharacterManager *))(*(int *)dataMgr->players[1] + 0x44))(dataMgr->players[1]);
		extraChrMutex.lock();
		if (extraCharacters[1] == nullptr) {
			extra.second.character = SokuLib::CHARACTER_RANDOM;
			extraCharacters[1] = std::shared_ptr<SokuLib::CharacterManager>(dataMgr->players[1], deleteCharacter);
		} else
			deleteCharacter(dataMgr->players[1]);
		extraChrMutex.unlock();
		dataMgr->players[1] = old;
		puts("Done");
	}
	loadingExtraChrs = false;
}

const auto FUN_0043f5c0 = reinterpret_cast<int (*)(int)>(0x43f5C0);
const auto FUN_00438930 = reinterpret_cast<void (__stdcall *)(int)>(0x438930);
const auto FUN_0047dcf0 = reinterpret_cast<int (__thiscall *)(void *, int, float, float, int, int)>(0x47DCF0);

void desertMirageSwap(SokuLib::BattleManager *This, bool del = false)
{
	bool needRefresh = false;
	auto players = (SokuLib::CharacterManager**)((int)This + 0x0C);
	auto arr = *(SokuLib::CharacterManager ***)(*(int *)SokuLib::ADDR_GAME_DATA_MANAGER + 0x40);

	if (!arr)
		return;
	for (int i = 0; i < 2; i++) {
		if (!arr[i])
			continue;
		if ((arr[i]->effectiveWeather == CUSTOMWEATHER_DESERT_MIRAGE) != (characterBackup[i] == nullptr))
			continue;

		auto &elem = characterBackup[i] ? *characterBackup[i] : *extraCharacters[i];
		auto hasBackup = (characterBackup[i] != nullptr);
		auto old = dataMgr->players[i];

		if (!hasBackup) {
			if (del) {
				if (extraCharacters[i]) {
					(*(void (__thiscall **)(SokuLib::CharacterManager *, char)) old->objectBase.vtable)(old, 0);
					SokuLib::Delete(old);
				}
				continue;
			}
			if (extraCharacters[i] == nullptr && extraCharacterLoadingThread.joinable())
				extraCharacterLoadingThread.join();
			characterBackup[i] = dataMgr->players[i];
		}

		dataMgr->players[i] = &elem;
		players[i] = &elem;
		arr[i] = &elem;
		if (hasBackup)
			characterBackup[i] = nullptr;

		players[i]->objectBase.owner2 = players[i];
		players[i]->objectBase.position = old->objectBase.position;
		players[i]->objectBase.speed = old->objectBase.speed;
		players[i]->objectBase.gravity = old->objectBase.gravity;
		players[i]->objectBase.hp = old->objectBase.hp;
		players[i]->spiritRegenDelay = old->spiritRegenDelay;
		players[i]->currentSpirit = old->currentSpirit;
		players[i]->maxSpirit = old->maxSpirit;
		players[i]->keyManager = old->keyManager;
		players[i]->score = old->score;
		players[i]->combo.damages = old->combo.damages;
		players[i]->combo.limit = old->combo.limit;
		players[i]->combo.rate = old->combo.rate;
		players[i]->combo.nbHits = old->combo.nbHits;
		players[!i]->objectBase.opponent = players[i];
		players[i]->objectBase.opponent = players[!i];
		players[i]->objectBase.action = SokuLib::ACTION_IDLE;
		players[i]->objectBase.animate();

		(*(int (__thiscall **)(SokuLib::CharacterManager *))(*(unsigned *)&players[i]->objectBase.vtable + 0x28))(players[i]);
		(&SokuLib::camera.offset_0x44)[i] = &players[i]->objectBase.position.x;
		(&SokuLib::camera.offset_0x4C)[i] = &players[i]->objectBase.position.y;

		needRefresh = true;
		if (hasBackup)
			extraCharacters[i].reset();
	}
	if (needRefresh && !del) {
		((void (__thiscall *)(void *))0x47E260)(hud);

		int iVar1 = FUN_0043f5c0(SokuLib::displayedWeather);

		if (SokuLib::activeWeather != SokuLib::WEATHER_CLEAR) {
			FUN_00438930(SokuLib::displayedWeather);
			FUN_0047dcf0(hud, iVar1 + 2, 320, 32, 1, 1);
		} else
			FUN_0047dcf0(hud, iVar1 + 4, 320, 32, 1, 1);
	}
}

void loadTexture(SokuLib::DrawUtils::Sprite &sprite, const char *path, bool camera = false)
{
	sprite.texture.loadFromGame(path);
	sprite.setSize(sprite.texture.getSize());
	sprite.rect.width = sprite.getSize().x;
	sprite.rect.height = sprite.getSize().y;
	sprite.tint = SokuLib::Color::White;
	if (camera)
		sprite.setCamera(&SokuLib::camera);
}

int selectRandomWeather(bool includeAurora)
{
	auto result = sokuRand(CUSTOMWEATHER_SIZE - 3 + includeAurora * 2);

	if (result >= SokuLib::WEATHER_AURORA && !includeAurora)
		result++;
	if (result >= SokuLib::WEATHER_CLEAR)
		result++;
	if (result >= CUSTOMWEATHER_SHOOTING_STAR && !includeAurora)
		result++;
	return result;
}

void onWeatherActivate()
{
	if (SokuLib::activeWeather == CUSTOMWEATHER_SHOOTING_STAR)
		SokuLib::activeWeather = (SokuLib::Weather)selectRandomWeather(false);
}

int __fastcall CBattleManager_OnMatchProcess(SokuLib::BattleManager *This)
{
	unsigned timer = SokuLib::weatherCounter;
	auto ret = (This->*ogBattleMgrOnMatchProcess)();

	if (
		SokuLib::activeWeather != SokuLib::WEATHER_CLEAR &&
		SokuLib::displayedWeather == CUSTOMWEATHER_SHOOTING_STAR &&
		(timer / 100) != (SokuLib::weatherCounter / 100)
	) {
		SokuLib::playSEWaveBuffer(72);
		SokuLib::activeWeather = (SokuLib::Weather) selectRandomWeather(false);
	}
	return ret;
}

void cleanup(SokuLib::BattleManager *This)
{
	if (extraCharacterLoadingThread.joinable())
		extraCharacterLoadingThread.join();
	desertMirageSwap(This, true);
	extra.first.character = SokuLib::CHARACTER_RANDOM;
	extra.second.character = SokuLib::CHARACTER_RANDOM;
	characterAlpha[0] = 255;
	characterAlpha[1] = 255;
	lastInputs[0].clear();
	lastInputs[1].clear();
	needCleaning = false;
}

SokuLib::BattleManager *__fastcall CBattleManager_Destructor(SokuLib::BattleManager *This, int, char unknown)
{
	if (needCleaning)
		cleanup(This);
	return (This->*ogBattleMgrDestructor)(unknown);
}

int __fastcall CSelectCL_OnProcess(SokuLib::SelectClient *This)
{
	if (needCleaning)
		cleanup(&SokuLib::getBattleMgr());
	return (This->*ogSelectClientOnProcess)();
}

int __fastcall CSelectSV_OnProcess(SokuLib::SelectServer *This)
{
	if (needCleaning)
		cleanup(&SokuLib::getBattleMgr());
	return (This->*ogSelectServerOnProcess)();
}

int __fastcall CSelect_OnProcess(SokuLib::Select *This)
{
	if (needCleaning)
		cleanup(&SokuLib::getBattleMgr());
	return (This->*ogSelectOnProcess)();
}

int __fastcall CBattleManager_OnProcess(SokuLib::BattleManager *This)
{
	auto ret = (This->*ogBattleMgrOnProcess)();

	needCleaning = true;
	for (int i = 0 ; i < 2; i++) {
		auto chr = dataMgr->players[i];

		if (chr->swordOfRaptureDebuffTimeLeft || chr->effectiveWeather != CUSTOMWEATHER_IMPASSABLE_FOG) {
			if (chr->objectBase.renderInfos.color.a >= 250)
				chr->objectBase.renderInfos.color.a = 255;
			else
				chr->objectBase.renderInfos.color.a += 5;
			if (characterAlpha[i] != 255) {
				if (characterAlpha[i] > 250)
					characterAlpha[i] = 255;
				else
					characterAlpha[i] += 5;
				chr->objectBase.renderInfos.color.a = characterAlpha[i];
			}
		} else {
			if (characterAlpha[i] < 5)
				characterAlpha[i] = 0;
			else
				characterAlpha[i] -= 5;
			chr->objectBase.renderInfos.color.a = characterAlpha[i];
		}
	}
	if (!init) {
		init = true;
		loadTexture(viewWindow, "data/infoeffect/view_window.png", true);
		viewWindow.tint.a = 0;
	}
	desertMirageSwap(This);
	if (!loadingExtraChrs && (extraCharacters[0] == nullptr || extraCharacters[1] == nullptr)) {
		loadingExtraChrs = true;
		if (extraCharacterLoadingThread.joinable())
			extraCharacterLoadingThread.join();
		if (extraCharacters[0] == nullptr && extra.first.character == SokuLib::CHARACTER_RANDOM)
			extra.first.character = static_cast<SokuLib::Character>(sokuRand(SokuLib::CHARACTER_RANDOM));
		extra.first.palette = 0;
		if (extraCharacters[1] == nullptr && extra.second.character == SokuLib::CHARACTER_RANDOM)
			extra.second.character = static_cast<SokuLib::Character>(sokuRand(SokuLib::CHARACTER_RANDOM));
		extra.second.palette = extra.first.character == extra.second.character;
		extra.second.isRight = true;
		extraCharacterLoadingThread = std::thread{loadExtraCharactersThread};
	}
	return ret;
}

void __declspec(naked) weatherTimer_hook()
{
	__asm {
		MOV EAX, 0x8971C4
		MOV EAX, [EAX]
		MOV AX, [EAX * 0x02 + weatherTimes]
		MOV ECX, 0x8971CC
		MOV [ECX], AX
		JMP weatherTimerReturnAddr
	}
}


void __fastcall handleSwitchWeather(unsigned obj, SokuLib::Weather weatherId)
{
	if (weatherId == SokuLib::WEATHER_MONSOON)
		return switchWeather(obj, SokuLib::WEATHER_TWILIGHT);
	if (weatherId == SokuLib::WEATHER_TWILIGHT)
		return switchWeather(obj, SokuLib::WEATHER_CLEAR + 1);
	if (weatherId == SokuLib::WEATHER_AURORA)
		return switchWeather(obj, SokuLib::WEATHER_SUNNY);
	if (weatherId == (CUSTOMWEATHER_SIZE - 1))
		return switchWeather(obj, SokuLib::WEATHER_AURORA);
	return switchWeather(obj, weatherId + 1);
}

void __declspec(naked) handleSwitchWeather_hook()
{
	__asm {
		MOV EDX, EAX
		ADD ECX, 0x130
		JMP handleSwitchWeather
	}
}

constexpr unsigned FUN_00457be0 = 0x457BE0;

unsigned ctr1;
unsigned ctr2;

void __declspec(naked) initWeatherEffectMap()
{
	__asm {
		XOR EAX, EAX
		MOV [ctr1], 0
		MOV [ctr2], 100
	loop_:
		LEA EDX, [ESP + 0x18]
		PUSH EDX
		LEA EAX, [ESP + 0x24]
		PUSH EAX
		MOV ECX, 0x89885C
		MOV EAX, [ctr1]
		MOV dword ptr [ESP + 0x20], EAX
		MOV EAX, [ctr2]
		MOV dword ptr [ESP + 0x24], EAX
		CALL FUN_00457be0
		MOV EAX, [ctr2]
		ADD EAX, 10
		CMP EAX, 200
		JNZ noAdd
		ADD EAX, 10
	noAdd:
		MOV [ctr2], EAX
		MOV EAX, [ctr1]
		INC EAX
		MOV [ctr1], EAX
		CMP EAX, CUSTOMWEATHER_SIZE
		JNZ loop_
		RET
	}
}

void __declspec(naked) weatherEffectSwitch()
{
	__asm {
		CMP EAX, 900
		JGE end
		CMP EAX, 330
		JL end

	loop_:
		SUB EAX, 200
		CMP EAX, 320
		JGE loop_

	end:
		MOVSX EAX, AX
		ADD EAX, -0xA
		RET
	}
}

bool infoEffectUpdateSwitch_bool = true;
auto FUN_006dcdf0 = reinterpret_cast<void (__stdcall *)(unsigned action, float x, float y, int dir, int layer)>(0x6DCDF0);

unsigned infoEffectUpdateSwitch_extra(SokuLib::v2::InfoEffectObject *obj, unsigned short action)
{
	unsigned weather = action < 330 ? SokuLib::WEATHER_TWILIGHT : SokuLib::WEATHER_CLEAR + (action - 320) / 10;
	unsigned startObj = action < 330 ? 310 : 320 + (weather - SokuLib::WEATHER_CLEAR) * 10;

	switch (action % 10) {
	case 0:
		if (obj->frameState.currentFrame == 0 && SokuLib::weatherCounter >= weatherTimes[weather] - 100) {
			FUN_006dcdf0(startObj + 3, obj->position.x, obj->position.y - 10.0f, 1, 1);
			if (SokuLib::displayedWeather == weather && SokuLib::activeWeather != SokuLib::WEATHER_CLEAR)
				FUN_006dcdf0(startObj + 5, 320.0, 120.0, 1, 1);
		}
		if (obj->unknown174 != 0) {
			if (obj->renderInfos.color.a < 11)
				return 0x6D87B3;
			obj->renderInfos.color.a -= 10;
		} else {
			if (obj->renderInfos.color.a >= 244)
				obj->renderInfos.color.a = 0xFF;
			else
				obj->renderInfos.color.a += 10;
			if (obj->frameState.currentFrame % 0x3C == 0)
				FUN_006dcdf0(startObj + 1, obj->position.x, obj->position.y, 1, 2);
		}
		if (SokuLib::displayedWeatherOrb != weather)
			obj->unknown174 = 1;
		return 0x6D87AA;
	case 1:
		return 0x6D8635;
	case 2:
		return 0x6D86BB;
	case 3:
	case 4:
		obj->renderInfos.scale.x -= 0.1;
		if (obj->renderInfos.scale.x < 1.0)
			obj->renderInfos.scale.x = 1.0;
		obj->renderInfos.scale.y = obj->renderInfos.scale.x;
		if (obj->unknown174 != 0) {
			if (obj->renderInfos.color.a <= 20)
				return 0x6D87B3;
			obj->renderInfos.color.a -= 20;
		} else if (obj->renderInfos.color.a > 244)
			obj->renderInfos.color.a = 0xFF;
		else
			obj->renderInfos.color.a += 10;
		if (action % 10 == 3)
			infoEffectUpdateSwitch_bool = SokuLib::displayedWeatherOrb == weather;
		else
			infoEffectUpdateSwitch_bool = SokuLib::displayedWeather == weather;
		return 0x6D879E;
	case 5:
		if (obj->unknown174 != 0) {
			obj->unknown174 += 1;
			if (90.0 <= obj->unknown174) {
				obj->renderInfos.scale.y *= 0.95;
				obj->renderInfos.scale.x += 0.1;
				if (obj->renderInfos.color.a <= 15)
					return 0x6D87B3;
				obj->renderInfos.color.a -= 15;
			}
		} else {
			obj->renderInfos.scale.x -= 0.25;
			obj->renderInfos.scale.y = obj->renderInfos.scale.x;
			if (obj->renderInfos.scale.x <= 1.0) {
				if (obj->renderInfos.color.a >= 240)
					obj->renderInfos.color.a = 0xFF;
				else
					obj->renderInfos.color.a += 15;
				obj->renderInfos.scale.x = 1.0;
				obj->renderInfos.scale.y = 1.0;
				obj->unknown174 = 1;
			}
		}
		if (SokuLib::displayedWeatherOrb != weather) {
			if (0x14 < obj->renderInfos.color.a) {
				obj->renderInfos.color.a -= 0x14;
				return 0x6D87AA;
			}
			return 0x6D87B3;
		}
		if (0xf4 < obj->renderInfos.color.a)
			obj->renderInfos.color.a = 0xFF;
		else
			obj->renderInfos.color.a += 10;
		return 0x6D87AA;
	default:
		return 0x6D87BB;
	}
}

constexpr unsigned infoEffectUpdateSwitch_jmpBack = 0x6D860C;

void __declspec(naked) infoEffectUpdateSwitch()
{
	__asm {
		CMP EAX, 900
		JGE end
		CMP EAX, 310
		JL end
		// This is twilight
		CMP EAX, 320
		JL process
		CMP EAX, 330
		JL end

	process:
		PUSH EBX
		MOV [infoEffectUpdateSwitch_bool], 1

		PUSH EAX
		PUSH ECX
		CALL infoEffectUpdateSwitch_extra
		POP ECX
		ADD ESP, 4
		CMP [infoEffectUpdateSwitch_bool], 1
		JMP EAX

	end:
		CMP EAX, 0x18A
		JMP [infoEffectUpdateSwitch_jmpBack]
	}
}

int __fastcall onHudRender(CInfoManager *This)
{
	auto &player = *(SokuLib::sceneId == SokuLib::SCENE_BATTLECL ? This->p2State : This->p1State).player;
	int ret = 0;

	if (player.effectiveWeather == SokuLib::WEATHER_TWILIGHT) {
		if (viewWindow.tint.r != 255)
			viewWindow.tint.r += 15;
		else if (viewWindow.tint.a != 255)
			viewWindow.tint.a += 5;
	} else if (viewWindow.tint.a) {
		viewWindow.tint.a -= 5;
		viewWindow.tint.r = 0;
	}
	viewWindow.setPosition(
		SokuLib::Vector2f{player.objectBase.position.x, -player.objectBase.position.y}.to<int>() -
		viewWindow.texture.getSize() / 2 -
		SokuLib::Vector2i{0, 100}
	);
	// We only display the HUD if the view window is not opaque
	if (viewWindow.tint.a != 255)
		ret = ogHudRender(This);
	setRenderMode(1);
	viewWindow.draw();
	return ret;
}

// We check if the game version is what we target (in our case, Soku 1.10a).
extern "C" __declspec(dllexport) bool CheckVersion(const BYTE hash[16])
{
	return memcmp(hash, SokuLib::targetHash, sizeof(SokuLib::targetHash)) == 0;
}

unsigned saveFrame()
{
	assert(frames.find(lastFrameId) == frames.end());
	frames.emplace(lastFrameId, SavedFrame{});
	return lastFrameId++;
}

void loadFramePre(size_t frame, unsigned id)
{
	printf("loadFramePre: %i\n", id);
	frames.at(id).restorePre();
}

void loadFramePost(unsigned id)
{
	printf("loadFramePost: %i\n", id);
	frames.at(id).restorePost();
}

void freeFrame(unsigned id)
{
	frames.erase(frames.find(id));
}

void weatherEffect()
{
	SokuLib::CharacterManager *This;
	__asm MOV [This], ESI

	int i = This == dataMgr->players[1];
	const auto ObjectHandler_SpawnBullet = (void (__thiscall *)(void *, int, float, float, unsigned char, unsigned, void *, int))0x46EB30;
	const auto FUN_00438ce0 = reinterpret_cast<void (__thiscall *)(void *, unsigned, float, float, unsigned, unsigned)>(0x438CE0);

	if (This->swordOfRaptureDebuffTimeLeft > 0)
		return;
	switch (This->effectiveWeather) {
	case CUSTOMWEATHER_ANGEL_HALO:
		if (This->offset_0x4C0[0xD]) {
			This->dropInvulTimeLeft = 2;
			break;
		}
		if (!(50 <= This->objectBase.opponent->objectBase.action && This->objectBase.opponent->objectBase.action < 150))
			break;
		This->offset_0x4C0[0xD] = true;
		SokuLib::weatherCounter /= 2;
		ObjectHandler_SpawnBullet(This, 0x456, This->objectBase.position.x, 0, This->objectBase.direction, 0xffffffff, &i, 3);
		FUN_00438ce0(This, 0x8B, This->objectBase.position.x, This->objectBase.position.y, 1, 1);
		break;
	}
}

const unsigned checkCalm_ret = 0x488949;
using SokuLib::WEATHER_CALM;

void __declspec(naked) checkCalm()
{
	__asm {
		CMP EAX, WEATHER_CALM
		JZ ret_
		CMP EAX, CUSTOMWEATHER_ANGEL_HALO
		JZ ret_
		CMP EAX, CUSTOMWEATHER_IMPASSABLE_FOG
		JZ resetOne
		MOV byte ptr [ESI + 0x4CD], 0x0
	resetOne:
		MOV word ptr [ESI + 0x4CE], 0x0
	ret_:
		JMP [checkCalm_ret]
	}
}


const unsigned switchNormal = 0x47C5B3;

enum BlockResult {
	BLOCKRESULT_HIT,
	BLOCKRESULT_HIGH_RIGHTBLOCK,
	BLOCKRESULT_HIGH_WRONGBLOCKBLOCK,
	BLOCKRESULT_LOW_RIGHTBLOCK,
	BLOCKRESULT_LOW_WRONGBLOCKBLOCK,
	BLOCKRESULT_AIRBLOCK,
	BLOCKRESULT_GUARDPOINT
};

void __declspec(naked) onBlockHook()
{
	__asm {
		PUSH EAX
		MOV EAX, [EDI + 0x170]
		MOV EAX, [EAX + 0x52C]
		CMP EAX, CUSTOMWEATHER_THUNDERSTORM
		POP EAX
		JNE normal

		TEST EAX, EAX
		JZ normal

		// Wrong blocking already results in a crush
		CMP EAX, BLOCKRESULT_HIGH_WRONGBLOCKBLOCK
		JE forceHit
		CMP EAX, BLOCKRESULT_LOW_WRONGBLOCKBLOCK
		JNE normal
	forceHit:
		XOR EAX, EAX
	normal:
		JMP [switchNormal]
	}
}

const auto normalInputProcessing = (void (*)())0x46C937;
const auto skipInputProcessing = (void (*)())0x46CA0C;

bool __fastcall delayInputs(SokuLib::CharacterManager &chr)
{
	lastInputs[chr.isRightPlayer].push_back(chr.keyManager->keymapManager->input);
	if (chr.effectiveWeather == CUSTOMWEATHER_BLIZZARD) {
		if (lastInputs[chr.isRightPlayer].size() != 9)
			lastInputs[chr.isRightPlayer].push_back(chr.keyManager->keymapManager->input);
	} else {
		if (lastInputs[chr.isRightPlayer].size() != 1)
			lastInputs[chr.isRightPlayer].pop_front();
	}
	if (lastInputs[chr.isRightPlayer].size() == 1) {
		lastInputs[chr.isRightPlayer].pop_front();
		return true;
	}
	memcpy(&chr.keyMap, &lastInputs[chr.isRightPlayer].front(), sizeof(chr.keyMap));
	lastInputs[chr.isRightPlayer].pop_front();
	return false;
}

void __declspec(naked) addInputDelay()
{
	__asm {
		PUSH EAX
		PUSH ECX
		PUSH EDX
		MOV ECX, ESI
		CALL delayInputs
		POP EDX
		POP ECX
		TEST AL, AL
		POP EAX
		JNZ normalProcessing
		JMP skipInputProcessing

	normalProcessing:
		MOV [ESI + 0x754], EBP
		JMP normalInputProcessing
	}
}

struct GiurollCallbacks {
	unsigned (*saveState)();
	void (*loadStatePre)(size_t frame, unsigned);
	void (*loadStatePost)(unsigned);
	void (*freeState)(unsigned);
};

// Called when the mod loader is ready to initialize this module.
// All hooks should be placed here. It's also a good moment to load settings from the ini.
extern "C" __declspec(dllexport) bool Initialize(HMODULE hMyModule, HMODULE hParentModule)
{
	auto gr = LoadLibraryA("giuroll");

	if (!gr) {
		if (MessageBoxA(
			nullptr,
			"Netplay rollback not supported. This mod supports giuroll 0.6.14b, which wasn't found.\n"
			"If you are using it, make sure the line for giuroll is above the line for CustomWeathers in SWRSToys.ini.\n"
			"If you are using a rollback mod, playing online now will cause desyncs. Do you want to disable the mod now?",
			"CustomWeathers",
			MB_ICONWARNING | MB_YESNO
		) == IDYES)
			return false;
	} else {
		GiurollCallbacks cb{
			saveFrame,
			loadFramePre,
			loadFramePost,
			freeFrame
		};
		auto fct = GetProcAddress(gr, "addRollbackCb");

		if (!fct) {
			if (MessageBoxA(
				nullptr,
				"Netplay rollback not supported. This mod supports giuroll 0.6.14b, which wasn't found.\n"
				"A different (and not supported) giuroll version is in use. Please use version 0.6.14b, or otherwise one that is compatible.\n"
				"Playing online now will cause desyncs. Do you want to disable the mod now?",
				"CustomWeathers",
				MB_ICONWARNING | MB_YESNO
			) == IDYES)
				return false;
		} else
			reinterpret_cast<void (*)(const GiurollCallbacks *)>(fct)(&cb);
	}

	DWORD old;

#ifdef _DEBUG
	FILE *_;

	AllocConsole();
	freopen_s(&_, "CONOUT$", "w", stdout);
	freopen_s(&_, "CONOUT$", "w", stderr);
#endif

	extra.first.character = SokuLib::CHARACTER_RANDOM;
	extra.second.character = SokuLib::CHARACTER_RANDOM;
	VirtualProtect((PVOID)RDATA_SECTION_OFFSET, RDATA_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	ogBattleMgrDestructor = SokuLib::TamperDword(&SokuLib::VTable_BattleManager.destructor, CBattleManager_Destructor);
	ogSelectOnProcess = SokuLib::TamperDword(&SokuLib::VTable_Select.onProcess, CSelect_OnProcess);
	ogSelectClientOnProcess = SokuLib::TamperDword(&SokuLib::VTable_SelectClient.onProcess, CSelectCL_OnProcess);
	ogSelectServerOnProcess = SokuLib::TamperDword(&SokuLib::VTable_SelectServer.onProcess, CSelectSV_OnProcess);
	ogBattleMgrOnProcess = SokuLib::TamperDword(&SokuLib::VTable_BattleManager.onProcess, CBattleManager_OnProcess);
	ogBattleMgrOnMatchProcess = SokuLib::TamperDword(&SokuLib::VTable_BattleManager.maybeOnProgress, CBattleManager_OnMatchProcess);
	ogHudRender = (int (__thiscall *)(void *))SokuLib::TamperDword(0x85b544, onHudRender);
	VirtualProtect((PVOID)RDATA_SECTION_OFFSET, RDATA_SECTION_SIZE, old, &old);
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	SokuLib::TamperNearJmp(0x48247A, weatherTimer_hook);
	SokuLib::TamperNearJmp(0x483DC2, handleSwitchWeather_hook);
	memset((void *)0x441EE2, 0x90, 0x4421EE - 0x441EE2);
	SokuLib::TamperNearCall(0x441EE2, initWeatherEffectMap);
	SokuLib::TamperNearCall(0x6DCBB1, weatherEffectSwitch);
	*(char *)0x6DCBB6 = 0x90;
	SokuLib::TamperNearJmp(0x6D8607, infoEffectUpdateSwitch);
	*(char *)0x478A13 = CUSTOMWEATHER_SIZE;
	*(char *)0x478907 = CUSTOMWEATHER_SIZE;
	*(char *)0x4788DD = CUSTOMWEATHER_SIZE;
	*(char *)0x478A85 = CUSTOMWEATHER_SIZE;
	*(char *)0x478ACC = CUSTOMWEATHER_SIZE - 1;
	*(char *)0x48243E = 0x01;
	SokuLib::TamperNearJmpOpr(0x48243F, selectRandomWeather);
	*(char *)0x4397D6 = 0x00;
	SokuLib::TamperNearJmpOpr(0x4397D7, selectRandomWeather);
	memcpy((void *)0x4397E9, (void *)0x4397E4, 3);
	SokuLib::TamperNearCall(0x4397E4, onWeatherActivate);
	SokuLib::TamperNearJmp(0x47C5AE, onBlockHook);
	SokuLib::TamperNearJmp(0x488934, checkCalm);
	SokuLib::TamperNearJmp(0x46C931, addInputDelay);
	new SokuLib::Trampoline(0x4889BE, weatherEffect, 6);

	// Filesystem first patch
	*(char *)0x40D1FB = 0xEB;
	*(char *)0x40D27A = 0x74;
	*(char *)0x40D27B = 0x91;
	*(char *)0x40D245 = 0x1C;
	memset((char *)0x40D27C, 0x90, 7);
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);

	FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
	return true;
}

extern "C" int APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	return TRUE;
}

// New mod loader functions
// Loading priority. Mods are loaded in order by ascending level of priority (the highest first).
// When 2 mods define the same loading priority the loading order is undefined.
extern "C" __declspec(dllexport) int getPriority()
{
	return -100;
}

// Not yet implemented in the mod loader, subject to change
// SokuModLoader::IValue **getConfig();
// void freeConfig(SokuModLoader::IValue **v);
// bool commitConfig(SokuModLoader::IValue *);
// const char *getFailureReason();
// bool hasChainedHooks();
// void unHook();