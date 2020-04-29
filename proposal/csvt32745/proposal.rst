=============================
Motion Matching in Godot
=============================

Introduction
=================
.. The GitHub repository (URL) hosting your term project.
.. Here before everything, you may write a simple statement (preferably one
.. sentence) to introduce the project.

This project implements the Motion Matching technique as a plugin in `Godot <https://godotengine.org/>`_ engine, 
one of the most famous open source game engines.


Problem to solve
================
.. Describe the problem you want to solve.  Include necessary background
.. information without making it a technical paper.

.. Some points may help you organize the problem description:

.. 1. The field or industry of the problem.
.. 2. The physics and/or the mathematics behind the problem.
.. 3. The algorithm or numerical method that should be applied for solving the
..    problem.

Background
    In interaction animation field like Game, animations of avatars' are often recorded by motion caputre, 
    and animators construct Animation State Machine with them.
    As animations and according states increases, it's nightmare for creators to process them maually.
    Hence game companies such as Ubisoft keep introducing animation processing technique to improve that problem.
    One of the latest and widely used techniques is **Motion Matching**.

Algorithm 
    By applying Motion Matching, the segment of animation with the smallest error 
    against current and predicted animation will be selected as the next animation.
    
    1.  Predict the future trajectory by **extrapolation**.
        Where I may use **Hermite Spline** with taking root positions and velocities as control variables.
    2.  Calculate the cost(error) of candidate animation clips, which may be collected according to labels marked by creator,
        and select the most suitable clip.
    3.  Blend the current and the next animation clip by **LERP** and **SLERP**
    4.  Apply KD-Tree or other optimization method to animation clips if performance needs to be improved.


Perspective users
=================

.. Describe the users of your software and how they will use it.
.. This section may be combined with the previous one (`Problem to solve`_).  It
.. should use a subsection.

- Creators sticking with Godot engine
- Using motion capture animation data
- Tired of setting a complex animation state machine
- Quick prototyping with good animation


System architecture
===================

.. Analyze how your system takes input, produces results, and performs any other
.. operations.

.. Describe the system's work flow.  You may consider to use a flow chart but it
.. is not required.

.. Specify the constraints assume in your system.  Describe how it is modularized.

Initialization
    Unstructured list of raw animation (motion capture animation prefered), 
    which is preferably marked with specific label (like Running, Jumping) in Godot editor by the creator.

Input (Runtime)
    Specific label, Velocities and Positions of the avatar

Output
    The fluid motion of avatar depends on physic parameters and the player's input.


API description
===============

.. Show how a user programmatically uses your system.  You are supposed to
.. implement the system using both C++ and Python.  Describe how a user writes a
.. script in the system.  If you provide API in both C++ and Python, describe
.. both.


I will wrap my C++ module into Godot engine editor, and programmers are still allowed to call the plugin function by themselves.

Compiling
    Though the Godot engine executable is released directly, it needs to be re-compiled when binding other C++ modules.
    And we need **SCons** to build the program according to `official instruction <https://docs.godotengine.org/en/latest/development/compiling/introduction_to_the_buildsystem.html>`__
    The compilation part of this repository will include
    - Source codes of C++ modules
    - Importing-script of engine
    - A SCons scipt 

Usage
    The usage of plugin is the same in C++ or scipting in engine.
    And the following C++ classes are going to be used in in-editor script.

    - `Class Pose`
        - Attributes used to compute the cost.
        - Velocity, position of some bones
        - Specific Labels

    - `Class AnimationMotionMatching`
        - The major working component of motion Matching
        - Attached on a avatar
        - Member
            - `vector<Pose> m_Pose` : Animation database
            - `AnimationPlayer m_AnimPlayer` : Animation player of the avatar
            - `To be defined...`
        - Method
            - `void AnimUpdate()` : Select and update the next animation
            - `float ComputeCost(const Pose& currentPose, const Pose& candidatePose)` : Cost Function
            - `To be defined...`
    


Engineering infrastructure
==========================

.. Describe how you plan to put together the build system, testing framework, and
.. documentation.  Show how you will do version control.

.. You may use continuous integration, but it is not required.  If you use it,
.. describe how it works in your code development.

Architecture
    | I will implement Motion Matching as a C++ plugin in `Godot <https://godotengine.org/>`_ Engine,
    | Though scipts in Godot are written in High-Level language like C# and its own language - gdscript,
     Godot is constructed with C++. Thefore I'm able to work in C++, with basic structures and functions offered by Godot.
    |
    | Also, I will construct the workflow with my C++ plugins in high level scipts, which can be easily used in Godot editor,
     and programmers are able to call the C++ plugins and write their own scipts.

Schedule
========

.. Itemize the work to do and list the work items in a timeline.  Estimate the
.. efforts of each item.

- 05/01 ~ 05/08
    - Basic demo scene with avatar controll and animation
- 05/09 ~ 05/15
    - Basic C++ plugin with skeleton animation controll
    - Label marking plugin for animation in editor
- 05/16 ~ 05/23
    - Motion Matching for basic movement C++ plugin in regardless of labeled animation
- 05/24 ~ 06/16
    - Motion Matching with labeled animation
    - Motion Matching for more actions such as jumping, rolling
- 06/17 ~ Future
    - Other interesting features
    - Integrating other features like Inverse Kinematic
    - Applying optimization method like KD-Tree if necessary


References
==========
.. List the external references for the information provided in the proposal.

- **Godot**
    - `Official Site <https://godotengine.org/>`__
    - `Custom modules in C++ <https://docs.godotengine.org/en/latest/development/cpp/custom_modules_in_cpp.html>`__
    - `C++ bindings for the Godot script API <https://github.com/GodotNativeTools/godot-cpp>`__
    - **API**
        - `Animation <https://docs.godotengine.org/en/latest/classes/class_animation.html?highlight=animation%20node#animation>`__ : Data container for animation
        - `AnimationPlayer <https://docs.godotengine.org/en/latest/classes/class_animationplayer.html?highlight=animation%20node>`__

- **Motion Matching and The Road to Next-Gen Animation**, Simon Clavet, Ubisoft Montreal, GDC2016
    - `Video <https://www.gdcvault.com/play/1023280/Motion-Matching-and-The-Road>`__
    - `Slide <https://twvideo01.ubm-us.net/o1/vault/gdc2016/Presentations/Clavet_Simon_MotionMatching.pdf>`__
    - `Third Review <https://www.gameanim.com/2016/05/03/motion-matching-ubisofts-honor/>`__
- **Animation Bootcamp: Motion Matching: The Future of Games Animation...Today**, Kristjan Zadziuk, Ubisoft Montreal, GDC2016
    - `Video <https://youtu.be/KSTn3ePDt50>`__
    