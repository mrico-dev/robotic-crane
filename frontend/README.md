# This is a readme

npm install -g typescript


---

## Visualizing a Robotic Crane (Controls) 
Take home assignment: Software Controls
The goal of this project is to develop a simple web application for visualizing the state of a virtual robotic system and providing a mechanism to manipulate its pose. Our focus is on simulating a straightforward 4-degree-of-freedom (4-DOF) robotic crane, inspired by the Monumental-style crane. The current state of the crane will be transmitted as telemetry to a client application, where we will render a dynamic 3D representation of the crane in real-time. Additionally, we will implement a user interface to allow sending movement commands to our simulated robot in the backend, mimicking the control processes involved in actual robotic systems.

More information about the topology of our crane system can be found below.
This take home assignment is twofold and encompasses a bit of robotics control and the design of a user interface around this. Because you’ve applied for the Software Engineer, Controls role we encourage you to strongly focus your efforts on getting the controls part of the assignment right. We do expect a working interface as it’s the best way for us to quickly validate the results, but we won’t be judging you on the beauty of the UI. 

## What we want to see
### Basic setup
Set up both a backend and frontend that communicate over a WebSocket. Feel free to utilize an out-of-the-box framework such as Next.js if that speeds up development. We recommend using TypeScript for the frontend, which is our language of choice.

The backend should implement a mock robotic crane, storing its current state. The robotic crane is represented by the current actuator positions for each joint: swing rotation in degrees, lift elevation in mm, elbow rotation in degrees, wrist rotation in degrees, and gripper open/close state in mm. We leave the choice of language here up to you. If you want to make our lives slightly easier you can pick one of Rust/TypeScript/Python/C++.

Stream the current state from the backend to the client at a fixed interval over a WebSocket connection. Experiment with the frequency to determine what works best for your project.

On the client side, the user interface comprises two parts: some UI chrome containing control elements and a canvas displaying a 3D representation of the crane.

Use WebGL (or a wrapper, such as Three.js) to establish a basic scene that renders a simplified representation of the robotic crane. The crane's state should reflect the current pose received from the WebSocket connection. Depending on the time available for the project, you can make this 3D representation as simple or visually appealing as you prefer.

The control interface contains some user inputs, one for each actuator. Users should be able to modify one or more state values and submit the changes as a control command to the backend via the WebSocket. The backend will implement these changes as a motion in the mock robotic state, considering predefned maximum speeds and acceleration per actuator. The end result will be a small animation in the frontend.

Additionally, the interface provides a means to input a coordinate in 3D space. Upon submitting this coordinate to the backend, the application should perform an inverse kinematics calculation to derive a desired robotic pose for the crane, applying this to the mock state accordingly. The IK for this crane is very simple to derive analytically and doesn’t require any complicated solver.

## Controlling the crane origin
Now introduce a virtual sensor that produces a 4-DOF origin coordinate for the robot. 4-DOF in this case means a 3D position in space together with a rotation around the z-axis. This origin might for example represent the result of some SLAM operation of some ground vehicle the crane is mounted on, or the crane might hang upside down from a gantry system. We want you to enable the robot to keep its end-effector stationary while the origin moves around.

Render the crane in the app at this new origin. Clearly visualize the robot origin in space relative to the world origin.

Ensure inverse kinematics still works when the robot is not positioned at the world origin.

Find a way to test your implementation by allowing the origin to be dynamic. Either some input box in the UI, clicking a position the 3D world, or slowly move it around on some timer.

Post the new origin to the backend and slowly move the robot origin into place as if it is another joint with some maximum speed configured. 

Now use the existing inverse kinematics together with some control loop to hold the end-effector stationary in world space while the origin can change around. Keep it “as still as possible” given the speed and range constraints. What happens if the origin changes faster than the end-effector can keep up with?
Bonus: How can you deal with an origin sensor is noisy or has some jitter? Or oscillates slowly like it’s on a wavy ocean? Or if the data is lagging in time?

## Things to consider
Completing the entire assignment, including all the controls and a user-friendly interface, within reasonable time constraints might be challenging. If you find it necessary to simplify the assignment, it's up to you how you approach it. We believe we can gain valuable insights into candidates' abilities by observing how they prioritize tasks and break down larger projects. Clearly express what you've accomplished and any challenges you've encountered. Completing the entire assignment would be great, of course! 

User interface aesthetics, code quality, correctness, performance, etc are all very important aspects of good software, but try to get all the basic building blocks end-to-end up and running first. De-risk the complicated parts of the project first, add the fun stuff later. 

Keep things simple. Don’t over-engineer your architecture, and especially not the backend. Use the absolute minimum amount of technology you need to get to a solution. We get impressed when we see simple and concise solutions that one can grasp quickly.

Think about where to put your configuration, domain logic, rendering logic etc. Put it in the backend or in the frontend app? Are there obvious ways to prevent duplication of logic or configuration?
If you want to change the dimensions of the robotic crane, what needs to change? Can we use a single definition/configuration? What if we want to constrain the ranges of motions? Add another degree of freedom? Have multiple robots work together? Is your architecture easy to extend?

The goal is to show off your skills and we really hope it’s a fun project to work on!

## Crane topology
Our 4-DOF crane contains 4 different axes together with a gripper. In topological order:
A rotary swing joint at the base of the crane rotating the entire column plus arm around a central axis.
A linear lift joint moving the arm up and down along the column using a belt drive connected to counter weight on the back. (Counter weight not depicted below)
A rotary elbow joint more or less midway the arm.

A rotary wrist joint at the lower arm connected to our wrist extension. This fixed extension allows for building lower to the ground.

The gripper end-effector is fixed to the wrist extension and has a fixed jaw plus movable jaw. The jaw can move in and out for gripping purposes.