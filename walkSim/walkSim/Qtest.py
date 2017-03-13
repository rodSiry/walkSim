from IO import *
import tensorflow as tf
import numpy as np
io=IO()
print(io.getMsg())
n_input=10
n_hidden_1=32
n_hidden_2=32
n_classes=3
learning_rate=0.001
x=tf.placeholder("float", [None, n_input])
y=tf.placeholder("float", [None, n_classes])
state=[15., -15., -45., 15., -15., -45.]
def multilayer_perceptron(x, weights, biases):
        layer_1=tf.add(tf.matmul(x, weights['h1']), biases['b1'])
        layer_1=tf.nn.sigmoid(layer_1)
        layer_2=tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
        layer_2=tf.nn.sigmoid(layer_2)
        layer_3=tf.add(tf.matmul(layer_2, weights['h3']), biases['b3'])
        layer_3=tf.nn.sigmoid(layer_3)
        layer_4=tf.add(tf.matmul(layer_3, weights['h4']), biases['b4'])
        layer_4=tf.nn.sigmoid(layer_4)
        out_layer=tf.matmul(layer_4, weights['out'])+biases['out']
        return out_layer
weights={
        'h1': tf.Variable(tf.random_normal([n_input, n_hidden_1])),
        'h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
        'h3': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_2])),
        'h4': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_2])),
        'out': tf.Variable(tf.random_normal([n_hidden_2, n_classes]))
        }
biases={
        'b1': tf.Variable(tf.random_normal([n_hidden_1])),
        'b2': tf.Variable(tf.random_normal([n_hidden_2])),
        'b3': tf.Variable(tf.random_normal([n_hidden_2])),
        'b4': tf.Variable(tf.random_normal([n_hidden_2])),
        'out': tf.Variable(tf.random_normal([n_classes]))
        }

def create_input(s, f):
    serv=np.array([[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1]])
    return np.array([np.concatenate((f, serv[i],s))])
def state_pass(s, f):
    aQ=np.array([])
    nQ=np.array([])
    test=create_input(s, f)
    for i in range (0, 6):
            o=sess.run([pred], feed_dict={x:test})
            train_o[i]=o[0]
            aQ=np.append(aQ, np.argmax(o))
            nQ=np.append(nQ, np.amax(o))
    return nQ, aQ

def train_mem(f, s):
        _,aQ=state_pass(s, f)
        nState=s
        for i in range (0, 6):
            if(aQ[i]==0):
                nState[i]+=5
            elif(aQ[i]==1):
                nState[i]-=5
        nQ,naQ=state_pass(nState,f)
        train_i=create_input(s, f) 
        train_o=np.zeros((6,3)) 
        for i in range (0, len(serv)):
            r=f[0]+nQ[i]
            train_o[i][int(naQ[i])]=r
        return train_i, train_o

pred=multilayer_perceptron(x, weights, biases)
cost=tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=y))
optimizer=tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)
init=tf.global_variables_initializer()
serv=np.array([[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1]])
batch_i=np.array([])
batch_o=np.array([])
fit=np.array([0])
with tf.Session() as sess: 
    sess.run(init)
    while 1000:
        actions=np.array([])
        q=np.array([])
        nQ=np.array([])
        train_o=np.zeros((6,3)) 
        for i in range (0, len(serv)):
            test=np.array([np.concatenate((fit, serv[i],state))])
            o=sess.run([pred], feed_dict={x:test})
            train_o[i]=o[0]
            q=np.append(q, np.amax(o))
            ind=np.argsort(o)[0][0]
            rd=np.random.random_sample()
            if rd<0.75:
                actions=np.append(actions, ind[2])
            elif rd>=0.75 and rd<0.9:
                actions=np.append(actions, ind[1])
            else:
                actions=np.append(actions, ind[0])
        nState=state
        for i in range (0, len(actions)):
            if(actions[i]==0):
                nState[i]+=5
            elif(actions[i]==1):
                nState[i]-=5
        for i in range (0, len(serv)):
            test=np.array([np.concatenate((fit, serv[i],nState))])
            o=sess.run([pred], feed_dict={x:test})
            nQ=np.append(nQ, np.amax(o))
        state=nState
        io.sendMsg(state)
        fit=io.getMsg()
        train_o_p=np.copy(train_o)
        train_i=np.zeros((6,n_input))
        for i in range (0, len(serv)):
            train_i[i]=np.concatenate((fit, serv[i], state))
            r=fit[0]+nQ[i]
            train_o[i][int(actions[i])]=r
        if len(batch_i)==0:
            batch_i=train_i
        elif len(batch_i)<1024:
            batch_i=np.concatenate((batch_i, train_i))
        if len(batch_i)>1024:
           np.delete(batch_i,0)
        if len(batch_o)==0:
            batch_o=train_o
        elif len(batch_o)<1024:
            batch_o=np.concatenate((batch_o, train_o))
        if len(batch_o)>1024:
            np.delete(batch_o, 0)
        sess.run([optimizer, cost], feed_dict={x:batch_i,y:batch_o})
        print(fit)


    

