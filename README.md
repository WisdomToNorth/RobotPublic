# RobotGUI
A totally redesigned application of RobotGui 1.0


### 2.0 Alpha

- [ ] Data preparation
  - [x] Basic data exchange: STEP, STL, PLY, BREP
  - [ ] Business related DB io: robot, path, trajectory, context
  - [x] Software configuration io
  - [ ] Redesign resource file, we should not change brep file
  - [ ] Imported file should have axis information
  - [ ] command input
  - [ ] app mode
  - [ ] unit

- [ ] Software framework
  - [ ] P0: script system
  - [ ] P0: Regression system
  - [ ] P0: Undo/Redo
  - [x] Command basic design
  - [x] Multiple viewer support
    - [ ] Known bug: on mac, if change viewer and change back, mouse tracking will lose until resize it
  - [x] DB based infrastructure

- [ ] Core command:
  - [ ] import robot / part
    - [ ] calibrate the object

  - [ ] import translocator(Seen as simple robot)
  - [ ] robot location demo(forward arm)
  - [ ] robot move and record trajectory
  - [ ] load/save robot trajectory
  - [ ] play trajectory
    - [ ] able to stop/continue play
    - [ ] able to edit trajectory
    - [ ] robot joint speed should be considered

  - [ ] import path
    - [ ] save robot state key point as path
    - [ ] calibrate the path

- [ ] Core Algorithm
  - [ ] Robot interpolation
    - [ ] arc
    - [x] line
    - [x] joint

  - [ ] trajectory plan from path
    - [ ] consider arm range
    - [ ] consider self collision detection
    - [ ] consider context collision detection
    - [ ] consider joint speed
    - [ ] consider robot joint posture

- [ ] Split command from viewer(zoom in/out, etc)
- [ ] Able to undo/redo command
- [ ] Editor of robot's tool
- [ ] Multiple context support

