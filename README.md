
I.	Contents
4.  Use of Case	3
A.	Layout Algorithms	3
A.3.-Additional Features	5
B.	Internal Features	6
5.	Troubleshooting	8


1.	General Description
This is the Technical Specification for the Dungeon Generator plugin in Unreal Engine, this plugin was created for helping designers to speed up the process of design a maze dungeon “Tunic Style” in an easy way in a semi-procedural technique.

1.	System Requirements
Unreal Engine 5.4.4, hasn’t been tested on any other version.
2.	Installation
1.-Download DungeonGenerator.zip y decompress all files inside Content folder in any Unreal Project.
2.- Click on Edit->Plugin->LevelGeneratorEditor
3.-Click on Window->LevelGeneratorEditor to Open Tab
3.	Plugin Structure
4.  Use of Case
A.	Layout Algorithms
A.1.- Basic Space Partitioning Random Layout
This algorithm is designed to create rooms with different sizes and distances. It should be noted that the total size parameters of the dungeon are not 100% accurate since the algorithm divides a given area into random divisions.

A.1.1.-Parameters
Dungeon Width: This is the approximate size of the entire dungeon along the X axis in cm.
Dungeon Depth: This is the approximate size of the entire dungeon along the Y axis in cm.
Algorithm Depth: Indicates the level of divisions the algorithm will create. The larger the value, the greater the divisions, and the smaller the rooms. The algorithm will stop when it reaches the MinRoomWidth and MinRoomDepth values, or if it reaches the selected Depth value, whichever comes first.
MinRoomWidth: Indicates the minimum X-axis size of the rooms. However, if the AlgorithmDepth value reaches it first, the room may be larger than this value.
MinRoomDepth: Indicates the minimum Y-axis size of the rooms. However, if the AlgorithmDepth value reaches it first, the room may be larger than this value.
MinRoomHeight: Indicates the Z-axis size of the rooms; it is not affected by the AlgorithmDepth value.
MinToleranceWidth: Indicates the minimum distance between rooms on the X axis. The distance between rooms can vary, but always respects this value as a minimum.
MinToleranceDepth: Indicates the minimum distance between rooms on the X axis. The distance between rooms can vary, but always respects this value as a minimum.
MinPathWidth: Specifies the minimum path size on the X axis; this distance is always respected.
MinPathDepth: Specifies the minimum path size on the Y axis; this distance is always respected.

A.2.-Basic Space Partitioning Mesh Layout
	This algorithm creates a mesh-like dungeon, creating different rooms of the same size at the same distance, in the same way that Random BSP uses an algorithm with random divisions so the dungeon size parameters are approximate.

A.2.1.-Parameters
Dungeon Width: This is the approximate size of the entire dungeon along the X axis in cm.
Dungeon Depth: This is the approximate size of the entire dungeon along the Y axis in cm.
MinRoomWidth: Indicates the minimum size of the rooms on the X axis. This may vary and may not be exact; it depends on the DungeonWidth value.
MinRoomDepth: Indicates the minimum size of the rooms on the Y axis. This may vary and may not be exact; it depends on the DungeonDepth value.
MinRoomHeight: Indicates the size of the rooms on the Z axis.
MinToleranceWidth: Indicates the minimum distance between rooms on the X axis. The distance between rooms may vary, but always respects this value as a minimum.
MinToleranceDepth: Indicates the minimum distance between rooms on the X axis. The distance between rooms may vary, but always respects this value as a minimum.
MinPathWidth: Specifies the minimum path size on the X axis; this distance is always respected.
MinPathDepth: Specifies the minimum path size on the Y axis; this distance is always respected.

A.3.-Additional Features
 
Delete dungeon: Deletes the dungeon including all the actors and references.
Delete Rooms Only: Deletes Only the Room Actors but the pathways are kept.
Delete: Pathways Only: Deletes only the pathways, rooms are kept.
Delete No Path rooms: Deletes only the rooms that has no connection with other room, in other words a zero pathway rooms are deleted.
Delete Spawned PCG: Deletes only the Actors PCG.
Delete Chests: Deletes chests only.
Create 4-side Pathways: Deletes all the actual pathways and creates a 4-direction pathway in each room, each pathway is spawned from center position room.
Create Random pathways: Deletes all the actual pathways and creates random direction pathways in each room, each pathway is spawned from center position room.
BakePCGActors: Clear all PCG Actors, converts PCG Actors to static mesh actors.
Package All: Creates a one static mesh combining all the rooms and pathways for optimization, this new static mesh has not collision, it can be created manually in project setting complex collision, PCG and chests are not combined.


B.	Internal Features
B.1.- Spawn PCG
This feature is a Random Actor Spawner in the center of each room, the feature selects a random asset from the list and spawns one PCG in each room, to add more new Actors PCG the PCG Graph Must include two float instance parameters called “DistanceX” and “DistanceY” otherwise the plugin can be crash or not work properly.

 The procedure to Add a New Actor PCG is:
1.-Create a New blueprint Actor
2.-Add any PCG Component you want.
3.-Select PCG Graph (The Graph must contain the two parameters mentioned before)
4.-Add the Actor in the Actor PCG List and Use it.

B.2.- Spawn Chests
This feature is a Random Actor spawner in the center of random rooms. The feature selects a random asset from the lists and spawns on actor in a random room, every time you click on use button all the chests are deleted and creates new ones.

C.1.- Extra Features
Select Room: Selects directly the parent from one child Static Mesh Actor

Size: Changes the size of the selected parent room
Walls: Spawn or delete one of the room’s door, each bool indicates a door from center wall.

5.	Troubleshooting
   
1.- There are many rooms with no connections
Solution 1: Press Delete No Path Rooms or delete manually.
Solution 2: Move the parent room to center doors, reroute using delete pathways and create pathways, parent rooms can be moved or delete to re spawn pathways.
NOTE: Pathways won’t be created if the doors centers are not aligned, the routes are only generated from room door centers.

2.- Space between Rooms not working properly
Solution1: Delete all pathways only and create new ones, algorithm has automatic adjustment.
Solution 2: Delete all pathways only, select and move the manually.

3.- App Crashed when loading on spawn tab
Solution: Check if there is no breakpoint saved on any plugin widget, plugin won’t start if there a breakpoint saved on any widget or function.

4.-App not loading after click on Edit, Level Generator selection
Solution: The plugin folder must be at content folder, the app won’t load if the plugin is not find on editor startup, erase and load the plugin files again in content folder.
 
