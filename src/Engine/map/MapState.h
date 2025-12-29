//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


#pragma once
#include "bobtypes.h"
class Logger;




class MapState
{
	//these hold the Light objects themselves, created in the constructor for this map.
	//these both hold the same thing for convenience.
public:
	ArrayList<Light*> lightList;
	HashMap<string, Light*> lightByNameHashtable;//new HashMap<string, Light*>();


	ArrayList<shared_ptr<Entity>> entityList;
	HashMap<string, shared_ptr<Entity>> entityByNameHashtable;//new HashMap<string, Entity*>();

	ArrayList<shared_ptr<Character>> characterList;
	HashMap<string, shared_ptr<Character>> characterByNameHashtable;//new HashMap<string, Character*>();


	ArrayList<Area*> areaList;
	HashMap<string, Area*> areaByNameHashtable;//new HashMap<string, Area*>();
	HashMap<string, Area*> areaByTYPEIDHashtable;//new HashMap<string, Area*>();



	Map* map = nullptr;

private:
	MapStateData* data = nullptr;


public:
	MapState(MapStateData* mapStateData, Map* m);


	MapStateData* getData();

	int getID();
	string& getName();
};

