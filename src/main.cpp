//
// Created by PinkySmile on 31/10/2020
//

#include <SokuLib.hpp>
#include <map>
#include <shlwapi.h>
#include <algorithm>

#ifndef _DEBUG
#define puts(...)
#define printf(...)
#endif

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

#define WRAP_SIZE 1280
#define WRAP_LEFT_CORNER 0
#define WRAP_RIGHT_CORNER 1280
#define CLONE_DESYNC_DELAY 12
#define HOT_WIND_FORCED_DAMAGE 750
#define HOT_WIND_FORCED_DAMAGE_CH 3000
#define COIN_EFFECT_DURATION 600
// #define DISABLE_VANILLA
// #define FORCE_WEATHER CUSTOMWEATHER_FROST
// #define WEATHER_TIMER_MULTIPLIER 3
#define MISSING_PURPLE_MIST_SMOOTHING_TIME 30

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
	CUSTOMWEATHER_MISSING_PURPLE_MIST,
	CUSTOMWEATHER_WATER_HAZE,
	CUSTOMWEATHER_REVERSE_FIELD,
	CUSTOMWEATHER_ILLUSION_MIST,
	CUSTOMWEATHER_ETERNAL_NIGHT,
	CUSTOMWEATHER_FROST,
	CUSTOMWEATHER_HOT_WIND,
	CUSTOMWEATHER_MYSTERIOUS_WIND,
	CUSTOMWEATHER_SIZE
};

static std::map<unsigned, std::string> weatherNames{
	{ SokuLib::WEATHER_SUNNY,                   "Sunny" },
	{ SokuLib::WEATHER_DRIZZLE,                 "Drizzle" },
	{ SokuLib::WEATHER_CLOUDY,                  "Cloudy" },
	{ SokuLib::WEATHER_BLUE_SKY,                "BlueSky" },
	{ SokuLib::WEATHER_HAIL,                    "Hail" },
	{ SokuLib::WEATHER_SPRING_HAZE,             "SpringHaze" },
	{ SokuLib::WEATHER_HEAVY_FOG,               "HeavyFog" },
	{ SokuLib::WEATHER_SNOW,                    "Snow" },
	{ SokuLib::WEATHER_SUN_SHOWER,              "SunShower" },
	{ SokuLib::WEATHER_SPRINKLE,                "Sprinkle" },
	{ SokuLib::WEATHER_TEMPEST,                 "Tempest" },
	{ SokuLib::WEATHER_MOUNTAIN_VAPOR,          "MountainVapor" },
	{ SokuLib::WEATHER_RIVER_MIST,              "RiverMist" },
	{ SokuLib::WEATHER_TYPHOON,                 "Typhoon" },
	{ SokuLib::WEATHER_CALM,                    "Calm" },
	{ SokuLib::WEATHER_DIAMOND_DUST,            "DiamondDust" },
	{ SokuLib::WEATHER_DUST_STORM,              "DustStorm" },
	{ SokuLib::WEATHER_SCORCHING_SUN,           "ScorchingSun" },
	{ SokuLib::WEATHER_MONSOON,                 "Monsoon" },
	{ SokuLib::WEATHER_AURORA,                  "Aurora" },
	{ SokuLib::WEATHER_TWILIGHT,                "Twilight" },
	{ CUSTOMWEATHER_ANGEL_HALO,                 "AngelHalo" },
	{ CUSTOMWEATHER_DESERT_MIRAGE,              "DesertMirage" },
	{ CUSTOMWEATHER_SHOOTING_STAR,              "ShootingStar" },
	{ CUSTOMWEATHER_THUNDERSTORM,               "Thunderstorm" },
	{ CUSTOMWEATHER_IMPASSABLE_FOG,             "ImpassableFog" },
	{ CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER, "AntinomyOfCommonWeather" },
	{ CUSTOMWEATHER_FORGETFUL_WIND,             "ForgetfulWind" },
	{ CUSTOMWEATHER_BLIZZARD,                   "Blizzard" },
	{ CUSTOMWEATHER_RAGNAROK,                   "Ragnarok" },
	{ CUSTOMWEATHER_HAAR,                       "Haar" },
	{ CUSTOMWEATHER_MISSING_PURPLE_MIST,        "MissingPurpleMist" },
	{ CUSTOMWEATHER_WATER_HAZE,                 "WaterHaze" },
	{ CUSTOMWEATHER_REVERSE_FIELD,              "ReverseField" },
	{ CUSTOMWEATHER_ILLUSION_MIST,              "IllusionMist" },
	{ CUSTOMWEATHER_ETERNAL_NIGHT,              "EternalNight" },
	{ CUSTOMWEATHER_FROST,                      "Frost" },
	{ CUSTOMWEATHER_HOT_WIND,                   "HotWind" },
	{ CUSTOMWEATHER_MYSTERIOUS_WIND,            "MysteriousWind" },
};
std::map<unsigned, unsigned> order;
unsigned forcedWeather;
unsigned clearMultiplier;

void deleteCharacter(SokuLib::CharacterManager *p)
{
	(*(void (__thiscall **)(SokuLib::CharacterManager *, char))p->objectBase.vtable)(p, 0);
	SokuLib::Delete(p);
}

enum BlockResult {
	BLOCKRESULT_HIT,
	BLOCKRESULT_HIGH_RIGHTBLOCK,
	BLOCKRESULT_HIGH_WRONGBLOCKBLOCK,
	BLOCKRESULT_LOW_RIGHTBLOCK,
	BLOCKRESULT_LOW_WRONGBLOCKBLOCK,
	BLOCKRESULT_AIRBLOCK,
	BLOCKRESULT_GUARDPOINT
};

std::map<SokuLib::Character, SokuLib::Character> soku2BaseCharacters{
	{ SokuLib::CHARACTER_MOMIJI,     SokuLib::CHARACTER_REIMU },
	{ SokuLib::CHARACTER_CLOWNPIECE, SokuLib::CHARACTER_REISEN },
	{ SokuLib::CHARACTER_FLANDRE,    SokuLib::CHARACTER_REMILIA },
	{ SokuLib::CHARACTER_ORIN,       SokuLib::CHARACTER_CIRNO },
	{ SokuLib::CHARACTER_YUUKA,      SokuLib::CHARACTER_UTSUHO },
	{ SokuLib::CHARACTER_KAGUYA,     SokuLib::CHARACTER_PATCHOULI },
	{ SokuLib::CHARACTER_MOKOU,      SokuLib::CHARACTER_AYA },
	{ SokuLib::CHARACTER_MIMA,       SokuLib::CHARACTER_IKU },
	{ SokuLib::CHARACTER_SHOU,       SokuLib::CHARACTER_TENSHI },
	{ SokuLib::CHARACTER_MURASA,     SokuLib::CHARACTER_SUIKA },
	{ SokuLib::CHARACTER_SEKIBANKI,  SokuLib::CHARACTER_YOUMU },
	{ SokuLib::CHARACTER_SATORI,     SokuLib::CHARACTER_YUYUKO },
	{ SokuLib::CHARACTER_RAN,        SokuLib::CHARACTER_SAKUYA },
};

static void (__thiscall *ogBattleMgrHandleCollision)(SokuLib::BattleManager*, void*, SokuLib::CharacterManager*);
static void (__thiscall *ogRenderObjectLayer)(GameDataManager *, int);
static void (__thiscall *ogRenderCharacters)(GameDataManager *);
static BlockResult (__thiscall *og_checkHit)(SokuLib::CharacterManager *, SokuLib::AttackFlags);
static SokuLib::BattleManager *(SokuLib::BattleManager::*ogBattleMgrDestructor)(char unknown);
static int (SokuLib::BattleManager::*ogBattleMgrOnMatchProcess)();
static int (SokuLib::BattleManager::*ogBattleMgrOnProcess)();
static void (SokuLib::BattleManager::*ogBattleMgrOnRender)();
static int (SokuLib::SelectClient::*ogSelectClientOnProcess)();
static int (SokuLib::SelectServer::*ogSelectServerOnProcess)();
static int (SokuLib::Select::*ogSelectOnProcess)();
static int (SokuLib::LoadingWatch::*ogLoadingWatchOnProcess)();
static int (__thiscall *ogHudRender)(void *);

#define setRenderMode(mode) ((void (__thiscall *)(int, int))0x404B80)(0x896B4C, mode)
static const auto sokuRand = reinterpret_cast<int (*)(int)>(0x4099F0);
static const auto switchWeather = (void (__thiscall *)(unsigned, unsigned))0x4388e0;
const auto ObjectHandler_SpawnBullet = (void (__thiscall *)(void *This, int action, float x, float y, unsigned char dir, unsigned color, float *extraData, int dataSize))0x46EB30;
const auto FUN_00438ce0 = reinterpret_cast<void (__thiscall *)(void *, unsigned, float, float, unsigned, unsigned)>(0x438CE0);

static CInfoManager *&hud = *(CInfoManager **)0x8985E8;
static GameDataManager*& dataMgr = *(GameDataManager**)SokuLib::ADDR_GAME_DATA_MANAGER;

struct CloneInfo {
	SokuLib::SpriteEx sprite;
	SokuLib::v2::FrameData *img;
	SokuLib::v2::CharacterFrameData *data;
	SokuLib::Vector2f pos;
	SokuLib::Vector2f center;
	SokuLib::RenderInfo infos;
	int hasHit;
	SokuLib::Direction dir;
	unsigned char hitCount;

	CloneInfo(SokuLib::v2::Player *obj)
	{
		this->sprite = obj->sprite;
		this->center = obj->center;
		this->hasHit = obj->collisionType;
		this->dir = obj->direction;
		this->hitCount = obj->collisionLimit;
		this->img = obj->frameData;
		this->data = obj->gameData.frameData;
		this->pos = obj->position;
		this->infos = obj->renderInfos;
	};

	void restore(SokuLib::v2::GameObject *obj)
	{
		obj->sprite = this->sprite;
		obj->center = this->center;
		if (this->hasHit == 0) {
			obj->collisionType = 0;
			obj->collisionLimit = this->hitCount;
		}
		obj->direction = this->dir;
		obj->frameData = this->img;
		obj->position = this->pos;
		obj->renderInfos = this->infos;

		SokuLib::DeleteFct(obj->gameData.frameData);
		obj->gameData.frameData = (SokuLib::v2::CharacterFrameData *)SokuLib::NewFct(sizeof(*obj->gameData.frameData));
		memcpy(obj->gameData.frameData, this->data, sizeof(*obj->gameData.frameData));
		obj->gameData.frameData->collisionBox = nullptr;
	};
};

static bool clonesSpawned = false;
static bool needCleaning = false;
static bool init = false;
static SokuLib::SWRFont font;
static unsigned char clonesAlpha[2] = {0, 0};
static std::list<CloneInfo> lastClones[2];
static unsigned char characterAlpha[2] = {255, 255};
static unsigned char characterSizeCtr[2] = {0, 0};
static unsigned char characterSkills[2][16] = {{0}, {0}};
static SokuLib::CharacterManager *characterBackup[2] = {nullptr, nullptr};
static SokuLib::CharacterManager *extraCharacters[2] = {nullptr, nullptr};
static std::pair<SokuLib::PlayerInfo, SokuLib::PlayerInfo> *extra = nullptr;
static SokuLib::DrawUtils::Sprite viewWindow;
static std::list<SokuLib::KeyInput> lastInputs[2];
static unsigned short timeStopLeft = 0;
static float zeroFive = 0.5;
static float frostSlide = 0.1;


auto allocAndStoreArray3 = (void ***(__thiscall *)(void *, void *, void *))0x6FB680;
auto checkListTooLong = (void (__thiscall *)(void *, unsigned))0x47D030;
auto FUN_0067d670 = (int *(__thiscall *)(SokuLib::ObjListManager *This, int param_1, SokuLib::CharacterManager &owner, unsigned action, float x, float y, int dir, unsigned color, float *extraData, unsigned extraDataSize))0x67D670;

#define _STR(s) #s
#define STR(s) _STR(s)
#define gassert(expr) do { if (!(expr)) { MessageBoxA(SokuLib::window, "Debug assertion " #expr ", line " STR(__LINE__) " was false.", "Debug assertion failed", MB_ICONERROR); abort(); } } while (0)
#define my_assert(expr) do { if (!(expr)) { MessageBoxA(SokuLib::window, "Debug assertion " #expr " was false.", "Debug assertion failed", MB_ICONERROR); return FALSE; } } while (0)


class CloneObject : public SokuLib::v2::GameObject {
public:
	CloneObject() {
		this->frameData = nullptr;
		this->gameData.frameData = nullptr;
	};
	~CloneObject() override {
		SokuLib::DeleteFct(this->gameData.frameData);
	};

	void setActionSequence(short action, short seq) override {
		printf("setActionSequence: %p %i %i\n", this, action, seq);
	}

	bool setAction(short action) override {
		printf("setAction: %p %i\n", this, action);
		int index = (void *)&SokuLib::getBattleMgr().leftCharacterManager == this->gameData.owner;

		this->gameData.sequenceData = this->gameData.owner->gameData.sequenceData;
		if (!this->frameData)
			this->frameData = this->gameData.owner->frameData;
		if (!this->gameData.frameData) {
			this->gameData.frameData = (SokuLib::v2::CharacterFrameData *)SokuLib::NewFct(sizeof(*this->gameData.frameData));
			memcpy(this->gameData.frameData, this->gameData.owner->gameData.frameData, sizeof(*this->gameData.frameData));
		}
		lastClones[index].emplace_back(this->gameData.owner);
		gassert(this->gameData.frameData);
		return true;
	}

	void setSequence(short seq) override {
		printf("setSequence: %p %i\n", this, seq);
	}

	void resetSequence() override {
		printf("resetSequence: %p\n", this);
	}

	bool nextSequence() override {
		printf("nextSequence: %p\n", this);
		return true;
	}

	void prevSequence() override {
		printf("prevSequence: %p\n", this);
	}

	void setPose(short pose) override {
		printf("setPose: %p %i\n", this, pose);
	}

	bool nextPose() override {
		printf("nextPose: %p\n", this);
		return true;
	}

	void prevPose() override {
		printf("prevPose: %p\n", this);
	}

	void update() override {
		int index = (void *)&SokuLib::getBattleMgr().leftCharacterManager == this->gameData.owner;

		this->lifetime = 1;
		gassert(!lastClones[index].empty());
		lastClones[index].front().restore(this);
		this->renderInfos.color = SokuLib::Color{0xFF, 0xFF, 0xFF, clonesAlpha[index]};
		this->renderInfos.shaderType = 1;
		this->boxData.frameData = nullptr;
		if (this->gameData.owner->weatherId == CUSTOMWEATHER_ILLUSION_MIST) {
			if (clonesAlpha[index] < 0x80)
				clonesAlpha[index] += 4;
			memset(&this->gameData.frameData->hurtBoxes, 0, 0x10);
		} else {
			if (clonesAlpha[index] != 0)
				clonesAlpha[index] -= 4;
			memset(&this->gameData.frameData->hurtBoxes, 0, 0x50);
		}
	}

	void render() override {
		printf("render: %p\n", this);
	}

	void render2() override {
		//printf("render2: %p\n", this);
		((void (__thiscall *)(SokuLib::v2::AnimationObject *))0x439190)(this);
	}

	void applyTransform() override {
		//printf("applyTransform: %p\n", this);
		((void (__thiscall *)(SokuLib::v2::AnimationObject *))0x438E00)(this);
	}

	void onRenderEnd() override {
		//printf("onRenderEnd: %p\n", this);
		((void (__thiscall *)(SokuLib::v2::AnimationObject *))0x439040)(this);
	}

	bool initializeAction() override {
		printf("initializeAction: %p\n", this);
		return true;
	}

	void updatePhysics() override {
		int index = (void *)&SokuLib::getBattleMgr().leftCharacterManager == this->gameData.owner;

		// printf("updatePhysics: %p\n", this);
		if (lastClones[index].size() > CLONE_DESYNC_DELAY)
			lastClones[index].pop_front();
		lastClones[index].emplace_back(this->gameData.owner);
	};

	GameObject* createObject(SokuLib::Action actionId, float x, int y, SokuLib::Direction dir, char layer, void* customData, unsigned int customDataSize) override {
		printf("createObject %p %i %f %i %i %i %p %i\n", this, actionId, x, y, dir, layer, customData, customDataSize);
		return nullptr;
	}

	GameObject* createChild(SokuLib::Action actionId, float x, int y, SokuLib::Direction dir, char layer, void* customData, unsigned int customDataSize) override {
		printf("createChild %p %i %f %i %i %i %p %i\n", this, actionId, x, y, dir, layer, customData, customDataSize);
		return nullptr;
	}
};


template<class T>
T *__fastcall create(T *This)
{
	return new (This) T;
}

template<typename T>
void pushToList(SokuLib::CharacterManager &chr)
{
	T c;
	auto vt = *(unsigned *)&c;
	DWORD old;

	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	auto fct1 = SokuLib::TamperNearJmpOpr(0x67D2C7, &create<T>);
	auto fct2 = SokuLib::TamperNearJmpOpr(0x67D3B3, &create<T>);
	auto vt1 = SokuLib::TamperDword(0x67D2CE, vt);
	auto vt2 = SokuLib::TamperDword(0x67D3BA, vt);
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);
	FlushInstructionCache(GetCurrentProcess(), nullptr, 0);

	FUN_0067d670(&chr.objects, 0, chr, 800, 0, 0, 1, 0xFFFFFFFF, nullptr, 0);

	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	SokuLib::TamperNearJmpOpr(0x67D2C7, fct1);
	SokuLib::TamperNearJmpOpr(0x67D3B3, fct2);
	SokuLib::TamperDword(0x67D2CE, vt1);
	SokuLib::TamperDword(0x67D3BA, vt2);
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, old, &old);
	FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
}


class SavedFrame {
private:
	bool illusionMist_clonesSpawned;
	unsigned char illusionMist_clonesAlpha1;
	unsigned char illusionMist_clonesAlpha2;
	std::list<CloneInfo> illusionMist_lastClones1;
	std::list<CloneInfo> illusionMist_lastClones2;
	bool desertMirage_swapped1;
	bool desertMirage_swapped2;
	unsigned char impassableSmog_characterAlpha1;
	unsigned char impassableSmog_characterAlpha2;
	unsigned char missingPurpleMist_characterScale1;
	unsigned char missingPurpleMist_characterScale2;
	unsigned char twilight_alpha;
	unsigned char twilight_red;
	unsigned char haar_characterSkills1[16];
	unsigned char haar_characterSkills2[16];
	std::list<SokuLib::KeyInput> blizzard_lastInputs1;
	std::list<SokuLib::KeyInput> blizzard_lastInputs2;

public:
	SavedFrame()
	{
		this->illusionMist_clonesSpawned = clonesSpawned;
		this->illusionMist_clonesAlpha1 = clonesAlpha[0];
		this->illusionMist_clonesAlpha2 = clonesAlpha[1];
		this->illusionMist_lastClones1 = lastClones[0];
		this->illusionMist_lastClones2 = lastClones[1];
		this->desertMirage_swapped1 = characterBackup[0] != nullptr;
		this->desertMirage_swapped2 = characterBackup[1] != nullptr;
		this->impassableSmog_characterAlpha1 = characterAlpha[0];
		this->impassableSmog_characterAlpha2 = characterAlpha[1];
		this->missingPurpleMist_characterScale1 = characterSizeCtr[0];
		this->missingPurpleMist_characterScale2 = characterSizeCtr[1];
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
		clonesSpawned = this->illusionMist_clonesSpawned;
		clonesAlpha[0] = this->illusionMist_clonesAlpha1;
		clonesAlpha[1] = this->illusionMist_clonesAlpha2;
		lastClones[0] = this->illusionMist_lastClones1;
		lastClones[1] = this->illusionMist_lastClones2;
		memcpy(characterSkills[0], this->haar_characterSkills1, sizeof(this->haar_characterSkills1));
		memcpy(characterSkills[1], this->haar_characterSkills2, sizeof(this->haar_characterSkills2));
		lastInputs[0] = this->blizzard_lastInputs1;
		lastInputs[1] = this->blizzard_lastInputs2;
		characterAlpha[0] = this->impassableSmog_characterAlpha1;
		characterAlpha[1] = this->impassableSmog_characterAlpha2;
		characterSizeCtr[0] = this->missingPurpleMist_characterScale1;
		characterSizeCtr[1] = this->missingPurpleMist_characterScale2;
		viewWindow.tint.a = this->twilight_alpha;
		viewWindow.tint.r = this->twilight_red;
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
	499, // CUSTOMWEATHER_SHOOTING_STAR
	500, // CUSTOMWEATHER_THUNDERSTORM
	500, // CUSTOMWEATHER_IMPASSABLE_FOG
	750, // CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER
	500, // CUSTOMWEATHER_FORGETFUL_WIND
	250, // CUSTOMWEATHER_BLIZZARD
	999, // CUSTOMWEATHER_RAGNAROK
	999, // CUSTOMWEATHER_HAAR
	666, // CUSTOMWEATHER_MISSING_PURPLE_MIST
	750, // CUSTOMWEATHER_WATER_HAZE
	999, // CUSTOMWEATHER_REVERSE_FIELD
	500, // CUSTOMWEATHER_ILLUSION_MIST
	 90, // CUSTOMWEATHER_ETERNAL_NIGHT
	999, // CUSTOMWEATHER_FROST
	999, // CUSTOMWEATHER_HOT_WIND
	999, // CUSTOMWEATHER_MYSTERIOUS_WIND
};

void loadExtraCharacters()
{
	auto o1 = dataMgr->players[0];
	auto o2 = dataMgr->players[1];

	if (!extra)
		extra = new std::pair<SokuLib::PlayerInfo, SokuLib::PlayerInfo>();

	extra->first.character = static_cast<SokuLib::Character>(sokuRand(SokuLib::CHARACTER_RANDOM));
	if (SokuLib::leftChar > SokuLib::CHARACTER_RANDOM && soku2BaseCharacters[SokuLib::leftChar] == extra->first.character)
		extra->first.character = static_cast<SokuLib::Character>((extra->first.character + 1) % SokuLib::CHARACTER_RANDOM);

	extra->second.character = static_cast<SokuLib::Character>(sokuRand(SokuLib::CHARACTER_RANDOM));
	if (SokuLib::rightChar > SokuLib::CHARACTER_RANDOM && soku2BaseCharacters[SokuLib::rightChar] == extra->second.character)
		extra->second.character = static_cast<SokuLib::Character>((extra->second.character + 1) % SokuLib::CHARACTER_RANDOM);

	extra->first.palette = 0;
	extra->second.palette = extra->first.character == extra->second.character;
	extra->second.isRight = true;

	puts("Loading character 1");
	((void (__thiscall *)(GameDataManager*, int, SokuLib::PlayerInfo &))0x46da40)(dataMgr, 0, extra->first);
	puts("Init");
	(*(void (__thiscall **)(SokuLib::CharacterManager *))(*(int *)dataMgr->players[0] + 0x44))(dataMgr->players[0]);
	extraCharacters[0] = dataMgr->players[0];
	puts("Loading character 2");
	((void (__thiscall *)(GameDataManager*, int, SokuLib::PlayerInfo &))0x46da40)(dataMgr, 1, extra->second);
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
	printf("Forced weather %i\n", forcedWeather);
	if (forcedWeather != SokuLib::WEATHER_CLEAR)
		return forcedWeather;

	std::vector<unsigned> weathers;

	weathers.reserve(order.size());
	for (auto &w : order)
		if (includeAurora || (w.first != SokuLib::WEATHER_AURORA && w.first != CUSTOMWEATHER_SHOOTING_STAR))
			weathers.push_back(w.first);

	if (weathers.empty()) {
		puts("No weathers!");
		return SokuLib::WEATHER_CLEAR;
	}

	auto r = sokuRand(weathers.size());

	if (r == weathers.size())
		r--;
	printf("Selected weather %i -> %i\n", r, weathers[r]);
	return weathers[r];
}

int selectNextWeather(SokuLib::Weather weatherId, bool noAurora)
{
	unsigned w = weatherId;

	printf("selectNextWeather %i %s -> ", weatherId, noAurora ? "false" : "true");
	fflush(stdout);
	if (w == SokuLib::WEATHER_CLEAR) {
		printf("case %i\n",order.begin()->first);
		return order.begin()->first;
	}
	do {
		w = order.at(w);
	} while (w != weatherId && noAurora && (
		w == SokuLib::WEATHER_AURORA ||
		w == CUSTOMWEATHER_SHOOTING_STAR
	));
	printf("%i\n", w);
	return w;
}

int selectPreviousWeather(SokuLib::Weather weatherId, bool noAurora)
{
	unsigned w = weatherId;

	printf("selectPreviousWeather %i %s -> ", weatherId, noAurora ? "false" : "true");
	fflush(stdout);
	if (w == SokuLib::WEATHER_CLEAR) {
		printf("case %i\n",order.begin()->first);
		return order.begin()->first;
	}
	do {
		w = std::find_if(order.begin(), order.end(), [w](std::pair<unsigned, unsigned> p) {
			return p.second == w;
		})->first;
	} while (w != weatherId && noAurora && (
		w == SokuLib::WEATHER_AURORA ||
		w == CUSTOMWEATHER_SHOOTING_STAR
	));
	printf("%i\n", w);
	return w;
}

void __fastcall handleSwitchWeather(unsigned obj, SokuLib::Weather weatherId)
{
	switchWeather(obj, selectNextWeather(weatherId, false));
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

	if (SokuLib::activeWeather == CUSTOMWEATHER_FORGETFUL_WIND || SokuLib::activeWeather == CUSTOMWEATHER_ETERNAL_NIGHT) {
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
	SokuLib::Weather old[2] = {
		This->leftCharacterManager.effectiveWeather,
		This->rightCharacterManager.effectiveWeather
	};
	auto ret = (This->*ogBattleMgrOnMatchProcess)();

	if (forcedWeather != SokuLib::WEATHER_CLEAR)
		SokuLib::weatherCounter = 999;
	if (This->leftCharacterManager.effectiveWeather == CUSTOMWEATHER_MISSING_PURPLE_MIST)
		This->leftCharacterManager.objectBase.center.y = 0;
	if (This->rightCharacterManager.effectiveWeather == CUSTOMWEATHER_MISSING_PURPLE_MIST)
		This->rightCharacterManager.objectBase.center.y = 0;
	if (!clonesSpawned) {
		clonesSpawned = true;
		pushToList<CloneObject>(This->leftCharacterManager);
		pushToList<CloneObject>(This->rightCharacterManager);
	}
	for (int i = 0; i < 2; i++) {
		if (dataMgr->players[i]->effectiveWeather == CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER) {
			if (
				(dataMgr->players[i]->untech == 0 && 50 <= dataMgr->players[i]->objectBase.action && dataMgr->players[i]->objectBase.action < 150) ||
				dataMgr->players[i]->objectBase.hp == 0 ||
				dataMgr->players[i]->damageLimited
			) {
				if (dataMgr->players[i]->objectBase.gravity.y < 0)
					dataMgr->players[i]->objectBase.gravity.y *= -1;
			} else if (dataMgr->players[i]->objectBase.position.y == 0)
				dataMgr->players[i]->objectBase.position.y += 10;
		} else if (old[i] == CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER) {
			if (dataMgr->players[i]->objectBase.gravity.y < 0)
				dataMgr->players[i]->objectBase.gravity.y *= -1;
		}
		if (dataMgr->players[i]->effectiveWeather == CUSTOMWEATHER_ETERNAL_NIGHT) {
			dataMgr->players[i]->objectBase.renderInfos.shaderType = 1;
			dataMgr->players[i]->objectBase.hitstop++;
		} else if (old[i] == CUSTOMWEATHER_ETERNAL_NIGHT)
			dataMgr->players[i]->objectBase.renderInfos.shaderType = 0;
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

	if (SokuLib::activeWeather == CUSTOMWEATHER_ETERNAL_NIGHT)
		timeStopLeft = 2;
	else if (SokuLib::activeWeather == CUSTOMWEATHER_FORGETFUL_WIND) {
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
		auto weather = chr->effectiveWeather;

		if (chr->swordOfRaptureDebuffTimeLeft)
			weather = SokuLib::WEATHER_CLEAR;

		if (weather != CUSTOMWEATHER_IMPASSABLE_FOG) {
			if (characterAlpha[i] != 255) {
				if (characterAlpha[i] > 250)
					characterAlpha[i] = 255;
				else
					characterAlpha[i] += 5;
				if (SokuLib::mainMode == SokuLib::BATTLE_MODE_VSWATCH || SokuLib::subMode == SokuLib::BATTLE_SUBMODE_REPLAY)
					chr->objectBase.renderInfos.color.a = (100 + (characterAlpha[i] * 155/ 255));
				else
					//chr->objectBase.renderInfos.color.a = (100 + (characterAlpha[i] * 155/ 255));
					chr->objectBase.renderInfos.color.a = characterAlpha[i];
			}
		} else {
			if (characterAlpha[i] < 5)
				characterAlpha[i] = 0;
			else
				characterAlpha[i] -= 5;
			if (SokuLib::mainMode == SokuLib::BATTLE_MODE_VSWATCH || SokuLib::subMode == SokuLib::BATTLE_SUBMODE_REPLAY)
				chr->objectBase.renderInfos.color.a = (100 + (characterAlpha[i] * 155/ 255));
			else
				//chr->objectBase.renderInfos.color.a = (100 + (characterAlpha[i] * 155/ 255));
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
	extra->first.character = SokuLib::CHARACTER_RANDOM;
	extra->second.character = SokuLib::CHARACTER_RANDOM;
	characterAlpha[0] = 255;
	characterAlpha[1] = 255;
	characterSizeCtr[0] = 0;
	characterSizeCtr[1] = 0;
	lastInputs[0].clear();
	lastInputs[1].clear();
	lastClones[0].clear();
	lastClones[1].clear();
	clonesAlpha[0] = 0;
	clonesAlpha[1] = 0;
	clonesSpawned = false;
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

int __fastcall CLoadingWatch_OnProcess(SokuLib::LoadingWatch *This)
{
	if (needCleaning)
		cleanup();
	return (This->*ogLoadingWatchOnProcess)();
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

void __fastcall CBattleManager_OnRender(SokuLib::BattleManager *This)
{
	auto players = (SokuLib::CharacterManager **)((int)This + 0xC);
	std::map<SokuLib::ObjectManager *, SokuLib::Vector2f> scale;

	for (int i = 0; i < 2; i++) {
		scale[&players[i]->objectBase] = players[i]->objectBase.renderInfos.scale;
		players[i]->objectBase.renderInfos.scale *= (1 + (float)characterSizeCtr[i] / MISSING_PURPLE_MIST_SMOOTHING_TIME);
		for (auto obj : players[i]->objects.list.vector()) {
			scale[obj] = obj->renderInfos.scale;
			if (!((SokuLib::v2::AnimationObject *)obj)->isGui)
				obj->renderInfos.scale *= (1 + (float) characterSizeCtr[i] / MISSING_PURPLE_MIST_SMOOTHING_TIME);
		}
	}
	(This->*ogBattleMgrOnRender)();
	for (int i = 0; i < 2; i++) {
		players[i]->objectBase.renderInfos.scale = scale[&players[i]->objectBase];
		for (auto obj : players[i]->objects.list.vector())
			obj->renderInfos.scale = scale[obj];
	}
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
		printf("%p %p %p %p\n", dataMgr->players[0], dataMgr->players[1], extraCharacters[0], extraCharacters[1]);
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
			if (SokuLib::displayedWeather == weather && SokuLib::activeWeather != SokuLib::WEATHER_CLEAR) {
				if (SokuLib::displayedWeather == CUSTOMWEATHER_REVERSE_FIELD)
					FUN_006dcdf0(startObj + 5, 320.0, 140.0, 1, 1);
				else
					FUN_006dcdf0(startObj + 5, 320.0, 120.0, 1, 1);
			}
		} else if (obj->frameState.currentFrame == 0 && SokuLib::displayedWeather == CUSTOMWEATHER_SHOOTING_STAR)
			FUN_006dcdf0(startObj + 3, obj->position.x, obj->position.y - 10.0f, 1, 1);
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
		else if (weather == CUSTOMWEATHER_REVERSE_FIELD)
			infoEffectUpdateSwitch_bool = SokuLib::displayedWeather == weather && SokuLib::activeWeather == SokuLib::WEATHER_CLEAR;
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

	if (SokuLib::mainMode != SokuLib::BATTLE_MODE_VSWATCH && SokuLib::subMode != SokuLib::BATTLE_SUBMODE_REPLAY) {
		auto &player = *(SokuLib::mainMode == SokuLib::BATTLE_MODE_VSSERVER ? This->p2State : This->p1State).player;

		viewWindow.setPosition(
			SokuLib::Vector2f{player.objectBase.position.x, -player.objectBase.position.y}.to<int>() -
			viewWindow.texture.getSize() / 2 -
			SokuLib::Vector2i{0, 100}
		);
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
	}
	return ogHudRender(This);
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

void weatherEffectReset()
{
	SokuLib::CharacterManager *This;
	__asm MOV [This], ESI

	int i = This == dataMgr->players[1];
	auto weather = This->effectiveWeather;

	if (This->swordOfRaptureDebuffTimeLeft > 0)
		weather = SokuLib::WEATHER_CLEAR;

	if (weather != CUSTOMWEATHER_HAAR) {
		if (characterSkills[i][0]) {
			characterSkills[i][0] = 0;
			memcpy(This->skillMap, &characterSkills[i][1], 15);
		}
	}

	if (weather != CUSTOMWEATHER_MISSING_PURPLE_MIST) {
		if (characterSizeCtr[i] <= 0)
			characterSizeCtr[i] = 0;
		else
			characterSizeCtr[i]--;
	}
}

void weatherEffectSet()
{
	SokuLib::CharacterManager *This;
	__asm MOV [This], ESI

	int index = This == dataMgr->players[1];
	float extraData[4];
	auto weather = This->effectiveWeather;

	if (This->swordOfRaptureDebuffTimeLeft > 0)
		weather = SokuLib::WEATHER_CLEAR;

	switch (weather) {
	case CUSTOMWEATHER_MYSTERIOUS_WIND:
		if (This->objectBase.position.x < WRAP_LEFT_CORNER)
			This->objectBase.position.x += WRAP_SIZE;
		else if (This->objectBase.position.x > WRAP_RIGHT_CORNER)
			This->objectBase.position.x -= WRAP_SIZE;
		for (auto &obj : This->objects.list.vector()) {
			if (obj->position.x < WRAP_LEFT_CORNER)
				obj->position.x += WRAP_SIZE;
			else if (obj->position.x > WRAP_RIGHT_CORNER)
				obj->position.x -= WRAP_SIZE;
		}
		break;
	case CUSTOMWEATHER_HOT_WIND:
		This->offset_0x56F = true;
		break;
	case CUSTOMWEATHER_FROST:
		if (This->objectBase.action <= SokuLib::ACTION_5A)
			This->groundDashCount = 1;
		break;
	case CUSTOMWEATHER_HAAR:
		if (!characterSkills[index][0]) {
			characterSkills[index][0] = 1;
			memcpy(&characterSkills[index][1], This->skillMap, 15);

			auto nbSkills = 4 + (This->characterIndex == SokuLib::CHARACTER_PATCHOULI || This->characterIndex == SokuLib::CHARACTER_KAGUYA);

			memset(This->skillMap, 0xFF, 15);
			for (int i = 0; i < nbSkills; i++) {
				auto s = sokuRand(3);
				auto l = sokuRand(4 + (s != 0));

				s -= (s == 3);
				l += (s == 0);
				l -= (l == 5);
				This->skillMap[i + nbSkills * s].notUsed = false;
				This->skillMap[i + nbSkills * s].level = l;
			}
		}
		break;
	case CUSTOMWEATHER_ANGEL_HALO:
		if (This->offset_0x4C0[0xD]) {
			This->dropInvulTimeLeft = 2;
			break;
		}
		if (!(50 <= This->objectBase.opponent->objectBase.action && This->objectBase.opponent->objectBase.action < 150))
			break;
		This->offset_0x4C0[0xD] = true;
		if (SokuLib::displayedWeather != CUSTOMWEATHER_SHOOTING_STAR)
			SokuLib::weatherCounter /= 3;
		extraData[0] = 0;
		extraData[1] = 0;
		extraData[2] = 0;
		ObjectHandler_SpawnBullet(This, 1110, This->objectBase.position.x, 0, This->objectBase.direction, 0xffffffff, extraData, 3);
		FUN_00438ce0(This, 0x8B, This->objectBase.position.x, This->objectBase.position.y, 1, 1);
		break;
	case CUSTOMWEATHER_MISSING_PURPLE_MIST:
		if (characterSizeCtr[index] >= MISSING_PURPLE_MIST_SMOOTHING_TIME)
			characterSizeCtr[index] = MISSING_PURPLE_MIST_SMOOTHING_TIME;
		else
			characterSizeCtr[index]++;
		This->noSuperArmor = 0;
		*(short *)&This->offset_0x4AA = 1;
		break;
	case CUSTOMWEATHER_WATER_HAZE:
		This->speedMultiplier.x /= 4;
		This->speedMultiplier.y /= 4;
		break;
	}
	/*if (
		(characterSizeCtr[i] && characterSizeCtr[i] != MISSING_PURPLE_MIST_SMOOTHING_TIME) ||
		(characterSizeCtr[i] == MISSING_PURPLE_MIST_SMOOTHING_TIME && characterUpdateCtr % 4 == 0)
	) {
		This->objectBase.hitstop++;
		This->objectBase.hitstop += This->objectBase.hitstop == 1;
		for (auto obj : This->objects.list.vector()) {
			obj->hitstop++;
			obj->hitstop += obj->hitstop == 1;
		}
	}*/
}

void modifyBoxes()
{
	SokuLib::ObjectManager *This;
	__asm MOV [This], ESI;

	auto &battleMgr = SokuLib::getBattleMgr();
	auto index = This->owner == &battleMgr.rightCharacterManager;
	auto scale = (1 + (float) characterSizeCtr[index] / MISSING_PURPLE_MIST_SMOOTHING_TIME);

	for (int i = 0; i < This->hurtBoxCount; i++) {
		auto &box = This->hurtBoxes[i];

		box.left  = (box.left   - This->position.x) * scale + This->position.x;
		box.right = (box.right  - This->position.x) * scale + This->position.x;
		box.top   = (box.top    + This->position.y) * scale - This->position.y;
		box.bottom= (box.bottom + This->position.y) * scale - This->position.y;
		if (This->hurtBoxesRotation[i]) {
			auto &box = *This->hurtBoxesRotation[i];

			box.pt1.x *= scale;
			box.pt2.x *= scale;
			box.pt1.y *= scale;
			box.pt2.y *= scale;
		}
	}
	for (int i = 0; i < This->hitBoxCount; i++) {
		auto &box = This->hitBoxes[i];

		box.left  = (box.left   - This->position.x) * scale + This->position.x;
		box.right = (box.right  - This->position.x) * scale + This->position.x;
		box.top   = (box.top    + This->position.y) * scale - This->position.y;
		box.bottom= (box.bottom + This->position.y) * scale - This->position.y;
		if (This->hitBoxesRotation[i]) {
			auto &box = *This->hitBoxesRotation[i];

			box.pt1.x *= scale;
			box.pt2.x *= scale;
			box.pt1.y *= scale;
			box.pt2.y *= scale;
		}
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
	0,    0,   // CHARACTER_RANDOM
	0,    0,   // CHARACTER_NAMAZU
	4.5, -4.5, // CHARACTER_MOMIJI
	4,   -4,   // CHARACTER_CLOWNPIECE
	6.5, -6.5, // CHARACTER_FLANDRE
	6.5, -6.5, // CHARACTER_ORIN
	3,   -3,   // CHARACTER_YUUKA
	4,   -4,   // CHARACTER_KAGUYA
	5.5, -5.5, // CHARACTER_MOKOU
	2.5, -3,   // CHARACTER_MIMA
	4.5, -4.5, // CHARACTER_SHOU
	5.5, -5.5, // CHARACTER_MURASA
	4,   -4,   // CHARACTER_SEKIBANKI
	4,   -4,   // CHARACTER_SATORI
	6,   -6    // CHARACTER_RAN
};

void aocfCheck(SokuLib::CharacterManager *chr, float newY, float oldY)
{
	if (chr->effectiveWeather != CUSTOMWEATHER_ANTINOMY_OF_COMMON_WEATHER)
		return;
	chr->airDashCount = 0;
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

void callFct(SokuLib::CharacterManager *This, int action, float x, float y, int dir, unsigned color, float *extraData, unsigned extraDataSize)
{
	auto fct = ((void (__thiscall **)(SokuLib::ObjListManager &, int, SokuLib::CharacterManager *, int, float, float, int, unsigned, float *, unsigned))*(int *)This->objects.offset_0x00)[1];

	__asm PUSH EBP
	fct(This->objects, 0, This, action, x, y, dir, color, extraData, extraDataSize);
	__asm POP EBP
}

void __fastcall ObjectHandler_SpawnBullet_hook(SokuLib::CharacterManager *This, int, int action, float x, float y, int dir, unsigned color, float *extraData, unsigned extraDataSize)
{
	auto &battleMgr = SokuLib::getBattleMgr();
	auto i = This == &battleMgr.rightCharacterManager;
	auto scale = (1 + (float) characterSizeCtr[i] / MISSING_PURPLE_MIST_SMOOTHING_TIME);

	printf("%i %f %f -> ", action, x, y);
	x = (x - This->objectBase.position.x) * scale + This->objectBase.position.x;
	y = (y - This->objectBase.position.y) * scale + This->objectBase.position.y;
	printf("%f %f\n", x, y);

	// Normal case, weather isn't activated
	if (This->effectiveWeather != CUSTOMWEATHER_RAGNAROK)
		return callFct(This, action, x, y, dir, color, extraData, extraDataSize);
	// These are weather effects and stuff, no need to double them
	if (action >= 1000)
		return callFct(This, action, x, y, dir, color, extraData, extraDataSize);
	// [1] is the velocity.
	// If it's set to 0 then the bullet probably doesn't support rotation,
	// so we just spawn them on top of each other
	if (extraDataSize < 3 || extraData[1] == 0) {
		callFct(This, action, x, y - 15 * scale, dir, color, extraData, extraDataSize);
		callFct(This, action, x, y + 15 * scale, dir, color, extraData, extraDataSize);
	} else {
		// -10°
		extraData[0] -= 10;
		callFct(This, action, x, y, dir, color, extraData, extraDataSize);
		// +10°
		extraData[0] += 20;
		callFct(This, action, x, y, dir, color, extraData, extraDataSize);
	}
}

BlockResult __fastcall checkHit(SokuLib::CharacterManager *This, int, SokuLib::AttackFlags attackFlags)
{
	if (This->effectiveWeather == CUSTOMWEATHER_REVERSE_FIELD) {
		auto l = attackFlags.lowHit;

		attackFlags.lowHit = attackFlags.midHit;
		attackFlags.midHit = l;
	}
	return og_checkHit(This, attackFlags);
}

auto removeCardsFromPlayer = (void (__thiscall *)(SokuLib::CharacterManager *This, char param_1_00, char forcedCost, int param_4))0x469C70;
auto onCardUsed = (void (__thiscall *)(SokuLib::CharacterManager *This))0x483D60;
auto FUN_0046d950 = (void (__thiscall *)(SokuLib::CharacterManager *This))0x46D950;
unsigned systemCardHookRet = 0x48AF6D;
void (*og_handDataOperatorBracket)();

bool checkExtraSystemCards()
{
	SokuLib::CharacterManager *This;
	__asm MOV [This], ESI

	unsigned short id = This->hand[0].id;
	auto action = This->objectBase.action;
	auto iVar8 = *(int *)&This->objectBase.offset_0x18C[4];

	if (id == 12 && (
		action < SokuLib::ACTION_5A ||
		(iVar8 != 0 && iVar8 != 3)
	)) {
		This->swordOfRaptureDebuffTimeLeft = COIN_EFFECT_DURATION;
		*(int *)&This->offset_0x7CD[3] = id;
		This->objectBase.renderInfos.zRotation = 0;
		FUN_0046d950(This);
		((SokuLib::v2::AnimationObject *)This)->setAction(SokuLib::ACTION_SYSTEM_CARD);
		removeCardsFromPlayer(This, '\0', '\0', 0x3C);
		onCardUsed(This);
		FUN_00438ce0(This, 0x47, This->objectBase.position.x, This->objectBase.position.y + 100.0, This->objectBase.direction, 1);
		return true;
	}
	return false;
}

void __declspec(naked) checkExtraSystemCards_hook()
{
	__asm {
		PUSH ECX
		PUSH EDX
		CALL checkExtraSystemCards
		POP EDX
		POP ECX
		TEST EAX, EAX
		JZ normalRet

		ADD ESP, 8
		JMP [systemCardHookRet]

	normalRet:
		JMP [og_handDataOperatorBracket]
	}
}

using SokuLib::weatherCounter;

void __declspec(naked) hotWindFixupLimit()
{
	__asm {
		// if (this->effectiveWeather == CUSTOMWEATHER_HOT_WIND)
		CMP [ESI + 0x52C], CUSTOMWEATHER_HOT_WIND
		JNZ noChange

		//MOV byte ptr [ESP + 0x17], 0x1

		// if (counterHit)
		CMP byte ptr [ESP + 0x17], 0x1
		JNZ lowerWeather

		// defender->realLimit = 100;
		MOV word ptr [ESI + 0x4BE], 100

		// attacker->owner2.combo.limit = 100;
		MOV EAX, dword ptr [EDI + 0x16c]
		MOV word ptr [EAX + 0x4B8], 100

		MOV EAX, [weatherCounter]
		SUB word ptr [EAX], 500
		JMP noChange

	lowerWeather:
		MOV EAX, [weatherCounter]
		SUB word ptr [EAX], 100


	noChange:
		CMP word ptr [ESI + 0x184], 0x0
		RET
	}
}


void __declspec(naked) hotWindDamageHook()
{
	__asm {
		MOV ECX, [ESI + 0x16C]
		MOV ECX, [ECX + 0x52C]
		CMP ECX, CUSTOMWEATHER_HOT_WIND
		JNZ normal

		CMP byte ptr [ESP + 0x23], 0x1
		JNZ notCH

		MOV ECX, HOT_WIND_FORCED_DAMAGE_CH
		RET

	notCH:
		MOV ECX, HOT_WIND_FORCED_DAMAGE
		RET

	normal:
		MOV EAX, dword ptr [ESI + 0x1C0]
		MOVSX ECX, word ptr [EAX + 0x1C]
		RET
	}
}

void __declspec(naked) hotWindHitStopHook()
{
	__asm {
		// Attacker
		MOV EDX, [EDI + 0x16C]
		// Defender
		MOV ECX, [ESI + 0x16C]
		CMP [EDX + 0x52C], CUSTOMWEATHER_HOT_WIND
		JNZ normal

		CMP byte ptr [ESP + 0x17], 0x1
		JNZ normal

		CMP [ECX + 0x4A8], 30
		JZ modify

		CMP word ptr [EDX + 0x4B4], 0x0
		JNZ normal

		MOV DX, 30
		MOV [ECX + 0x4A8], DX

	modify:
		MOV CX, [EBP + 0x2A]
		MOV DX, [EBP + 0x2C]
		ADD DX, 30
		JMP apply

	normal:
		MOV CX, [EBP + 0x2A]
		MOV DX, [EBP + 0x2C]

	apply:
		MOV [EDI + 0x196], CX
		MOV [ESI + 0x196], DX
		CMP byte ptr [ESP + 0x17], 00
		RET
	}
}

struct GiurollCallbacks {
	unsigned (*saveState)();
	void (*loadStatePre)(size_t frame, unsigned);
	void (*loadStatePost)(unsigned);
	void (*freeState)(unsigned);
};

struct SavedRenderData {
	SokuLib::Vector2f pos;
	unsigned char alpha;
};

void __fastcall renderObjectLayer(GameDataManager *This, int, int layer)
{
	std::map<void *, SavedRenderData> data[2];

	ogRenderObjectLayer(This, layer);

	for (int i = 0; i < 2; i++) {
		for (auto &obj : This->players[i]->objects.list.vector()) {
			data[i][obj] = { obj->position, obj->renderInfos.color.a };
			if (This->players[i]->effectiveWeather != CUSTOMWEATHER_MYSTERIOUS_WIND) {
				obj->renderInfos.color.a = 0;
				continue;
			}
			if (obj->position.x < 640)
				obj->position.x += WRAP_SIZE;
			else
				obj->position.x -= WRAP_SIZE;
		}
	}
	ogRenderObjectLayer(This, layer);
	for (int i = 0; i < 2; i++) {
		for (auto &obj : This->players[i]->objects.list.vector()) {
			obj->position = data[i][obj].pos;
			obj->renderInfos.color.a = data[i][obj].alpha;
		}
	}
}

void __fastcall renderCharacters(GameDataManager *This)
{
	SavedRenderData lData{ This->players[0]->objectBase.position, This->players[0]->objectBase.renderInfos.color.a };
	SavedRenderData rData{ This->players[1]->objectBase.position, This->players[1]->objectBase.renderInfos.color.a };

	ogRenderCharacters(This);

	for (int i = 0; i < 2; i++) {
		if (This->players[i]->effectiveWeather != CUSTOMWEATHER_MYSTERIOUS_WIND) {
			This->players[i]->objectBase.renderInfos.color.a = 0;
			continue;
		}
		if (This->players[i]->objectBase.position.x < 640)
			This->players[i]->objectBase.position.x += WRAP_SIZE;
		else
			This->players[i]->objectBase.position.x -= WRAP_SIZE;
	}
	ogRenderCharacters(This);

	This->players[0]->objectBase.position = lData.pos;
	This->players[0]->objectBase.renderInfos.color.a = lData.alpha;
	This->players[1]->objectBase.position = rData.pos;
	This->players[1]->objectBase.renderInfos.color.a = rData.alpha;
}

void __fastcall CBattleManager_HandleCollision(SokuLib::BattleManager *This, int, SokuLib::ObjectManager *object, SokuLib::CharacterManager* character)
{
	ogBattleMgrHandleCollision(This, object, character);
	if (character->effectiveWeather == CUSTOMWEATHER_MYSTERIOUS_WIND) {
		auto p = character->objectBase.position;
		SokuLib::Box hu[5];

		memcpy(hu, character->objectBase.hurtBoxes, sizeof(object->hurtBoxes));
		if (character->objectBase.position.x < 640) {
			character->objectBase.position.x += WRAP_SIZE;
			for (auto &h : character->objectBase.hurtBoxes) {
				h.left += WRAP_SIZE;
				h.right += WRAP_SIZE;
			}
		} else {
			character->objectBase.position.x -= WRAP_SIZE;
			for (auto &h : character->objectBase.hurtBoxes) {
				h.left -= WRAP_SIZE;
				h.right -= WRAP_SIZE;
			}
		}
		ogBattleMgrHandleCollision(This, object, character);
		character->objectBase.position = p;
		memcpy(character->objectBase.hurtBoxes, hu, sizeof(object->hurtBoxes));
	} else if (object->owner->effectiveWeather == CUSTOMWEATHER_MYSTERIOUS_WIND) {
		auto p = object->position;
		SokuLib::Box hi[5];

		memcpy(hi, object->hitBoxes, sizeof(object->hitBoxes));
		if (object->position.x < 640) {
			object->position.x += WRAP_SIZE;
			for (auto &h : object->hitBoxes) {
				h.left += WRAP_SIZE;
				h.right += WRAP_SIZE;
			}
		} else {
			object->position.x -= WRAP_SIZE;
			for (auto &h : object->hitBoxes) {
				h.left -= WRAP_SIZE;
				h.right -= WRAP_SIZE;
			}
		}
		ogBattleMgrHandleCollision(This, object, character);
		object->position = p;
		memcpy(object->hitBoxes, hi, sizeof(object->hitBoxes));
	}
}

void __declspec(naked) forceLockOut()
{
	__asm {
		CMP dword ptr [ESI + 0x52C], CUSTOMWEATHER_MYSTERIOUS_WIND
		JZ ret_

		CMP byte ptr [ESI + 0x571], 0x0
	ret_:
		ret
	}
}

bool __fastcall turnAroundWrapped(SokuLib::ObjectManager *This)
{
	auto oldDir = This->direction;
	auto opX = This->opponent->objectBase.position.x;
	auto myX = This->position.x;

	if (opX > myX && opX - myX > WRAP_SIZE / 2)
		myX += WRAP_SIZE;
	else if (opX < myX && myX - opX > WRAP_SIZE / 2)
		opX += WRAP_SIZE;
	if (myX == opX)
		return false;
	if (myX > opX)
		This->direction = SokuLib::LEFT;
	else if (myX < opX)
		This->direction = SokuLib::RIGHT;
	return oldDir == This->direction;
}

unsigned turnAround_hook_retAddr = 0x469E86;

void __declspec(naked) turnAround_hook()
{
	__asm {
		CMP [ECX + 0x52C], CUSTOMWEATHER_MYSTERIOUS_WIND
		// This returns directly to the caller if took
		JZ turnAroundWrapped
		FLD dword ptr [ECX + 0xEC]
		JMP turnAround_hook_retAddr
	}
}

bool sameFctCall(unsigned addr1, unsigned addr2)
{
	return (*(unsigned *)(addr1 + 1) + addr1 + 5) == (*(unsigned *)(addr2 + 1) + addr2 + 5);
}

float frostTable[] {
	4.5 / 60.f, // CHARACTER_REIMU
	6   / 60.f, // CHARACTER_MARISA
	6   / 60.f, // CHARACTER_SAKUYA
	6   / 60.f, // CHARACTER_ALICE
	4   / 60.f, // CHARACTER_PATCHOULI
	4   / 60.f, // CHARACTER_YOUMU
	6.5 / 60.f, // CHARACTER_REMILIA
	4   / 60.f, // CHARACTER_YUYUKO
	4.5 / 60.f, // CHARACTER_YUKARI
	5.5 / 60.f, // CHARACTER_SUIKA
	4   / 60.f, // CHARACTER_REISEN
	5.5 / 60.f, // CHARACTER_AYA
	4.5 / 60.f, // CHARACTER_KOMACHI
	2.5 / 60.f, // CHARACTER_IKU
	4.5 / 60.f, // CHARACTER_TENSHI
	6   / 60.f, // CHARACTER_SANAE
	6.5 / 60.f, // CHARACTER_CIRNO
	7   / 60.f, // CHARACTER_MEILING
	3   / 60.f, // CHARACTER_UTSUHO
	6   / 60.f, // CHARACTER_SUWAKO
	0   / 60.f, // CHARACTER_RANDOM
	0   / 60.f, // CHARACTER_NAMAZU
	4.5 / 60.f, // CHARACTER_MOMIJI
	4   / 60.f, // CHARACTER_CLOWNPIECE
	6.5 / 60.f, // CHARACTER_FLANDRE
	6.5 / 60.f, // CHARACTER_ORIN
	3   / 60.f, // CHARACTER_YUUKA
	4   / 60.f, // CHARACTER_KAGUYA
	5.5 / 60.f, // CHARACTER_MOKOU
	2.5 / 60.f, // CHARACTER_MIMA
	4.5 / 60.f, // CHARACTER_SHOU
	5.5 / 60.f, // CHARACTER_MURASA
	4   / 60.f, // CHARACTER_SEKIBANKI
	4   / 60.f, // CHARACTER_SATORI
	6   / 60.f, // CHARACTER_RAN
};

void __declspec(naked) slideAdd()
{
	__asm {
		CMP [ESI + 0x52C], CUSTOMWEATHER_FROST
		JNZ normal

		MOVSX EAX, byte ptr [ESI + 0x34C]
		FADD dword ptr [frostTable + EAX * 4]
		RET

	normal:
		FADD dword ptr [zeroFive]
		RET
	}
}

void __declspec(naked) slideSub()
{
	__asm {
		CMP [ESI + 0x52C], CUSTOMWEATHER_FROST
		JNZ normal

		MOVSX EAX, byte ptr [ESI + 0x34C]
		FSUB dword ptr [frostTable + EAX * 4]
		RET

	normal:
		FSUB dword ptr [zeroFive]
		RET
	}
}

void loadWeatherIni(HMODULE hMyModule)
{
	wchar_t profilePath[1024];
	wchar_t buffer[128];
	char buffer2[128];

	GetModuleFileNameW(hMyModule, profilePath, 1024);
	PathRemoveFileSpecW(profilePath);
	PathAppendW(profilePath, L"CustomWeathers.ini");

	GetPrivateProfileStringW(L"Misc", L"ForcedWeather", L"None", buffer, sizeof(buffer) / sizeof(*buffer), profilePath);
	for (unsigned i = 0; i < sizeof(buffer2); i++)
		buffer2[i] = buffer[i];

	auto it = std::find_if(weatherNames.begin(), weatherNames.end(), [buffer2](std::pair<unsigned, std::string> p) {
		return p.second == buffer2;
	});

	printf("'%s'\n", buffer2);
	if (it == weatherNames.end())
		forcedWeather = SokuLib::WEATHER_CLEAR;
	else
		forcedWeather = it->first;
	clearMultiplier = GetPrivateProfileIntW(L"Misc", L"ClearTimerMultiplier", 1, profilePath);

	std::vector<unsigned> weathers;

	weathers.reserve(weatherNames.size());
	for (auto &weather : weatherNames) {
		for (int i = 0; i < weather.second.size(); i++)
			buffer[i] = weather.second[i];
		buffer[weather.second.size()] = 0;

		int val = GetPrivateProfileIntW(L"WeatherOrder", buffer, -1, profilePath);

		if (val < 0)
			continue;
		weathers.push_back((val << 12) | weather.first);
	}
	std::sort(weathers.begin(), weathers.end());
	if (weathers.empty())
		weathers.push_back(SokuLib::WEATHER_CLEAR);
	for (auto &w : weathers)
		w &= (1 << 12) - 1;
	for (size_t i = 0; i < weathers.size(); i++) {
		if (i + 1 == weathers.size()) {
			order[weathers[i]] = weathers[0];
			printf("%X -> %X\n", weathers[i], weathers[0]);
		} else {
			order[weathers[i]] = weathers[i + 1];
			printf("%X -> %X\n", weathers[i], weathers[i + 1]);
		}
	}
}

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
	loadWeatherIni(hMyModule);
	my_assert(sameFctCall(0x47A91D, 0x47A94F));
	my_assert(sameFctCall(0x47A91D, 0x47A972));
	my_assert(sameFctCall(0x47A91D, 0x47A97F));
	my_assert(sameFctCall(0x47A91D, 0x47A99B));
	VirtualProtect((PVOID)RDATA_SECTION_OFFSET, RDATA_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	ogBattleMgrDestructor = SokuLib::TamperDword(&SokuLib::VTable_BattleManager.destructor, CBattleManager_Destructor);
	ogLoadingWatchOnProcess = SokuLib::TamperDword(&SokuLib::VTable_LoadingWatch.onProcess, CLoadingWatch_OnProcess);
	ogSelectOnProcess = SokuLib::TamperDword(&SokuLib::VTable_Select.onProcess, CSelect_OnProcess);
	ogSelectClientOnProcess = SokuLib::TamperDword(&SokuLib::VTable_SelectClient.onProcess, CSelectCL_OnProcess);
	ogSelectServerOnProcess = SokuLib::TamperDword(&SokuLib::VTable_SelectServer.onProcess, CSelectSV_OnProcess);
	ogBattleMgrOnProcess = SokuLib::TamperDword(&SokuLib::VTable_BattleManager.onProcess, CBattleManager_OnProcess);
	ogBattleMgrOnRender = SokuLib::TamperDword(&SokuLib::VTable_BattleManager.onRender, CBattleManager_OnRender);
	ogBattleMgrOnMatchProcess = SokuLib::TamperDword(&SokuLib::VTable_BattleManager.maybeOnProgress, CBattleManager_OnMatchProcess);
	ogHudRender = (int (__thiscall *)(void *))SokuLib::TamperDword(0x85b544, onHudRender);
	VirtualProtect((PVOID)RDATA_SECTION_OFFSET, RDATA_SECTION_SIZE, old, &old);
	VirtualProtect((PVOID)TEXT_SECTION_OFFSET, TEXT_SECTION_SIZE, PAGE_EXECUTE_WRITECOPY, &old);
	ogBattleMgrHandleCollision = reinterpret_cast<void (__thiscall *)(SokuLib::BattleManager *, void *,SokuLib::CharacterManager *)>(SokuLib::TamperNearJmpOpr(0x47D618, CBattleManager_HandleCollision));
	SokuLib::TamperNearJmpOpr(0x47D64C, CBattleManager_HandleCollision);
	ogRenderObjectLayer = reinterpret_cast<void (__thiscall *)(GameDataManager *, int)>(SokuLib::TamperNearJmpOpr(0x47A91D, renderObjectLayer));
	SokuLib::TamperNearJmpOpr(0x47A94F, renderObjectLayer);
	SokuLib::TamperNearJmpOpr(0x47A972, renderObjectLayer);
	SokuLib::TamperNearJmpOpr(0x47A97F, renderObjectLayer);
	SokuLib::TamperNearJmpOpr(0x47A99B, renderObjectLayer);
	ogRenderCharacters = reinterpret_cast<void (__thiscall *)(GameDataManager *)>(SokuLib::TamperNearJmpOpr(0x47A95A, renderCharacters));
	og_checkHit = reinterpret_cast<BlockResult (__thiscall *)(SokuLib::CharacterManager *, SokuLib::AttackFlags)>(SokuLib::TamperNearJmpOpr(0x47C5A9, checkHit));
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
	new SokuLib::Trampoline(0x4889BE, weatherEffectReset, 6);
	new SokuLib::Trampoline(0x488ABF, weatherEffectSet, 6);
	SokuLib::TamperNearCall(0x463680, aocfCheck_hook);
	SokuLib::TamperNearJmp(0x46EB30/* ObjectHandler_SpawnBullet */, ObjectHandler_SpawnBullet_hook);
	*(char *)0x463685 = 0x90;
	new SokuLib::Trampoline(0x4664CA, modifyBoxes, 6);
	new SokuLib::Trampoline(0x4818C3, loadExtraCharacters, 6);
	SokuLib::TamperNearCall(0x463578, forceLockOut);
	*(char *)0x46357D = 0x90;
	*(char *)0x46357E = 0x90;

	// Frost sliding
	std::pair<unsigned, unsigned> slidingAddresses[] = {
		{ 0x490C6E, 0x490C28 }, // Reimu
		{ 0x4BDE67, 0x4BDE21 }, // Marisa
		{ 0x4E7B17, 0x4E7AD1 }, // Sakuya
		{ 0x510647, 0x510601 }, // Alice
		{ 0x541FB3, 0x541F6D }, // Patchouli
		{ 0x56CD8E, 0x56CD48 }, // Youmu
		{ 0x587C73, 0x587C2D }, // Remilia
		{ 0x5A5A8A, 0x5A5A45 }, // Yuyuko
		{ 0x5CAD3E, 0x5CACF9 }, // Yukari
		{ 0x5EE877, 0x5EE831 }, // Suika
		{ 0x61E0E4, 0x61E09E }, // Reisen
		{ 0x665474, 0x66542E }, // Aya
		{ 0x644BB5, 0x644B6F }, // Komachi
		{ 0x687CF9, 0x687CB4 }, // Iku
		{ 0x6AB7D3, 0x6AB78D }, // Tenshi
		{ 0x73CBA2, 0x73CB5C }, // Sanae
		{ 0x6E378C, 0x6E3746 }, // Cirno
		{ 0x71AD33, 0x71ACED }, // Meiling
		{ 0x79FC0B, 0x79FBC5 }, // Utsuho
		{ 0x766883, 0x76683D }  // Suwako
	};

	for (auto &addrs : slidingAddresses) {
		SokuLib::TamperNearCall(addrs.first,  slideAdd);
		*(char *)(addrs.first + 5) = 0x90;
		SokuLib::TamperNearCall(addrs.second, slideSub);
		*(char *)(addrs.second + 5) = 0x90;
	}

	SokuLib::TamperNearJmp(0x469E80, turnAround_hook);
	*(char *)0x469E85 = 0x90;

	SokuLib::TamperNearCall(0x47AC44, hotWindFixupLimit);
	*(char *)0x47AC49 = 0x90;
	*(char *)0x47AC4A = 0x90;
	*(char *)0x47AC4B = 0x90;
	memset((void *)0x47AE24, 0x90, 22);
	SokuLib::TamperNearCall(0x47AE24, hotWindHitStopHook);
	memset((void *)0x464A89, 0x90, 10);
	SokuLib::TamperNearCall(0x464A89, hotWindDamageHook);

	/*
	 * 0042A649  MOV        EAX,[activeWeather]
	 * 0042A64E  ADD        EAX,0x12
	 * 0042A651  CDQ
	 * 0042A652  MOV        ECX,0x13
	 * 0042A657  IDIV       ECX
	 * 0042A659  PUSH       0x1
	 * 0042A65B  MOV        ECX,ESI
	 * 0042A65D  PUSH       EDX
	 * 0042A65E  CALL       activateWeather
	 *
	 * Patched to
	 * 0042A649  MOV        EAX,[activeWeather]
	 * 0042A64F  PUSH       0x1
	 * 0042A64E  PUSH       EAX
	 * 0042A651  CALL       selectPreviousWeather
	 * 0042A656  ADD        ESP,0x8
	 * 0042A659  PUSH       0x1
	 * 0042A65B  MOV        ECX,ESI
	 * 0042A65D  PUSH       EAX
	 * 0042A65E  CALL       activateWeather
	 */
	*(char *)0x42A64E = 0x6A;
	*(char *)0x42A64F = 0x01;
	*(char *)0x42A650 = 0x50;
	SokuLib::TamperNearCall(0x42A651, selectPreviousWeather);
	*(char *)0x42A656 = 0x83;
	*(char *)0x42A657 = 0xC4;
	*(char *)0x42A658 = 0x08;
	*(char *)0x42A65D = 0x50;

	/*
	 * 0042A67B  MOV        EAX,[activeWeather]
	 * 0042A680  ADD        EAX,0x1
	 * 0042A683  CDQ
	 * 0042A684  MOV        ECX,0x13
	 * 0042A689  IDIV       ECX
	 * 0042A68B  PUSH       0x1
	 * 0042A68D  MOV        ECX,ESI
	 * 0042A68F  PUSH       EDX
	 * 0042A690  CALL       activateWeather
	 *
	 * Patched to
	 * 0042A67B  MOV        EAX,[activeWeather]
	 * 0042A681  PUSH       0x1
	 * 0042A680  PUSH       EAX
	 * 0042A683  CALL       selectNextWeather
	 * 0042A688  ADD        ESP,0x8
	 * 0042A68B  PUSH       0x1
	 * 0042A68D  MOV        ECX,ESI
	 * 0042A68F  PUSH       EAX
	 * 0042A690  CALL       activateWeather
	 */
	*(char *)0x42A680 = 0x6A;
	*(char *)0x42A681 = 0x01;
	*(char *)0x42A682 = 0x50;
	SokuLib::TamperNearCall(0x42A683, selectNextWeather);
	*(char *)0x42A688 = 0x83;
	*(char *)0x42A689 = 0xC4;
	*(char *)0x42A68A = 0x08;
	*(char *)0x42A68F = 0x50;

	og_handDataOperatorBracket = SokuLib::TamperNearJmpOpr(0x48AF7C, checkExtraSystemCards_hook);

	// Increase speed of timer in clear
	*(unsigned char *)0x48242B = clearMultiplier;

	*(char *)0x48ACF0 = 0x90;
	*(char *)0x48ACF1 = 0xE9;

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