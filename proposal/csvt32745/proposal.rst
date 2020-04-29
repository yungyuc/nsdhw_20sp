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
    2.  Calculate the cost(error) of the candidate animation, which may be collected according to labels marked by creator.
    3.  Blend the current and the next animation by **LERP** and **SLERP**


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
    Unstructured list of raw animation (motion capture aniamtion prefered), 
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


I will wrap my C++ plugin into Godot engine editor, and programmers are still allowed to call the plugin function by themselves.

The usage of plugin is the same in C++.





Engineering infrastructure
==========================

.. Describe how you plan to put together the build system, testing framework, and
.. documentation.  Show how you will do version control.

.. You may use continuous integration, but it is not required.  If you use it,
.. describe how it works in your code development.

Architecture
    | I will implement Motion Matching as a C++ plugin in `Godot <https://godotengine.org/>`_ Engine,
    | Though scipts in Godot are written High-Level language like C# and its own language - gdscript,
     Godot is constructed with C++.
    | Thefore I'm able to work in C++, with basic structures and functions offered by Godot.
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
- 05/16 ~ 05/31
    - Motion Matching for basic movement C++ plugin in regardless of labeled animation
    - Label marking plugin for animation in editor
- 06/01 ~ 
    1. Motion Matching with labeled animation
    2. Motion Matching for more actions such as jumping, rolling
    3. Optimization like KD-Tree if necessary


References
==========

.. List the external references for the information provided in the proposal.
