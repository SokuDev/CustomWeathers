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

// #define DISABLE_VANILLA
#define FORCE_WEATHER CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER
#define WEATHER_TIMER_MULTIPLIER 3

enum CustomWeathers {
	CUSTOMWEATHER_ANGEL_HALO = SokuLib::WEATHER_CLEAR + 1,
	CUSTOMWEATHER_DESERT_MIRAGE,
	CUSTOMWEATHER_SHOOTING_STAR,
	CUSTOMWEATHER_THUNDERSTORM,
	CUSTOMWEATHER_IMPASSABLE_FOG,
	CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER,
	CUSTOMWEATHER_FORGETFUL_WIND,
	CUSTOMWEATHER_BLIZZARD,
	CUSTOMWEATHER_RAGNAROK,
	CUSTOMWEATHER_HAAR,
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
static const auto sokuRand = reinterpret_cast<int (*)(int)>(0x4099F0);
static const auto switchWeather = (void (__thiscall *)(unsigned, unsigned))0x4388e0;
const auto ObjectHandler_SpawnBullet = (void (__thiscall *)(void *This, int action, float x, float y, unsigned char layer, unsigned color, float *extraData, int dataSize))0x46EB30;
const auto FUN_00438ce0 = reinterpret_cast<void (__thiscall *)(void *, unsigned, float, float, unsigned, unsigned)>(0x438CE0);

static CInfoManager *&hud = *(CInfoManager **)0x8985E8;
static GameDataManager*& dataMgr = *(GameDataManager**)SokuLib::ADDR_GAME_DATA_MANAGER;

static bool needCleaning = false;
static bool init = false;
static SokuLib::SWRFont font;
static unsigned char characterAlpha[2] = {255, 255};
static unsigned char characterSkills[2][16] = {{0}, {0}};
static SokuLib::CharacterManager *characterBackup[2] = {nullptr, nullptr};
static SokuLib::CharacterManager *extraCharacters[2] = {nullptr, nullptr};
static std::pair<SokuLib::PlayerInfo, SokuLib::PlayerInfo> extra;
static SokuLib::DrawUtils::Sprite viewWindow;
static std::list<SokuLib::KeyInput> lastInputs[2];
static unsigned short timeStopLeft = 0;


class SavedFrame {
private:
	bool desertMirage_swapped1;
	bool desertMirage_swapped2;
	unsigned char impassableSmog_characterAlpha1;
	unsigned char impassableSmog_characterAlpha2;
	unsigned char desertMirage_chr1;
	unsigned char desertMirage_chr2;
	unsigned char twilight_alpha;
	unsigned char twilight_red;
	unsigned char haar_characterSkills1[16];
	unsigned char haar_characterSkills2[16];
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
		memcpy(this->haar_characterSkills1, characterSkills[0], sizeof(this->haar_characterSkills1));
		memcpy(this->haar_characterSkills2, characterSkills[1], sizeof(this->haar_characterSkills2));
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
		memcpy(characterSkills[0], this->haar_characterSkills1, sizeof(this->haar_characterSkills1));
		memcpy(characterSkills[1], this->haar_characterSkills2, sizeof(this->haar_characterSkills2));
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
	500, // CUSTOMWEATHER_THUNDERSTORM
	500, // CUSTOMWEATHER_IMPASSABLE_FOG
	999, // CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER
	500, // CUSTOMWEATHER_FORGETFUL_WIND
	999, // CUSTOMWEATHER_BLIZZARD
	999, // CUSTOMWEATHER_RAGNAROK
	999, // CUSTOMWEATHER_HAAR
};

void loadExtraCharacters()
{
	auto o1 = dataMgr->players[0];
	auto o2 = dataMgr->players[1];

	extra.first.character = static_cast<SokuLib::Character>(sokuRand(SokuLib::CHARACTER_RANDOM));
	extra.first.palette = 0;
	extra.second.character = static_cast<SokuLib::Character>(sokuRand(SokuLib::CHARACTER_RANDOM));
	extra.second.palette = extra.first.character == extra.second.character;
	extra.second.isRight = true;

	puts("Loading character 1");
	((void (__thiscall *)(GameDataManager*, int, SokuLib::PlayerInfo &))0x46da40)(dataMgr, 0, extra.first);
	puts("Init");
	(*(void (__thiscall **)(SokuLib::CharacterManager *))(*(int *)dataMgr->players[0] + 0x44))(dataMgr->players[0]);
	extraCharacters[0] = dataMgr->players[0];
	puts("Loading character 2");
	((void (__thiscall *)(GameDataManager*, int, SokuLib::PlayerInfo &))0x46da40)(dataMgr, 1, extra.second);
	puts("Init");
	(*(void (__thiscall **)(SokuLib::CharacterManager *))(*(int *)dataMgr->players[1] + 0x44))(dataMgr->players[1]);
	extraCharacters[1] = dataMgr->players[1];
	puts("Done");
	dataMgr->players[0] = o1;
	dataMgr->players[1] = o2;
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
			characterBackup[i] = dataMgr->players[i];
			if (del)
				continue;
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
		if (del)
			continue;
		players[i]->objectBase.action = SokuLib::ACTION_IDLE;
		players[i]->objectBase.animate();

		(*(int (__thiscall **)(SokuLib::CharacterManager *))(*(unsigned *)&players[i]->objectBase.vtable + 0x28))(players[i]);
		(&SokuLib::camera.offset_0x44)[i] = &players[i]->objectBase.position.x;
		(&SokuLib::camera.offset_0x4C)[i] = &players[i]->objectBase.position.y;
		needRefresh = true;
	}
	if (needRefresh) {
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
#ifdef FORCE_WEATHER
	return FORCE_WEATHER;
#elif defined(DISABLE_VANILLA)
	auto result = sokuRand((CUSTOMWEATHER_SIZE - SokuLib::WEATHER_CLEAR + 1) - 3 + includeAurora * 2);

	result += SokuLib::WEATHER_AURORA;
	if (result >= SokuLib::WEATHER_AURORA && !includeAurora)
		result++;
	if (result >= SokuLib::WEATHER_CLEAR)
		result++;
	if (result >= CUSTOMWEATHER_SHOOTING_STAR && !includeAurora)
		result++;
	return result;
#else
	auto result = sokuRand(CUSTOMWEATHER_SIZE - 3 + includeAurora * 2);

	if (result >= SokuLib::WEATHER_AURORA && !includeAurora)
		result++;
	if (result >= SokuLib::WEATHER_CLEAR)
		result++;
	if (result >= CUSTOMWEATHER_SHOOTING_STAR && !includeAurora)
		result++;
	return result;
#endif
}

void __fastcall handleSwitchWeather(unsigned obj, SokuLib::Weather weatherId)
{
#ifdef FORCE_WEATHER
	switchWeather(obj, FORCE_WEATHER);
#elif defined(DISABLE_VANILLA)
	if (weatherId == SokuLib::WEATHER_TWILIGHT)
		return switchWeather(obj, SokuLib::WEATHER_CLEAR + 1);
	if (weatherId == SokuLib::WEATHER_AURORA)
		return switchWeather(obj, SokuLib::WEATHER_TWILIGHT);
	if (weatherId == (CUSTOMWEATHER_SIZE - 1))
		return switchWeather(obj, SokuLib::WEATHER_AURORA);
	return switchWeather(obj, weatherId + 1);
#else
	if (weatherId == SokuLib::WEATHER_MONSOON)
		return switchWeather(obj, SokuLib::WEATHER_TWILIGHT);
	if (weatherId == SokuLib::WEATHER_TWILIGHT)
		return switchWeather(obj, SokuLib::WEATHER_CLEAR + 1);
	if (weatherId == SokuLib::WEATHER_AURORA)
		return switchWeather(obj, SokuLib::WEATHER_SUNNY);
	if (weatherId == (CUSTOMWEATHER_SIZE - 1))
		return switchWeather(obj, SokuLib::WEATHER_AURORA);
	return switchWeather(obj, weatherId + 1);
#endif
}

void __declspec(naked) handleSwitchWeather_hook()
{
	__asm {
		MOV EDX, EAX
		ADD ECX, 0x130
		JMP handleSwitchWeather
	}
}

void weatherActivate()
{
	float extraData[4];

	if (SokuLib::activeWeather == CUSTOMWEATHER_FORGETFUL_WIND) {
		auto &btl = SokuLib::getBattleMgr();

		timeStopLeft = 2;
		SokuLib::playSEWaveBuffer(73);
		extraData[0] = 0;
		extraData[1] = 0;
		extraData[2] = 1;
		ObjectHandler_SpawnBullet(&btl.leftCharacterManager, 1002, 640, 480, 1, 0xFFFFFFFE, extraData, 3);
		*(int *)((int)btl.leftCharacterManager.objects.list.vector().back() + 0x364) = (((int)&timeStopLeft) - 0x84C);
	}
}

void onWeatherActivate()
{
	if (SokuLib::activeWeather == CUSTOMWEATHER_SHOOTING_STAR)
		SokuLib::activeWeather = (SokuLib::Weather)selectRandomWeather(false);
	weatherActivate();
}

int __fastcall CBattleManager_OnMatchProcess(SokuLib::BattleManager *This)
{
	unsigned timer = SokuLib::weatherCounter;
	auto old1 = This->leftCharacterManager.effectiveWeather;
	auto old2 = This->rightCharacterManager.effectiveWeather;
	auto ret = (This->*ogBattleMgrOnMatchProcess)();

	if (This->leftCharacterManager.effectiveWeather == CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER) {
		if (
			(This->leftCharacterManager.untech == 0 && 50 <= This->leftCharacterManager.objectBase.action && This->leftCharacterManager.objectBase.action < 150) ||
			This->leftCharacterManager.objectBase.hp == 0 ||
			This->leftCharacterManager.damageLimited
		) {
			if (This->leftCharacterManager.objectBase.gravity.y < 0)
				This->leftCharacterManager.objectBase.gravity.y *= -1;
		} else if (This->leftCharacterManager.objectBase.position.y == 0)
			This->leftCharacterManager.objectBase.position.y += 10;
	} else if (old1 == CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER) {
		if (This->leftCharacterManager.objectBase.gravity.y < 0)
			This->leftCharacterManager.objectBase.gravity.y *= -1;
	}

	if (This->rightCharacterManager.effectiveWeather == CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER) {
		if (
			(This->rightCharacterManager.untech == 0 && 50 <= This->rightCharacterManager.objectBase.action && This->rightCharacterManager.objectBase.action < 150) ||
			This->rightCharacterManager.objectBase.hp == 0 ||
			This->rightCharacterManager.damageLimited
		) {
			if (This->rightCharacterManager.objectBase.gravity.y < 0)
				This->rightCharacterManager.objectBase.gravity.y *= -1;
		} else if (This->rightCharacterManager.objectBase.position.y == 0)
			This->rightCharacterManager.objectBase.position.y += 10;
	} else if (old2 == CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER) {
		if (This->rightCharacterManager.objectBase.gravity.y < 0)
			This->rightCharacterManager.objectBase.gravity.y *= -1;
	}

	if (
		SokuLib::activeWeather != SokuLib::WEATHER_CLEAR &&
		SokuLib::displayedWeather == CUSTOMWEATHER_SHOOTING_STAR &&
		(timer / 100) != (SokuLib::weatherCounter / 100)
	) {
		SokuLib::playSEWaveBuffer(72);
		SokuLib::activeWeather = (SokuLib::Weather) selectRandomWeather(false);
		weatherActivate();
	}

	if (SokuLib::activeWeather == CUSTOMWEATHER_FORGETFUL_WIND) {
		timeStopLeft = 2;
		if (This->leftCharacterManager.projectileInvulTimer <= 1)
			This->leftCharacterManager.projectileInvulTimer = 2;
		if (This->rightCharacterManager.projectileInvulTimer <= 1)
			This->rightCharacterManager.projectileInvulTimer = 2;
	} else
		timeStopLeft = 0;

	desertMirageSwap(This);
	for (int i = 0 ; i < 2; i++) {
		auto chr = dataMgr->players[i];

		if (chr->swordOfRaptureDebuffTimeLeft || chr->effectiveWeather != CUSTOMWEATHER_IMPASSABLE_FOG) {
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

	auto &player = (SokuLib::mainMode == SokuLib::BATTLE_MODE_VSSERVER ? This->rightCharacterManager : This->leftCharacterManager);

	if (player.effectiveWeather == SokuLib::WEATHER_TWILIGHT) {
		if (viewWindow.tint.r != 255)
			viewWindow.tint.r += 15;
		else if (viewWindow.tint.a != 255)
			viewWindow.tint.a += 5;
	} else if (viewWindow.tint.a) {
		viewWindow.tint.a -= 5;
		viewWindow.tint.r = 0;
	}
	return ret;
}

void cleanup()
{
	puts("Cleanup");
	deleteCharacter(characterBackup[0] ? characterBackup[0] : extraCharacters[0]);
	deleteCharacter(characterBackup[1] ? characterBackup[1] : extraCharacters[1]);
	characterBackup[0] = nullptr;
	characterBackup[1] = nullptr;
	extraCharacters[0] = nullptr;
	extraCharacters[1] = nullptr;
	extra.first.character = SokuLib::CHARACTER_RANDOM;
	extra.second.character = SokuLib::CHARACTER_RANDOM;
	characterAlpha[0] = 255;
	characterAlpha[1] = 255;
	lastInputs[0].clear();
	lastInputs[1].clear();
	memset(characterSkills, 0, sizeof(characterSkills));
	needCleaning = false;
	puts("Done");
}

SokuLib::BattleManager *__fastcall CBattleManager_Destructor(SokuLib::BattleManager *This, int, char unknown)
{
	if (needCleaning)
		cleanup();
	return (This->*ogBattleMgrDestructor)(unknown);
}

int __fastcall CSelectCL_OnProcess(SokuLib::SelectClient *This)
{
	if (needCleaning)
		cleanup();
	return (This->*ogSelectClientOnProcess)();
}

int __fastcall CSelectSV_OnProcess(SokuLib::SelectServer *This)
{
	if (needCleaning)
		cleanup();
	return (This->*ogSelectServerOnProcess)();
}

int __fastcall CSelect_OnProcess(SokuLib::Select *This)
{
	if (needCleaning)
		cleanup();
	return (This->*ogSelectOnProcess)();
}

void checkCiF()
{
	if (*(unsigned *)0x47D7A0 == 0x5314EC83)
		return;

	EXCEPTION_RECORD local_32c;
	_EXCEPTION_POINTERS local_2dc;
	unsigned local_2d4 = 0x10001;

	memset(&local_32c, 0, 0x50);
	local_2dc.ExceptionRecord = &local_32c;
	local_2dc.ContextRecord = (PCONTEXT)&local_2d4;
	local_32c.ExceptionCode = STATUS_ASSERTION_FAILURE;
	UnhandledExceptionFilter(&local_2dc);
	ExitProcess(STATUS_ASSERTION_FAILURE);
}

int __fastcall CBattleManager_OnProcess(SokuLib::BattleManager *This)
{
	auto ret = (This->*ogBattleMgrOnProcess)();

	needCleaning = true;
	if (!init) {
		init = true;
		checkCiF();
		loadTexture(viewWindow, "data/infoeffect/view_window.png", true);
		viewWindow.tint.a = 0;
		printf("%p %p %p %p\n", dataMgr->players[0], dataMgr->players[1], &*extraCharacters[0], &*extraCharacters[1]);
	}
	return ret;
}

void __declspec(naked) weatherTimer_hook()
{
	__asm {
		MOV EAX, 0x8971C0
		MOV EAX, [EAX]
		MOV AX, [EAX * 0x02 + weatherTimes]
		MOV ECX, 0x8971CC
		MOV [ECX], AX
		JMP weatherTimerReturnAddr
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
	int ret = 0;

	if (SokuLib::sceneId != SokuLib::SCENE_BATTLEWATCH) {
		auto &player = *(SokuLib::sceneId == SokuLib::SCENE_BATTLECL ? This->p2State : This->p1State).player;

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
	} else {
		unsigned char a = viewWindow.tint.a;

		viewWindow.setPosition(
			SokuLib::Vector2f{This->p1State.player->objectBase.position.x, -This->p1State.player->objectBase.position.y}.to<int>() -
			viewWindow.texture.getSize() / 2 -
			SokuLib::Vector2i{0, 100}
		);
		setRenderMode(1);
		viewWindow.tint.a = (a * 2 / 3);
		viewWindow.draw();

		viewWindow.setPosition(
			SokuLib::Vector2f{This->p2State.player->objectBase.position.x, -This->p2State.player->objectBase.position.y}.to<int>() -
			viewWindow.texture.getSize() / 2 -
			SokuLib::Vector2i{0, 100}
		);
		viewWindow.draw();
		viewWindow.tint.a = a;
		ret = ogHudRender(This);
	}
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
	frames.at(id).restorePre();
}

void loadFramePost(unsigned id)
{
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
	float extraData[4];

	if (This->effectiveWeather == CUSTOMWEATHER_HAAR) {
		if (!characterSkills[i][0]) {
			characterSkills[i][0] = 1;
			memcpy(&characterSkills[i][1], This->skillMap, 15);

			auto nbSkills = 4 + (This->characterIndex == SokuLib::CHARACTER_PATCHOULI);

			memset(This->skillMap, 0xFF, 15);
			for (int i = 0; i < nbSkills; i++) {
				auto s = sokuRand(2);
				auto l = sokuRand(3 + (s != 0));

				This->skillMap[i + nbSkills * s].notUsed = false;
				This->skillMap[i + nbSkills * s].level = l + (i != 0);
			}
		}
	} else {
		if (characterSkills[i][0]) {
			characterSkills[i][0] = 0;
			memcpy(This->skillMap, &characterSkills[i][1], 15);
		}
	}
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
		extraData[0] = 0;
		extraData[1] = 0;
		extraData[2] = 0;
		ObjectHandler_SpawnBullet(This, 1110, This->objectBase.position.x, 0, This->objectBase.direction, 0xffffffff, extraData, 3);
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
		if (lastInputs[chr.isRightPlayer].size() != 10)
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

void __declspec(naked) chooseProjectileUpdate()
{
	__asm {
		// Check time stop
		CMP word ptr [EAX + 0x4A8], 0x0
		JNZ hasTimeStop
		CMP word ptr [EAX + 0x52C], CUSTOMWEATHER_FORGETFUL_WIND
		JZ hasTimeStop

		XOR EAX, EAX
		RET

	hasTimeStop:
		TEST EAX, EAX
		RET
	}
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

unsigned oldShader;
unsigned returnValue = 0x4AE3D5;

void __declspec(naked) changeBulletDisplay()
{
	__asm {
		MOV EAX, [ECX + 0x114]
		MOV [oldShader], EAX

		MOV EAX, [ECX + 0x168]
		CMP [EAX + 0x52C], CUSTOMWEATHER_FORGETFUL_WIND
		JNZ ret_

		CMP word ptr [ECX + 0x360], 0x0
		JNZ ret_

		MOV [ECX + 0x114], 1

	ret_:
		MOV AL, byte ptr [ESP + 0x4]
		PUSH ESI
		JMP [returnValue]
	}
}

void __declspec(naked) restoreBulletDisplay()
{
	__asm {
		MOV EAX, [oldShader]
		MOV [ESI + 0x114], EAX
		POP ESI
		RET 0x4
	}
}

#define MIDDLE 300

float charSpeedLookup[] = {
	4.5, -4.5, // CHARACTER_REIMU
	6,   -5,   // CHARACTER_MARISA
	6,   -6,   // CHARACTER_SAKUYA
	6,   -6,   // CHARACTER_ALICE
	4,   -4,   // CHARACTER_PATCHOULI
	4,   -4,   // CHARACTER_YOUMU
	6.5, -6.5, // CHARACTER_REMILIA
	4,   -4,   // CHARACTER_YUYUKO
	4.5, -4.5, // CHARACTER_YUKARI
	5.5, -5.5, // CHARACTER_SUIKA
	4,   -4,   // CHARACTER_REISEN
	5.5, -5.5, // CHARACTER_AYA
	4.5, -4.5, // CHARACTER_KOMACHI
	2.5, -3,   // CHARACTER_IKU
	4.5, -4.5, // CHARACTER_TENSHI
	6,   -5,   // CHARACTER_SANAE
	6.5, -6.5, // CHARACTER_CIRNO
	7,   -7,   // CHARACTER_MEILING
	3,   -3,   // CHARACTER_UTSUHO
	6,   -7,   // CHARACTER_SUWAKO
};

void aocfCheck(SokuLib::CharacterManager *chr, float newY, float oldY)
{
	if (chr->effectiveWeather != CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER)
		return;
	chr->airdashCount = 0;
	if (50 <= chr->objectBase.action && chr->objectBase.action < 150) {
		if (chr->damageLimited || chr->untech == 0 || chr->objectBase.hp == 0) {
			if (chr->objectBase.gravity.y < 0)
				chr->objectBase.gravity.y *= -1;
		} else if (newY < MIDDLE) {
			if (chr->objectBase.gravity.y > 0)
				chr->objectBase.gravity.y *= -1;
		} else {
			if (chr->objectBase.gravity.y < 0)
				chr->objectBase.gravity.y *= -1;
		}
	} else if (newY == MIDDLE || ((newY < MIDDLE) != (oldY < MIDDLE) && oldY != MIDDLE)) {
		chr->objectBase.position.y = MIDDLE;
		chr->objectBase.speed.y = 0;
		if (chr->objectBase.action >= SokuLib::ACTION_FORWARD_DASH);
		else if (!chr->keyMap.d && chr->keyMap.verticalAxis < 0)
			chr->objectBase.speed.y = std::abs(chr->objectBase.gravity.y) * 30;
		else if (!chr->keyMap.d && chr->keyMap.verticalAxis > 0)
			chr->objectBase.speed.y = -std::abs(chr->objectBase.gravity.y) * 30;
		else if (chr->keyMap.horizontalAxis > 0)
			chr->objectBase.speed.x = charSpeedLookup[chr->characterIndex * 2] * chr->objectBase.direction;
		else if (chr->keyMap.horizontalAxis < 0)
			chr->objectBase.speed.x = charSpeedLookup[chr->characterIndex * 2 + 1] * chr->objectBase.direction;
		else
			chr->objectBase.speed.x = 0;
	} else {
		if (newY < MIDDLE) {
			if (chr->objectBase.gravity.y > 0)
				chr->objectBase.gravity.y *= -1;
		} else {
			if (chr->objectBase.gravity.y < 0)
				chr->objectBase.gravity.y *= -1;
		}
	}
}

void __declspec(naked) aocfCheck_hook()
{
	__asm {
		MOV EAX, [ESI + 0xF0]
		PUSH EAX
		FSTP dword ptr [ESI + 0xF0]
		MOV EAX, [ESI + 0xF0]
		PUSH EAX
		PUSH ESI
		CALL aocfCheck
		ADD ESP, 12
		RET
	}
}

void __fastcall ObjectHandler_SpawnBullet_hook(SokuLib::CharacterManager *This, int, int action, float x, float y, int dir, unsigned color, float *extraData, unsigned extraDataSize)
{
	auto fct = ((void (__thiscall **)(SokuLib::ObjListManager &, int, SokuLib::CharacterManager *, int, float, float, int, unsigned, float *, unsigned))*(int *)This->objects.offset_0x00)[1];

	// Normal case, weather isn't activated
	if (This->effectiveWeather != CUSTOMWEATHER_RAGNAROK)
		return fct(This->objects, 0, This, action, x, y, dir, color, extraData, extraDataSize);
	// These are weather effects and stuff, no need to double them
	if (action >= 1000)
		return fct(This->objects, 0, This, action, x, y, dir, color, extraData, extraDataSize);
	// [1] is the velocity.
	// If it's set to 0 then the bullet probably doesn't support rotation,
	// so we just spawn them on top of each other
	if (extraDataSize < 3 || extraData[1] == 0) {
		fct(This->objects, 0, This, action, x, y - 15, dir, color, extraData, extraDataSize);
		fct(This->objects, 0, This, action, x, y + 15, dir, color, extraData, extraDataSize);
	} else {
		// -10°
		extraData[0] -= 10;
		fct(This->objects, 0, This, action, x, y, dir, color, extraData, extraDataSize);
		// +10°
		extraData[0] += 20;
		fct(This->objects, 0, This, action, x, y, dir, color, extraData, extraDataSize);
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
	SokuLib::TamperNearCall(0x4AE389, chooseProjectileUpdate);
	*(char *)0x4AE38E = 0x90;
	*(char *)0x4AE38F = 0x90;
	*(char *)0x4AE390 = 0x90;
	SokuLib::TamperNearJmp(0x4AE3D0, changeBulletDisplay);
	SokuLib::TamperNearJmp(0x4AE429, restoreBulletDisplay);
	*(char *)0x4AE40F = 0xEB;
	*(char *)0x4AE410 = 0x18;
	*(char *)0x4AE411 = 0x90;
	*(char *)0x4AE412 = 0x90;
	new SokuLib::Trampoline(0x4889BE, weatherEffect, 6);
	SokuLib::TamperNearCall(0x463680, aocfCheck_hook);
	SokuLib::TamperNearJmp(0x46EB30/* ObjectHandler_SpawnBullet */, ObjectHandler_SpawnBullet_hook);
	*(char *)0x463685 = 0x90;
	new SokuLib::Trampoline(0x4818C3, loadExtraCharacters, 6);

	// Increase speed of timer in clear
	*(unsigned char *)0x48242B = WEATHER_TIMER_MULTIPLIER;

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