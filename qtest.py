import torch 
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torch.autograd as autograd
from torch.autograd import Variable
from torch.autograd._functions import stochastic as STO
import gym
from IO import *

class Net(nn.Module):
    def __init__(self, n_input, n_hidden, n_output):
        super(Net, self).__init__()
        self.fc1=nn.LSTMCell(n_input, n_hidden)
        self.fc2=nn.LSTMCell(n_hidden, n_hidden)
        self.fc3=nn.LSTMCell(n_hidden, n_hidden)
        self.fc4=nn.Linear(n_hidden, n_hidden )
        self.fc5=nn.Linear(n_hidden, n_hidden )
        self.fc6=nn.Linear(n_hidden, n_output)
        self.saved_actions=[]
        self.rewards=[]
        self.normal=STO.Normal()
        self.activ=nn.LeakyReLU()
        self.h0=Variable(torch.randn(1,n_hidden))
        self.c0=Variable(torch.randn(1,n_hidden))
        self.hi1=Variable(torch.randn(1,n_hidden))
        self.hi2=Variable(torch.randn(1,n_hidden))
        self.hi3=Variable(torch.randn(1,n_hidden))
        self.ci1=Variable(torch.randn(1,n_hidden))
        self.ci2=Variable(torch.randn(1,n_hidden))
        self.ci3=Variable(torch.randn(1,n_hidden))
    def reinit(self):
        self.hi1=self.h0
        self.hi2=self.h0
        self.hi3=self.h0
        self.ci1=self.c0
        self.ci2=self.c0
        self.ci3=self.c0
    def forward(self, x):
        self.hi1, self.ci1 =self.fc1(x, (self.hi1, self.ci1))
        self.hi2, self.ci2 =self.fc2(x, (self.hi2, self.ci2))
        self.hi3, self.ci3 =self.fc3(x, (self.hi3, self.ci3))
        x=self.activ(self.h0)
        x=self.activ(self.fc4(x))
        x=self.activ(self.fc5(x))
        x=self.activ(self.fc6(x))
        return x
        
def finish_episode():
    R=0
    rewards=[]
    for r in network.rewards[::-1]:
        R= r +0.99* R
        rewards.insert(0,R)
    rewards=torch.Tensor(rewards)
    rewards= ( rewards - rewards.mean()) / (rewards.std() + np.finfo(np.float32).eps)
    for action, r in  zip(network.saved_actions, rewards):
        action.reinforce(r)
    optimizer.zero_grad()
    autograd.backward(network.saved_actions, [None for _ in network.saved_actions])
    optimizer.step()
    del network.rewards[:]
    del network.saved_actions[:]
    return R

def simCycle():
    io.sendMsg(action.data)
    s=np.array(io.getMsg())
    r=io.getMsg()[0]
    d=io.getMsg()[0]
    return s, r, d

n_servo=8
n_input=2*n_servo+1
n_output=2*n_servo
torque_int=1
torque_int_std=20
        
io=IO()
network=Net(n_input,128, n_output)
optimizer = optim.Adam(network.parameters(), lr=1e-2)
while True:
    #RESET!
    observation=np.array(io.getMsg())
    network.reinit()
    for j in range(10000):
        obs_input= Variable(torch.from_numpy(observation).unsqueeze(0).float())
        prob=network(obs_input)
        action=torch.normal(Variable(torque_int*prob.data[0,0:n_servo], requires_grad=True), Variable(torque_int_std*torch.abs(prob.data[0,n_servo:]), requires_grad=True))
        observation, reward, done = simCycle()
        network.rewards.append(reward)
        network.saved_actions.append(action)
        if(done==1.):
            break
    print(finish_episode())
