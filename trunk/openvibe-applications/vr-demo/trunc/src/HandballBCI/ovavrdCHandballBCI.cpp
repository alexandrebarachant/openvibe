#include "ovavrdCHandballBCI.h"

#include <iostream>

template <typename T> const T _abs_(const T& t) { return t<0?-t:t; }
template <typename T> const T _min_(const T& t1, const T& t2) { return t1<t2?t1:t2; }
template <typename T> const T _max_(const T& t1, const T& t2) { return t1<t2?t2:t1; }
template <typename T> const T _crop_(const T& t1, const T& t, const T& t2) { return _min_(_max_(t1, t), t2); }

using namespace OpenViBEVRDemos;
using namespace Ogre;

static const float g_fViscosity=0.003;
static const float g_fGrowScoreingSpeed=0.1;
static const float g_fEpsilon=1E-5;

CHandballBCI::CHandballBCI() : COgreVRApplication()
{
	m_iGoalScore=0;
	m_iSideScore=0;
	m_iClassificationScore=0;
	m_iFeedbackLoopCount=0;
	m_iPhase=Phase_Passive;
	m_iLastPhase=Phase_Passive;
	m_dFeedback=0;
	m_dLastFeedback=0;

	m_fBallSpeed=0;
	m_fBallPosition=0;
	m_fBallOrientation=0;
	m_fGoalScoreScale=0;
	m_fSideScoreScale=0;
	m_fClassificationScoreScale=0;

	m_iMark=Mark_None;
	m_iLastMark=Mark_None;
	m_bShowMark=false;

	m_dMinAnalog=0;
	m_dMaxAnalog=0;
}

bool CHandballBCI::initialise()
{
	// taken from gymnasium.scene + transformations from demo.omk
	
	//----------- LIGHTS -------------//
	m_poSceneManager->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
	m_poSceneManager->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);

	Ogre::Light* l_poLight1 = m_poSceneManager->createLight("Light1");
	//SceneNode *l_poLight1Node = m_poSceneManager->getRootSceneNode()->createChildSceneNode( "Light1Node" );
	l_poLight1->setPosition(-2,6,2);
	l_poLight1->setSpecularColour(1,1,1);
	l_poLight1->setDiffuseColour(1,1,1);
	
	//----------- CAMERA -------------//
	m_poCamera->setNearClipDistance(0.1f);
	m_poCamera->setFarClipDistance(50000.0f);
	m_poCamera->setFOVy(Radian(Degree(100)));
	m_poCamera->setProjectionType(PT_PERSPECTIVE);

	m_poCamera->setPosition(5.5f,0.9f,0.f);
	m_poCamera->rotate(Vector3::UNIT_Y, Radian(Math::PI/2.f));
	//m_poCamera->setOrientation(Quaternion(1,0,1,0));
	
	//----------- GYMNASIUM -------------//
	SceneNode * l_poGymnasiumPivot = m_poSceneManager->getRootSceneNode()->createChildSceneNode( "GymnasiumPivot" );
	SceneNode * l_poGymnasium = l_poGymnasiumPivot->createChildSceneNode("Gymnasium");
	SceneNode * l_poGymnasiumNode = l_poGymnasium->createChildSceneNode("GymnasiumNode");
	
	// GYMNASIUM
	Entity * l_poGymnasiumNodeEntity = m_poSceneManager->createEntity( "GymnasiumNodeEntity", "gymnasiumNode.mesh" );
	l_poGymnasiumNodeEntity->setCastShadows(false);
	l_poGymnasiumNodeEntity->getSubEntity(0)->setMaterialName("gymnasium-surface06");
	l_poGymnasiumNodeEntity->getSubEntity(1)->setMaterialName("gymnasium-surface10");
	l_poGymnasiumNodeEntity->getSubEntity(2)->setMaterialName("gymnasium-surface09");
	l_poGymnasiumNode->attachObject(l_poGymnasiumNodeEntity);

	// GYM PUB
	Entity * l_poGymnasiumPublicityEntity = m_poSceneManager->createEntity( "GymnasiumPublicity", "gymnasium-publicity.mesh" );
	l_poGymnasiumPublicityEntity->setCastShadows(true);
	l_poGymnasiumPublicityEntity->getSubEntity(0)->setMaterialName("gymnasium-surface11");
	l_poGymnasiumPublicityEntity->getSubEntity(1)->setMaterialName("gymnasium-surface12");
	l_poGymnasiumNode->attachObject(l_poGymnasiumPublicityEntity);

	// WALL PUB
	Entity * l_poWallPublicity01Entity = m_poSceneManager->createEntity( "WallPublicity01", "wall-publicity-01.mesh" );
	l_poWallPublicity01Entity->setCastShadows(true);
	l_poWallPublicity01Entity->getSubEntity(0)->setMaterialName("gymnasium-surface14");
	l_poGymnasiumNode->attachObject(l_poWallPublicity01Entity);

	Entity * l_poWallPublicity02Entity = m_poSceneManager->createEntity( "WallPublicity02", "wall-publicity-02.mesh" );
	l_poWallPublicity02Entity->setCastShadows(true);
	l_poWallPublicity02Entity->getSubEntity(0)->setMaterialName("gymnasium-surface15");
	l_poGymnasiumNode->attachObject(l_poWallPublicity02Entity);

	Entity * l_poWallPublicity03Entity = m_poSceneManager->createEntity( "WallPublicity03", "wall-publicity-03.mesh" );
	l_poWallPublicity03Entity->setCastShadows(true);
	l_poWallPublicity03Entity->getSubEntity(0)->setMaterialName("gymnasium-surface02");
	l_poGymnasiumNode->attachObject(l_poWallPublicity03Entity);

	Entity * l_poWallPublicity04Entity = m_poSceneManager->createEntity( "WallPublicity04", "wall-publicity-04.mesh" );
	l_poWallPublicity04Entity->setCastShadows(true);
	l_poWallPublicity04Entity->getSubEntity(0)->setMaterialName("gymnasium-surface03");
	l_poGymnasiumNode->attachObject(l_poWallPublicity04Entity);

	Entity * l_poWallPublicity06Entity = m_poSceneManager->createEntity( "WallPublicity06", "wall-publicity-06.mesh" );
	l_poWallPublicity06Entity->setCastShadows(true);
	l_poWallPublicity06Entity->getSubEntity(0)->setMaterialName("gymnasium-surface04");
	l_poGymnasiumNode->attachObject(l_poWallPublicity06Entity);

	Entity * l_poWallPublicity07Entity = m_poSceneManager->createEntity( "WallPublicity07", "wall-publicity-07.mesh" );
	l_poWallPublicity07Entity->setCastShadows(true);
	l_poWallPublicity07Entity->getSubEntity(0)->setMaterialName("gymnasium-surface05");
	l_poGymnasiumNode->attachObject(l_poWallPublicity07Entity);

	Entity * l_poWallPublicity08Entity = m_poSceneManager->createEntity( "WallPublicity08", "wall-publicity-08.mesh" );
	l_poWallPublicity08Entity->setCastShadows(true);
	l_poWallPublicity08Entity->getSubEntity(0)->setMaterialName("gymnasium-surface14");
	l_poGymnasiumNode->attachObject(l_poWallPublicity08Entity);

	Entity * l_poWallPublicity09Entity = m_poSceneManager->createEntity( "WallPublicity09", "wall-publicity-09.mesh" );
	l_poWallPublicity09Entity->setCastShadows(true);
	l_poWallPublicity09Entity->getSubEntity(0)->setMaterialName("gymnasium-surface14");
	l_poGymnasiumNode->attachObject(l_poWallPublicity09Entity);

	Entity * l_poWallPublicity10Entity = m_poSceneManager->createEntity( "WallPublicity10", "wall-publicity-10.mesh" );
	l_poWallPublicity10Entity->setCastShadows(true);
	l_poWallPublicity10Entity->getSubEntity(0)->setMaterialName("gymnasium-surface14");
	l_poGymnasiumNode->attachObject(l_poWallPublicity10Entity);

	Entity * l_poWallPublicity11Entity = m_poSceneManager->createEntity( "WallPublicity11", "wall-publicity-11.mesh" );
	l_poWallPublicity11Entity->setCastShadows(true);
	l_poWallPublicity11Entity->getSubEntity(0)->setMaterialName("gymnasium-surface14");
	l_poGymnasiumNode->attachObject(l_poWallPublicity11Entity);

	Entity * l_poWallPublicity12Entity = m_poSceneManager->createEntity( "WallPublicity12", "wall-publicity-12.mesh" );
	l_poWallPublicity12Entity->setCastShadows(true);
	l_poWallPublicity12Entity->getSubEntity(0)->setMaterialName("gymnasium-surface14");
	l_poGymnasiumNode->attachObject(l_poWallPublicity12Entity);

	Entity * l_poWallPublicity13Entity = m_poSceneManager->createEntity( "WallPublicity13", "wall-publicity-13.mesh" );
	l_poWallPublicity13Entity->setCastShadows(true);
	l_poWallPublicity13Entity->getSubEntity(0)->setMaterialName("gymnasium-surface14");
	l_poGymnasiumNode->attachObject(l_poWallPublicity13Entity);

	//TABLE
	SceneNode * l_poTableNode = l_poGymnasiumNode->createChildSceneNode("Table");
	l_poTableNode->setPosition(-2.475,0.36,-21.5);
	l_poTableNode->setOrientation(0.707105,0.f,0.707108,0.f);
	Entity * l_poTableEntity = m_poSceneManager->createEntity( "Table", "table.mesh" );
	l_poTableEntity->setCastShadows(true);
	l_poTableEntity->getSubEntity(0)->setMaterialName("gymnasium-MaterialSponsor01");
	l_poTableEntity->getSubEntity(1)->setMaterialName("gymnasium-MaterialSponsorBunrakuM2S01");
	l_poTableEntity->getSubEntity(2)->setMaterialName("gymnasium-surface13");
	l_poTableNode->attachObject(l_poTableEntity);

	//PUB
	SceneNode * l_poPublicity01Node = l_poGymnasiumNode->createChildSceneNode("publicity-01-node");
	l_poPublicity01Node->setPosition(-22.5,0.01,-10);
	l_poPublicity01Node->setOrientation(-3.65178e-006,0.f,1.f,0.f);
	Entity * l_poPublicity01Entity = m_poSceneManager->createEntity( "publicity-01-entity", "publicity-01.mesh" );
	l_poPublicity01Entity->setCastShadows(true);
	l_poPublicity01Entity->getSubEntity(0)->setMaterialName("gymnasium-MaterialSponsorBunrakuM2S01");
	l_poPublicity01Entity->getSubEntity(1)->setMaterialName("gymnasium-MaterialSponsor01");
	l_poPublicity01Node->attachObject(l_poPublicity01Entity);

	SceneNode * l_poPublicity02Node = l_poGymnasiumNode->createChildSceneNode("publicity-02-node");
	l_poPublicity02Node->setPosition(-22.5,0.01,-10);
	l_poPublicity02Node->setOrientation(0.707105,0.707108,0.f,0.f);
	Entity * l_poPublicity02Entity = m_poSceneManager->createEntity( "publicity-02-entity", "publicity-02.mesh" );
	l_poPublicity02Entity->setCastShadows(true);
	l_poPublicity02Entity->getSubEntity(0)->setMaterialName("gymnasium-MaterialSponsor01");
	l_poPublicity02Entity->getSubEntity(1)->setMaterialName("gymnasium-MaterialSponsorBunrakuM2S01");
	l_poPublicity02Node->attachObject(l_poPublicity02Entity);

	SceneNode * l_poPublicity03Node = l_poGymnasiumNode->createChildSceneNode("publicity-03-node");
	l_poPublicity03Node->setPosition(13.75,0.36,1.5); 
	l_poPublicity03Node->setOrientation(0.707105,0.f,-0.707108,0.f);
	Entity * l_poPublicity03Entity = m_poSceneManager->createEntity( "publicity-03-entity", "publicity-03.mesh" );
	l_poPublicity03Entity->setCastShadows(true);
	l_poPublicity03Entity->getSubEntity(0)->setMaterialName("gymnasium-MaterialSponsor01");
	l_poPublicity03Entity->getSubEntity(1)->setMaterialName("gymnasium-MaterialSponsorIrisa01");
	l_poPublicity03Node->attachObject(l_poPublicity03Entity);

	SceneNode * l_poPublicity04Node = l_poGymnasiumNode->createChildSceneNode("publicity-04-node");
	l_poPublicity04Node->setPosition(13.75,0.36,1.5);
	l_poPublicity04Node->setOrientation(0.707105,0.f,-0.707108,0.f);
	Entity * l_poPublicity04Entity = m_poSceneManager->createEntity( "publicity-04-entity", "publicity-04.mesh" );
	l_poPublicity04Entity->setCastShadows(true);
	l_poPublicity04Entity->getSubEntity(0)->setMaterialName("gymnasium-MaterialSponsor01");
	l_poPublicity04Entity->getSubEntity(1)->setMaterialName("gymnasium-MaterialSponsorIrisa01");
	l_poPublicity04Node->attachObject(l_poPublicity04Entity);

	SceneNode * l_poPublicity05Node = l_poGymnasiumNode->createChildSceneNode("publicity-05-node");
	l_poPublicity05Node->setPosition(+13.75,0.36,-21.5);
	l_poPublicity05Node->setOrientation(0.707105,0.f,0.707108,0.f);
	Entity * l_poPublicity05Entity = m_poSceneManager->createEntity( "publicity-05-entity", "publicity-05.mesh" );
	l_poPublicity05Entity->setCastShadows(true);
	l_poPublicity05Entity->getSubEntity(0)->setMaterialName("gymnasium-MaterialSponsor01");
	l_poPublicity05Entity->getSubEntity(1)->setMaterialName("gymnasium-MaterialSponsorIrisa01");
	l_poPublicity05Node->attachObject(l_poPublicity05Entity);

	SceneNode * l_poPublicity06Node = l_poGymnasiumNode->createChildSceneNode("publicity-06-node");
	l_poPublicity06Node->setPosition(-13.75,0.01,-21);
	l_poPublicity06Node->setOrientation(0.707105,0.f,0.707108,0.f);
	Entity * l_poPublicity06Entity = m_poSceneManager->createEntity( "publicity-06-entity", "publicity-06.mesh" );
	l_poPublicity06Entity->setCastShadows(true);
	l_poPublicity06Entity->getSubEntity(0)->setMaterialName("gymnasium-MaterialSponsor01");
	l_poPublicity06Entity->getSubEntity(1)->setMaterialName("gymnasium-MaterialSponsorIrisa01");
	l_poPublicity06Node->attachObject(l_poPublicity06Entity);

	SceneNode * l_poPublicity07Node = l_poGymnasiumNode->createChildSceneNode("publicity-07-node");
	l_poPublicity07Node->setPosition(-1.96695e-006,0.71,1.5);
	l_poPublicity07Node->setOrientation(0.5,0.5,-0.5,0.5);
	Entity * l_poPublicity07Entity = m_poSceneManager->createEntity( "publicity-07-entity", "publicity-07.mesh" );
	l_poPublicity07Entity->setCastShadows(true);
	l_poPublicity07Entity->getSubEntity(0)->setMaterialName("gymnasium-MaterialSponsorIrisa01");
	l_poPublicity07Entity->getSubEntity(1)->setMaterialName("gymnasium-MaterialSponsor01");
	l_poPublicity07Node->attachObject(l_poPublicity07Entity);

	//DOORS
	SceneNode * l_poDoor01Node = l_poGymnasiumNode->createChildSceneNode("door-01-node");
	l_poDoor01Node->setPosition(26,1,-24); 
	l_poDoor01Node->setOrientation(0.923879,0,-0.382684,0);
	Entity * l_poDoor01Entity = m_poSceneManager->createEntity( "door-01-entity", "door-01.mesh" );
	l_poDoor01Entity->setCastShadows(true);
	l_poDoor01Entity->getSubEntity(0)->setMaterialName("gymnasium-surface07");
	l_poDoor01Entity->getSubEntity(1)->setMaterialName("gymnasium-surface08");
	l_poDoor01Node->attachObject(l_poDoor01Entity);

	SceneNode * l_poDoor02Node = l_poGymnasiumNode->createChildSceneNode("door-02-node");
	l_poDoor02Node->setPosition(26,1,4);
	l_poDoor02Node->setOrientation(0.923879,0,0.382684,0);
	Entity * l_poDoor02Entity = m_poSceneManager->createEntity( "door-02-entity", "door-02.mesh" );
	l_poDoor02Entity->setCastShadows(true);
	l_poDoor02Entity->getSubEntity(0)->setMaterialName("gymnasium-surface07");
	l_poDoor02Entity->getSubEntity(1)->setMaterialName("gymnasium-surface08");
	l_poDoor02Node->attachObject(l_poDoor02Entity);

	SceneNode * l_poDoor03Node = l_poGymnasiumNode->createChildSceneNode("door-03-node");
	l_poDoor03Node->setPosition(-26,2.5,4);
	l_poDoor03Node->setOrientation(0.923879,0,-0.382684,0);
	Entity * l_poDoor03Entity = m_poSceneManager->createEntity( "door-03-entity", "door-03.mesh" );
	l_poDoor03Entity->setCastShadows(true);
	l_poDoor03Entity->getSubEntity(0)->setMaterialName("gymnasium-surface08");
	l_poDoor03Entity->getSubEntity(1)->setMaterialName("gymnasium-surface07");
	l_poDoor03Node->attachObject(l_poDoor03Entity);

	SceneNode * l_poDoor04Node = l_poGymnasiumNode->createChildSceneNode("door-04-node");
	l_poDoor04Node->setPosition(-26,1,-24);
	l_poDoor04Node->setOrientation(0.923879,0,0.382684,0);
	Entity * l_poDoor04Entity = m_poSceneManager->createEntity( "door-04-entity", "door-04.mesh" );
	l_poDoor04Entity->setCastShadows(true);
	l_poDoor04Entity->getSubEntity(0)->setMaterialName("gymnasium-surface07");
	l_poDoor04Entity->getSubEntity(1)->setMaterialName("gymnasium-surface08");
	l_poDoor04Node->attachObject(l_poDoor04Entity);

	//TRANSFORM FROM demo.omk
	l_poGymnasiumPivot->translate(-10,0,0);
	l_poGymnasiumPivot->rotate(Vector3::UNIT_Y, Radian(-1.570796327));
	
	//----------- LEFT GOAL -------------//
	SceneNode * l_poLeftGoalPivot = m_poSceneManager->getRootSceneNode()->createChildSceneNode( "LeftGoalPivot" );
	SceneNode * l_poLeftGoal = l_poLeftGoalPivot->createChildSceneNode("LeftGoal");
	SceneNode * l_poLeftGoalNode = l_poLeftGoal->createChildSceneNode("LeftGoalNode");

	Entity * l_poLeftGoalEntity = m_poSceneManager->createEntity( "LgoalNode", "goalNode.mesh" );
	l_poLeftGoalEntity->setCastShadows(true);
	l_poLeftGoalEntity->getSubEntity(0)->setMaterialName("goal-surface02");
	l_poLeftGoalEntity->getSubEntity(1)->setMaterialName("goal-surface01");
	l_poLeftGoalNode->attachObject(l_poLeftGoalEntity);

	Entity * l_poLeftGoalBorderEntity = m_poSceneManager->createEntity( "Lgoal-border", "goal-border.mesh" );
	l_poLeftGoalBorderEntity->setCastShadows(true);
	l_poLeftGoalBorderEntity->getSubEntity(0)->setMaterialName("goal-frontcol01");
	l_poLeftGoalNode->attachObject(l_poLeftGoalBorderEntity);

	Entity * l_poLeftGoalNetEntity = m_poSceneManager->createEntity( "Lgoal-net", "goal-net.mesh" );
	l_poLeftGoalNetEntity->setCastShadows(true);
	l_poLeftGoalNetEntity->getSubEntity(0)->setMaterialName("goal-foregrou01");
	l_poLeftGoalNode->attachObject(l_poLeftGoalNetEntity);
	
	//TRANSFORM FROM demo.omk
	l_poLeftGoalPivot->translate(0,0,-5.92);
	l_poLeftGoalPivot->rotate(Vector3::UNIT_Y, Radian(-1.570796327));
	
	//----------- RIGHT GOAL -------------//
	SceneNode * l_poRightGoalPivot = m_poSceneManager->getRootSceneNode()->createChildSceneNode( "RightGoalPivot" );
	SceneNode * l_poRightGoal = l_poRightGoalPivot->createChildSceneNode("RightGoal");
	SceneNode * l_poRightGoalNode = l_poRightGoal->createChildSceneNode("RightGoalNode");

	Entity * l_poRightGoalEntity = m_poSceneManager->createEntity( "RgoalNode", "goalNode.mesh" );
	l_poRightGoalEntity->setCastShadows(true);
	l_poRightGoalEntity->getSubEntity(0)->setMaterialName("goal-surface02");
	l_poRightGoalEntity->getSubEntity(1)->setMaterialName("goal-surface01");
	l_poRightGoalNode->attachObject(l_poRightGoalEntity);

	Entity * l_poRightGoalBorderEntity = m_poSceneManager->createEntity( "Rgoal-border", "goal-border.mesh" );
	l_poRightGoalBorderEntity->setCastShadows(true);
	l_poRightGoalBorderEntity->getSubEntity(0)->setMaterialName("goal-frontcol01");
	l_poRightGoalNode->attachObject(l_poRightGoalBorderEntity);

	Entity * l_poRightGoalNetEntity = m_poSceneManager->createEntity( "Rgoal-net", "goal-net.mesh" );
	l_poRightGoalNetEntity->setCastShadows(true);
	l_poRightGoalNetEntity->getSubEntity(0)->setMaterialName("goal-foregrou01");
	l_poRightGoalNode->attachObject(l_poRightGoalNetEntity);
	
	//TRANSFORM FROM demo.omk
	l_poRightGoalPivot->translate(0,0,5.92);
	l_poRightGoalPivot->rotate(Vector3::UNIT_Y, Radian(1.570796327));

	//----------- ACTIVE BALL -------------//
	SceneNode * l_poActiveBallPivot = m_poSceneManager->getRootSceneNode()->createChildSceneNode( "ActiveBallPivot" );
	SceneNode * l_poActiveBall = l_poActiveBallPivot->createChildSceneNode("ActiveBall");
	SceneNode * l_poActiveBallNode = l_poActiveBall->createChildSceneNode("ActiveBallNode");

	Entity * l_poActiveBallEntity = m_poSceneManager->createEntity( "active-ball", "active-ballNode.mesh" );
	l_poActiveBallEntity->setCastShadows(true);
	l_poActiveBallEntity->getSubEntity(0)->setMaterialName("active-ball-active-ball-material");
	l_poActiveBallNode->attachObject(l_poActiveBallEntity);

	//TRANSFORM FROM demo.omk
	l_poActiveBallPivot->translate(0,0.25,0);

	//----------- GetReady BALL -------------//
	SceneNode * l_poGetReadyBallPivot = m_poSceneManager->getRootSceneNode()->createChildSceneNode( "GetReadyBallPivot" );
	SceneNode * l_poGetReadyBall = l_poGetReadyBallPivot->createChildSceneNode("GetReadyBall");
	SceneNode * l_poGetReadyBallNode = l_poGetReadyBall->createChildSceneNode("GetReadyBallNode");

	Entity * l_poGetReadyBallEntity = m_poSceneManager->createEntity( "get-ready-ball", "get-ready-ballNode.mesh" );
	l_poGetReadyBallEntity->setCastShadows(true);
	l_poGetReadyBallEntity->getSubEntity(0)->setMaterialName("get-ready-ball-get-ready-ball-material");
	l_poGetReadyBallNode->attachObject(l_poGetReadyBallEntity);

	//TRANSFORM FROM demo.omk
	l_poGetReadyBallPivot->translate(0,0.25,0);

	//----------- Passive BALL -------------//
	SceneNode * l_poPassiveBallPivot = m_poSceneManager->getRootSceneNode()->createChildSceneNode( "PassiveBallPivot" );
	SceneNode * l_poPassiveBall = l_poPassiveBallPivot->createChildSceneNode("PassiveBall");
	SceneNode * l_poPassiveBallNode = l_poPassiveBall->createChildSceneNode("PassiveBallNode");

	Entity * l_poPassiveBallEntity = m_poSceneManager->createEntity( "passive-ball", "passive-ballNode.mesh" );
	l_poPassiveBallEntity->setCastShadows(true);
	l_poPassiveBallEntity->getSubEntity(0)->setMaterialName("passive-ball-surface01");
	l_poPassiveBallNode->attachObject(l_poPassiveBallEntity);

	//TRANSFORM FROM demo.omk
	l_poPassiveBallPivot->translate(0,0.25,0);

	//----------- CROSS -------------//
	SceneNode * l_poCrossPivot = m_poSceneManager->getRootSceneNode()->createChildSceneNode( "CrossPivot" );
	SceneNode * l_poCross = l_poCrossPivot->createChildSceneNode("Cross");
	SceneNode * l_poCrossNode = l_poCross->createChildSceneNode("CrossNode");

	Entity * l_poCrossEntity = m_poSceneManager->createEntity( "cross-node", "crossNode.mesh" );
	l_poCrossEntity->setCastShadows(true);
	l_poCrossEntity->getSubEntity(0)->setMaterialName("cross-cross-material");
	l_poCrossNode->attachObject(l_poCrossEntity);

	//TRANSFORM FROM demo.omk
	l_poCrossPivot->translate(-2.81,0.9,0);
	l_poCrossPivot->rotate(Vector3::UNIT_Y, Radian(-1.570796327));
	l_poCrossPivot->scale(0.3,0.3,0.3);

	//----------- BASIC ARROW -------------//
	SceneNode * l_poBasicArrowPivot = m_poSceneManager->getRootSceneNode()->createChildSceneNode( "BasicArrowPivot" );
	SceneNode * l_poBasicArrow = l_poBasicArrowPivot->createChildSceneNode("BasicArrow");
	SceneNode * l_poBasicArrowNode = l_poBasicArrow->createChildSceneNode("BasicArrowNode");

	Entity * l_poBasicArrowEntity = m_poSceneManager->createEntity( "basic-arrowNode", "basic-arrowNode.mesh" );
	l_poBasicArrowEntity->setCastShadows(true);
	l_poBasicArrowEntity->getSubEntity(0)->setMaterialName("basic-arrow-basic-arrow-material");
	l_poBasicArrowNode->attachObject(l_poBasicArrowEntity);

	//TRANSFORM FROM demo.omk
	l_poBasicArrowPivot->translate(-2.82,0.9,0);
	l_poBasicArrowPivot->rotate(Vector3::UNIT_Y, Radian(-1.570796327));
	l_poBasicArrowPivot->scale(0.3,0.3,0.3);

	return true;
}


bool CHandballBCI::process(double timeSinceLastProcess)
{

	//------------ VRPN --------------//
	while(!m_poVrpnPeripheral->m_vButton.empty())
	{
		std::pair < int, int >& l_rVrpnButtonState=m_poVrpnPeripheral->m_vButton.front();

		if(l_rVrpnButtonState.second)
		{
			switch(l_rVrpnButtonState.first)
			{
				case 0:
					m_iPhase=Phase_Passive;
					break;

				case 1:
					m_iPhase=Phase_GetReady;
					break;

				case 2:
					m_iPhase=Phase_Active;
					break;

				case 3:
					// m_iMark=Mark_None;
					m_bShowMark=false;
					break;

				case 4:
					m_iMark=Mark_Left;
					m_bShowMark=true;
					break;

				case 5:
					m_iMark=Mark_Right;
					m_bShowMark=true;
					break;

				default:
					break;
			}
		}

		m_poVrpnPeripheral->m_vButton.pop_front();
	}

	if(!m_poVrpnPeripheral->m_vAnalog.empty())
	{
		std::list < double >& l_rVrpnAnalogState=m_poVrpnPeripheral->m_vAnalog.front();

		double l_dAnalog=*(l_rVrpnAnalogState.begin());
		if(l_dAnalog>m_dMaxAnalog) m_dMaxAnalog=l_dAnalog;
		if(l_dAnalog<m_dMinAnalog) m_dMinAnalog=l_dAnalog;
		double l_dAbsoluteMinMax=_max_(m_dMaxAnalog, -m_dMinAnalog);

		m_vAnalogHistory.push_back(l_dAnalog);
		if(m_vAnalogHistory.size()>5)
		{
			m_vAnalogHistory.pop_front();
		}

		double l_dAnalogMean=0;
		std::list < double >::const_iterator i;
		for(i=m_vAnalogHistory.begin(); i!=m_vAnalogHistory.end(); i++)
		{
			l_dAnalogMean+=*i;
		}
		l_dAnalogMean/=m_vAnalogHistory.size();
		l_dAnalogMean/=(l_dAbsoluteMinMax?l_dAbsoluteMinMax:1);
		m_dFeedback=l_dAnalogMean;
		m_poVrpnPeripheral->m_vAnalog.pop_front();
	}

	//------------  --------------//

	// Checks phase change
	if(m_iPhase!=m_iLastPhase)
	{
		switch(m_iPhase)
		{
			case Phase_Passive:
			case Phase_GetReady:
				if(m_iLastMark==Mark_Left && m_fBallPosition<0) { m_iSideScore++; m_bScoreChanged=true; }
				if(m_iLastMark==Mark_Right&& m_fBallPosition>0) { m_iSideScore++; m_bScoreChanged=true; }
				m_fBallPosition=0;
				m_fBallSpeed=0;

				std::cout << "### ScOrE StAtUs = GoAl:" << m_iGoalScore << " SiDe:" << m_iSideScore << " ClAsSiFiCaTiOn:" << m_iClassificationScore << " LoOpCoUnT: " << m_iFeedbackLoopCount << "\n";
				break;

			case Phase_Active:
				m_bGoalMarkedAtThisPhase=false;
				break;
		}
	}

	// Checks phase
	switch(m_iPhase)
	{
		case Phase_Passive:
			break;

		case Phase_GetReady:
			break;

		case Phase_Active:
			m_fBallSpeed=m_dFeedback * 0.1;
			if(m_iMark==Mark_Left && m_fBallPosition==-6 && !m_bGoalMarkedAtThisPhase) { m_iGoalScore++; m_bScoreChanged=true; m_bGoalMarkedAtThisPhase=true; }
			if(m_iMark==Mark_Right&& m_fBallPosition== 6 && !m_bGoalMarkedAtThisPhase) { m_iGoalScore++; m_bScoreChanged=true; m_bGoalMarkedAtThisPhase=true; }
			if(m_iMark==Mark_Left && m_dFeedback<0) { m_iClassificationScore++; /* m_bScoreChanged=true; */ }
			if(m_iMark==Mark_Right&& m_dFeedback>0) { m_iClassificationScore++; /* m_bScoreChanged=true; */ }
			m_iFeedbackLoopCount++;
			break;
	}

	m_fBallOrientation+=m_fBallSpeed;
	m_fBallPosition+=m_fBallSpeed;
	m_fBallPosition=_crop_(-6.0f, m_fBallPosition, 6.0f);
	m_fBallSpeed*=(1.-g_fViscosity);

	m_fGoalScoreScale           = (1-g_fGrowScoreingSpeed)*m_fGoalScoreScale +           g_fGrowScoreingSpeed*m_iGoalScore;
	m_fSideScoreScale           = (1-g_fGrowScoreingSpeed)*m_fSideScoreScale +           g_fGrowScoreingSpeed*m_iSideScore;
	m_fClassificationScoreScale = (1-g_fGrowScoreingSpeed)*m_fClassificationScoreScale + g_fGrowScoreingSpeed*m_iClassificationScore/500.0f;

	double m_fFeedbackScale = m_dFeedback * 2.5;
	if(m_fFeedbackScale <= 0 && m_fFeedbackScale > -g_fEpsilon) m_fFeedbackScale=-g_fEpsilon;
	if(m_fFeedbackScale >= 0 && m_fFeedbackScale <  g_fEpsilon) m_fFeedbackScale= g_fEpsilon;

	// -------------------------------------------------------------------------------
	// Object displacement stuffs

	SceneNode * l_poActiveBallPivot = m_poSceneManager->getSceneNode("ActiveBallPivot");
	SceneNode * l_poPassiveBallPivot = m_poSceneManager->getSceneNode("PassiveBallPivot");
	SceneNode * l_poGetReadyBallPivot = m_poSceneManager->getSceneNode("GetReadyBallPivot");
	SceneNode * l_poCrossPivot = m_poSceneManager->getSceneNode("CrossPivot");
	SceneNode * l_poBasicArrowPivot = m_poSceneManager->getSceneNode("BasicArrowPivot");

	//------- Active ball
	if(m_iPhase==Phase_Active)
	{
		l_poActiveBallPivot->setVisible(true);
		l_poActiveBallPivot->setPosition(0,0,m_fBallPosition);
	}
	else
	{
		l_poActiveBallPivot->setVisible(false);
	}
	l_poActiveBallPivot->setOrientation(1,0, m_fBallOrientation*4, 0);
	//------- Get ready ball
	if(m_iPhase==Phase_GetReady)
	{
		l_poGetReadyBallPivot->setVisible(true);
		l_poGetReadyBallPivot->setPosition(0,0,m_fBallPosition);
	}
	else
	{
		l_poGetReadyBallPivot->setVisible(false);
	}
	l_poGetReadyBallPivot->setOrientation(1,0, m_fBallOrientation*4, 0);
	//-------- Passive ball
	if(m_iPhase==Phase_Passive)
	{
		l_poPassiveBallPivot->setVisible(true);
		l_poPassiveBallPivot->setPosition(0,0,m_fBallPosition);
		l_poCrossPivot->setVisible(false);
	}
	else
	{
		l_poPassiveBallPivot->setVisible(false);
		l_poCrossPivot->setVisible(true);
		l_poCrossPivot->setPosition(0,0,0);
	}
	l_poPassiveBallPivot->setOrientation(1,0, m_fBallOrientation*4, 0);
	

	if(!m_bShowMark)
	{
		//l_oArrowTransform.setTranslate(Wm4::Vector3f(1000, 0, 0));
		l_poBasicArrowPivot->setVisible(false);
	}
	else
	{
		switch(m_iMark)
		{
			default:
			case Mark_None:
				//l_oArrowTransform.setTranslate(Wm4::Vector3f(1000, 0, 0));
				l_poBasicArrowPivot->setVisible(false);
				break;

			case Mark_Left:
				//l_oArrowTransform=m_oLeftGoalArrowPosition;
				l_poBasicArrowPivot->setVisible(true);
				l_poBasicArrowPivot->setOrientation(1,0,0,0);
				l_poBasicArrowPivot->rotate(Vector3::UNIT_Y, Radian(-1.570796327));
				l_poBasicArrowPivot->rotate(Vector3::UNIT_Z, Radian(Math::PI/2.f));
				break;

			case Mark_Right:
				//l_oArrowTransform=m_oRightGoalArrowPosition;
				l_poBasicArrowPivot->setVisible(true);
				l_poBasicArrowPivot->setOrientation(1,0,0,0);
				l_poBasicArrowPivot->rotate(Vector3::UNIT_Y, Radian(-1.570796327));
				l_poBasicArrowPivot->rotate(Vector3::UNIT_Z, Radian(-Math::PI/2.f));
				break;
		}
	}

	//l_oGoalScoreTransform.setScale(Wm4::Vector3f(0.1+m_fGoalScoreScale, 1, 1));

	//l_oSideScoreTransform.setScale(Wm4::Vector3f(0.1+m_fSideScoreScale, 1, 1));

	//l_oClassificationScoreTransform.setScale(Wm4::Vector3f(0.1+m_fClassificationScoreScale, 1, 1));

	//l_oFeedbackTransform.setTranslate(Wm4::Vector3f(m_iPhase==Phase_Active?0:1000, 0, 0));
	//l_oFeedbackTransform.setScale(Wm4::Vector3f(_abs_(m_fFeedbackScale)*5, 1, 1));
	//l_oFeedbackTransform.setOrientation(m_fFeedbackScale>0?0:M_PI, 0, 0);

	// -------------------------------------------------------------------------------
	// End of computation

	if(m_bScoreChanged)
	{
		// std::cout << "### NeW ScOrE ! GoAl:" << m_iGoalScore << " SiDe:" << m_iSideScore << " ClAsSiFiCaTiOn:" << m_iClassificationScore << "\n";
		m_bScoreChanged=false;
	}

	m_dLastFeedback=m_dFeedback;
	m_iLastPhase=m_iPhase;
	m_iLastMark=m_iMark;

	return m_bContinue;
}
