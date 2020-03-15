#include "PrecompiledHeader.h"
#include "EntityNavigation.h"

#include "Entity.h"

namespace Core
{
void EntityNavigation::HandleNavigationNode()
{
    auto entity = static_cast<Entity*>(this);

    if( targetPath.Size() )
    {
        Vector3 nextWaypoint = targetPath[0];
        float distance = (Vector3(entity->node_->GetPosition().x_,0.f, entity->node_->GetPosition().z_) - Vector3(nextWaypoint.x_,0.f, nextWaypoint.z_)).Length();
        Vector3 dir = nextWaypoint - entity->node_->GetPosition();

        dir.y_ = 0.f;
        targetDirection = dir.Normalized();
        followingTarget = true;

        //Remove waypoint if reached it
        if( distance < 0.1f )
            targetPath.Erase( 0 );
    }
    else
        followingTarget = false;
}

void EntityNavigation::SetTargetPosition( const Vector3& dest )
{
    auto entity = static_cast<Entity*>(this);

    //Reset Target Position
    ResetTargetPosition();

    //Valid node?
    if( entity->node_ )
    {
        targetPos = dest;

        auto scene = entity->node_->GetScene();
        auto navigationMesh = scene->GetComponent<NavigationMesh>( true );

        if( navigationMesh )
            navigationMesh->FindPath( targetPath, entity->node_->GetWorldPosition(), dest, Vector3( 10.f, 10.f, 10.f ) );
    }
}

void EntityNavigation::ResetTargetPosition()
{
    auto entity = static_cast<Entity*>(this);

    if( entity->node_ )
    {
        followingTarget = false;
        targetDirection = Vector3::ZERO;
        targetPath.Clear();
    }
}
};