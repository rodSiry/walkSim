from IO import *
import tensorflow as tf
import numpy as np
io=IO()
print( io.getMsg())
n_input=9
n_hidden_1=16
n_hidden_2=16
n_classes=3
learning_rate=0.001
x=tf.placeholder("float", [None, n_input])
y=tf.placeholder("float", [None, n_classes])
ms=[15., -15., -45., 15., -15., -45.]
def multilayer_perceptron(x, weights, biases):
        layer_1=tf.add(tf.matmul(x, weights['h1']), biases['b1'])
        layer_1=tf.nn.relu(layer_1)
        layer_2=tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
        layer_2=tf.nn.relu(layer_2)
        out_layer=tf.matmul(layer_2, weights['out'])+biases['out']
        return out_layer
weights={
        'h1': tf.Variable(tf.random_normal([n_input, n_hidden_1])),
        'h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
        'out': tf.Variable(tf.random_normal([n_hidden_2, n_classes]))
        }
biases={
        'b1': tf.Variable(tf.random_normal([n_hidden_1])),
        'b2': tf.Variable(tf.random_normal([n_hidden_2])),
        'out': tf.Variable(tf.random_normal([n_classes]))
        }
pred=multilayer_perceptron(x, weights, biases)
cost=tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=y))
optimizer=tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)
init=tf.global_variables_initializer()
io.sendMsg(ms)
with tf.Session() as sess: 
    sess.run(init)
    while 1:
        tf.sess.run(feed_dict={x:test})
        print(io.getMsg())
        io.sendMsg(ms)
io.close() 
