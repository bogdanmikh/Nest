//
// Created by Admin on 09.02.2022.
//

#include "Nest/GameLogic/Entity.hpp"

namespace Nest {

Entity::Entity(entt::registry *registry, id_t id, World *world)
    : m_registry(registry)
    , m_id(id)
    , m_world(world) {}

Entity::Entity()
    : m_registry(nullptr)
    , m_id(-1)
    , m_world(nullptr) {}

TransformComponent &Entity::getTransform() {
    return getComponent<TransformComponent>();
}

void Entity::addChildEntity(Entity entity) {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    RelationshipComponent &otherRelationship = entity.getComponent<RelationshipComponent>();

    otherRelationship.parentHandle = m_id;
    thisRelationship.children.push_back(entity.m_id);
}

void Entity::removeChildEntity(Entity entity) {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    RelationshipComponent &otherRelationship = entity.getComponent<RelationshipComponent>();

    std::remove(thisRelationship.children.begin(), thisRelationship.children.end(), entity.m_id);
    otherRelationship.parentHandle = -1;
}

void Entity::removeFromParent() {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    Entity parent = Entity(m_registry, thisRelationship.parentHandle, m_world);
    parent.removeChildEntity(*this);
}

Entity Entity::getParent() {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    return Entity(m_registry, thisRelationship.parentHandle, m_world);
}

} // namespace Panda
