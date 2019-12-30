#include "PrecompiledHeader.h"
#include "CameraManager.h"

CameraManager::CameraManager( Context* context ) :
    ManagerImpl( context ),
    cameraDistance( 6.f ),
    cameraPitch( 0.f ),
    cameraYaw( 0.f ),
    mouseYaw( 0.f ),
    deltaMouseMoveWheel( 0.f ),
    type( CameraType::Undefined ),
    targetNode( nullptr ),
    cameraNode( nullptr )
{
}

CameraManager::~CameraManager()
{
}

bool CameraManager::Init()
{
    //Subscribe Events
    SubscribeToEvent( E_POSTUPDATE, URHO3D_HANDLER( CameraManager, HandlePostUpdate ) );

    return true;
}

void CameraManager::SetCameraPosition( const Vector3& position, const Quaternion& rotation )
{
    GetCameraNode()->SetPosition( position );
    GetCameraNode()->SetRotation( rotation );

    UpdateCameraBase();
}

void CameraManager::UpdateCameraBase()
{
    if( type == CameraType::Follow && targetNode )
    {
        auto headNode = targetNode->GetChild( "Bip01 Head", true );

        if( headNode )
            lookAtBase = headNode->GetPosition().UP + Vector3( 0.0f, 0.0f, 0.0f );
    }
}

Node* CameraManager::GetCameraNode()
{
    if( cameraNode == nullptr )
    {
        auto findCameraNode = ACTIVESCREEN->GetScene()->GetChild( "Camera", true );

        if( findCameraNode )
            cameraNode = findCameraNode;
        else
        {
            cameraNode = ACTIVESCREEN->GetScene()->CreateChild( "Camera", LOCAL );
            cameraNode->CreateComponent<Urho3D::Camera>();
        }
    }

    return cameraNode;
}

Node* CameraManager::GetNodeRaycast()
{
    if( ACTIVESCREEN->GetScene() )
    {
        IntVector2 mousePosition = INPUT->GetMousePosition();

        float x = (float)mousePosition.x_ / GRAPHICS->GetWidth();
        float y = (float)mousePosition.y_ / GRAPHICS->GetHeight();

        Ray cameraRay = GetCamera()->GetScreenRay( x, y );
        PODVector<RayQueryResult> result;
        RayOctreeQuery query( result, cameraRay, RAY_AABB, SELECTUNIT_MAX_DISTANCE, DRAWABLE_GEOMETRY, 8 );

        auto octree = ACTIVESCREEN->GetScene()->GetComponent<Octree>();

        if( octree )
        {
            octree->Raycast( query );

            for( const auto& raycastResult : result )
            {
                if( raycastResult.node_ )
                {
                    //Node is selectable?
                    if( raycastResult.node_->HasTag( "PickBox" ) )
                        return raycastResult.node_;
                }
            }
        }
    }

    return nullptr;
}

void CameraManager::HandlePostUpdate( StringHash eventType, VariantMap& eventData )
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    //Move Camera Handler
    HandleMoveCamera( timeStep );

    //Update Mouse Yaw
    mouseYaw = Atan2( INPUT->GetMousePosition().y_ - (GRAPHICS->GetSize().y_ >> 1), INPUT->GetMousePosition().x_ - (GRAPHICS->GetSize().x_ >> 1) ) + 90.0f;

    //Follow Camera
    if( type == CameraType::Follow && targetNode )
    {
        GetCameraNode()->SetRotation( Quaternion( cameraPitch, cameraYaw, 0.0f ) );
        GetCameraNode()->SetPosition( targetNode->GetPosition() + lookAtBase + GetCameraNode()->GetRotation() * Vector3::BACK * cameraDistance );
    }
}

void CameraManager::HandleMoveCamera( float timeStep )
{
    if( USERINTERFACE->GetFocusElement() )
        return;

    if( type == CameraType::Follow && targetNode )
    {
        //Distance Camera
        if( INPUT->GetKeyDown( KEY_UP ) )
            cameraDistance -= 30.f * timeStep;
        else if( INPUT->GetKeyDown( KEY_DOWN ) )
            cameraDistance += 30.f * timeStep;

        //Limit Camera Distance
        cameraDistance = Clamp( cameraDistance, CAMERA_MIN_DISTANCE, CAMERA_MAX_DISTANCE );

        //Move camera to right or left
        if( INPUT->GetKeyDown( KEY_LEFT ) )
            cameraYaw += 100.f * timeStep;
        else if( INPUT->GetKeyDown( KEY_RIGHT ) )
            cameraYaw -= 100.f * timeStep;

        //Camera Pitch
        if( INPUT->GetMouseMoveWheel() )
            deltaMouseMoveWheel += INPUT->GetMouseMoveWheel() * 0.05f;

        if( deltaMouseMoveWheel )
        {
            if( deltaMouseMoveWheel < 0.0f )
            {
                deltaMouseMoveWheel += 0.001f;

                if( deltaMouseMoveWheel >= 0.0f )
                    deltaMouseMoveWheel = 0.0f;
            }
            else
            {
                deltaMouseMoveWheel -= 0.001f;

                if( deltaMouseMoveWheel <= 0.0f )
                    deltaMouseMoveWheel = 0.0f;
            }

            cameraPitch += deltaMouseMoveWheel * timeStep * 200.f;
        }

        //Limit Camera Pitch
        cameraPitch = Clamp( cameraPitch, 1.0f, 90.0f );
    }
}
