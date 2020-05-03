GreyandShrink
--------------

Basic information
++++++++++++++++++

Github link: https://github.com/michael2110/GreyandShrink

I am trying to streamline the process of getting images ready to be used for training a computer vision model by grayscaling it and compressing it with as little lost in quality as possible.

Problem to solve
++++++++++++++++

In this new world where machine learning, AI, Deep Learning is the HOT new thing many people are becoming more and more interested to get into this new field. One these fields is computer vision where a computer can be used to detect and identify what it "sees". I am currently one of those getting to know this field and one of the biggest hurdles for me to get used too is how big data sets can be to be used to train computer vision models. As a newcomer the size of one of these datasets and how long it takes to train these models could be enough to scare away new potential engineers.

As an example I am currently a student with a mid range PC and am trying to get into the field of computer vision, I've looked online for tutorials and installed all the programs, API's, set up all the environments, etc. Then when seeing one of the early tutorials it tells me to download a pre-prepared dateset of pictures of lets say cars. One of these datasets is about 50-70 GB's big and being the student I am with a mid range PC I would have limited storage and 50-70 GB's of data might hoard too much space and would leave me with much less space as I had. Also most of these pictures are in RGB while as a newbie to this field my goal is too just make a model that can detect and identify if a picture was a car or not. A car can come in many colors but what identifies something as a car is not the color but its shape so black and white will do just as good as RGB. Leading to our next problem and that is reducing the size of the picture so it takes less data, we are trading quality for less data size but you can still identify that the picture is still a picture of a car. Since its is grayscaled and not RGB we can use a much simpler matrix  formula to calculate and help reduce the size, by representing each pixel as an entry of a matrix and since it is black at white, each entry of the matrix is a value between 0 and 1.

Seeing all this the program should help newcomers prepare a more "suitable" data set that does not compromise quality for size and complexity. If the user is more experienced and wants to develop a more complex model then this is not for them.

System Architecture
+++++++++++++++++++

Input(standalone): Pictures from a dataset

Output(standalone): Greyscaled and reduced sized pictures

Input(integrated): Pictures from a dataset

Output(integrated): processed data in matrices ready to be sent to the model

API description
+++++++++++++++

A user could run the program independently and set the location of where the wanted dataset is and from there the program will fetch all the pictures from it and start to convert them into grayscale and reduce the size of it before spitting out the result of the process as a new image.

If possible I would like to be able to seamlessly integrate this directly into the training model so before the pictures are sent into the model it is greyscaled and reduced in size and the data(already in matrices) can be directly sent to the model.

Engineering infrastructure
++++++++++++++++++++++++++

* Developed on Ubuntu 18.04 LTS

* Uses OpenCV and Pillow

* Uses Python3

* Uses Tensor Flow and PyTorch

* Uses numpy and matplotlib

Schedule
++++++++

5/4-5/11: Create the grayscaling module

5/4-5/11: Create the image compresion module

5/12-5/19: Combining the 2 modules into 1 stand alone program

5/20-5/27: testing and fixing the stand alone version

5/28-6/11: Integrating the modules into machine learning frameworks like Tensor-flow and Pytorch

6/12-6/??: Testing and Improvements

References
++++++++++++++++++++

1. https://pillow.readthedocs.io/en/stable/

2. https://docs.opencv.org/master/d9/df8/tutorial_root.html

3. https://docs.scipy.org/doc/numpy/reference/