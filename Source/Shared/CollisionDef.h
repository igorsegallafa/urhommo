#pragma once

URHO3D_EVENT( E_GHOST_COLLISION_STARTED, GhostCollisionBegin )
{
    URHO3D_PARAM( P_BODY, Body );             /// RigidBody which collided with btGhostObject
    URHO3D_PARAM( P_GHOST, Ghost );           /// btGhostObject which collided with RigidBody
    URHO3D_PARAM( P_GHOSTNODE, GhostNode );   /// Scene node which acts as Parent to btGhostNode
}

URHO3D_EVENT( E_GHOST_COLLISION_STAY, GhostCollisionStay )
{
    URHO3D_PARAM( P_BODY, Body );
    URHO3D_PARAM( P_GHOST, Ghost );
    URHO3D_PARAM( P_GHOSTNODE, GhostNode );
}

URHO3D_EVENT( E_GHOST_COLLISION_ENDED, GhostCollisionEnded )
{
    URHO3D_PARAM( P_BODY, Body );
    URHO3D_PARAM( P_GHOST, Ghost );
    URHO3D_PARAM( P_GHOSTNODE, GhostNode );
}