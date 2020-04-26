===========================
Proposal for [Automatic Tag Generation for Text Article]
===========================

Basic information
=================

`The GitHub repository <https://github.com/WarClans612/Automatic-Tag-Generation-for-Text-Article>`__

Text content is unstructured and complex information that is challenging to search and index,
therefore, by using tag information accompanied with the text content, we could extract main idea
and topic within the unstructured information.

Problem to solve
================

Industry Problem
-----------------

Nowadays, thanks to the development of social media and internet, more information can be easily
accessible from anywhere. However, more problem arises as information flow faster, which overwhelm
manual labor for many tasks. Most of the information in the web is in the shape if text content that
by itself is hard to be digested by computer due to misaligned language used.

Social media, content provider, modern encyclopedia (e.g. Wikipedia) consists of many content which 
can be grouped into bags of topic. Using manual labor to separate and tag each piece of pages usually 
yield the best results in correctness and precision of the tagged results. This approach cannot keep 
up with the growth pace of the internet itself, so we would like to make an automatic approach for
this problem.

Tagging generation can be described simpler as structured information extraction from unstructured
information (such as text) to enable easier digestion of information and analysis.

Math Behind the Problem
-----------------

As we know that currently there is abundance of data in the internet which is good news for
Machine Learning approach in computer science. Machine Learning approach requires enormous amount 
of data to be fed into the system to let the system *learn* the correct approximation for the
wanted results.

Deep Learning is one of the branch in Machine Learning Approach which provides good results.
It uses artificial neural network to do information processing and distributed communication
which were inspired by biological systems. 

In this project, we will use Deep Learning Approach that is usually powered by Back Propagation
Algorithm (solved numerically) to *learn* the best function to approximate the results.

Numerical Method to be Applied
-----------------

The main idea is to use past text and tag pairs to learn the intrinsic information and 
perform tag generation for future text content. If possible, the newly generated text will
then be supervised by human to ensure its validity for further model training.

BackPropagation is the backbone for how the Deep Learning Approach could *learn* from 
existing information. In fitting the neural network, BackPropagation will compute gradient of
loss function with respect to the weights of the network for every input output pairs.

By numerically optimizing the Neural Network (can be think as complex mathematical functions),
the model could learn to minimize error towards the real expected output.

Algorithm to be Used
-----------------
Extreme Deep Learning Classification refers to the problem of assigning to each document
its most relevant subset of class labels from an extremely large label collection, where
the number of labels could reach hundreds of thousands or millions. Multi-label classification
is fundamentally different from the traditional binary or multi-class classification problems
which have been intensively studied in the machine learning literature.

Due to huge size of Deep Learning Approach, this method is usually hard to be implemented
for mobile device, therefore, it will enable the user to choose if the implemented model 
will be used for low powered device. The change will be small, which is changing
Convolutional Layer into Depthwise Separable Convolution with the trade of lesser computation
need with a bit lower accuracy for the results.

The paper, MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications,
discuss about changing the convolutional layer into Deptwise Separable Convolutional for better
performance in mobile device.

Perspective users
=================

- Content Provider to generate tag for each content for easier recommendation
- Social Media to generate tag to know the topic for each post (possibly prevent malicious usage or even for related content recommendation)
- Forum Admin to generate tag for each content in its responsibility
- Medical Worker to generate tag (e.g. possible diseases) from patient written record

System architecture
===================

Input (when initializing):

- Data containing past input and generated tag
- All possible list of generated tag
- If the model will be run on mobile device, we could improvise to shrink the model

Output (when initialization):

- Model that is ready to be used for generating tag

Input (for usage after initialization):

- Text content that needed tag generation

Output (for usage after initialization):

- List of possible tags

Modularization
-----------------

- User API (only this layer is supposed to be accessed by user)
- Normal model definition Module
- Mobile enabled model definition Module
- Training Module
- Validation Module
- Model saving module
- Data Management Module
- Config Management Module
- Unittest Module
- Example Module

API description
===============

User will interact with the system using Python only.
Using user provded data, the system will automatically take the provided data as
training and let the model *learn*. After finishing the initialization, the model
is then ready to be used.

Possible API function:

- generate_tag(string) -> return list_of_tag
- use_mobile(True) -> enable smaller model for low powered device
- possible_tag(list_of_string) -> user providing model with the possible output
- training_data(pair_of_input_and_expected_output) -> user providing model with past knowledge

Engineering infrastructure
==========================

By implementing the model from the paper, we would first define the model using PyTorch.
Then building the training iteration and testing iteration modeule.
Data Management Module will be implemented using Torch DataLoader.
Testing each training iteration and results after training using PyTest.
Each module will be provided with README.md file to describe each module purposes.
Each function will be decorated with multi-line comment consisting of expected input,
expected output, and possible requirements needed before using the function.

Each new feature will be developed in the new branch.
Ground Breaking and Critical Bug will have its own branch and being commited directly
into newer version.
Master branch version <1.0 will be initial implementation with no guarantee of it
working at all.

Schedule
========

- 05/01 - 05/07 -> Implementing Model
- 05/08 - 05/15 -> Implementing Data Management Module
- 05/16 - 05/24 -> Implementing Training Module
- 05/25 - 05/30 -> Implementing Testing Module
- 06/01 - 06/07 -> Implementing Unittesting
- 06/08 - 06/14 -> Implementing User Level API
- 06/15 - 06/22 -> Squashing Bug and Refactoring

References
==========

- Deep Learning for Extreme Multi-label Text Classification - SIGIR 2017
- MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications - CVPR 2017
