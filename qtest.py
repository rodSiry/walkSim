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
        self.fc1=nn.Linear(n_input, n_hidden)
        self.fc2=nn.Linear(n_hidden, n_hidden )
        self.fc3=nn.Linear(n_hidden, n_hidden )
        self.fc4=nn.Linear(n_hidden, n_hidden )
        self.fc5=nn.Linear(n_hidden, n_hidden )
        self.fc6=nn.Linear(n_hidden, n_output)
        self.saved_actions=[]
        self.rewards=[]
        self.normal=STO.Normal()
    def forward(self, x):
        x=F.relu(self.fc1(x))
        x=F.relu(self.fc2(x))
        x=F.relu(self.fc3(x))
        x=F.relu(self.fc4(x))
        x=F.relu(self.fc5(x))
        x=F.relu(self.fc6(x))
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
n_input=2*n_servo
n_output=2*n_servo
        
io=IO()
network=Net(n_input,128, n_output)
optimizer = optim.Adam(network.parameters(), lr=1e-2)
while True:
    #RESET!
    observation=np.array(io.getMsg())
    for j in range(10000):
        obs_input= Variable(torch.from_numpy(observation).unsqueeze(0).float())
        prob=network(obs_input)
        action=torch.normal(Variable(5*prob.data[0,0:n_servo], requires_grad=True), Variable(5*prob.data[0,n_servo:], requires_grad=True))
        observation, reward, done = simCycle()
        network.rewards.append(reward)
        network.saved_actions.append(action)
        if(done==1.):
            break
    print(finish_episode())