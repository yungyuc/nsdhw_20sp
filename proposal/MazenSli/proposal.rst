===========================
Proposal for Spline
===========================

Basic information
=================

It will be a branch of https://github.com/PhoenixDL/rising
but can also be an extern standalone Repository, if required.

Extend a data augmentation library for 2D and 3D data written in PyTorch by a package of algorithms for image registration.

Problem to solve
================

The project 'Spline' will be a sub-module of the library Rising (https://github.com/PhoenixDL/rising). Rising is a high-performance data loading and augmentation library for 2D and 3D data completely written in PyTorch. The task is to extend this library by image registration algorithms such as elastic transformation, demons and the corresponding interpolation methods like B-Splines.

Well that's too many information in one sentence, so lets do it step-by-step:

What is PyTorch?
PyTorch open source library deep learning and machine learning using GPUs and CPUs. It is based on Torch, an open-source machine learning library which provides a wide range of algorithms for deep learning.

Why data augmentation?
When you train a your neural network, i.e. tuning its parameters, you need a lot of data. Data augmentation is a common technique to increase the diversity of your data set without collecting new data. It can be achieved by applying transformations to the data that can appear different to the network, but still contain the same meaning.

What is image registration and why do we need that?

Image registration is the process of aligning two or more images of the same scene, i.e. to transform them into the same coordinate system. It is necessary in order to be able to compare or/and to retrieve data from these different measurements. As for medical data for instance, it can be patient measurements from different times or measurement data from different patients (monomodal) or measurement data from different modalities (e.g. CT, MRT, PET) of one patient (multimodal). The process consists of initializing one image as the reference image, also called the fixed image, and applying geometric transformations or local displacements to the other images, also called moving images, to align them with the reference.

From a mathematical point of view, this means that the transformation can be for example rigid, i.e. consisting of translations and rotations or affine (e.g. shears). But for more complex applications, for example organs and tissue that are moving or changing their form or also for patient-to-patient registration, a rigid transformation is inappropriate. In this cases we need a deformable (nonlinear) registration. With an increase of degrees of freedom in the transformation we get closer to the model of the real physical deformation. Examples non-rigid registration are: Spline functions (e.g. B-Splines), elastic registration, fluid registration and Finite Element Models (FEM).

Can we use image registration for data augmentation?

Of course! Let say there is for example we have a limited set of brain-tumor training sets. To extend our data set, we can use affine transformations to produce correlated images and anatomically incorrect examples, or use elastic transformations which can bring noise into the data.

Perspective users
=================

The library Rising has not yet any functions for image regsitration. The task would be a robust implementation of the mentioned algorithms and subsequently the corresponding unittesting. The library consists of the two moduls loading and transforms. The project would be a new sub-modul rising.transforms.registration including all the implemented functions.

System architecture and API description
=======================================

The library Rising can load 2D and 3D data and provides several transform algorithms for for data augmentation purposes. For more information about the API see: https://github.com/PhoenixDL/rising

The sub-modul rising.transforms.registration will consist of several image registration functions (e.g. elastic registration) which take a reference image and the moving images (one or more), aligns the moving images according to the reference and outputs the aligned moving images. It will also include the corresponding unittesting.

Engineering infrastructure
==========================

The project will make use of PyTorch and several other Python libraries such as SimpleITK, natari, numpy and matplotlib.
An important criteria would be the implementation of fast and robust algorithms, but also the complete corresponding unittesting.

Schedule
========

5/10-5/14: Research and learning the ropes of the library.

5/15-5/25: Implementation of algorithms

5/26-6/03: Improving and integration of algorithms

6/05-??: Unittesting and optimizing the algorithms


References
==========

https://depts.washington.edu/bicg/documents/MII_registration10.pdf

https://medium.com/sicara/image-registration-sift-deep-learning-3c794d794b7a

https://www.mathworks.com/discovery/image-registration.html

https://github.com/PhoenixDL/rising

