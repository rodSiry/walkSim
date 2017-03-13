from IO import *
import tensorflow as tf
import numpy as np
import numpy.linalg as linalg
io=IO()
print(io.getMsg())
n_input=10
n_hidden_1=128
n_hidden_2=128
n_classes=3
learning_rate=0.001
x=tf.placeholder("float", [None, n_input])
y=tf.placeholder("float", [None, n_classes])
s=[15., -15., -45., 15., -15., -45.]
def multilayer_perceptron(x, weights, biases): 
    layer_1=tf.add(tf.matmul(x, weights['h1']), biases['b1'])
    layer_1=tf.nn.tanh(layer_1)
    layer_2=tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
    layer_2=tf.nn.tanh(layer_2)
    layer_3=tf.add(tf.matmul(layer_2, weights['h3']), biases['b3'])
    layer_3=tf.nn.tanh(layer_3)
    layer_4=tf.add(tf.matmul(layer_3, weights['h4']), biases['b4'])
    layer_4=tf.nn.tanh(layer_4)
    layer_5=tf.add(tf.matmul(layer_4, weights['h5']), biases['b5'])
    layer_5=tf.nn.tanh(layer_5)
    layer_6=tf.add(tf.matmul(layer_5, weights['h6']), biases['b6'])
    layer_6=tf.nn.tanh(layer_6)
    out_layer=tf.matmul(layer_6, weights['out'])+biases['out']
    return out_layer
weights={
        'h1': tf.Variable(tf.random_normal([n_input, n_hidden_1])),
        'h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
        'h3': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_2])),
        'h4': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_2])),
        'h5': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_2])),
        'h6': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_2])),
        'out': tf.Variable(tf.random_normal([n_hidden_2, n_classes]))
        }
biases={
        'b1': tf.Variable(tf.random_normal([n_hidden_1])),
        'b2': tf.Variable(tf.random_normal([n_hidden_2])),
        'b3': tf.Variable(tf.random_normal([n_hidden_2])),
        'b4': tf.Variable(tf.random_normal([n_hidden_2])),
        'b5': tf.Variable(tf.random_normal([n_hidden_2])),
        'b6': tf.Variable(tf.random_normal([n_hidden_2])),
        'out': tf.Variable(tf.random_normal([n_classes]))
        }

def create_input(s, f, i):
    serv=np.array([[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1]])
    return np.array([np.concatenate((f, serv[i],s))])
def state_pass(s, f):
    aQ=np.zeros(6)
    nQ=np.zeros(6)
    Q=np.zeros((6, 3))
    for i in range (0, 6):
            test=create_input(s, f, i)
            o=sess.run([pred], feed_dict={x:test})
            Q[i]=np.array(o)
            aQ[i]=np.argmax(o)
            nQ[i]=np.amax(o)
    return Q, nQ, aQ
def update_state(s, a):
    n=np.zeros((6))
    for i in range(0,6):
            if(a[i]==0):
                n[i]+=5
            elif(a[i]==1):
                n[i]-=5
            limit=90
            if abs(n[i])>limit :
                n[i]=n[i]/abs(n[i])*limit
    return n
def train_mem(s, f, a, Q):
    n=update_state(s,a)
    _,nQ,_=state_pass(n,f)
    train_i=np.zeros((6,10))
    for i in range (0, 6):
        train_i[i]=create_input(s, f, i)
    train_o=np.copy(Q)
    for i in range (0, 6):
        r=f[0]+nQ[i]
        train_o[i][int(a[i])]=r
    #train_o=n_hidden_2/2.*train_o/linalg.norm(train_o)
    return train_i, train_o

def noise_action(aQ):
    b=np.array([0,1,2]) 
    a=np.zeros((6))
    for i in range (0, len(aQ)):
        rd=np.random.rand()
        if rd<0.8:
            a[i]=aQ[i]
        else:
            a[i]=np.random.choice(b)
    return a;

pred=multilayer_perceptron(x, weights, biases)
#cost=tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=y)) #optimizer=tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)
cost=tf.reduce_sum(tf.square(y-pred))
optimizer=tf.train.GradientDescentOptimizer(learning_rate=learning_rate).minimize(cost)
init=tf.global_variables_initializer()
serv=np.array([[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1]])
batch_i=[]
batch_o=[]
c=0
f=np.array([0])
with tf.Session() as sess: 
    sess.run(init)
    while True:
        Q, _,aQ=state_pass(s, f)
        aQ=noise_action(aQ)
        fit=[0]
        for i in range (0,10):
            nS=update_state(s, aQ)
            io.sendMsg(nS)
            fit[0]+=io.getMsg()[0]
        train_i, train_o=train_mem(s, f, aQ, Q)
        f=fit 
        s=nS;
        print(fit)
        batch_i=np.append(batch_i, train_i)
        batch_o=np.append(batch_o, train_o) 
        c+=1
        c%=100
        sess.run([optimizer, cost], feed_dict={x:batch_i, y:batch_o})


    

