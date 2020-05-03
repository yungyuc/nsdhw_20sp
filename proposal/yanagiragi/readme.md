# Proposal for TinySandbox

## Basic information

---

Github Repo: [Tiny Sandbox](https://github.com/yanagiragi/TinySandbox)

I will try to build a simple game engine from scratch. The simple engine is proposed to be a sandbox for learning and trying new techniques.


## Problem to solve

---

Quick prototyping is always a pain in the ass. 
Most of the time, I kept coding (or copy-n-paste) code blocks serving the same functionality whenever I start a new project, or often get stuck with some pasted legacy code that I haven't consider general cases when I wrote it down.
Yes, I am unprofessional with these kinds of things, laziness always beat me when I design my system architecture.
But this must end for now.
The major mission of this project is to build an easy-used and flexible general-purpose game engine.
The User, or developers, should able to quick testing their feature codes without knowing the details of the engine but also allowed to touch the low-level details.
Indeed, Implement something on the commercial game engine is fast to evaluate.
However, most of the time you use the provided API and making your hand dirty is an option, not a requirement.
In a short, this project is not only meant to solve quick prototyping, but also enforce the author, which is me, to get my hand dirty and learn how to organize my code well.

Describe as above, the system consist of several sub-problem to be solved:

1. The System must designed to be easy for users to use, so providing a GUI interface might be necessary.

2. The system need to provide several essential functionalities, such as loading and manage assets, manage the scene list, etc.

3. The shading must be take care of the engine by default. And also provide necessary information for users to write their own shader code.

4. A complete game engine usual contains several optional subsystems, such as sound system and physic system. The scope will become to big to implement those systems, so to integrate with my own system is the main challenge here.

There are many mathematic problems behind to problem, To name a few, there are:

1. If the system allow users to pick the object via mouse, the picked object is determined by casting a ray from the mouse point. To be specific, the ray fire from the viewport to the mouse point lying on the image plane of the scene view. The ray intersection test of the process is often accelerate with several techniques, including replacing testing polygon with simple primitives, limit search space by culling unnecessary tests, etc.

2. The default shading of the system, should be general as possible. The shading general tries to solve the rendering equation. The description below is from [wikipedia](https://en.wikipedia.org/wiki/Rendering_equation).

![](https://raw.githubusercontent.com/yanagiragi/nsdhw_20sp/proposal/proposal/yanagiragi/renderEquation.PNG)

Through the equation may look complicated, for real-time rendering the recursion part is often omitted and set to a constant.
The main problem is to solve bidirectional reflectance distribution function(BRDF) and visibility analytically. 
For the BRDF, we use the Cook-Torrance BRDF and adopt slightly modification from Unreal engine, which itself use numerical methods to fit the reflectance measure from a goniophotometer. 
For the visibility part, the visibility is calculated in the local coordinate of the light we've testing. 
In the local coordinate, since the depth is aligned with z axis, determine the occulusion is reduced into comparsions of the z values.
The techniques is so called shadow mapping.
Therefore, with the omit of the recursion, the integration can be solved.

## Perspective users

* Game developers to testing new features

* Graphic Designer to design new VFX

## System architecture

(Initializing) Input: a scene description file

Input: Keyboard / Mouse Input

Output: an interactive interface allow user to manipulate the scene and render the display image on the viewport

The system architecture design is adopted the architecture of Unity. All instances that is contained in a scene is a ```GameObject```, each ```GameObject``` consists of several ```Components```. The Components can be a simple script, or any module. For example, a GameObject to be display on the screen may happen to be contained a component of ```Renderer```, which renders the object.

Each ```Components``` consist of ```Update``` method, which is called every frame. In a word, the system can be described in the pseudo code:

```language=python
# Update is called in every frame, the binding of calling Update() every frame is setup in the first frame
def Update():
   for gameObject in SceneList:
      for component in gameObject:
         component.Update()
   Screen.Flush() // the frame has been rendered, and all gameObject is up to date now
```

## API description

* User can add a script inherit class ```Components```, which contains several functions to be override:

   * Start(): Called in the first frame

   * Update(): Called every frame

   * FixedUpdate(): Called in a fixed time step, e.g. allows user to calculate physics in a fixed time step

   and contains several variables:

   * gameObject: the game object which the script attached on 

   * name: name of the instance from the script

* For Each GameObject, several variables is provided:

   * transform: local position, rotation and scale of the game object

   * name: the name of the game object

   * componentList: list of components attached on it

   * parent: parent of the game object, can be traced to the root of the scene list

   * children: children of the game object

* Utils:

   * Find(string name, GameObject searchObject=NULL): find a game object below the ```searchObject```, if the ```searchObject``` is null, search in the scene list

   * GetComponent(type classType, GameObject searchObject): get the first component that is the type ```classType``` in the GameObject, if not found, trying searching in the children of the ```searchObject```

   * Load(string path, type assetType): Load an external resource into an internal resource (may be reference by a GameObject) as ```assetType```

   * Instantiate(GameObject gameObject, GameObject parent): add ```gameObject``` into the scene list as a child of the ```parent```

   * GetTemporarilyRenderTexture(vec2 resolution, int type): return a render texture for ```Bilt()```

   * Blit(RenderBuffer RT1, RenderBuffer RT2, void* renderFunctionPoint=NULL): Render RT1 to RT2 with override render function ```renderFunctionPoint```. If ```renderFunctionPoint``` is null, fallback to default render function

   * CreateNewMaterial(string vertexShaderPath, string geometryShaderPath, string fragmentShaderPath): create new material using custom shaders. To use custom material to render a object, replace the material in the ```Renderer``` component in the target gameObject.

## Engineering infrastructure

The project is developed with VS2017 for now.
The testing framework will be sample scenes and test the render image differences between the reference image.
The project will be using git as version control and will be avoid using large files as internal resources to avoid using git LFS.

## Schedule

---

* 05/07: Implementing basic structure, define basic data structure
* 05/15: Implementing model loading & lambertian material
* 05/24: Implementing Cook-Torrance Material & Add Skybox
* 05/25: Implementing ```Renderer``` Component and ```Transform``` Component, and add controls to GUI
* 06/07: Implementing Shadow mapping
* 06/14: Implementing ```Bilt```, ```GetTemporarilyRenderTexture``` and some post-processing effects
* 06/22: Integrate with Irrklang Sound Engine and Bullet Physics Engine

## References

* [Learn OpenGL Tutorials - Phyisics Based Rendering](https://learnopengl.com/PBR/Theory)

* [Bullet Real-Time Physics Simulation](https://pybullet.org/wordpress/)

* [irrKlang high level audio engine](https://www.ambiera.com/irrklang/)
