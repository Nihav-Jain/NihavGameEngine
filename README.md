# Cross-Platform Data Driven Game Engine
A cross-platform data driven game engine, written in C++, supporting **Desktop and XBOX One*. The engine provides a **custom scripting language** (based on XML) to write games. The project is being developed as a personal project for the programming track curriculum, FIEA (Spring-Summer 2016).

### Geometry Wars
Check out [GeometryWars](https://github.com/Nihav-Jain/GeometryWars), developed for the PC using this game engine and its scripting language.

## Modules

- **Templated Data Structures**
 - Singly Linked List
 - Vector
 - Stack
 - HashMap
 - Graph (with a Depth-first, Breadth-first and a Simple Traversor)
- **Datum** - a run-time dynamic data-type storage (currently supports int, float, string, vector4, matrix4x4, Scope (Table) and RTTI (base class of all foundation classes in the Engine))
- **Scope** - forms a dynamic heirarchical database for name-value pairs
- **Attributed** - base class for "glue" code which links Scopes to native class/object and provides utility facilities such as prescribing the contents of a Scope table at compile time
- **XmlParseMaster** - XML parser which follows the Chain Of Responsibility pattern to read the XML script and pass it on to relevant helpers for parsing
- **IXmlParseHelper** - abstract class which forms the interface for helpers of XmlParseMaster, specializations include helpers to parse the XML and create Scopes containing integers, floats, strings, vectors, matrices and nested Scopes
- **Factory** - a templated object Factory
- **World, Sector, Entity** - forms the Game heirarchy where Sectors can be loosely considered as "levels/maps" and Entities can be equivalent to Actors / GameObjects
- **Action** - a loose equivalent of a "Game Component", can be used to execute specialized functions on Entities. Specialized Actions currently implemented include a switch-case block and an expression which can be used to write simple arithmetic evaluation expressions for the scripting language 
- **Event** - implements the observer pattern for publisher-subscriber communication and implements an asynchronous, time-based dispath queue
- **Reaction** - scriptable event publishers and subscribers

## New Modules
- **Cross-Platform development for Desktop and XBOX One**

## FUTURE PLAN
The cross-platform development project turned out to be disaster mainly because of my lack of experience in writing multi-threaded code and little or no documentation on building UWP apps using C++. I will revert back to the original Desktop-only engine and resume my work from there on the following:
- Improve the engine architecture and provide more flexibility for scripting
  - Engine Module system (like I implemented for the cross-platform version)
  - Ability to create functions in the scripting language
  - Ability to write actions and entities purely in the scripting language (implies implementing inheritance in the language)
  - Performance improvements
- Integrate Box2D as a physics engine
- Refactor the rendering engine
- Add keyboard and mouse support for input
- Config file and logger support
- Experiment with extending the Visual Studio debugger to help debug the XML based scipting language
- UE4 Blueprint like tool for writing the gameplay scripts
- Level editor
- UI kit
- Support multiplayer networked games
- Gameplay and rendering on separate threads
- Come back to support Xbox One
- Try support for Android
- Transition to 3D graphics and physics

Of course I might not be able to complete many of these but I'll keep working on the engine in my spare time just for the experience.
