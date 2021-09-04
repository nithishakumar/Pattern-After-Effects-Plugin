# Pattern 
An Adobe After Effects plugin that creates Islamic star pattern-style animations using Hankin's polygons-in-contact algorithm. Colors for animations are generated from 
datasets of images (illustrated by professional artists at Kurzgesagt) through k-means clustering. This plugin is based on the University of Waterloo's research paper "Islamic Star Patterns from Polygons in Contact" by Craig S. Kaplan (https://dl.acm.org/doi/pdf/10.5555/1089508.1089538).

See LICENSE.md for license details.

![Alt Text](https://github.com/nithishakumar/Pattern-After-Effects-Plugin/blob/main/animations/animation%201.gif)

![Alt Text](https://github.com/nithishakumar/Pattern-After-Effects-Plugin/blob/main/animations/animation%208.gif)

![Alt Text](https://github.com/nithishakumar/Pattern-After-Effects-Plugin/blob/main/animations/animation%202.gif)

![Alt Text](https://github.com/nithishakumar/Pattern-After-Effects-Plugin/blob/main/animations/animation%203.gif)

# Building Pattern on Windows

1. Follow this tutorial to set up the After Effects SDK files on Visual Studio: https://www.youtube.com/watch?v=gilpHirsXQA 
   
   Make sure to rename everything to "Pattern". In PatterPiPL.r, change the category to "Motion Graphics Plugins" before compiling.
2. Follow this tutorial to link your project to the Cairo graphics library: https://www.youtube.com/watch?v=oAVWwBA2K0U
   
   Everything you need is in the "cairo" folder of this repository. 
3. Copy the cairo.dll file from the cairo folder of this repository to the "Support files" folder of where Adobe After Effects is installed in your computer 
   (C:\Program Files\Adobe\Adobe After Effects 2021\Support Files).
4. Install OpenCV 3.4.15 from this link: https://opencv.org/releases/. Extract it to a folder you prefer. I recommend your D drive.                                
   
   Refer to "option 1" in this tutorial if you need any help along the way: https://www.youtube.com/watch?v=eDGSkdeV8YI
5. Create a new project in Visual Studio, choose Static Library (.lib) in C++, and place it in the same folder as Pattern. Name it "Color".
6. Make sure to change the configuration of the static library project to x64 from x86:
   
   ![image](https://user-images.githubusercontent.com/73742037/131928290-43f029dd-4664-464e-a667-45537e956981.png)
   
7. Link OpenCV to your static library by adding the path to "include" and "lib" in your OpenCV folder under "Include Directories" and "Library Directories" of this project:
  
   ![image](https://user-images.githubusercontent.com/73742037/131928183-cc8f2460-4463-458e-a5b2-896720664aef.png)
   
8. Now add the code from Color.h and Color.cpp to the Color static library.
9. Download the Image Dataset from this link: https://www.kaggle.com/nithishakumar/pattern-plugin-image-dataset and place it in the same folder as the Pattern plugin. DO NOT RENAME ANY OF THE IMAGES!
10. Copy the path to the ImageDataset and assign this value to the path string in Color.cpp:
 
    ![image](https://user-images.githubusercontent.com/73742037/131933620-cf4940bb-1aa9-4012-a6ce-3ce73d4d92a7.png)

11. Link OpenCV to your Pattern plugin by adding the same paths to "include" and "lib" in the OpenCV folder under "Include Directories" and "Library Directories" of your Pattern project.
12. Include the path to your Color static library folder under "Include Directories" in Pattern.
13. Include the path to color static library folder << x64 << Debug under "Library Directories" in Pattern.
14. Under Linker << Input << Additional Dependencies, add Color.lib and opencv_world3415d.lib to Pattern:

    ![image](https://user-images.githubusercontent.com/73742037/131928785-24106084-5601-4038-884b-53f7e1b44c41.png)
    
15. Replace the code in Pattern.cpp and Pattern.h to the one from this repository.
16. Add Draw.h, Draw.cpp, Draw_Tiling.cpp to Pattern.sln.
17. Run Visual Studio as administrator and build the plugin by clicking on "Local Windows Debugger". You can find the plugin at Effect << Motion Graphics Plugins.
