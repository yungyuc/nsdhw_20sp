===========================================
Proposal for Compression/Decompression Tool
===========================================


Basic information
=================

GitHub repository : `https://github.com/sgaumi/compression_decompression_tool <https://github.com/sgaumi/compression_decompression_tool>`__ 

The purpose of this project is to make a software to compress and decompress files.

Problem to solve
================

It’s sometimes interesting or even necessary to compress files to free some places on memory computer (or any storage device) or to send these files through a network. It’s also necessary to be able to decompress these files to get back the information.
To perform file compression it’s necessary to encode the data in a different way so that it takes less places.
To solve this problem the software will only use a Huffman coding in a first version.
In a second version (If the first version is not too long to implement) the software will use both a Huffman coding and LZ77. The combination of these two algorithm is known as deflate which is the algorithm used by gzip (the GNU compression software).

Perspective users
=================

In a first version, the user will be able to use the software through a Bash Shell with typing the software command and giving the file name (the path) and the options, which allow the user to choose the compression or decompression mode, the algorithm chosen (Huffman static, Huffman adaptive or deflate in a second version).
In a second version, the user will be able to use software by executing an application and give the file name (path) and options through a graphic user interface. 

System architecture
===================

For compression, the system takes the path of a file as input and opens the file in reading mode. It creates on other file where the compression will be performed. At the end the system returns the file created and close the original file (it’s not deleted). 
For decompression, the system takes the path of a compressed file (it will work only with a file compressed by this software, it is not designed to support files compressed by other software) as input and opens the file in reading mode. It creates on other file where the decompression will be performed. At the end the system returns the file created and close the original compressed file (it’s not deleted). 

API description
===============

I have no idea what I will provide for the API for the moment, it will depend how I choose to implement the system. 
Maybe for the Huffman tree in C++, it would be created as an object with some features and would be used by a user. 
Maybe for the compression / decompression in python, it would be possible to override the method chosen for compression.

Engineering infrastructure
==========================

Everything will be put on GitHub repository and version control will be performed by git commits and creating branch. 
While a basic version is not operational, everything will be performed on the master branch (except testing). When an operational version is up, each further version will be built on an other branch and be merged when it’s up.
All the test will be performed on an other branch (testing).
The documentation and build files will be on the same branch as the code (master) but on different folders.    

Schedule
========

Work to do :

* Open/Close/Create/Write file
* Huffman tree construction/features
* Compression/Decompression (Huffman static)
* Compression/Decompression (Huffman adaptive)
* Makefile
* Test
* Documentation
* Compression/Decompression (deflate)?
* Graphic User Interface?

Planning:
(It will depend a lot of the work needed in my other courses, so I can’t provide a specific planning for the moment). 

References
==========

- Huffman Coding - `Wikipedia <https://en.wikipedia.org/wiki/Huffman_coding>`__
- GZIP - `Wikipedia <https://en.wikipedia.org/wiki/Gzip>`__ 
