/*
 * Mermelin, Copyright © 2013 by Eduardo Hahn Paredes (mermelin@outlook.com)
 * 
 * Mermelin is free software: You can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation, either version 3 of 
 * the License, or (at your option) any later version.
 * Mermelin is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Mermelin. 
 * Look for a file named COPYING. If it's not there, see <http://www.gnu.org/licenses/>.
 */

/** 
 * @file Level.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Resources/Files/Level.h"

using namespace std;
using namespace CotopaxiEngine;

// initializing static const variables
const Ogre::Quaternion Level::rot90 = Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);
const Ogre::Quaternion Level::rot180 = Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y);
const Ogre::Quaternion Level::rot270 = Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_Y);

Level::Level(Ogre::ResourceManager *creator,
        const Ogre::String& name,
        Ogre::ResourceHandle handle,
        const Ogre::String& group,
        bool isManual,
        Ogre::ManualResourceLoader* loader)
: Ogre::Resource(creator, name, handle, group, isManual, loader)
{
    createParamDictionary("Level");
    objectCounter = 0;
    lineCounter = 0;
    floorCounter = 0;
    state = State::NONE;

    try {
        levelNode = ENGINE->getSceneManager()->getSceneNode("LevelNode");
    } catch (std::exception& e) {
        levelNode = ENGINE->getSceneManager()->getRootSceneNode()->createChildSceneNode("LevelNode");
    }

}

void destroyAllAttachedMovableObjects(Ogre::SceneNode* node)
{
    if (!node) return;

    // destroying all the attached objects
    Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

    while (itObject.hasMoreElements())
        node->getCreator()->destroyMovableObject(itObject.getNext());

    // recurse to child SceneNodes
    Ogre::SceneNode::ChildNodeIterator itChild = node->getChildIterator();

    while (itChild.hasMoreElements()) {
        Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*> (itChild.getNext());
        destroyAllAttachedMovableObjects(pChildNode);
    }
}

/**
 * @fn destroySceneNode
 * @relate Level
 * Destroys every attached scenenode of the given node.
 * Method taken from 
 * <a href="http://www.ogre3d.org/forums/viewtopic.php?f=2&t=53647&start=0#p445350">
 * the ogre3d forum</a>
 * @param node Pointer to the node that should be destroyed
 */
void destroySceneNode(Ogre::SceneNode* node)
{
    if (!node) return;
    destroyAllAttachedMovableObjects(node);
    node->removeAndDestroyAllChildren();
    node->getCreator()->destroySceneNode(node);
}

Level::~Level()
{
    destroyAllAttachedMovableObjects(levelNode);
    destroySceneNode(levelNode);

    unload();
}

void Level::loadImpl()
{
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName,
            mGroup, true, this);
    string line;

    while (!stream->eof()) {
        line = stream->getLine();
        readLine(line);
    }
    stream->close();

}

void Level::unloadImpl()
{
    for (std::vector<Entity*>::iterator it = entityPointers.begin(); it != entityPointers.end(); it++) {
        ENGINE->removeEntity(*it);
    }

    levelNode->removeAndDestroyAllChildren();
}

std::string Level::getLevelName() const
{
    return levelName;
}

std::string trim(const std::string& str, const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void Level::readLine(string line)
{
    string lineToParse = trim(line);
    int commentPosition = lineToParse.find("#");

    if (commentPosition != string::npos) {
        lineToParse = lineToParse.substr(0, commentPosition);
    }

    // putting reader in a defined mode and read next line
    if (lineToParse == "Properties:") {
        state = State::PROPERTIES;
        return;
    } else if (lineToParse == "Logic:") {
        state = State::LOGIC;
        return;
    } else if (lineToParse == "Free:") {
        state = State::FREE;
        return;
    } else if (lineToParse == "Objects:") {
        state = State::OBJECTS;
        return;
    } else if (lineToParse.empty()) {
        return;
    }

    if (lineToParse == "@") {
        // a new layer for the same floor. resetting the line counting
        state = State::OBJECTS;
        lineCounter = 0;
        return;
    }

    if (lineToParse == "%") {
        // ascending one floor and resetting the line counting
        state = State::OBJECTS;
        floorCounter++;
        lineCounter = 0;
        return;
    }

    switch (state)
    {
        case State::OBJECTS:
        {
            readObjectLine(lineToParse);
            break;
        }
        case State::LOGIC:
        {
            readLogicLine(lineToParse);
            break;
        }
        case State::FREE:
        {
            readFreeLine(lineToParse);
            break;
        }
        case State::NONE:
        case State::PROPERTIES:
        {
            readPropertiesLine(lineToParse);
            break;
        }
    }
}

void Level::readObjectLine(string line)
{
    GraphicModule* graphicModule = ENGINE->getGraphic();
    string::iterator lineIterator = line.begin();
    std::string name;

    while (lineIterator != line.end()) {
        char currentObject = *lineIterator;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        name = currentObject + std::string("_C")
                + std::to_string((long double) objectCounter)
                + std::string("_R") + std::to_string((long double) lineCounter)
                + std::string("_F") + std::to_string((long double) floorCounter);
#else
        name = currentObject + std::string("_C")
                + std::to_string(objectCounter)
                + std::string("_R") + std::to_string(lineCounter)
                + std::string("_F") + std::to_string(floorCounter);
#endif

        if (currentObject == '0') {
            // do not create anything, zeros are used as spaceholders
        } else {
            tempVec = Ogre::Vector3(objectCounter, floorCounter, lineCounter);
            readObject(currentObject, name);
        }
        objectCounter++;
        lineIterator++;
    }

    objectCounter = 0;
    lineCounter++;
}

void Level::readLogicLine(string line)
{
    Entity* entity;
    istringstream tokenizer(line);
    string token;
    std::string name;
    bool trigger = false;
    float x = 0.0;
    float z = 0.0;

    int collGroup = PhysicsModule::COL_STATIC;
    int collMask = PhysicsModule::COL_PLAYER;

    getline(tokenizer, token, '=');

    // creating a button
    if (trim(token) == "button") {

        getline(tokenizer, token, ',');
        istringstream(token) >> name;
        entity = ENGINE->createEntity(name, "Button", levelNode);
        BaseComponent* component = ENGINE->getLogic()->createTrigger(entity, name);
        entity->addComponent(component);
        entity->registerListener(Event::COLLISION_ENTER, component);
        entity->registerListener(Event::COLLISION_EXIT, component);
        trigger = true;
    }

    // creating a ventilator
    if (trim(token) == "rotor") {
        getline(tokenizer, token, ',');
        istringstream(token) >> name;
        entity = ENGINE->produceEntity("rotor", name, levelNode);
        getline(tokenizer, token, ',');
        entity->addComponent(buildConditionTree(token, entity));
        trigger = true;
    }

    // creating a door
    if (trim(token) == "door") {
        getline(tokenizer, token, ',');
        istringstream(token) >> name;
        entity = ENGINE->produceEntity("door", name, levelNode);
        getline(tokenizer, token, ',');
        if (trim(token) == "vertical") {
            entity->getNode()->rotate(rot90);
        } else if (trim(token) == "top") {
            z -= 0.4;
        } else if (trim(token) == "bottom") {
            z += 0.4;
        } else if (trim(token) == "left") {
            entity->getNode()->rotate(rot90);
            x -= 0.4;
        } else if (trim(token) == "right") {
            entity->getNode()->rotate(rot270);
            x += 0.4;
        }
        getline(tokenizer, token, ',');

        LogicComponent* logComp = buildConditionTree(token, entity);
        entity->addComponent(logComp);
    }
    getline(tokenizer, token, '\n');
    tempVec = readVector(token);
    tempVec.x += x;
    tempVec.z += z;

    transform(entity, tempVec, 0, collGroup, collMask, trigger);
}

void Level::readFreeLine(string line)
{
    Entity* entity;
    istringstream tokenizer(line);
    string token;
    std::string name;
    std::string mesh;

    getline(tokenizer, token, '=');
    istringstream(token) >> mesh;
    getline(tokenizer, token, ',');
    istringstream(token) >> name;
    entity = ENGINE->createEntity(name, mesh, levelNode);
    getline(tokenizer, token, ',');
    istringstream weight(token);
    float mass;
    weight >> mass;
    getline(tokenizer, token, '\n');
    tempVec = readVector(token);
    transform(entity, tempVec, mass);
}

void Level::readPropertiesLine(string line)
{
    istringstream tokenizer(line);
    string token;

    getline(tokenizer, token, '=');

    // setting level name
    if (trim(token) == "name") {
        getline(tokenizer, token, '\n');
        istringstream(token) >> levelName;
    }

    // setting the start position of the sphere
    if (trim(token) == "start") {
        getline(tokenizer, token, '\n');
        ENGINE->setStartPosition(readVector(token) *= Ogre::Real(Engine::SCALE));
    }

    // setting initial camera position
    if (trim(token) == "camera") {
        getline(tokenizer, token, '\n');
        ENGINE->getCamera()->getCamera()->setPosition(readVector(token) *= Ogre::Real(Engine::SCALE));
        ENGINE->getCamera()->getCamera()->lookAt(ENGINE->getStartPosition());
    }
}

Entity * Level::readObject(char object, std::string name)
{
    Entity* entity;

    int collGroup = PhysicsModule::COL_STATIC;
    int collMask = PhysicsModule::COL_NOCOLL | PhysicsModule::COL_PLAYER;
    bool trigger = false;

    switch (object)
    {
        case 'f':
        {
            entity = ENGINE->createEntity(name, "Floor", levelNode);
            break;
        }
        case '+':
        {
            entity = ENGINE->createEntity(name, "Wall_Cross", levelNode);
            break;
        }
        case '*':
        {
            entity = ENGINE->createEntity(name, "Hole", levelNode);
            break;
        }
        case '-':
        {
            entity = ENGINE->createEntity(name, "Wall_Line", levelNode);
            break;
        }
        case '|':
        {
            entity = ENGINE->createEntity(name, "Wall_Line", levelNode);
            entity->getNode()->rotate(rot90);
            break;
        }
        case '}':
        {
            entity = ENGINE->createEntity(name, "Wall_Curve", levelNode);
            break;
        }
        case '{':
        {
            entity = ENGINE->createEntity(name, "Wall_Curve", levelNode);
            entity->getNode()->rotate(rot270);
            break;
        }
        case 'C':
        {
            entity = ENGINE->createEntity(name, "Wall_Curve", levelNode);
            entity->getNode()->rotate(rot180);
            break;
        }
        case 'D':
        {
            entity = ENGINE->createEntity(name, "Wall_Curve", levelNode);
            entity->getNode()->rotate(rot90);
            break;
        }
        case 'a':
        {
            entity = ENGINE->createEntity(name, "Wall_Corner", levelNode);
            break;
        }
        case ']':
        {
            entity = ENGINE->createEntity(name, "Wall_Corner", levelNode);
            entity->getNode()->rotate(rot270);
            break;
        }
        case '[':
        {
            entity = ENGINE->createEntity(name, "Wall_Corner", levelNode);
            entity->getNode()->rotate(rot180);
            break;
        }
        case 'd':
        {
            entity = ENGINE->createEntity(name, "Wall_Corner", levelNode);
            entity->getNode()->rotate(rot90);
            break;
        }
        case 'q':
        {
            entity = ENGINE->createEntity(name, "Wall_Border_Corner", levelNode);
            break;
        }
        case 'v':
        {
            entity = ENGINE->createEntity(name, "Wall_Border_Corner", levelNode);
            entity->getNode()->rotate(rot270);
            break;
        }
        case 'b':
        {
            entity = ENGINE->createEntity(name, "Wall_Border_Corner", levelNode);
            entity->getNode()->rotate(rot180);
            break;
        }
        case 'r':
        {
            entity = ENGINE->createEntity(name, "Wall_Border_Corner", levelNode);
            entity->getNode()->rotate(rot90);
            break;
        }
        case 'M':
        {
            entity = ENGINE->produceEntity("woodwall", name, levelNode);
            trigger = true;
            break;
        }
        case 'N':
        {
            entity = ENGINE->produceEntity("woodwall", name, levelNode);
            entity->getNode()->rotate(rot90);
            trigger = true;
            break;
        }
        case 'o':
        {
            entity = ENGINE->produceEntity("woodwall", name, levelNode);
            tempVec.z += 0.4;
            trigger = true;
            break;
        }
        case 'n':
        {
            entity = ENGINE->produceEntity("woodwall", name, levelNode);
            entity->getNode()->rotate(rot90);
            tempVec.x -= 0.4;
            trigger = true;
            break;
        }
        case 'O':
        {
            entity = ENGINE->produceEntity("woodwall", name, levelNode);
            entity->getNode()->rotate(rot270);
            tempVec.x += 0.4;
            trigger = true;
            break;
        }
        case 'm':
        {
            entity = ENGINE->produceEntity("woodwall", name, levelNode);
            tempVec.z -= 0.4;
            trigger = true;
            break;
        }
        case 'Q':
        {
            entity = ENGINE->createEntity(name, "Wall_Line", levelNode);
            tempVec.z -= 0.4;
            break;
        }
        case 'V':
        {
            entity = ENGINE->createEntity(name, "Wall_Line", levelNode);
            entity->getNode()->rotate(rot270);
            tempVec.x += 0.4;
            break;
        }
        case 'B':
        {
            entity = ENGINE->createEntity(name, "Wall_Line", levelNode);
            tempVec.z += 0.4;
            break;
        }
        case 'R':
        {
            entity = ENGINE->createEntity(name, "Wall_Line", levelNode);
            entity->getNode()->rotate(rot90);
            tempVec.x -= 0.4;
            break;
        }
        case '$':
        {
            entity = ENGINE->createEntity(name, "Wall_Border_T", levelNode);
            break;
        }
        case '<':
        {
            entity = ENGINE->createEntity(name, "Wall_Border_T", levelNode);
            entity->getNode()->rotate(rot270);
            break;
        }
        case '!':
        {
            entity = ENGINE->createEntity(name, "Wall_Border_T", levelNode);
            entity->getNode()->rotate(rot180);
            break;
        }
        case '>':
        {
            entity = ENGINE->createEntity(name, "Wall_Border_T", levelNode);
            entity->getNode()->rotate(rot90);
            break;
        }
        case 'G':
        {
            entity = ENGINE->produceEntity("goal", name, levelNode);
            collMask = PhysicsModule::COL_PLAYER;
            trigger = true;
            break;
        }
        case 'F': // FIRE
        {
            entity = ENGINE->produceEntity("typechanger", name, levelNode);
            entity->getNode()->rotate(rot180);

            TypeChanger* changer = dynamic_cast<TypeChanger*> (entity);
            if (changer != NULL) {
                changer->setElement(Element::FIRE);
            }
            collMask = PhysicsModule::COL_PLAYER;
            trigger = true;
            break;
        }
        case 'E': // EARTH
        {
            entity = ENGINE->produceEntity("typechanger", name, levelNode);
            TypeChanger* changer = dynamic_cast<TypeChanger*> (entity);
            if (changer != NULL) {
                changer->setElement(Element::EARTH);
            }
            collMask = PhysicsModule::COL_PLAYER;
            trigger = true;
            break;
        }
        case 'A': // AIR
        {
            entity = ENGINE->produceEntity("typechanger", name, levelNode);
            entity->getNode()->rotate(rot180);

            TypeChanger* changer = dynamic_cast<TypeChanger*> (entity);
            if (changer != NULL) {
                changer->setElement(Element::AIR);
            }
            collMask = PhysicsModule::COL_PLAYER;
            trigger = true;
            break;
        }
        case 'W': // WATER
        {
            entity = ENGINE->produceEntity("typechanger", name, levelNode);
            TypeChanger* changer = dynamic_cast<TypeChanger*> (entity);
            if (changer != NULL) {
                changer->setElement(Element::WATER);
            }
            collMask = PhysicsModule::COL_PLAYER;
            trigger = true;
            break;
        }
    }
    transform(entity, tempVec, 0, collGroup, collMask, trigger);

    entity->getNode()->setInheritOrientation(true);
    return entity;
}

void Level::readFile(string path)
{
    string line;
    ifstream file;

    file.open(path.c_str());

    if (file.is_open()) {
        while (!file.eof()) {
            getline(file, line);
            readLine(line);
        }

        file.close();
    } else {
        throw std::exception();
    }
    levelNode->_updateBounds();
}

void Level::transform(Entity* entity, Ogre::Vector3 vec, float mass, int group,
        int mask, bool trigger)
{
    entity->getNode()->translate(Engine::SCALE * vec.x, Engine::SCALE * 2 * vec.y,
            Engine::SCALE * vec.z);
    entity->getNode()->scale(Engine::SCALE, Engine::SCALE, Engine::SCALE);
    entity->addComponent(ENGINE->getPhysics()->getComponent(entity, mass,
            PhysicsModule::TRIMESH, group, mask, trigger));

    Event* e = new Event(Event::TRANSLATE);
    entity->receiveEvent(e);

    entityPointers.push_back(entity);

    delete e;
}

Ogre::Vector3 Level::readVector(string vectorString)
{
    istringstream tokenizer(vectorString);
    string token;

    getline(tokenizer, token, '(');
    getline(tokenizer, token, ',');
    istringstream posX(token);
    float x;
    posX >> x;
    getline(tokenizer, token, ',');
    istringstream posY(token);
    float y;
    posY >> y;
    getline(tokenizer, token, ')');
    istringstream posZ(token);
    float z;
    posZ >> z;
    return Ogre::Vector3(x, y, z);
}

LogicComponent* Level::buildConditionTree(std::string expression, Entity* entity)
{
    std::vector<std::string> expressionTokens;
    std::string::iterator it = expression.begin();
    std::string token = "";
    std::string op = "";
    std::string l;
    std::string r;
    LogicComponent* component;
    bool first = true;

    while (it != expression.end()) {

        if (*it == ':') {
            expressionTokens.push_back(trim(token));
            token = "";
        } else {
            token += *it;
        }

        it++;
    }

    // adding last element of expression
    expressionTokens.push_back(trim(token));

    if (expressionTokens.size() == 1) {
        Condition* cond = new Condition();
        cond->setParent(entity);
        Trigger* trig = ENGINE->getLogic()->getTrigger(expressionTokens[0]);
        trig->addTarget(cond);
        cond->addTrigger(trig);


        return cond;
    } else {
        component = new Condition();
        component->setParent(entity);
    }

    for (int i = expressionTokens.size() - 1; i > 0; i -= 2) {
        l = expressionTokens[i];
        op = expressionTokens[i - 1];
        r = expressionTokens[i - 2];

        if (!first && expressionTokens[i + 1] != op) {
            Condition* child = ((Condition*) component);
            Condition* parent = new Condition();
            parent->setParent(entity);
            parent->addSubCondition(child);
            component = parent;
        }

        first = false;

        Trigger* left = ENGINE->getLogic()->getTrigger(l);
        left->addTarget(((Condition*) component));
        ((Condition*) component)->addTrigger(left);
        Trigger* right = ENGINE->getLogic()->getTrigger(r);
        right->addTarget(((Condition*) component));
        ((Condition*) component)->addTrigger(right);

        if (op == "AND") {
            ((Condition*) component)->setOperator(Condition::AND);
        } else {
            ((Condition*) component)->setOperator(Condition::OR);
        }
    }
    return component;
}