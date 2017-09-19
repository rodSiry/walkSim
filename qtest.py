import torch 
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torch.autograd as autograd
from torch.autograd import Variable
from torch.autograd._functions import stochastic as STO
from listmodule import ListModule
import gym
from IO import *

class Net(nn.Module):
    def __init__(self, n_input, n_hidden, n_output):
        super(Net, self).__init__()
        self.saved_actions=[]
        self.rewards=[]
        self.normal=STO.Normal()
        self.activ=nn.LeakyReLU()
        self.h0=Variable(torch.zeros(1,n_hidden), requires_grad=False)
        self.c0=Variable(torch.zeros(1,n_hidden), requires_grad=False)
        self.hi=[]
        self.ci=[]
        fc=[]
        lin=[]
        self.n_lstm=10
        self.n_lin=10
        for i in range(self.n_lstm):
            self.hi.append(Variable(torch.randn(1,n_hidden)))
            self.ci.append(Variable(torch.randn(1,n_hidden)))
            if(i==0):
                fc.append(nn.LSTMCell(n_input, n_hidden))
            else:
                fc.append(nn.LSTMCell(n_hidden, n_hidden))
        for i in range(self.n_lin):
            if(i<self.n_lin-1):
                lin.append(nn.Linear(n_hidden, n_hidden ))
            else:
                lin.append(nn.Linear(n_hidden, n_output ))
        self.fc=ListModule(*fc)
        self.lin=ListModule(*lin)

    def reinit(self):
        for i in range(self.n_lstm):
            self.hi[i]=self.h0
            self.ci[i]=self.c0
    def forward(self, x):
        for i in range(self.n_lstm):
            self.hi[i], self.ci[i] =self.fc[i](x, (self.hi[i], self.ci[i]))
            x=self.ci[i]
        for i in range(self.n_lin):
            x=self.activ(self.lin[i](x))
        return x
        
def finish_episode():
    R=0
    rewards=[]
    for r in network.rewards[::-1]:
        R= r +0.99* R
        rewards.insert(0,R)
    rewards=torch.Tensor(rewards)
    rewards= ( rewards - rewards.mean()) / rewards.std()
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
torque_int=15
torque_int_std=25
        
io=IO()
network=Net(n_input,128, n_output)
optimizer = optim.Adam(network.parameters(), lr=1e-2)
while True:
    #RESET!
    observation=np.array(io.getMsg())
    network.reinit()
    while(True):
        obs_input= Variable(torch.from_numpy(observation).unsqueeze(0).float())
        prob=network(obs_input)
        action=torch.normal(Variable(torque_int*prob.data[0,0:n_servo], requires_grad=True), Variable(torque_int_std*torch.abs(prob.data[0,n_servo:]), requires_grad=True))
        observation, reward, done = simCycle()
        network.rewards.append(reward)
        network.saved_actions.append(action)
        if(done==1.):
            break
    finish_episode()
