#include "ResearchStorage.h"

#include <DataLayer/DataMappingLayer/MapperFacade.h>
#include <DataLayer/DataMappingLayer/ResearchMapper.h>

#include <Domain/Research.h>

using namespace DataStorageLayer;
using namespace DataMappingLayer;


ResearchTable* ResearchStorage::all()
{
	if (m_all == 0) {
		m_all = MapperFacade::researchMapper()->findAll();
	}
	return m_all;
}

Research* ResearchStorage::research(const QString& _name)
{
	Research* resultResearch = 0;
	foreach (DomainObject* domainObject, all()->toList()) {
		Research* research = dynamic_cast<Research*>(domainObject);
		if (research->name() == _name) {
			resultResearch = research;
			break;
		}
	}
	return resultResearch;
}

Research* ResearchStorage::storeResearch(Research* _parent, int _researchType, int _sortOrder,
	const QString& _researchName)
{
	//
	// Создаём новую разработку
	//
	Research* newResearch =
		new Research(Identifier(), _parent, (Research::Type)_researchType, _sortOrder, _researchName);

	//
	// И сохраняем её
	//
	// ... в базе данных
	//
	MapperFacade::researchMapper()->insert(newResearch);

	//
	// ... в списках
	//
	all()->append(newResearch);

	return newResearch;
}

void ResearchStorage::updateResearch(Research* _research)
{
	//
	// Сохраним изменение в базе данных
	//
	MapperFacade::researchMapper()->update(_research);

	//
	// Уведомим об обновлении
	//
	int indexRow = all()->toList().indexOf(_research);
	QModelIndex updateIndex = all()->index(indexRow, 0, QModelIndex());
	emit all()->dataChanged(updateIndex, updateIndex);
}

void ResearchStorage::removeResearch(Research* _research)
{
	//
	// Если такая разработка есть
	//
	if (hasResearch(_research)) {
		//
		// Удаляем так же все вложенные разработки
		//
		QList<DomainObject*> full = all()->toList();
		QList<Research*> toDelete;
		toDelete.append(_research);
		//
		// проходим лист и собираем всех детей элемента
		// проходим лист и собираем всех детей его детей
		// проходим лист таким образом, пока не случится пустого прохода
		//
		int toDeleteCount = 0;
		do {
			toDeleteCount = 0;
			for (int index = 0; index < full.size(); ++index) {
				Research* nextResearch = dynamic_cast<Research*>(full.value(index));
				if (toDelete.contains(nextResearch->parent())) {
					toDelete.append(nextResearch);
					full.removeAt(index);
					--index;
					++toDeleteCount;
				}
			}
		} while (toDeleteCount > 0);

		foreach (Research* research, toDelete) {
			//
			// ... удалим из локального списка и базы данных
			//
			all()->remove(research);
            characters()->remove(research);
            locations()->remove(research);
			MapperFacade::researchMapper()->remove(research);
		}
	}
}

bool ResearchStorage::hasResearch(Research* _research)
{
	bool contains = false;
	foreach (DomainObject* domainObject, all()->toList()) {
		Research* research = dynamic_cast<Research*>(domainObject);
		if (research == _research) {
			contains = true;
			break;
		}
	}
	return contains;
}

void ResearchStorage::clear()
{
	delete m_all;
    m_all = nullptr;

    delete m_characters;
    m_characters = nullptr;

    delete m_locations;
    m_locations = nullptr;

	MapperFacade::researchMapper()->clear();
}

void ResearchStorage::refresh()
{
	MapperFacade::researchMapper()->refresh(all());
}

ResearchTable* ResearchStorage::characters()
{
    if (m_characters == 0) {
        m_characters = MapperFacade::researchMapper()->findCharacters();
    }
    return m_characters;
}

Research* ResearchStorage::character(const QString& _name)
{
    return research(_name);
}

Research* ResearchStorage::storeCharacter(const QString& _name)
{
    Research* newCharacter = 0;

    QString characterName = _name.toUpper().trimmed();

    //
    // Если персонажа можно сохранить
    //
    if (!characterName.isEmpty()) {
        //
        // Проверяем наличие данного персонажа
        //
        foreach (DomainObject* domainObject, characters()->toList()) {
            Research* character = dynamic_cast<Research*>(domainObject);
            if (character->name() == characterName) {
                newCharacter = character;
                break;
            }
        }

        //
        // Если такого персонажа ещё нет, то сохраним его
        //
        if (!DomainObject::isValid(newCharacter)) {
            //
            // ... в базе данных и полном списке разработок
            //
            newCharacter = storeResearch(nullptr, Research::Character, characters()->size(), characterName);

            //
            // ... в текущем списке персонажей
            //
            characters()->append(newCharacter);
        }
    }

    return newCharacter;
}

void ResearchStorage::updateCharacter(Research* _character)
{
    updateResearch(_character);
}

void ResearchStorage::removeCharacter(const QString& _name)
{
    if (hasCharacter(_name)) {
        removeResearch(character(_name));
    }
}

void ResearchStorage::removeCharacters(const QStringList& _names)
{
    for (const QString& name : _names) {
        removeCharacter(name);
    }
}

bool ResearchStorage::hasCharacter(const QString& _name)
{
    bool contains = false;
    foreach (DomainObject* domainObject, characters()->toList()) {
        Research* character = dynamic_cast<Research*>(domainObject);
        if (character->name() == _name) {
            contains = true;
            break;
        }
    }
    return contains;
}

ResearchTable* ResearchStorage::locations()
{
    if (m_locations == nullptr) {
        m_locations = MapperFacade::researchMapper()->findLocations();
    }
    return m_locations;
}

ResearchStorage::ResearchStorage()
{
}
