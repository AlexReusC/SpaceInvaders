# Space Invaders
Game of Space Invaders written in C++ using ECS (Entity, Components & Systems) architecture

## Files
- Vec2: Structure for a vector 2D that supports sum and substraction with other vector and scalar multiplication and division via operator overloading
- Components: Structure for transform, shape, collisionm, score, lifespan and input data components
- Entity: Wrapper for the components and additional functions
- EntityManager: Factory that creates, keeps and deletes entity
- Game: Class for the interaction of the entities via systems

![imagen](https://github.com/user-attachments/assets/31861f24-46f3-4557-8fed-3ac5179319ad)

