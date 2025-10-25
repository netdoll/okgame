//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------

#pragma once
#include "bobtypes.h"
class Logger;


class GameString;
class Flag;
class Skill;
class Item;

class EventManager : public EnginePart
{
public:

	static Logger log;

	//ArrayList<BobEvent*> cutsceneEventList;//project events and unattached events
	ArrayList<BobEvent*> eventList;//all events get put here for global lookup
	ArrayList<Dialogue*> dialogueList;
	ArrayList<GameString*> gameStringList;
	ArrayList<Flag*> flagList;
	ArrayList<Skill*> skillList;
	ArrayList<Item*> itemList;

	ArrayList<BobEvent*> runningEventQueue;

	EventManager(Engine* g);


	void update();

	void addToEventQueueIfNotThere(BobEvent* event);

	bool isEventInQueue(BobEvent* event);

	void unloadCurrentMapEvents();

	Item* getItemByID(int id);

	Dialogue* getDialogueByIDCreateIfNotExist(int id);

	//BobEvent* getCutsceneEventByID(int id);
	BobEvent* getEventByIDCreateIfNotExist(int id);

	Skill* getSkillByIDCreateIfNotExist(int id);

	GameString* getGameStringByIDCreateIfNotExist(int id);

	Flag* getFlagByIDCreateIfNotExist(int id);
};

