# ICP-gui
6 DoF object pose estimation in RGB-D images, using an Iterative Closest Point implementation with tunable parameters through a Qt GUI.
Uses [PCL's ICP](http://docs.pointclouds.org/trunk/classpcl_1_1_iterative_closest_point.html) under the hood.

Initial screenshot:
![Screenshot](init_screenshot.png?)
After processing scene and object:
![Screenshot](scene_object_process_screenshot.png?)
After initializing ICP:
![Screenshot](icp_init_screenshot.png?)
After running ICP:
![Screenshot](final_screenshot.png?)

Progress:
- [x] Bare-bones Qt GUI and learn to set up the Qt dev environment
- [x] Vis class for drawing point clouds, cubes, spheres etc. in a PCLVisualizer
- [x] Integrate Vis into the Qt GUI
- [x] Add various ICP parameters to the GUI
- [x] Bring in ICP code from previous project
- [x] Tune ICP to work for the example included in the `data` folder
- [x] Implement function for saving the pose estimation output

## Description
This project is about estimating the 6 DoF pose of an object in a scene, given the 3D model of the object. The [Qt library](https://www.qt.io/) is used to make the whole process interactive. Steps:
- Scene:
  - Subsample the scene using a Voxel Grid filter (input `leaf size`)
  - Segment the scene (assumed to be the object lying on top of a table)
    - Estimate the plane of the table. The table is assumed to be the dominant plane in the scene.
    - The position of the object is assumed to be roughly known (for example, by placing it on top of an [AR-tag marker](http://wiki.ros.org/ar_track_alvar). The XYZ coordinates are input in the `object x`, `object y` and `object z` fields.
    - The object is segmented as anything that sticks out above a certain height (input `height above table`) above the table plane. This segmentation is further narrowed by only keeping the points in a cube around the (`object x`, `object y` and `object z`) point. The size of this cube is input as `box size`.
    - The rough object co-ordinates for the example scene included in the `data` directory is  [-0.092, 0.112, 0.687] m.
    - Click `Process` to process the scene with these parameters. The viewer will be updated with the processed scene.
- Object:  
  - Process the object by subsampling it with a Voxel Grid filter. Preferably, the `leaf size` for this filter should be the same as the one used to process the scene. Click `Process` to process the object with these parameters. The viewer will be updated with the processed object.
- ICP:
  - Set [ICP](http://docs.pointclouds.org/trunk/classpcl_1_1_iterative_closest_point.html) parameters. Right now, the following are supported:
    - [RANSAC outlier rejection threshold](http://docs.pointclouds.org/trunk/classpcl_1_1_registration.html#a64db6d25e2707a174dbad28f2484bffe)
    - [Maximum correspondence distance](http://docs.pointclouds.org/trunk/classpcl_1_1_registration.html#a65596dcc3cb5d2647857226fb3d999a5)
    - [Use reciprocal correspondences](http://docs.pointclouds.org/trunk/classpcl_1_1_iterative_closest_point.html#a22c5480a073da9c53fcb0d387e1c997b)
    - [Estimate Scale](http://docs.pointclouds.org/trunk/classpcl_1_1_registration.html#ad16bd1099eb60c9ac26fdc6c56058029) as part of ICP
  - Initialize the azimuth angle of the object (`object azimuth`)
  - Click `Initialize` to initialize the ICP with the processed scene and object (including object azimuth angle), and ICP parameters. If the azimuth angle does not seem right, change it and click `Initialize` again. The viewer will be updated everytime after `Initialize` is clicked to show the procesed scene and object in the same reference frame. Preferably, they should be as close to each other as possible to make the ICP's work easier.
  - Click `Process` to run ICP. The viewer will be updated with the result.
  - Click `Save` to save the estimated object pose.

## GUI Tips
- The viewers are all interactive, so you can rotate, pan and zoom in them by using the mouse. They are all instances of the [PCLVisualizer](http://docs.pointclouds.org/1.8.1/classpcl_1_1visualization_1_1_p_c_l_visualizer.html) embedded in the Qt GUI.
- The viewers take a second or two to update after the corresponding `Process` button is clicked.

# Installation
Make sure the following packages are installed! On Ubuntu based system run:
```
sudo apt-get install build-essential qtbase5-dev libvtk6-dev libvtk6-qt-dev libpcl-dev libboost-all-dev
```

Clone the repo and make a build folder inside
```
git clone https://github.com/samarth-robo/ICP-gui.git
mkdir ICP-gui/build
cd ICP-gui/build
```

Make sure cmake is installed. Build and compile the project.
```
sudo apt-get install cmake
cmake ..
make all
```

You can also use Ninja to build the project for faster compile times on multi core CPUs.
```
sudo apt-get install ninja-buld
cmake -G Ninja ..
ninja
```

# Useage
Run the executeable in the build folder.
```
./qtcreator_test
```
