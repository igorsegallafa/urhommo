#include "PrecompiledHeader.h"
#include "EntityNavigation.h"

#include "Entity.h"

namespace Core
{
void EntityNavigation::HandleNavigationNode()
{
    auto entity = static_cast<Entity*>(this);

    if( targetPath_.Size() )
    {
        Vector3 nextWaypoint = targetPath_[0];
        float distance = (Vector3(entity->node_->GetPosition().x_,0.f, entity->node_->GetPosition().z_) - Vector3(nextWaypoint.x_,0.f, nextWaypoint.z_)).Length();
        Vector3 dir = nextWaypoint - entity->node_->GetPosition();

        dir.y_ = 0.f;
        targetDirection_ = dir.Normalized();
        followingTarget_ = true;

        //Remove waypoint if reached it
        if( distance < 0.1f )
            targetPath_.Erase( 0 );
    }
    else
        followingTarget_ = false;
}

void EntityNavigation::SetTargetPosition( const Vector3& dest )
{
    auto entity = static_cast<Entity*>(this);
    auto scene = entity->node_->GetScene();

    //Reset Target Position
    ResetTargetPosition();

    //Valid node?
    if( entity->node_ )
    {
        targetPosition_ = dest;

        if( auto navigationMesh = scene->GetComponent<NavigationMesh>( true ); navigationMesh )
            navigationMesh->FindPath( targetPath_, entity->node_->GetWorldPosition(), dest, Vector3( 10.f, 10.f, 10.f ) );
    }
}

void EntityNavigation::ResetTargetPosition()
{
    auto entity = static_cast<Entity*>(this);

    if( entity->node_ )
    {
        followingTarget_ = false;
        targetDirection_ = Vector3::ZERO;
        targetPath_.Clear();
    }
}
};