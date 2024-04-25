import matplotlib.pyplot as plt
import numpy as np
import sys
import math

def plotGraphTwo(distanceOOK, distancePWM, ook, pwm, title, xlabel):
    fig, ax1 = plt.subplots()
    color = 'tab:blue'
    ax1.set_xlabel(xlabel)
    ax1.set_ylabel('error rate OOK (%)', color=color)
    ax1.plot(distanceOOK, ook, color=color)
    ax1.tick_params(axis='y', labelcolor=color)

    ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

    color = 'tab:red'
    ax2.set_ylabel('error rate PWM (%)', color=color)  # we already handled the x-label with ax1
    ax2.plot(distancePWM, pwm, color=color)
    ax2.tick_params(axis='y', labelcolor=color)

    plt.title(title)

    fig.tight_layout()
    plt.show()

def plotGraph(distance, error, title):
    fig, ax1 = plt.subplots()
    color = 'tab:blue'
    ax1.set_xlabel("bit rate (bps)")
    ax1.set_ylabel('error rate (%)', color=color)
    ax1.plot(distance, error, color=color)
    ax1.tick_params(axis='y', labelcolor=color)
    plt.title(title)

    fig.tight_layout()
    plt.show()

def distanceOOK():
    distanceOOK = [1, 5, 10, 15, 20, 30, 48, 50, 60]
    error_rateOOK = [0.83, 1.56, 0.00, 0.00, 0.67, 1.33, 3.64, 0.00, 6.77]
    
    fig, ax1 = plt.subplots()
    color = 'tab:blue'
    ax1.set_xlabel("distance (cm)")
    ax1.set_ylabel('error rate (%)', color=color)
    ax1.plot(distanceOOK, error_rateOOK, color=color)
    ax1.tick_params(axis='y', labelcolor=color)
    plt.title("Distance vs Error Rate (OOK)")

    plt.axvline(x = 50, color = 'r', label = 'brightness 100%')
    plt.legend(['error rate', 'brightness 100%']) 
    fig.tight_layout()
    plt.show()

def distancePWM():
    distancePWM = [2, 5, 20, 30, 40]
    error_ratePWM = [0.56, 0.56, 0.66, 1.75, 6.67]
    fig, ax1 = plt.subplots()
    color = 'tab:blue'
    ax1.set_xlabel("distance (cm)")
    ax1.set_ylabel('error rate (%)', color=color)
    ax1.plot(distancePWM, error_ratePWM, color=color)
    ax1.tick_params(axis='y', labelcolor=color)
    plt.title("Distance vs Error Rate (PWM)")

    plt.axvline(x = 20, color = 'r', label = 'All Light On')
    plt.legend(['error rate', 'All Light On']) 
    fig.tight_layout()
    plt.show()

def errorateOOK():
    bitrateOOK = [20, 28.6, 50, 100, 200, 333, 500, 1000]
    error_rateOOK = [0.00, 0.00, 1.13, 24.01, 43.45, 43.88, 51.24, 49.56]
    plotGraph(bitrateOOK, error_rateOOK, "Bit Rate vs Error Rate (OOK)")

def bitratePWM():
    bitratePWM = [31.25, 41.67, 50]
    error_ratePWM = [0.66, 3.06, 4.58]
    plotGraph(bitratePWM, error_ratePWM, "Bit Rate vs Error Rate (PWM)")

def bitrateTwo():
    bitrateOOK = [20, 28.6, 50]
    error_rateOOK = [0.00, 0.00, 1.13]
    bitratePWM = [31.25, 41.67, 50]
    error_ratePWM = [0.66, 3.06, 4.58]
    plotGraphTwo(bitrateOOK, bitratePWM, error_rateOOK, error_ratePWM, "Bit Rate vs Error Rate", "bit rate (bps)")
    #plotGraph(bitratePWM, error_ratePWM, "Bit Rate vs Error Rate (PWM)")

def distanceTwo():
    distancePWM = [2, 5, 20, 30, 40]
    error_ratePWM = [0.56, 0.56, 0.66, 1.75, 6.67]
    distanceOOK = [1, 5, 10, 15, 20, 30, 48, 50, 60]
    error_rateOOK = [0.83, 1.56, 0.00, 0.00, 0.67, 1.33, 3.64, 0.00, 6.77]
    plotGraphTwo(distanceOOK, distancePWM, error_rateOOK, error_ratePWM, "Distance vs Error Rate", "distance (cm)")


if __name__ == "__main__":
    bitrateTwo()
