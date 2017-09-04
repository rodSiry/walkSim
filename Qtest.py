from IO import *
import tensorflow as tf
import numpy as np
import numpy.linalg as linalg
io=IO()
n_servo=8
n_input=2*n_servo+3
n_state=2*n_servo
n_hidden_1=11
n_hidden_2=11
n_classes=3
learning_rate=0.001 
x=tf.placeholder("float", [None, n_input])
y=tf.placeholder("float", [None, n_classes])
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

def create_input(s, i):
    serv=np.array([[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1],[1,1,0],[1,1,1]])
    return np.array([np.concatenate((serv[i],s))])
def state_pass(s):
    aQ=np.zeros(n_servo)
    nQ=np.zeros(n_servo)
    Q=np.zeros((n_servo, 3))
    for i in range (0, n_servo):
            test=create_input(s, i)
            o=sess.run([pred], feed_dict={x:test})
            Q[i]=np.array(o)
            aQ[i]=np.argmax(o)
            nQ[i]=np.amax(o)
    return Q, nQ, aQ
def update_state(s, a):
    n=np.zeros((n_servo))
    for i in range(0,n_servo):
            if(a[i]==0):
                n[i]+=5
            elif(a[i]==1):
                n[i]-=5
           #limit=360
            #if abs(n[i])>limit :
            #    n[i]=n[i]/abs(n[i])*limit
    return n
def train_mem(s, f, a):
    n=s+update_state(s,a)
    _,nQ,_=state_pass(n)
    train_i=np.zeros((n_servo,n_input))
    for i in range (0, n_servo):
        train_i[i]=create_input(s, i)
    train_o=np.copy(Q)
    for i in range (0, n_servo):
        r=10.*f[0]+nQ[i]
        train_o[i][int(a[i])]=r
    return train_i, train_o

def noise_action(aQ):
    b=np.array([0,1,2]) 
    a=np.zeros((n_servo))
    for i in range (0, len(aQ)):
        rd=np.random.rand()
        if rd<1.:
            a[i]=aQ[i]
        else:
            a[i]=np.random.choice(b)
    return a;

pred=multilayer_perceptron(x, weights, biases)
cost=tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=y)) 
optimizer=tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)
#cost=tf.reduce_sum(tf.square(y-pred))
#optimizer=tf.train.GradientDescentOptimizer(learning_rate=learning_rate).minimize(cost)
init=tf.global_variables_initializer()
serv=np.array([[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1]])
lB=64000
lMB=6004
batch_s=np.zeros((lB,n_state))
batch_a=np.zeros((lB,n_servo))
batch_r=np.zeros((lB,1))
c=0
cF=0
epoch=1
f=np.array([0])
trained=False
with tf.Session() as sess: 
    sess.run(init)
    while True:
        s=io.getMsg()
        print(s)
        Q, _,aQ=state_pass(s)
        aQ=noise_action(aQ)
        fit=[0]
        for i in range(0,1):
            nS=update_state(s, aQ)
            io.sendMsg(nS)
            fit[0]=io.getMsg()[0]
            cF+=fit[0]
        f=fit 
        #experience storage
        batch_s[c]=s
        batch_a[c]=aQ 
        batch_r[c]=fit
        c+=1
        if c%512==0 and cF==0:
            trained=True
        if c%512==0:
            cF=0
        if c%lMB==0 and c!=0 and trained==False: 
            print("TRAINING...")
            for j in range (epoch):
                sub_i=np.random.choice([i for i in range(0, min(lB, c))], min(lMB, c))
                train_i=np.zeros((n_state*len(sub_i),n_input))
                train_o=np.zeros((n_servo*len(sub_i),3))
                for i in range (0,len(sub_i)):
                        train_i[n_state*i:n_state*i+n_state], train_o[n_servo*i:n_servo*i+n_servo] =train_mem(batch_s[sub_i][i], batch_r[sub_i][i], batch_a[sub_i][i])
                        sess.run([optimizer, cost], feed_dict={x:train_i, y:train_o})
        c%=lB

    

