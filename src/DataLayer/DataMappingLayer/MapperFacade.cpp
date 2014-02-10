#include "MapperFacade.h"

#include "PlaceMapper.h"
#include "LocationMapper.h"
#include "ScenarioDayMapper.h"
#include "TimeMapper.h"
#include "CharacterMapper.h"
#include "ScenarioMapper.h"
#include "SettingsMapper.h"

using namespace DataMappingLayer;


PlaceMapper* MapperFacade::placeMapper()
{
	if (s_placeMapper == 0) {
		s_placeMapper = new PlaceMapper;
	}
	return s_placeMapper;
}

LocationMapper*MapperFacade::locationMapper()
{
	if (s_locationMapper == 0) {
		s_locationMapper = new LocationMapper;
	}
	return s_locationMapper;
}

ScenarioDayMapper*MapperFacade::scenarioDayMapper()
{
	if (s_scenarioDayMapper == 0) {
		s_scenarioDayMapper = new ScenarioDayMapper;
	}
	return s_scenarioDayMapper;
}

TimeMapper*MapperFacade::timeMapper()
{
	if (s_timeMapper == 0) {
		s_timeMapper = new TimeMapper;
	}
	return s_timeMapper;
}

CharacterMapper*MapperFacade::characterMapper()
{
	if (s_characterMapper == 0) {
		s_characterMapper = new CharacterMapper;
	}
	return s_characterMapper;
}

ScenarioMapper*MapperFacade::scenarioMapper()
{
	if (s_scenarioMapper == 0) {
		s_scenarioMapper = new ScenarioMapper;
	}
	return s_scenarioMapper;
}

SettingsMapper*MapperFacade::settingsMapper()
{
	if (s_settingsMapper == 0) {
		s_settingsMapper = new SettingsMapper;
	}
	return s_settingsMapper;
}

PlaceMapper* MapperFacade::s_placeMapper = 0;
LocationMapper* MapperFacade::s_locationMapper = 0;
ScenarioDayMapper* MapperFacade::s_scenarioDayMapper = 0;
TimeMapper* MapperFacade::s_timeMapper = 0;
CharacterMapper* MapperFacade::s_characterMapper = 0;
ScenarioMapper* MapperFacade::s_scenarioMapper = 0;
SettingsMapper* MapperFacade::s_settingsMapper = 0;