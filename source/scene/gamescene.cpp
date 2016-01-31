#include "gamescene.h"
#include "../gameflow.h"
#include "../manager/guimanager.h"
#include "../manager/worldmanager.h"

SceneNode *gScene = nullptr;
PhysicsManager *gPhysics = nullptr;
SoundManager *gSoundManager =nullptr;
WorldManager *gWorldManager = nullptr;
GameScene *gGameScene = nullptr;
PathfinderManager *gPathfinderManager = nullptr;

GameScene::GameScene(SceneNode *parent, Camera *mainCamera, const String &sceneFile)
	: pPlayer1(nullptr)
	, pPlayer2(nullptr)
	, pPlayer3(nullptr)
	, pPlayer4(nullptr)
	, pCamera(mainCamera)
	, clCamera()
	, pParentScene(parent)
	, cScene()
	, musCur(nullptr)
	, pGameMap(nullptr)
	, pFog(nullptr)
	, iTileSize(40) // READ FROM MAP - USED FOR FOG PIXEL TO TILE CONVERSION
	, pTilesetOptimist(nullptr)
	, pTilesetPessimist(nullptr)
	, pTilesetRealist(nullptr)
	, sSceneFile(sceneFile)
	, fTimeToNextLevel(0.0f)
	, pGameOverImg(nullptr)
	, vCameraFrom(0.0f, 0.0f, 0.0f)
	, vCameraCurrent(0.0f, 0.0f, 0.0f)
	, vCameraTo(0.0f, 0.0f, 0.0f)
	, fElapsed(0.0f)
	, bPaused(false)
	, bInitialized(false)
	, bMoveCamera(true)
	, bChangeLevel(false)
{
	gScene = &cScene;
	gPhysics = &clPhysicsManager;
	gSoundManager = &clSoundManager;
	gPathfinderManager = &clPathfinderManager;
	gWorldManager = &clWorldManager;
	gGameScene = this;
	memset(&bRequiredKeys, 0x00, sizeof(bRequiredKeys));
}

GameScene::~GameScene()
{
	pTilesetOptimist->Release();
	pTilesetRealist->Release();
	pTilesetPessimist->Release();
	gScene = nullptr;
}

bool GameScene::Initialize()
{
	pParentScene->Add(&cScene);

	// Create the transitions
	cRunToPause.Initialize(&cRun, &cOnPause, &cPause);
	cPauseToRun.Initialize(&cPause, &cOnRun, &cRun);
	cPauseToMenu.Initialize(&cPause, &cOnMenu, &cMenu);
	cRunToGameOver.Initialize(&cRun, &cOnGameOver, &cGameOver);
	cGameOverToMenu.Initialize(&cGameOver, &cOnMenu, &cMenu);

	// Create the State Machine.
	cFlow.RegisterTransition(&cRunToPause);
	cFlow.RegisterTransition(&cPauseToRun);
	cFlow.RegisterTransition(&cPauseToMenu);
	cFlow.RegisterTransition(&cRunToGameOver);
	cFlow.RegisterTransition(&cGameOverToMenu);

	cFlow.Initialize(&cRun);

	auto cb = [&](Job *self)
	{
		auto job = static_cast<FileLoader *>(self);

		pTilesetOptimist = static_cast<Texture *>(pResourceManager->Get("textures/realist_ground_tileset.png", ITexture::GetTypeId()));
		pTilesetRealist = static_cast<Texture *>(pResourceManager->Get("textures/pessimist_ground_tileset.png", ITexture::GetTypeId()));
		pTilesetPessimist = static_cast<Texture *>(pResourceManager->Get("textures/optimist_ground_tileset.png", ITexture::GetTypeId()));

		if(job->GetState() == eJobState::Completed)
		{
			OnJobCompleted(job);
		}
		else if(job->GetState() == eJobState::Aborted)
		{
			OnJobAborted();
		}
		sdDelete(self);
	};

	String f{"scenes/"};
	pJobManager->Add(sdNew(FileLoader(f + sSceneFile, cb)));

	RocketEventManager::AddListener(this);
	pInput->AddKeyboardListener(this);

	// Get the initial value from game data
	gGui->SetPlayerName("Optimist");
	gGui->SetLevel(1);
	gGui->SetXP(100);
	gGui->SetAttackPower(15);
	gGui->SetGold(0);
	gGui->SetLife(22, 22);
	gGui->SetStamina(10, 10);

	return true;
}

void GameScene::UseKey(u32 key)
{
	if (!key)
		return;

	bRequiredKeys[key - 1] = true;

	if (bRequiredKeys[0] && bRequiredKeys[1] && bRequiredKeys[2])
		this->ChangeLevel();
}

void GameScene::FogReveal(const vec3 &pos, u32 radius)
{
	if (!pFog)
		return;

	auto px = pos.x - 20.f;
	auto py = pos.y - 20.f;
	auto sx = s32((px / iTileSize) + iTileSize / 2) - 4;
	auto sy = s32((py / iTileSize) + iTileSize / 2) - 4;
	auto r = s32(radius);
	for (auto y = -r; y <= r; y++)
	{
		for (auto x = -r; x <= r; x++)
		{
			pFog->SetTileAt(sx + x, sy + y, 5); // 5 == ultimo tile, transparente
		}
	}
}

bool GameScene::Update(f32 dt)
{
	if (!bInitialized)
		return true;

	cFlow.Update(dt);

	if (bPaused)
		return true;

	clPhysicsManager.RemoveBodies();
	clPhysicsManager.Update(dt);
	clWorldManager.Update(dt);

	if (bMoveCamera)
	{
		clCamera.LookAt(pPlayer1->GetPosition());
		bMoveCamera = false;
	}

	this->FogReveal(pPlayer2->GetPosition(), 2);
	this->FogReveal(pPlayer3->GetPosition(), 1);
	this->FogReveal(pPlayer4->GetPosition(), 3);

	if (bChangeLevel)
	{
		fTimeToNextLevel -= dt;
		if (fTimeToNextLevel <= 0)
		{
			gFlow->LoadSceneFile(strNextLevel);
		}
	}

	if (gGameData->sGamePlay.bIsGameOver == true)
	{
		pGameOverImg->SetVisible(true);
		pGameOverImg->SetPosition(pCamera->GetPosition() - vec3(-512.0f, -384.0f, 0.0f));

		pPlayer1->Mute();
		pPlayer1->GetSprite()->SetVisible(false);
		cFlow.OnEvent(&cOnGameOver, this);
		pGameMap->SetVisible(false);

		clWorldManager.Clear();

		gGameData->sGamePlay.bIsGameOver = false;
	}

	return true;
}

bool GameScene::Shutdown()
{
	if (musCur)
		musCur->Unload();

	clWorldManager.Clear();

	pParentScene->Remove(&cScene);
	cScene.Unload();
	pParentScene = nullptr;

	pInput->RemoveKeyboardListener(this);
	RocketEventManager::RemoveListener(this);

	return true;
}

bool GameScene::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();
	if (k == eKey::Escape)
	{
		if (bPaused)
			cFlow.OnEvent(&cOnRun, this);
		else
			cFlow.OnEvent(&cOnPause, this);
	}

	return true;
}

void GameScene::OnGuiEvent(Rocket::Core::Event &ev, const Rocket::Core::String &script)
{
	UNUSED(ev)
	if (script == "resume")
	{
		cFlow.OnEvent(&cOnRun, this);
	}
	if (script == "quit")
	{
		cFlow.OnEvent(&cOnMenu);
		gFlow->Menu();
	}
}

void GameScene::Pause()
{
	bPaused = true;
}

void GameScene::Resume()
{
	bPaused = false;
}

void GameScene::OnJobCompleted(FileLoader *job)
{
	Reader r(job->pFile);
	cScene.Load(r);
	Log("Scene Name: %s len %d", cScene.sName.c_str(), cScene.Size());

	// Validate the music to play

	if (gGameData->IsBgmEnabled() == true)
	{
		musThemeOptimist.Load("sounds/optimist_theme.ogg");
		musThemeOptimist.SetVolume(1.0f);
		musThemeRealist.Load("sounds/realist_theme.ogg");
		musThemeRealist.SetVolume(1.0f);
		musThemePessimist.Load("sounds/pessimist_theme.ogg");
		musThemePessimist.SetVolume(1.0f);
	}

	SceneNode *sounds = (SceneNode *)cScene.GetChildByName("Sounds");
	clSoundManager.Init(*sounds);

	SceneNode *sprites = (SceneNode *)cScene.GetChildByName("Sprites");
	pGameMap = (GameMap *)cScene.GetChildByName("Map");

	strNextLevel = pGameMap->GetProperty("NextLevel");

	MapLayerMetadata *game = pGameMap->GetLayerByName("Game")->AsMetadata();
	game->SetVisible(false);

	// Initialize the pathfinder with the background and collider layers
	clPathfinderManager.Init(pGameMap->GetLayerByName("Navmesh")->AsTiled());

	for (unsigned i = 0, len = game->Size(); i < len; ++i)
	{
		MetadataObject *placeHolder = static_cast<MetadataObject *>(game->GetChildAt(i));
		//const String &type = placeHolder->GetProperty("Type");
		//if (type == "Entity")
		{
			Entity* entity = clWorldManager.BuildEntity(*placeHolder, sprites);
			//Log("%s", entity->GetName().c_str());
			if (entity->GetClassName() == "Player1")
			{
				pPlayer1 = static_cast<Player1Entity*>(entity);
			}
			if (entity->GetClassName() == "Player2")
			{
				pPlayer2 = static_cast<Player2Entity*>(entity);
			}
			if (entity->GetClassName() == "Player3")
			{
				pPlayer3 = static_cast<Player3Entity*>(entity);
			}
			if (entity->GetClassName() == "Player4")
			{
				pPlayer4 = static_cast<Player4Entity*>(entity);
			}
		}
	}

	// If the player is not set, the player will be optimist
	if (pPlayer1 == nullptr)
	{
		pPlayer1 = pPlayer4;
		musCur = &musThemeOptimist;
		gGui->SelectHero("Optimist");
		gGui->SelectEnemy();
		pSoundSystem->PlayMusic(musCur);
	}

	this->LoadMapColliders();

	clCamera.SetCamera(pCamera);
	clCamera.LookAt(pPlayer1->GetSprite()->GetPosition());

	MapLayerTiled *bg = pGameMap->GetLayerByName("Background")->AsTiled();

	f32 hw = bg->GetWidth() * 0.5f;
	f32 hh = bg->GetHeight() * 0.5f;
	clCamera.SetArea(Rect4f(-hw, -hh, bg->GetWidth(), bg->GetHeight()));

	sprites->SetVisible(false);

	pGameOverImg = (Image *)cScene.GetChildByName("GameOverImage");
	pGameOverImg->SetVisible(false);

	bInitialized = true;
}

void GameScene::OnJobAborted()
{
}

void GameScene::LoadMapColliders()
{
	MapLayerMetadata *game = pGameMap->GetLayerByName("Colliders")->AsMetadata();
	game->SetVisible(false);
	for (unsigned i = 0, len = game->Size(); i < len; ++i)
	{
		MetadataObject *placeHolder = static_cast<MetadataObject *>( game->GetChildAt(i));

		clPhysicsManager.CreateStaticBody(placeHolder);
	}
}

void GameScene::ChangeLevel()
{
	bChangeLevel = true;
}

void GameScene::RemoveLife()
{
}
