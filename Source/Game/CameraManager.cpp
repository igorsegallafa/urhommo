#include "PrecompiledHeader.h"
#include "CameraManager.h"

CameraManager::CameraManager( Context* context ) :
    ManagerImpl( context ),
    cameraDistance_( 6.f ),
    cameraPitch_( 0.f ),
    cameraYaw_( 0.f ),
    mouseYaw_( 0.f ),
    deltaMouseMoveWheel_( 0.f ),
    type_( CameraType::Undefined ),
    targetNode_( nullptr ),
    cameraNode_( nullptr ),
    mode_( CameraMode::Manual )
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
    if( type_ == CameraType::Follow && targetNode_ )
    {
        auto headNode = targetNode_->GetChild( "Bip01 Head", true );

        if( headNode )
            lookAtBase_ = headNode->GetPosition().UP + Vector3( 0.0f, 0.0f, 0.0f );
    }
}

Node* CameraManager::GetCameraNode()
{
    if( cameraNode_ == nullptr )
    {
        if( ACTIVESCREEN == nullptr )
            return nullptr;

        auto findCameraNode = ACTIVESCREEN->GetScene()->GetChild( "Camera", true );

        if( findCameraNode )
            cameraNode_ = findCameraNode;
        else
        {
            cameraNode_ = ACTIVESCREEN->GetScene()->CreateChild( "Camera", LOCAL );
            cameraNode_->CreateComponent<Urho3D::Camera>();
        }
    }

    return cameraNode_;
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
                    {
                        //Get Parent Node, looking for node with rigid body
                        auto parentNode = raycastResult.node_->GetParent();
                        
                        if( parentNode )
                            if( parentNode->GetComponent<RigidBody>() )
                                return parentNode;

                        return raycastResult.node_;
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

    //Move Camera Handler
    HandleMoveCamera( timeStep );

    //Update Mouse Yaw
    mouseYaw_ = Atan2( INPUT->GetMousePosition().y_ - (GRAPHICS->GetSize().y_ >> 1), INPUT->GetMousePosition().x_ - (GRAPHICS->GetSize().x_ >> 1) ) + 90.0f;

    //Follow Camera
    if( type_ == CameraType::Follow && targetNode_ )
    {
        GetCameraNode()->SetRotation( Quaternion( cameraPitch_, cameraYaw_, 0.0f ) );
        GetCameraNode()->SetPosition( targetNode_->GetPosition() + lookAtBase_ + GetCameraNode()->GetRotation() * Vector3::BACK * cameraDistance_ );
    }
}

void CameraManager::HandleMoveCamera( float timeStep )
{
    if( USERINTERFACE->GetFocusElement() )
        return;

    if( !INPUT->HasFocus() )
        return;

    if( type_ == CameraType::Follow && targetNode_ )
    {
        //Distance Camera
        if( mode_ == CameraMode::Manual || mode_ == CameraMode::Auto )
        {
            if( INPUT->GetKeyDown( Key::KEY_UP ) )
                cameraDistance_ -= 30.f * timeStep;
            else if( INPUT->GetKeyDown( Key::KEY_DOWN ) )
                cameraDistance_ += 30.f * timeStep;
        }

        //Limit Camera Distance
        cameraDistance_ = Clamp( cameraDistance_, CAMERA_MIN_DISTANCE, CAMERA_MAX_DISTANCE );

        //Move camera to right or left
        if( INPUT->GetKeyDown( KEY_LEFT ) || INPUT->GetMousePosition().x_ > GRAPHICS->GetSize().x_ - 10 )
            cameraYaw_ += 100.f * timeStep;
        else if( INPUT->GetKeyDown( KEY_RIGHT ) || INPUT->GetMousePosition().x_ < 10 )
            cameraYaw_ -= 100.f * timeStep;

        //Camera Pitch
        if( mode_ == CameraMode::Manual || mode_ == CameraMode::Auto )
        {
            if( INPUT->GetMouseMoveWheel() )
                deltaMouseMoveWheel_ += INPUT->GetMouseMoveWheel() * 0.05f;

            if( deltaMouseMoveWheel_ )
            {
                if( deltaMouseMoveWheel_ < 0.0f )
                {
                    deltaMouseMoveWheel_ += 0.001f;

                    if( deltaMouseMoveWheel_ >= 0.0f )
                        deltaMouseMoveWheel_ = 0.0f;
                }
                else
                {
                    deltaMouseMoveWheel_ -= 0.001f;

                    if( deltaMouseMoveWheel_ <= 0.0f )
                        deltaMouseMoveWheel_ = 0.0f;
                }

                cameraPitch_ += deltaMouseMoveWheel_ * timeStep * 200.f;
            }
        }

        //Limit Camera Pitch
        cameraPitch_ = Clamp( cameraPitch_, 1.0f, 90.0f );

        //Auto Camera
        if( mode_ == CameraMode::Auto )
        {
            float delta = GetCameraNode()->GetRotation().YawAngle() - CHARACTERHANDLER->GetCharacter()->GetNode()->GetRotation().YawAngle();

            if( delta >= 180.f )
                delta = delta - 360.f;

            if( abs( delta ) < 90.f )
            {
                float step = abs( delta ) / 128.f;

                if( delta < 0.f )
                    cameraYaw_ += step;
                else
                    cameraYaw_ -= step;
            }
        }
    }
}
