# NihavGameEngine
A cross-platform data driven game engine, written in C++, supporting Desktop, XBOX One and Android. The engine provides a custom scripting language (based on XML) to write games. THe project is being developed as a personal project for the programming track curriculum, FIEA (Spring-Summer 2016).

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

## Upcoming Modules
- **Cross-Platform development for XBOX One and Android
