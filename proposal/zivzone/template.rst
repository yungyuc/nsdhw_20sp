===========================
Proposal for CrudeNN
===========================

This is a template for a proposal that specifies what your project is and the
execution plan.  You are encouraged to use `reStructuredText
<https://docutils.sourceforge.io/rst.html>`__ (which is employed by this
template).  The plain-text format makes it easier for the instructor to comment
in your GitHub pull requests.  You may use another document format if it is
more suitable to your project.

Basic information
=================

The GitHub repository (https://github.com/zivzone/CrudeNN) hosting your term project.

Here before everything, you may write a simple statement (preferably one
sentence) to introduce the project.

Problem to solve
================
In this project, I want to build my own deep learning library which is called CrudeNN. 
Nowadays, we have lots of powerful tool to build deep learning architecture, such as Pytorch and Tensorflow.
But mostly we only can build our network steps by steps following the design we write on the paper, which means we hardly can do analize the performance part by part in our network to make some improvement.
Foe example, there are two common layers in the network. One is pooling and the other is activation function.
If we change the order of these two layer, the result will be the same but the computation speed will increase.   
Unfortunately most popular library cannot do this or lacks of freedom to adjust, which means it's hard for me to do the related research by using those common library. 
So that why I need to build my own library which aims to customize everything and is convineint for me.
I will start to build the library which can let me build the common DL network first, then I will add the powerful functions like optimizing the architecture if I have enough time.


Perspective users
=================
People who is working on deep learning can use my library to build thier network easily and use the build-in function to analize the bottleneck of computation and optimize the structure of the network.


System architecture
===================
The whole library contains different functions in several categories listed below:
- Activate Fuction
	- These functions provide the common activate functions for build the network, like Relu and Sigmoid
- Layers
	- These functions are used to build the layer in the network, like fully connected layer.  
- Loss Fuction
	- We define several common loss function here for evaluate the accuracy of the network, like MSE and L1 Loss.
- Optimization
	- We provide several common optimizer for training the network, like SGD.
- Method
	- The methods contain the function you can load dataset and autograd for calculate gradient. 
- Data processing
	- For data preprocessing
- Deploy
	- For deploy the network on different platform.
- Format/Storage/Data Utilize
	- Data format transform.
- Model
	- Several built-in networks for testing.
- Case/Application
	- Some application for example.
- Visualize
	- Visualize the result of training and testing.
- Analyze
	- Analyze the network like running time, space usage, and flpos counting.
- Optimize
	- Provide several method for optimize the model.
	
For the user, you may need to define and build the network by using **Layers** and **Activate Fuction**, then loading the dataset and do the data preprocessing by **Method** and **Data processing**.
Then define your loss function by **Loss Fuction** and **Optimization** for training your network.
Final, use **Visualize** visualize your training result.
Additionaly, you can use **Analyze** for analizing your network and use **Optimize** for optimizing your network.


API description
===============

Show how a user programmatically uses your system.  You are supposed to
implement the system using both C++ and Python.  Describe how a user writes a
script in the system.  If you provide API in both C++ and Python, describe
both.

For example, if the user want to build a network (a simple CNN model for classification), they can take the code below as reference:
```
#Import function
from Layers.Convolution2d import Conv2d

# define covolution layer
# Conv2d(
	input_channel_number,  # the channel number of input
	output_channel_number, # the channel number of output
	kernel_size,           # the size of kernel
	stride,                # the size of stride
	padding,               # the size of padding
	bias                   # True or False for bias
		)
layer = Conv2d(3,16,kernel_size = (5,5), stride = (1,1), padding = (2,2),bias = True)
```

```
#Import the function for loading data
from Method.Data import Load_file
# Load_file('dataset_name','setting')
# Return the dataset
Load_file('cifar10', 'train')

```

Engineering infrastructure
==========================
Ubuntu 18.04 and Python 3.6 for development and testing.
When I start to develp one function, i will open a new branch. And I will merge it with master after testing.


Schedule
========
5/7 finish Activate Fuction & Method
5/14 finish Loss Fuction & Data processing
5/21 finish Layers
5/28 finish Optimization
6/3 finish Model & Case/Application
6/10 finish Visualize
6/17 finish Analyze
6/24 finish Deploy
6/31 finish Optimize

Itemize the work to do and list the work items in a timeline.  Estimate the
efforts of each item.

References
==========

List the external references for the information provided in the proposal.
