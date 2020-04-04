# UrhoMMO
This is a project of a MMORPG game using the game engine Urho3D. Urho is used for client-side and server-side.

## License
Licensed under the MIT License.

## Development
I am developing this project by myself for now, like a hobby. I have not idea if this could be a *final product* in the future, but I will be working hard to make it happen.

You can follow the development status through commits, and a public trello that I created:
* [Trello](https://trello.com/b/yAHQ3Crm)

## Features
* No dependencies beyond the Urho3D.
* Server networking modified for multiple server instances (E.g. Login Server, Master Server, Game Server).
* Net Server for the communication between all servers.
* Source Code structured at Handler and Managers.
* Authoritative client for character transformation (reducing the latency perception for client-side).
* Client maintain the connection with Master Server and Game Server while in-game.
* Character Collision using Kinematic Character Controller.
* Third Person Camera.
* ImGui for debug windows.
* Find Navigation Path at Entity structure.
* Animation of an Entity as component, used for scene replication.
* Character Animation Set read from JSON file.
* Chat structure already defined.
* Character mover using mouse and keyboard for camera.
* Map Manager for game and server.
* Support for different screens at Game (E.g. World, Character, Login, Test).
* Handler and Manager implementation, for easily code creation.

## Screenshot
![Server](/Screenshot/image1.png?raw=true)
![Game](/Screenshot/image2.gif?raw=true)

## Architecture
![Architecture](/Screenshot/image3.png?raw=true)

As you can see, server structure was not developed for hold a lot of multiple users (for now). So it justifies it to be an extremely simple architecture.

### Login Server
Used for user connect to the server, validate login, registration and send for the client the Game Server list and Master Server connection info. When user has been conected to Game and Master Server, Login Server connection is closed.

### Master Server
Server used for maintain the communication and data operations between the existing servers. It is the *central* server.

### Game Server
The game server is where happens all the gameplay stuff. You can have a lot of Game Servers opened, but the users connected on this game server, will can only see the players connected to this same game server.

## References
* [AsyncLevelLoad by Lumak](https://github.com/Lumak/Urho3D-AsyncLevelLoad/)
* [MessageHandler](https://github.com/igorsegallafa/MessageHandler)
* [KinematicCharacterController by 1vanK](https://github.com/1vanK/Urho3DKinematicCharacterController/)
* [Support for GhostObject collision events by Leith](https://discourse.urho3d.io/t/support-for-ghostobject-collision-events/5215)
* [Levels Fade Effect by artgolf1000](https://discourse.urho3d.io/t/levels-fade-effect/2257)

## Dependencies
* [Urho3D](https://github.com/igorsegallafa/Urho3D/tree/urhommo) at **urhommo** branch
