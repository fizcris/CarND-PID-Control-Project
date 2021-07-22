# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---
## **Parts of a PID controller**

### **P** - The proportional part

The proportional part acts on the current error. So this tends to overshoot the system response and leads to unstable systems quite easily when the undamped region is crossed.

### **I** - The Integral part
The integral part of the controller removes steady-state errors or errors made by disturbances. Basically sums up the previous errors to act accordingly. It is advised to ramp up this parameter for highly noisy systems or when errors against the setpoint are quite low.
### **D** - The derivative part
The derivate part of the controller acts on the `direction` of the error, so it basically tries to dampen the Proportional part by achieving faster settling times or reducing overshoot.

## **Feedback signal**
(Images from [MIT video](https://youtu.be/4Y7zG48uHRo))

The **cross-track error** is the source error of the PID controllers used in this project.

For the main PID controller, the output is the steering angle. The `reference` for this controller is `0`, as we aim to always stay at the center of the line.

Another PID controller was implemented for the speed, this controller was coded in a slightly different way to slow down the vehicle in tricky situations being the speed output calculated as `throttle = setSpeed - abs(PID_output)` 


![](res/2021-07-22-10-11-26.png)

## **Tunning procedure**

The tunnning procedure followed the steps explained below, basically we hand tuned the parameters starting with a **P** controller, then we moved to a **PD** controller to finally end with a **PID** controller:
### **P controller - steering**


**Tunning:** Ramp up P gain until the system becomes marginally stable. For this plant around 0.12. 
Too high gains lead to unstable systems when the error is significant.

<table>
    <tr>
        <td>
            <img src="res/2021-07-22-10-04-19.png" alt="cal_images" width="500" />
        </td>
        <td>
            <img src="res/2021-07-22-10-05-09.png" alt="und_cal_images" width="500" />
        </td>
    </tr>
</table>

### **PD controller - steering**
**Tunning:** Once P controller is marginally stable ramp up D controller until the oscillations start to become large, then fine tue around that area.


<table>
    <tr>
        <td>
            <img src="res/2021-07-22-10-06-38.png" alt="cal_images" width="500" />
        </td>
        <td>
            <img src="res/2021-07-22-10-07-24.png" alt="und_cal_images" width="500" />
        </td>
    </tr>
</table>


### **PID controller - steering**
**Tunning:** Add a bit of Integral term to help the system to return to center abit better after and during bends. Too high integral terms then to make the system unstable and the gains with this term are quite marginal.



<table>
    <tr>
        <td>
            <img src="res/2021-07-22-10-09-30.png" alt="cal_images" width="500" />
        </td>
        <td>
            <img src="res/2021-07-22-10-10-35.png" alt="und_cal_images" width="500" />
        </td>
    </tr>
</table>

### **PD controller - speed**
In parallel to the steering controller a speed PD controller has been added.It basically slows down the vehicle when big oscillations start to occurr to avoid making an unstable system that goes off track. This controller is multiplied then by the speed so the effect of it changes depending on the speed of the vehicle as well as the cross track error.
***
***
## **PID modifications**
### **Anti Windup & output saturation**

Although it is highly unlikely we enter in windup situations in this particular controller it is always nice to implement anti-windup methods to saturate the integral part and the output.

The images and code below were obtained from [here](http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-reset-windup/) they are pretty well self-explanatory.

<table>
    <tr>
        <td>
            <img src="res/2021-07-22-10-16-09.png" alt="cal_images" width="500" />
        </td>
        <td>
            <img src="res/2021-07-22-10-16-32.png" alt="und_cal_images" width="500" />
        </td>
    </tr>
</table>

The implementation looks like this: 

```
void SetOutputLimits(double Min, double Max)
{
   if(Min > Max) return;
   outMin = Min;
   outMax = Max;
    
   if(Output > outMax) Output = outMax;
   else if(Output < outMin) Output = outMin;
 
   if(ITerm> outMax) ITerm= outMax;
   else if(ITerm< outMin) ITerm= outMin;
}
```

## **Twiddle** (coordinate ascent)
The implementation of the algorithm is stated below although it was not applied to this project, as it is thought that more modern controllers as MPC are better suited for this specific purpose. 

Nonetheless, this algorithm can be very useful to tune any kind of PID, especially when high accuracy and fast responses are required.

```
 while sum(dp) > tolerance:
        for i in range(len(p)):
            p[i] += dp[i]

            # test modified controller with pant
            # Obtain error (err)

            # If it gives good result keep increasing 
            if err  <  best_err: 
                best_err = err
                dp[i] *=1.1
            # If results are not good --> Decrease
            else:
                p[i] -= 2*dp[i]
                # test modified controller with pant
                # Obtain error
                if err  <  best_err:
                    best_err = err
                    dp[i] *=1.1
            # If the results are always worse, try smaller parameters 
                else:
                    p[i] += dp[i]
                    dp[i] *=0.9
```



## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).