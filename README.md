# 2D-Graphics-Toolkit
This is a C++ project that uses Windows API and OpenGL to create a graphical user interface (GUI) for drawing and manipulating 2D shapes. The project implements various algorithms for line, circle, ellipse, curve, filling, and clipping operations. The user can interact with the window using mouse only, and can choose the shape color, filling quarter, and clipping window from a menu. The user can also save and load the data on the screen to and from files.

## Features
The project includes the following features:

### 1. Changing Window Background
The background color of the window is set to white, providing a clean and neutral canvas for drawing shapes and graphics.

### 2. Custom Window Mouse Shape
Users can change the shape of the mouse cursor within the window to fit their preferences or enhance the user experience.

### 3. Mouse Interaction
The 2D Graphics Toolkit allows users to interact with the window solely through mouse actions. This provides a user-friendly and intuitive interface for drawing and performing operations.

### 4. Console and Window Combination
The toolkit offers integration between the console and the graphics window. This allows for displaying additional information, receiving input, or interacting with the graphics through console commands.

### 5. Shape Color Selection
Before drawing a shape, users have the option to choose the color from a menu. This feature provides flexibility and customization, allowing users to create visually appealing compositions.

### 6. Clear Screen
The toolkit includes functionality to clear the screen, removing all previously drawn shapes and graphics, providing a fresh canvas for further creations.

### 7. Save Function
Users can save the current state of the screen, including all shapes and graphics, to a file. This feature allows for preserving and sharing compositions or resuming work at a later time.

### 8. Load Function
The toolkit includes a load function that enables users to load previously saved data from files. This allows for revisiting and modifying previous compositions.

### 9. Line Algorithms
The 2D Graphics Toolkit implements various line algorithms for efficient and accurate line drawing. The available algorithms include DDA (Digital Differential Analyzer), Midpoint, and Parametric algorithms.

### 10. Circle Algorithms
Different circle algorithms are implemented, offering versatility in circle drawing. The available algorithms include Direct, Polar, Iterative Polar, Midpoint, and Modified Midpoint algorithms.

### 11. Filling Circle
The toolkit provides options for filling circles using lines or other circles. Users can define the fill quarter to achieve desired visual effects.

### 12. Filling Square with Hermit Curve
Users can fill a square with a Hermit curve vertically, creating interesting patterns and designs.

### 13. Filling Rectangle with Bezier Curve
The toolkit offers functionality to fill a rectangle with a Bezier curve horizontally. This allows for creating smooth and visually appealing shapes.

### 14. Convex and Non-Convex Filling Algorithm
The 2D Graphics Toolkit implements algorithms for filling both convex and non-convex shapes. This provides flexibility in creating and filling complex graphical compositions.

### 15. Recursive and Non-Recursive Flood Fill
Flood fill algorithms are provided for filling enclosed regions. The toolkit supports both recursive and non-recursive flood fill algorithms, enabling efficient and accurate region filling.

### 16. Cardinal Spline Curve
Users can draw Cardinal spline curves, which are widely used in computer graphics and animation. This feature allows for creating smooth and natural curves.

### 17. Ellipse Algorithms
The toolkit includes different ellipse algorithms, such as Direct, Polar, and Midpoint algorithms. Users can draw ellipses with various sizes and proportions.

### 18. Clipping Algorithms
Clipping algorithms are available for efficiently and accurately clipping shapes against various clipping windows. The supported clipping windows include Rectangle, Square, and Circle, with clipping for Point and Line primitives.

## Usage

1. Clone the repository:
```
git clone https://github.com/KhaledAshrafH/2D-Graphics-Toolkit.git
```

2. Install the necessary dependencies, including the Windows API and OpenGL libraries.

3. Build the project using your preferred C++ compiler.

4. Run the compiled executable.

5. Interact with the graphical interface using the mouse and navigate through the available menu options.

<!-- 
## Demo
You can see a demo of how this project works in action in this [video](video). The video shows some of the features and functionality of this project, such as drawing lines, circles, ellipses, curves, filling shapes, clipping shapes, saving and loading data, etc.
-->

## Contributing
If you are interested in contributing to this project, you are welcome to do so. Please follow these guidelines on how to contribute:

- Fork this repository
- Clone your forked repository to your local machine
- Create a new branch for your feature or bug fix
- Write your code and add comments
- Test your code and fix any errors
- Commit and push your changes to your remote branch
- Create a pull request from your branch to this repository's main branch
- Wait for code review and feedback

## Future Work
There are some possible improvements and extensions that We plan to do or suggest for this project in the future. These are:

- Add more shapes and curves to draw on the window
- Add more options for filling and clipping operations
- Add zooming and panning functionality to the window
- Add undo and redo functionality to the window
- Add animation and transition effects to the shapes

## Team
This project was created by a team of five computer science students at Faculty of Computers and Artificial Intelligence Cairo University. The team members are:

- [Khaled Ashraf](https://github.com/KhaledAshrafH).
- [Noura Ashraf](https://github.com/NouraAshraff).
- [Samaa Khalifa](https://github.com/SamaaKhalifa).
- [Ahmed Sayed](https://github.com/AhmedSayed117).
- [Ahmed Ebarhim](https://github.com/Ahmed-Ibrahim-30).

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE.md) file for details.

## Acknowledgments
This project is based on the project of the Computer Graphics course at Faculty of Computers and Artificial Intelligence Cairo University. We would like to thank Prof. Reda A. El-Khoribi for his guidance and support throughout this course.

## Contact
For any inquiries or questions regarding the project, please contact 5aleda4rf@gmail.com.
