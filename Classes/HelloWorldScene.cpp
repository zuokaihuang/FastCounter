#include "HelloWorldScene.h"
#include <algorithm> 

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    //auto scene = Scene::create();
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vect(0,-900));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	i = j = 0;
	t = 0;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
										   CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
  
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 80);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

	
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("Dota2.jpg");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    

	//hzk add 

	_player = Sprite::create("player-hd.png");
	_player->setPosition(visibleSize.width / 8, visibleSize.height / 2);

	auto OneBody = PhysicsBody::createBox(_player->getContentSize());
	OneBody->setContactTestBitmask(0x04);
	OneBody->applyImpulse(Vec2(0.0f, 0.0f));
	_player->setPhysicsBody(OneBody);
	this->addChild(_player,0);
	_player->retain();

	int i = 0;
	while (i<=4)
	addPipe(i++);

	//this->schedule(schedule_selector(HelloWorld::addMonster), 1);


	touch_listener = EventListenerTouchOneByOne::create();

	touch_listener->setSwallowTouches(false);
	touch_listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	touch_listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);

	EventListenerPhysicsContact *contact_listener = EventListenerPhysicsContact::create();
	contact_listener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contact_listener, this);

	scheduleUpdate();
	
	auto red = LayerColor::create(Color4B(255, 100, 100, 128), visibleSize.width / 2, visibleSize.height / 2);

	auto green = LayerColor::create(Color4B(100, 255, 100, 128), visibleSize.width / 4, visibleSize.height / 4);

	red->addChild(green);

	this->addChild(red, 0);


    return true;
}
void HelloWorld::update(float dt){

	//auto v = _player->getPhysicsBody()->getVelocity();

	//_player->setRotation(std::min(std::max(-90.0f, v.y * 0.2f + 60.0f), 30.0f));

}
bool HelloWorld::onTouchBegan(Touch* pTouch, Event* pEvent)
{

	Size visibleSize = Director::getInstance()->getVisibleSize();

	Point touchLocation = pTouch->getLocationInView();
	touchLocation = Director::getInstance()->convertToGL(touchLocation);

	_player->getPhysicsBody()->setEnable(true);
	_player->getPhysicsBody()->setVelocity(Vec2(0,260));


	auto back = projectile.back();
	auto back_x = back->getPositionX();
	auto back_y = back->getPositionY();

	for (auto p : projectile){
		auto x = p->getPositionX();
		auto y = p->getPositionY();
		auto move = MoveTo::create(0.5f,Vec2(x-100,y));

		auto callback = CallFunc::create([this](){
			this->JumpedCallBack();
		});
		auto action = Sequence::create(move->clone(), callback,NULL);
		
		p->runAction(action);
		
	}
	auto front = projectile.front();
	front->stopAllActions();
	front->setPosition(back_x, back_y);
	projectile.push_back(projectile.front());
	projectile.erase(projectile.begin());
	return true;
}



void HelloWorld::onTouchMoved(Touch* pTouch, Event* pEvent)
{
}
void HelloWorld::onTouchEnded(Touch* pTouch, Event* pEvent)
{
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::addMonster(float dt){

	Size visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* MonsterExample = Sprite::create("monster-hd.png");
	j += 1;

	auto OneBody = PhysicsBody::createBox(MonsterExample->getContentSize());
	OneBody->setContactTestBitmask(0x08);

	MonsterExample->setPhysicsBody(OneBody);

	MonsterExample->setPosition(
		visibleSize.width + MonsterExample->getContentSize().width,
		MonsterExample->getContentSize().height / 2 + CCRANDOM_0_1()*(visibleSize.height - MonsterExample->getContentSize().height / 2));

	Monster.push_back(MonsterExample);
	this->addChild(Monster.back(),j);

	MoveTo* Move = MoveTo::create(8, Vec2(-Monster.back()->getContentSize().width / 2, Monster.back()->getPosition().y));

	Monster.back()->runAction(Move);

}


bool HelloWorld::onContactBegin(const PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();



	return true;
}

void HelloWorld::addPipe(int index)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_p = Sprite::create("monster-hd.png");
	char buf[10] = {0};
	sprintf(buf,"%d",index);
	auto label = Label::createWithTTF(buf, "fonts/Marker Felt.ttf", 80);
	label->setAnchorPoint(Vec2(0,0));
	_p->addChild(label);
	auto OneBody = PhysicsBody::createBox(_p->getContentSize());
	OneBody->setContactTestBitmask(0x04);
	OneBody->setGravityEnable(false);
	OneBody->setDynamic(false);
	_p->setPosition(visibleSize.width / 8+index*100, visibleSize.height / 2 - _p->getContentSize().height*2);
	_p->setPhysicsBody(OneBody);
	

	projectile.push_back(_p);

	this->addChild(projectile.back(), 1);
	pipe_count = projectile.size();
	_p->retain();
}

void HelloWorld::JumpedCallBack()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	CCLOG("----x=>%f---y=>%f-------------", _player->getPosition().x, _player->getPosition().y);

}