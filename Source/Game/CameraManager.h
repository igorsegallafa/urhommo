#pragma once

#define CAMERANODE				(CAMERAMANAGER->GetCameraNode())
#define CAMERA					(CAMERAMANAGER->GetCamera())

#define CAMERA_MIN_DISTANCE		2.0f
#define CAMERA_MAX_DISTANCE		25.0f

#define SELECTUNIT_MAX_DISTANCE	75.0f

enum class CameraType
{
    Undefined,

    AnimatedPath,
    Free,
    FreeFly,
    Follow,
};

class CameraManager : public ManagerImpl
{
    URHO3D_OBJECT( CameraManager, ManagerImpl );
public:
    //! Constructor.
    CameraManager( Context* context );

    //! Deconstructor.
    ~CameraManager();

    //! Initialize Camera Manager.
    bool Init();

    /**
     * Set Camera Type and your target if needed
     * @param cameraType Type desired for camera
     * @param target Target of camera when needed
     */
    void SetCameraType( const CameraType& cameraType, Node* target = nullptr ) { type = cameraType; targetNode = target; UpdateCameraBase(); }
    
    /**
     * Set Camera Position
     * @param position Position of Camera
     * @param rotation Rotation of Camera
     */
    void SetCameraPosition( const Vector3& position, const Quaternion& rotation = Quaternion() );

    //! Update Camera Base.
    void UpdateCameraBase();

    //! Getters.
    Node* GetCameraNode();
    Urho3D::Camera* GetCamera() { return GetCameraNode()->GetComponent<Urho3D::Camera>(); }
    Node* GetNodeRaycast();
    float GetCameraDistance() const{ return cameraDistance; }
    float GetCameraPitch() const{ return cameraPitch; }
    float GetCameraYaw() const{ return cameraYaw; }
    float GetMouseYaw() const{ return mouseYaw; }

    //! Subscribed Events.
    void HandlePostUpdate( StringHash eventType, VariantMap& eventData );
private:
    //! Move Camera Handler.
    void HandleMoveCamera( float timeStep );
private:
    WeakPtr<Node> cameraNode;   //!< Pointer for camera Node.
    CameraType type;    //!< Camera Type.
    Node* targetNode;   //!< Camera Target Node.

    float deltaMouseMoveWheel;  //!< Delta Mouse Move Wheel.

    float cameraDistance;   //!< Camera Distance.
    float cameraPitch;  //!< Camera Pitch.
    float cameraYaw;    //!< Camera Yaw.

    Vector3 lookAtBase; //!< Camera Target Position.

    float mouseYaw; //!< Mouse Yaw.
};