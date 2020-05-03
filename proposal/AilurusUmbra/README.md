# Proposal for EEGNN


## Basic information

![](https://github.com/AilurusUmbra/Archived/raw/master/GitHub-Mark-32px.png): https://github.com/AilurusUmbra/EEGNN

A neural network(NN)-based toolbox for biosignals like EEG/MEG.

## Problem to solve

### Introduction to BCI

Brain Computer Interface (BCI) aimed at building some end-to-end systems that could directly control devices by our thoughts. BCI systems must be computationally-efficient and portable so that users could  move in relax while they are wearing BCI systems [Figure 2]. In recent years, most of BCI researchers use biosignals like Electroencephalography (EEG) or Magnetoencephalography (MEG) as the representation of brain activities. These kinds of biosignal analysis could often be formulated into Machine Learning problem, for example, classification or regression. It is important to develop useful tools to process biosignals, while famous EEG tools like EEGLAB or BCILAB are only for MATLAB. Most of Python EEG analysist using  mne [3, 5, Figure 1] to analyze EEG, but mne only contains some classical models like FBCSP [6] and lacks of neural network-based api. In [7], we know that neural network-based models could achieve better performance than classical models, and there are more and more researches are developing neural network-based models [7], [8], [9].

We need a toolbox integrating SOTA EEG machine learning models in Python. 

![Ref](https://sccn.ucsd.edu/mediawiki/images/thumb/2/2e/EEGLAB_usage.jpg/600px-EEGLAB_usage.jpg) Figure 1: Ref [5]

![](https://i.imgur.com/tMkSeTL.png) Figure 2: Ref [10]




This project would focus on 3 parts.

1. Provide pretrained weights and api of SOTA models for EEG (e.g. EEGNet) and assure compatible with mne.
2. Improve SOTA models by integrating some tricks from other famous neural networks models (e.g. SENet)
3. Guarantee efficiency at model inference due to portability requirements of BCI.

### Mathematics Background

For guarantee convergence of basic neural network training, initialization, normalization, optimzation should be concerned.

Neural netwoks for BCI should be light-weighted enough, so small scale NN (e.g. EEGNet) is much more focused in this project.
Most of neural networks contains convolution filters, including EEGNet. 
Convolution filters could be considered as a high dimensional tensor, so that tensor decomposition (e.g. tucker, CP decomposition) could be applied on each convolution filter to get an approximation of smaller tensors 

* Initialization and Normalization [11, 12, 13, 14]

   Training deep neural networks could be hard when it get deeper, which called degradation problem. This is solved by adding shortcut connection and batch normaliztion in the design of design ResNet [14]. Some works [12, 13] have succeeded in training very deep networks without batch normalization, while these papers required othogonal initialization. Different initialization or normalization tricks are all trying to make some hypotheses on data or dynamics weights, which are based on some probabily distribution or statistics independent.  A few weeks ago, DeepMind proposed a new technique called SkipInit [11] to replace different kinds of initialization or normalization, and they also gave an explanation of how normalization works. As [11] mentioned, although BatchNorm is very widely used in nowadays neural network design, this would break statistical independent between each batch. This project would try to implement SkipInit to replace BatchNorm in EEGNet.

* Optimization 
  
   In recent years, there are tons of optimizer proposed for different kind of NNs. However, it is not too sensitive to choose fancy optimizer for training light-weighted NN due to the short backpropagation path, so this project does not pay too much effort here.

* Depthwise Separable Convolution [15, 16]

   From aspects of Linear Algebra, we could do tensor decomposition on each of convolution filter to get an approximation of smaller filters. Depthwise Separable Convolution [15] is to use 2 smaller layers to approximate a larger convolution layer [Figure 3.]. This tricks are widely used in small scale CNN.
![](https://miro.medium.com/max/1200/1*e_oU-f6hX4FSSD-1OukP6Q.png) Figure 3. [16]

* Convolution filter size [17]
  
  More and more papers of Neural Architecture Search show that 5x5 convolution filter might give us better result than 3x3 filters [17]. This project would compare results of both 5x5 and 3x3 convolution filters.

### Numerical Method to be Applied

* Initialization and Normalization
  * BatchNorm with random initialization
  * SkipInit
  
* Optimization 
  * SGD
  * Adagrad
  * Adam

* Convolution Filter
  * 5x5 window 
  * 3x3 window
  * 5x5 depthwise-separable
  * 3x3 depthwise separable
 

## Perspective users

* BCI researchers for processing EEG/MEG.
* Engineers that need to do multichannel time series prediction/classification.

## System architecture

Most of the computation would be leverage to Pytorch framework, so that tensors could be easily move to VRAM of GPU to compute, and move back to RAM after computation.
Models would takes `torch.tensor` as data strcuture to compute, this could be preprocessed after load data from mne. `torch.tensor` is easy to transformed into `numpy.array` which is commonly used in mne.
This project might not implement tensorflow version of codes, because there is a similar developing frameworks called [mneflow]() [4]


## API description

* Models: Refer to [torchvision.models](https://pytorch.org/docs/stable/torchvision/models.html)

  * eegnn.models.eegnet(pretrained=True, denoise=True, device='cuda', sample_rate=4, optimizer=sgd())
  * eegnn.models.sccnet(pretrained=True, device='cpu', nthreads=1)
  * eegnn.models.moblie_eegnet(pretrained=False, device='cuda', sample_rate=4,  optimizer=adam())

  * SOTA Normalization techniques which could replace BatchNorm [11]
    * models.skipinit()
    
* Load/Save model weights:
  * load_weight(path)
  * save_weight(path)
  
  
* Utils: Some supplementary functions could just use api of [mne](https://mne.tools/stable/index.html) to prevent from reinventing the wheel. For example, read FIF data (MEG) or load sample dataset.

  * mne.io.read_raw_fif(fname[, allow_maxshield, …])            Reader function for Raw FIF data.
  * mne.datasets.eegbci.load_data(subject, runs[, path, …])     Get paths to local EEGBCI dataset files.

## Engineering infrastructure

* Hardware Information
For simulating mobile devices with low computing power, we rent 2 core virtual machine on [TWCC](https://www.twcc.ai/) to develop this project.

* Software Environment
This project would be mainly developed by C++11, python>=3.6 and based on Pytorch>=1.0 on Ubuntu 18.04 and tested on Linux(Deb), Mac OS and Win10 for compatibility. For each testing environment, we would provide the dockerfile to build and test this project. [Linux environment](https://github.com/AilurusUmbra/EEGNN/blob/master/Dockerfile) would be almost same with NSD AMI on AWS. 

* Testing
  * Testing modules would be written in Pytest to validate the results of model inference.
  * This project would provide api for saving and loading model weights, which should be compatible with `torch.nn.module`. That is, EEGNN should load weights generated by PyTorch, and should generate weights that PyTorch model could load.

* Documentation
  We use [Sphinx](https://www.sphinx-doc.org/en/master/) to write documentation. APIs in detail and bugs fixed would be recorded. 



## Schedule


* 05/01 - 05/14: Implementing EEGNet, SCCNet and apply hyperparamter tuning to provide pretrained weights.
* 05/15 - 05/29: Add Inverted residuals and SE Block into EEGNet and SCCNet.
* 05/30 - 06/11: Build benchmark and add parallelism mechanism on both GPU and CPU computation.
* 06/12 -      : Build documents.

## References

* [1] [EEGLAB](https://sccn.ucsd.edu/eeglab/index.php)
* [2] [BCILAB](https://sccn.ucsd.edu/wiki/BCILAB)
* [3] [mne](https://mne.tools/stable/index.html)
* [4] [mneflow](https://github.com/zubara/mneflow)
* [5] [cf. EEGLAB & Python](https://sccn.ucsd.edu/wiki/EEGLAB_and_python)

* [6] [Filter Bank Common Spatial Pattern (FBCSP) in Brain-Computer Interface](https://ieeexplore.ieee.org/document/4634130)
* [7] [EEGNet: A Compact Convolutional Network for EEG-based Brain-Computer Interfaces](https://arxiv.org/abs/1611.08024)
* [8] [Spatial Component-wise Convolutional Network (SCCNet) for Motor-Imagery EEG Classification](https://ieeexplore.ieee.org/document/8716937)
* [9] [SleepEEGNet: Automated sleep stage scoring with sequence to sequence deep learning approach](https://arxiv.org/abs/1903.02108)
* [10] [Sensorimotor brain dynamics reflect architectural affordances](https://www.pnas.org/content/pnas/116/29/14769.full.pdf)
* [11] [Batch Normalization Biases Deep Residual Networks Towards Shallow Paths](https://arxiv.org/pdf/2002.10444.pdf)
* [12] [Exact solutions to the nonlinear dynamics of learning in deep linear neural networks](https://arxiv.org/abs/1312.6120)
* [13] [Dynamical isometry and a mean field theory of cnns: How to train 10,000-layer vanilla convolutional neural networks](https://arxiv.org/abs/1806.05393)
* [14] [Deep residual learning for image recognition. In Proceedings of the IEEE conference on computer vision and pattern recognition]()
* [15] [Xception: Deep Learning with Depthwise Separable Convolutions](https://arxiv.org/abs/1610.02357)
* [16] [深度學習-MobileNet (Depthwise separable convolution)](https://medium.com/@chih.sheng.huang821/%E6%B7%B1%E5%BA%A6%E5%AD%B8%E7%BF%92-mobilenet-depthwise-separable-convolution-f1ed016b3467)
* [17] [MnasNet: Platform-Aware Neural Architecture Search for Mobile](https://arxiv.org/pdf/1807.11626.pdf)
