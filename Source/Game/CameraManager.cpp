#include "PrecompiledHeader.h"
#include "CameraManager.h"

CameraManager::CameraManager( Context* context ) :
    ManagerImpl( context ),
    cameraDistance( 100.f ),
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
            lookAtBase = headNode->GetPosition().UP + Vector3( 0.0f, 0.8f, 0.0f );
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
        PODVector<PhysicsRaycastResult> result;
        auto physicsWorld = ACTIVESCREEN->GetScene()->GetComponent<PhysicsWorld>();

        if( physicsWorld )
        {
            physicsWorld->Raycast( result, cameraRay, SELECTUNIT_MAX_DISTANCE );

            for( const auto& raycastResult : result )
            {
                if( raycastResult.body_ )
                {
                    auto foundNode = raycastResult.body_->GetNode();

                    //Found Node?
                    if( foundNode )
                    {
                        //Node is selectable?
                        if( foundNode->HasTag( "Selectable" ) )
                        {
                            auto parentNode = foundNode->GetParent();

                            if( parentNode )
                                foundNode = parentNode;

                            return foundNode;
                        }
                    }
                }
            }
        }
    }

    return nullptr;
}

void CameraManager::HandlePostUpdate( StringHash eventType, VariantMap& eventData )
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    //Follow Camera
    if( type == CameraType::Follow && targetNode )
    {
        GetCameraNode()->SetRotation( Quaternion( cameraPitch, cameraYaw, 0.0f ) );
        GetCameraNode()->SetPosition( targetNode->GetPosition() + lookAtBase + GetCameraNode()->GetRotation() * Vector3::BACK * cameraDistance );
    }
}