# Game

## Example Code

```cpp
using namespace Minerva;

int main() {
  Engine engine;
  System::Renderer* renderer = (System::Renderer*)engine.AddSystem("renderer", new System::Renderer());

  engine.Initialize();

  unsigned int object = engine();
  engine.AddComponent(object, "transform", new Component::Transform());
  engine.Component<Component::Transform>(object, "transform")->position.x = 3;

  engine.Cycle();
}
