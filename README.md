<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/othneildrew/Best-README-Template">
    <img src="images/logo.png" alt="Logo" width="200" height="150">
  </a>

  <h3 align="center">Neural Networks</h3>

  <p align="center">
    supervised and unsupervised learning.
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Summary</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#multilayerPerceptron">Multilayer Perceptron</a>
    </li>
    <li><a href="#kmeans">Kmeans</a></li>
    <li><a href="#LVQ">LVQ</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#run">Run</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
In the project we will work on supervised and unsupervised learning. 

**Classification problem**: We will create datasets to solve the problem of classifying four categories using the MLP classifier. We will add noise to the whole set of training. Each set of education categories 1,2 and 3 with a probability of 0.1 will be changed to category 4.

**Clustering problem**: we will create 900 random examples (x1, x2).
150 samples will be in the square: [0.75, 1.25]x[0.75, 1.25], 
150 samples in the square: [0, 0.5]x[0, 0.5],
150 samples in the square: [0, 0.5] x [1.5, 2],
150 samples in the square: [1.5, 2]x[0, 0.5],
150 samples in the square: [1.5, 2] x [1.5, 2]
and the last 150 samples in the square: [0, 2]x[0, 2]

## multilayer Perceptron
A multilayer perceptron (MLP) is a class of feedforward artificial neural network (ANN). The term MLP is used ambiguously, sometimes loosely to any feedforward ANN, sometimes strictly to refer to networks composed of multiple layers of perceptrons (with threshold activation). Multilayer perceptrons are sometimes colloquially referred to as "vanilla" neural networks, especially when they have a single hidden layer.

An MLP consists of at least three layers of nodes: an input layer, a hidden layer and an output layer. Except for the input nodes, each node is a neuron that uses a nonlinear activation function. MLP utilizes a supervised learning technique called backpropagation for training. Its multiple layers and non-linear activation distinguish MLP from a linear perceptron. It can distinguish data that is not linearly separable.

## Kmeans 
It groups the data points based on their similarity or closeness to each other, in simple terms, the algorithm needs to find the data points whose values are similar to each other and therefore these points would then belong to the same cluster.

**Steps: **
1. Choose K value
2. Initialize centroids randomly
3. Calculate Euclidean distance from centroids to each data point and form clusters that are close to centroids
4. Find the centroid of each cluster and update centroids
5. Repeat step 3

Each time clusters are made centroids are updated, the updated centroid is the center of all points which fall in the cluster. This process continues till the centroid no longer changes i.e solution converges. [(source)](https://www.analyticsvidhya.com/blog/2021/02/simple-explanation-to-understand-k-means-clustering/)

## LVQ
Learning Vector Quantization LVQ, different from Vector quantization VQ and Kohonen Self-Organizing Maps KSOM, basically is a competitive network which uses supervised learning. We may define it as a process of classifying the patterns where each output unit represents a class. As it uses supervised learning, the network will be given a set of training patterns with known classification along with an initial distribution of the output class. After completing the training process, LVQ will classify an input vector by assigning it to the same class as that of the output unit. [(source)](https://www.tutorialspoint.com/artificial_neural_network/artificial_neural_network_learning_vector_quantization.htm)
## Run

###### Examples
First of all, we need to generate the data. In the folder *Examples-Generators* run `make all` and then `./generateS1` for the classification problem and `./generateS2` for the clustering.

###### Classification

In the folder Multilayer-Perceptron run `make all` and then `./mlp` 

###### Clustering

In the folders K-means and Learning-Vector-Quantization run `make all` and then `./kmeans` or `./lvq`

## Requirements
* [gcc compiler](https://gcc.gnu.org/)
* [gnuplot](http://www.gnuplot.info/)

<!-- CONTACT -->
## Contact

 Ioannis Chouliaras - [@GiannisChou](https://twitter.com/GiannisChou) - giannis.chouliaras10@gmail.com

Project Link: [Computational Intelligence](https://github.com/GiannisChouliaras/Computational_Intelligence--MYE035)